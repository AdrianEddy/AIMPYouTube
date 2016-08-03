#include "FileSystem.h"
#include "Config.h"
#include "AIMPYoutube.h"
#include "YouTubeAPI.h"
#include <algorithm>
#include <windows.h>

HRESULT WINAPI FileSystem::HTTPStream::SetSize(const INT64 Value) {
    m_size = Value;
    if (m_buffer) {
        delete[] m_buffer;
    }
    m_buffer = new unsigned char[m_size];
    return S_OK;
}
FileSystem::HTTPStream::~HTTPStream() {
    delete[] m_buffer;
}
void FileSystem::HTTPStream::Wait() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock);
}
void FileSystem::HTTPStream::Signal() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.notify_all();
}
HRESULT WINAPI FileSystem::HTTPStream::Seek(const INT64 Offset, int Mode) {
    switch (Mode) {
        case AIMP_STREAM_SEEKMODE_FROM_CURRENT:   m_position += Offset; break;
        case AIMP_STREAM_SEEKMODE_FROM_BEGINNING: m_position = Offset; break;
        case AIMP_STREAM_SEEKMODE_FROM_END:       m_position = m_size - Offset; break;
    }
    return S_OK;
}
int WINAPI FileSystem::HTTPStream::Read(unsigned char *Buffer, unsigned int Count) {
    while (m_bufferSize - m_position <= 0 && m_bufferSize < m_size)
        Wait();

    Count = min(Count, m_bufferSize - m_position);

    memcpy(Buffer, m_buffer + m_position, Count);
    m_position += Count;

    return Count;
}
HRESULT WINAPI FileSystem::HTTPStream::Write(unsigned char *Buffer, unsigned int Count, unsigned int *Written) {
    memcpy(m_buffer + m_bufferSize, Buffer, Count);
    m_bufferSize += Count;
    Signal();

    return S_OK;
}

FileSystem::EventListener::EventListener() {
    m_stream = new FileSystem::HTTPStream();
    m_stream->AddRef();
}
void WINAPI FileSystem::EventListener::OnAccept(IAIMPString *ContentType, const INT64 ContentSize, BOOL *Allow) {
    ContentType->AddRef();
    ContentType->Release();
    *Allow = true;
    m_stream->SetSize(ContentSize);
    m_stream->Signal();
}
void WINAPI FileSystem::EventListener::OnComplete(IAIMPErrorInfo *ErrorInfo, BOOL Canceled) {

}
void WINAPI FileSystem::EventListener::OnProgress(const INT64 Downloaded, const INT64 Total) {

}

FileSystem::FileSystem(IAIMPCore *core) : m_core(core) {
    m_core->QueryInterface(IID_IAIMPServiceHTTPClient, reinterpret_cast<void **>(&m_httpClient));
}

HRESULT WINAPI FileSystem::GetValueAsInt32(int PropertyID, int *Value) {
    if (PropertyID == AIMP_FILESYSTEM_PROPID_READONLY) {
        *Value = 1;
        return S_OK;
    } else {
        return E_INVALIDARG;
    }
}

HRESULT WINAPI FileSystem::GetValueAsObject(int PropertyID, REFIID IID, void **Value) {
    if (PropertyID == AIMP_FILESYSTEM_PROPID_SCHEME) {
        static AIMPString name(L"youtube");
        name->AddRef();
        *Value = name;
        return S_OK;
    } else {
        return E_INVALIDARG;
    }
}

HRESULT WINAPI FileSystem::CreateStream(IAIMPString *FileName, IAIMPStream **Stream) {
    HRESULT ret = E_FAIL;
    if (Config::TrackInfo *ti = Tools::TrackInfo(FileName)) {
        EventListener *listener = new EventListener();
        *Stream = listener->m_stream;

        std::wstring url = YouTubeAPI::GetStreamUrl(ti->Id);

        uintptr_t *taskId = nullptr;
        ret = m_httpClient->Get(AIMPString(url), 0, *Stream, listener, nullptr, reinterpret_cast<void **>(&taskId));
        if (ret == S_OK) {
            listener->m_stream->Wait();
        }
    }
    return ret;
}

HRESULT WINAPI FileSystem::Process(IAIMPString *FileName) {
    if (Config::TrackInfo *ti = Tools::TrackInfo(FileName)) {
        ShellExecute(Plugin::instance()->GetMainWindowHandle(), L"open", ti->Permalink.c_str(), NULL, NULL, SW_SHOWNORMAL);
        return S_OK;
    }
    return E_FAIL;
}

HRESULT WINAPI FileSystem::CopyToClipboard(IAIMPObjectList *Files) {
    std::string text;
    for (int i = 0, n = Files->GetCount(); i < n; ++i) {
        IAIMPString *str = nullptr;
        Files->GetObject(i, IID_IAIMPString, reinterpret_cast<void **>(&str));
        if (Config::TrackInfo *ti = Tools::TrackInfo(str)) {
            text += Tools::ToString(ti->Permalink) + "\r\n";
        }
        str->Release();
    }

    OpenClipboard(Plugin::instance()->GetMainWindowHandle());
    EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (!hg) {
        CloseClipboard();
        return E_FAIL;
    }
    memcpy(GlobalLock(hg), text.c_str(), text.size() + 1);
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
    CloseClipboard();
    GlobalFree(hg);
    return S_OK;
}
