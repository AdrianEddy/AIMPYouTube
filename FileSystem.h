#pragma once

#include "SDK/apiInternet.h"
#include "SDK/apiFileManager.h"
#include "AIMPString.h"
#include "Tools.h"
#include "IUnknownInterfaceImpl.h"
#include <mutex>
#include <condition_variable>

class FileSystem : public IUnknownInterfaceImpl<IAIMPExtensionFileSystem>, 
                   public IAIMPFileSystemCommandDropSource, 
                   public IAIMPFileSystemCommandOpenFileFolder, 
                   public IAIMPFileSystemCommandCopyToClipboard {
public:
    typedef IUnknownInterfaceImpl<IAIMPExtensionFileSystem> Base;

    class HTTPStream : public IUnknownInterfaceImpl<IAIMPStream> {
    public:
        ~HTTPStream();
        virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID *ppvObj) {
            if (!ppvObj) return E_POINTER;
            if (riid == IID_IAIMPStream) {
                *ppvObj = this;
                AddRef();
                return S_OK;
            }
            return E_NOINTERFACE;
        }
        virtual INT64 WINAPI GetPosition() { return m_position; }
        virtual INT64 WINAPI GetSize() { return m_size; }

        virtual HRESULT WINAPI SetSize(const INT64 Value);

        virtual HRESULT WINAPI Seek(const INT64 Offset, int Mode);
        virtual int WINAPI Read(unsigned char *Buffer, unsigned int Count);
        virtual HRESULT WINAPI Write(unsigned char *Buffer, unsigned int Count, unsigned int *Written);

        void Wait();
        void Signal();

    private:
        std::condition_variable m_cv;
        std::mutex m_mutex;
        unsigned char *m_buffer{nullptr};

        INT64 m_position{0};
        INT64 m_bufferSize{0};
        INT64 m_size{0};
    };

    class EventListener : public IUnknownInterfaceImpl<IAIMPHTTPClientEvents> {
    public:
        EventListener();

        void WINAPI OnAccept(IAIMPString *ContentType, const INT64 ContentSize, BOOL *Allow);
        void WINAPI OnComplete(IAIMPErrorInfo *ErrorInfo, BOOL Canceled);
        void WINAPI OnProgress(const INT64 Downloaded, const INT64 Total);

    private:
        HTTPStream *m_stream;
        friend class FileSystem;
    };

    FileSystem(IAIMPCore *core);
    ~FileSystem() {
        if (m_httpClient)
            m_httpClient->Release();
    }

    virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppvObj) {
        if (!ppvObj) return E_POINTER;
        if (riid == IID_IAIMPExtensionFileSystem) {
            *ppvObj = static_cast<IAIMPExtensionFileSystem *>(this);
            AddRef();
            return S_OK;
        }
        if (riid == IID_IAIMPFileSystemCommandDropSource) {
            *ppvObj = static_cast<IAIMPFileSystemCommandDropSource *>(this);
            AddRef();
            return S_OK;
        }
        if (riid == IID_IAIMPFileSystemCommandOpenFileFolder) {
            *ppvObj = static_cast<IAIMPFileSystemCommandOpenFileFolder *>(this);
            AddRef();
            return S_OK;
        }
        if (riid == IID_IAIMPFileSystemCommandCopyToClipboard) {
            *ppvObj = static_cast<IAIMPFileSystemCommandCopyToClipboard *>(this);
            AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }
    virtual HRESULT WINAPI GetValueAsInt32(int PropertyID, int *Value);
    virtual HRESULT WINAPI GetValueAsObject(int PropertyID, REFIID IID, void **Value);

    virtual void WINAPI BeginUpdate() { }
    virtual void WINAPI EndUpdate() { }
    virtual HRESULT WINAPI Reset() { return E_NOTIMPL; }
    virtual HRESULT WINAPI GetValueAsFloat(int PropertyID, double *Value) { return E_NOTIMPL;  }
    virtual HRESULT WINAPI GetValueAsInt64(int PropertyID, INT64 *Value) { return E_NOTIMPL; }
    virtual HRESULT WINAPI SetValueAsFloat(int PropertyID, const double Value) { return E_NOTIMPL; }
    virtual HRESULT WINAPI SetValueAsInt32(int PropertyID, int Value) { return E_NOTIMPL; }
    virtual HRESULT WINAPI SetValueAsInt64(int PropertyID, const INT64 Value) { return E_NOTIMPL; }
    virtual HRESULT WINAPI SetValueAsObject(int PropertyID, IUnknown *Value) { return E_NOTIMPL; }

    virtual ULONG WINAPI AddRef(void) { return Base::AddRef(); }
    virtual ULONG WINAPI Release(void) { return Base::Release(); }

    virtual HRESULT WINAPI CreateStream(IAIMPString *FileName, IAIMPStream **Stream);

    virtual HRESULT WINAPI IAIMPFileSystemCommandOpenFileFolder::CanProcess(IAIMPString *FileName) {
        return S_OK;
    }
    virtual HRESULT WINAPI IAIMPFileSystemCommandOpenFileFolder::Process(IAIMPString *FileName);
    virtual HRESULT WINAPI CopyToClipboard(IAIMPObjectList *Files);

private:
    IAIMPCore *m_core{nullptr};
    IAIMPServiceHTTPClient *m_httpClient{nullptr};
};
