#pragma once

#include <functional>

class TcpServer {
    typedef std::function<bool(TcpServer *, char *, std::string &)> RequestFunc;

public:
    TcpServer(int port, RequestFunc callback);
    ~TcpServer();

    bool Start();

    inline void setDeleteOnFinish(bool v) { m_deleteOnFinish = v; }

private:
    static void ThreadFunc(void *arg);

    bool m_deleteOnFinish;
    int m_port;
    RequestFunc m_callback;
};

