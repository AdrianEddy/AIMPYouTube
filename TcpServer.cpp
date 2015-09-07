#include "TcpServer.h"

#include <winsock2.h>
#include <process.h>
#pragma comment(lib,"ws2_32.lib")

TcpServer::TcpServer(int port, RequestFunc callback) : m_port(port), m_callback(callback), m_deleteOnFinish(true) {

}

void TcpServer::ThreadFunc(void *arg) {
    TcpServer *parent = static_cast<TcpServer *>(arg);
    if (!parent)
        return;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        OutputDebugString(L"WSAStartup failed\n");
        if (parent->m_deleteOnFinish)
            delete parent;
        return;
    }
    SOCKET s;
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        OutputDebugString(L"Could not create socket\n");
        if (parent->m_deleteOnFinish)
            delete parent;
        return;
    }

    struct sockaddr_in server, client;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(parent->m_port);

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        OutputDebugString(L"bind failed\n");
        if (parent->m_deleteOnFinish)
            delete parent;
        return;
    }

    listen(s, 3);

    int c = sizeof(struct sockaddr_in);
    do {
        SOCKET new_socket = accept(s, (struct sockaddr *)&client, &c);
        if (new_socket == INVALID_SOCKET) {
            OutputDebugString(L"accept failed\n");
            if (parent->m_deleteOnFinish)
                delete parent;
            return;
        }
        char request[2048];
        memset(request, 0, sizeof(request));

        recv(new_socket, request, sizeof(request), 0);

        std::string response;
        if (parent->m_callback(parent, request, response)) {
            if (!response.empty()) {
                send(new_socket, response.c_str(), response.size(), 0);
            }
            closesocket(new_socket);
            closesocket(s);
            WSACleanup();
            if (parent->m_deleteOnFinish) {
                delete parent;
            }
            break;
        }
    } while (true);
}

bool TcpServer::Start() {
    HANDLE hThread = (HANDLE)_beginthread(ThreadFunc, 0, this);
    return hThread != INVALID_HANDLE_VALUE;
}

TcpServer::~TcpServer() {

}
