#pragma once

#include "SDK/apiInternet.h"
#include "IUnknownInterfaceImpl.h"
#include <functional>

class AimpHTTP {
    typedef std::function<void(unsigned char *, int)> CallbackFunc;

    class EventListener : public IUnknownInterfaceImpl<IAIMPHTTPClientEvents> {
    public:
        EventListener(CallbackFunc callback, bool isFile = false);

        void WINAPI OnAccept(IAIMPString *ContentType, const INT64 ContentSize, BOOL *Allow);
        void WINAPI OnComplete(IAIMPErrorInfo *ErrorInfo, BOOL Canceled);
        void WINAPI OnProgress(const INT64 Downloaded, const INT64 Total);

    private:
        bool m_isFileStream;
        CallbackFunc m_callback;
        IAIMPStream *m_stream;
        friend class AimpHTTP;
    };

public:
    static bool Init(IAIMPCore *Core);
    static void Deinit();

    static bool Put(const std::wstring &url, CallbackFunc callback = nullptr);
    static bool Delete(const std::wstring &url, CallbackFunc callback = nullptr);
    static bool Get(const std::wstring &url, CallbackFunc callback, bool synchronous = false);
    static bool Download(const std::wstring &url, const std::wstring &destination, CallbackFunc callback);
    static bool Post(const std::wstring &url, const std::string &body, CallbackFunc callback, bool synchronous = false);

private:
    struct ThreadParams {
        std::string request;
        std::string host;
        CallbackFunc callback;
    };
    static void RawRequestThread(void *args);
    static bool RawRequest(const std::string &method, const std::wstring &, CallbackFunc callback);

    AimpHTTP();
    AimpHTTP(const AimpHTTP&);
    AimpHTTP& operator=(const AimpHTTP&);

    static IAIMPCore *m_core;
    static IAIMPServiceHTTPClient *m_httpClient;
};
