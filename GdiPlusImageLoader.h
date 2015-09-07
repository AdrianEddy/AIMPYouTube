#pragma once

#include <gdiplus.h>

extern HINSTANCE g_hInst;

class GdiPlusImageLoader {
public:
    GdiPlusImageLoader(UINT id, LPCTSTR pType = RT_RCDATA) : m_hBuffer(nullptr), m_gdiBitmap(nullptr) {
        if (HRSRC hResource = ::FindResource(g_hInst, MAKEINTRESOURCE(id), pType)) {
            if (DWORD imageSize = ::SizeofResource(g_hInst, hResource)) {
                if (const void *pResourceData = ::LockResource(::LoadResource(g_hInst, hResource))) {
                    if (m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize)) {
                        if (void *pBuffer = ::GlobalLock(m_hBuffer)) {
                            CopyMemory(pBuffer, pResourceData, imageSize);

                            IStream *pStream = nullptr;
                            if (SUCCEEDED(::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream))) {
                                m_gdiBitmap = Gdiplus::Bitmap::FromStream(pStream);
                                pStream->Release();
                                if (m_gdiBitmap) {
                                    if (m_gdiBitmap->GetLastStatus() == Gdiplus::Ok)
                                        return;

                                    delete m_gdiBitmap;
                                    m_gdiBitmap = nullptr;
                                }
                            }
                            ::GlobalUnlock(m_hBuffer);
                        }
                        ::GlobalFree(m_hBuffer);
                        m_hBuffer = nullptr;
                    }
                }
            }
        }
    }

    GdiPlusImageLoader(const std::wstring &path) : m_hBuffer(nullptr), m_gdiBitmap(nullptr) {
        m_gdiBitmap = Gdiplus::Bitmap::FromFile(path.c_str());
        if (m_gdiBitmap->GetLastStatus() != Gdiplus::Ok) {
            delete m_gdiBitmap;
            m_gdiBitmap = nullptr;
        }
    }

    ~GdiPlusImageLoader() {
        delete m_gdiBitmap;
        m_gdiBitmap = nullptr;
        if (m_hBuffer) {
            ::GlobalUnlock(m_hBuffer);
            ::GlobalFree(m_hBuffer);
            m_hBuffer = nullptr;
        }
    }
    operator Gdiplus::Bitmap *() { return m_gdiBitmap; }
    Gdiplus::Bitmap *operator->() { return m_gdiBitmap; }

private:
    HGLOBAL m_hBuffer;
    Gdiplus::Bitmap *m_gdiBitmap;
};
