#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#include "AimpHTTP.h"
#include "AIMPYouTube.h"
#include "AIMPString.h"
#include "Tools.h"
#include <process.h>
#include "SDK/apiFileManager.h"

bool AimpHTTP::m_initialized = false;
IAIMPServiceHTTPClient *AimpHTTP::m_httpClient = nullptr;
std::set<AimpHTTP::EventListener *> AimpHTTP::m_handlers;

AimpHTTP::EventListener::EventListener(CallbackFunc callback, bool isFile) : m_isFileStream(isFile), m_callback(callback) {
    AimpHTTP::m_handlers.insert(this);
}

AimpHTTP::EventListener::~EventListener() {
    AimpHTTP::m_handlers.erase(this);
}

void WINAPI AimpHTTP::EventListener::OnAccept(IAIMPString *ContentType, const INT64 ContentSize, BOOL *Allow) {
    ContentType->AddRef();
    ContentType->Release();
    *Allow = AimpHTTP::m_initialized && Plugin::instance()->core();
}

void WINAPI AimpHTTP::EventListener::OnAcceptHeaders(IAIMPString *Header, BOOL *Allow) {
    Header->AddRef();
    Header->Release();
    *Allow = AimpHTTP::m_initialized && Plugin::instance()->core();
}

void WINAPI AimpHTTP::EventListener::OnComplete(IAIMPErrorInfo *ErrorInfo, BOOL Canceled) {
    if (m_stream) {
        if (AimpHTTP::m_initialized && Plugin::instance()->core()) {
            if (m_isFileStream) {
                m_stream->Release();
                if (m_callback)
                    m_callback(nullptr, 0);
                return;
            }

            if (m_callback || m_imageContainer) {
                int s = (int)m_stream->GetSize();
                unsigned char *buf = new unsigned char[s + 1];
                buf[s] = 0;
                m_stream->Seek(0, AIMP_STREAM_SEEKMODE_FROM_BEGINNING);
                m_stream->Read(buf, s);

                if (m_callback) {
                    m_callback(buf, s);
                } else if (m_imageContainer) {
                    if (s <= m_maxSize) {
                        (*m_imageContainer)->SetDataSize(s);
                        memcpy((*m_imageContainer)->GetData(), buf, s);
                    } else {
                        (*m_imageContainer)->Release();
                        *m_imageContainer = nullptr;
                    }
                }
                delete[] buf;
            }
        }
        m_stream->Release();
    }
}

void WINAPI AimpHTTP::EventListener::OnProgress(const INT64 Downloaded, const INT64 Total) {

}

bool AimpHTTP::Get(const std::wstring &url, CallbackFunc callback, bool synchronous) {
    if (!AimpHTTP::m_initialized || !Plugin::instance()->core())
        return false;

    EventListener *listener = new EventListener(callback);
    Plugin::instance()->core()->CreateObject(IID_IAIMPMemoryStream, reinterpret_cast<void **>(&(listener->m_stream)));

    return SUCCEEDED(m_httpClient->Get(AIMPString(url), synchronous ? AIMP_SERVICE_HTTPCLIENT_FLAGS_WAITFOR : 0, listener->m_stream, listener, 0, reinterpret_cast<void **>(&(listener->m_taskId))));
}

bool AimpHTTP::Download(const std::wstring &url, const std::wstring &destination, CallbackFunc callback) {
    if (!AimpHTTP::m_initialized || !Plugin::instance()->core())
        return false;

    EventListener *listener = new EventListener(callback, true);
    IAIMPServiceFileStreaming *fileStreaming = nullptr;
    if (SUCCEEDED(Plugin::instance()->core()->QueryInterface(IID_IAIMPServiceFileStreaming, reinterpret_cast<void **>(&fileStreaming)))) {
        fileStreaming->CreateStreamForFile(AIMPString(destination), AIMP_SERVICE_FILESTREAMING_FLAG_CREATENEW, -1, -1, &(listener->m_stream));
        fileStreaming->Release();
    }
    
    return SUCCEEDED(m_httpClient->Get(AIMPString(url), 0, listener->m_stream, listener, 0, reinterpret_cast<void **>(&(listener->m_taskId))));
}

bool AimpHTTP::DownloadImage(const std::wstring &url, IAIMPImageContainer **Image, int maxSize) {
    if (!AimpHTTP::m_initialized || !Plugin::instance()->core())
        return false;

    EventListener *listener = new EventListener(nullptr);
    Plugin::instance()->core()->CreateObject(IID_IAIMPMemoryStream, reinterpret_cast<void **>(&(listener->m_stream)));

    if (SUCCEEDED(Plugin::instance()->core()->CreateObject(IID_IAIMPImageContainer, reinterpret_cast<void **>(Image)))) {
        listener->m_imageContainer = Image;
        listener->m_maxSize = maxSize;

        return SUCCEEDED(m_httpClient->Get(AIMPString(url), AIMP_SERVICE_HTTPCLIENT_FLAGS_WAITFOR, listener->m_stream, listener, 0, reinterpret_cast<void **>(&(listener->m_taskId))));
    }
    return false;
}

bool AimpHTTP::Post(const std::wstring &url, const std::string &body, CallbackFunc callback, bool synchronous) {
    if (!AimpHTTP::m_initialized || !Plugin::instance()->core())
        return false;

    IAIMPStream *postData = nullptr;
    if (SUCCEEDED(Plugin::instance()->core()->CreateObject(IID_IAIMPMemoryStream, reinterpret_cast<void **>(&postData)))) {
        postData->Write((unsigned char *)(body.data()), body.size(), nullptr);

        EventListener *listener = new EventListener(callback);
        Plugin::instance()->core()->CreateObject(IID_IAIMPMemoryStream, reinterpret_cast<void **>(&(listener->m_stream)));

        bool ok = SUCCEEDED(m_httpClient->Post(AIMPString(url), synchronous ? AIMP_SERVICE_HTTPCLIENT_FLAGS_WAITFOR : 0, listener->m_stream, postData, listener, 0, reinterpret_cast<void **>(&(listener->m_taskId))));
        postData->Release();
        return ok;
    }
    return false;
}

bool AimpHTTP::Init(IAIMPCore *Core) {
    m_initialized = SUCCEEDED(Core->QueryInterface(IID_IAIMPServiceHTTPClient, reinterpret_cast<void **>(&m_httpClient)));

    return m_initialized;
}

void AimpHTTP::Deinit() {
    m_initialized = false;

    std::unordered_set<uintptr_t *> ids;
    for (auto x : m_handlers) ids.insert(x->m_taskId);
    for (auto x : ids) m_httpClient->Cancel(x, AIMP_SERVICE_HTTPCLIENT_FLAGS_WAITFOR);

    if (m_httpClient) {
        m_httpClient->Release();
        m_httpClient = nullptr;
    }
}

bool AimpHTTP::Put(const std::wstring &url, CallbackFunc callback) {
    return RawRequest("PUT", url, callback);
}

bool AimpHTTP::Delete(const std::wstring &url, CallbackFunc callback) {
    return RawRequest("DELETE", url, callback);
}

void AimpHTTP::RawRequestThread(void *args) {
    ThreadParams *params = static_cast<ThreadParams *>(args);
    std::string request = params->request;
    std::string host = params->host;
    AimpHTTP::CallbackFunc callback = params->callback;
    delete params;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        DebugA("WSAStartup failed.\n");
        return;
    }

    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *targetAdressInfo = NULL;
    DWORD getAddrRes = getaddrinfo(host.c_str(), NULL, &hints, &targetAdressInfo);
    if (getAddrRes != 0 || targetAdressInfo == NULL) {
        DebugA("Could not resolve the Host Name\n");
        WSACleanup();
        return;
    }

    SOCKADDR_IN sockAddr;
    sockAddr.sin_addr = ((struct sockaddr_in *)targetAdressInfo->ai_addr)->sin_addr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(80);
    freeaddrinfo(targetAdressInfo);

    SOCKET webSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (webSocket == INVALID_SOCKET) {
        DebugA("Creation of the Socket Failed\n");
        WSACleanup();
        return;
    }

    if (connect(webSocket, (SOCKADDR *)&sockAddr, sizeof(sockAddr)) != 0) {
        DebugA("Could not connect\n");
        closesocket(webSocket);
        WSACleanup();
        return;
    }

    const char *httpRequest = request.c_str();
    int sentBytes = send(webSocket, httpRequest, strlen(httpRequest), 0);
    if (sentBytes < (int)request.size() || sentBytes == SOCKET_ERROR) {
        DebugA("Could not send the request to the Server");
        closesocket(webSocket);
        WSACleanup();
        return;
    }

    char buffer[10240];
    ZeroMemory(buffer, sizeof(buffer));
    int dataLen;
    if ((dataLen = recv(webSocket, buffer, sizeof(buffer), 0) > 0)) {
        if (char *body = strstr(buffer, "\r\n\r\n")) {
            body += 4;
            if (callback && m_initialized && Plugin::instance()->core())
                callback(reinterpret_cast<unsigned char *>(body), strlen(body));
        }
    }

    closesocket(webSocket);
    WSACleanup();
};

bool AimpHTTP::RawRequest(const std::string &method, const std::wstring &url, CallbackFunc callback) {
    // Not perfect but does its job

    std::string narrow_url = Tools::ToString(url);
    const char *urlc = narrow_url.c_str();
    if (urlc = strstr(urlc, "://")) {
        urlc += 3;
        if (const char *request = strstr(urlc, "/")) {
            ThreadParams *p = new ThreadParams();
            p->request += method + " ";
            p->request += request;
            p->host = std::string(urlc, request);
            p->request += " HTTP/1.1\r\nHost: " + p->host + "\r\nConnection: close\r\n\r\n";
            p->callback = callback;

            HANDLE hThread = (HANDLE)_beginthread(AimpHTTP::RawRequestThread, 0, p);
            return hThread != INVALID_HANDLE_VALUE;
        }
    }
    return false;
}
