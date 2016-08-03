#pragma once

#include "SDK/apiInternet.h"
#include "IUnknownInterfaceImpl.h"
#include <functional>
#include <set>

class AimpHTTP {
    typedef std::function<void(unsigned char *, int)> CallbackFunc;

    class EventListener : public IUnknownInterfaceImpl<IAIMPHTTPClientEvents>, IAIMPHTTPClientEvents2 {
        typedef IUnknownInterfaceImpl<IAIMPHTTPClientEvents> Base;
    public:
        EventListener(CallbackFunc callback, bool isFile = false);
        ~EventListener();

        void WINAPI OnAccept(IAIMPString *ContentType, const INT64 ContentSize, BOOL *Allow);
        void WINAPI OnComplete(IAIMPErrorInfo *ErrorInfo, BOOL Canceled);
        void WINAPI OnProgress(const INT64 Downloaded, const INT64 Total);
        void WINAPI OnAcceptHeaders(IAIMPString *Header, BOOL *Allow);


        virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID *ppvObj) {
            if (!ppvObj) return E_POINTER;

            if (riid == IID_IAIMPHTTPClientEvents) {
                *ppvObj = this;
                AddRef();
                return S_OK;
            }
            if (riid == IID_IAIMPHTTPClientEvents2) {
                *ppvObj = static_cast<IAIMPHTTPClientEvents2 *>(this);;
                AddRef();
                return S_OK;
            }

            return E_NOINTERFACE;
        }
        virtual ULONG WINAPI AddRef(void) { return Base::AddRef(); }
        virtual ULONG WINAPI Release(void) { return Base::Release(); }

    private:
        bool m_isFileStream{ false };
        CallbackFunc m_callback{ nullptr };
        IAIMPStream *m_stream{ nullptr };
        IAIMPImageContainer **m_imageContainer{ nullptr };
        int m_maxSize{ 0 };
        uintptr_t *m_taskId{ nullptr };
        friend class AimpHTTP;
    };

public:
    static bool Init(IAIMPCore *Core);
    static void Deinit();

    static bool Put(const std::wstring &url, CallbackFunc callback = nullptr);
    static bool Delete(const std::wstring &url, CallbackFunc callback = nullptr);
    static bool Get(const std::wstring &url, CallbackFunc callback, bool synchronous = false);
    static bool Download(const std::wstring &url, const std::wstring &destination, CallbackFunc callback);
    static bool DownloadImage(const std::wstring &url, IAIMPImageContainer **Image, int maxSize = 0);
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

    static bool m_initialized;
    static IAIMPServiceHTTPClient *m_httpClient;

    static std::set<EventListener *> m_handlers;
};
