#include "AIMPString.h"
#include <codecvt>

IAIMPCore *AIMPString::m_core = nullptr;
static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> wStrConverter;

AIMPString::AIMPString() {
    if (!m_core)
        return;

    m_core->CreateObject(IID_IAIMPString, reinterpret_cast<void **>(&m_string));
}

AIMPString::AIMPString(const std::wstring &string) {
    if (!m_core)
        return;

    if (SUCCEEDED(m_core->CreateObject(IID_IAIMPString, reinterpret_cast<void **>(&m_string)))) {
        m_string->SetData(const_cast<wchar_t *>(string.data()), string.size());
    }
}

AIMPString::AIMPString(const wchar_t *string) {
    if (!m_core)
        return;

    if (SUCCEEDED(m_core->CreateObject(IID_IAIMPString, reinterpret_cast<void **>(&m_string)))) {
        m_string->SetData(const_cast<wchar_t *>(string), wcslen(string));
    }
}

AIMPString::AIMPString(const rapidjson::Value &val) {
    if (!m_core)
        return;

    if (SUCCEEDED(m_core->CreateObject(IID_IAIMPString, reinterpret_cast<void **>(&m_string)))) {
        if (val.IsString() && val.GetStringLength() > 0) {
            const char *ptr = val.GetString();
            std::wstring str = wStrConverter.from_bytes(ptr, ptr + val.GetStringLength());
            m_string->SetData(const_cast<wchar_t *>(str.data()), str.size());
        } else {
            m_string->Release();
            m_string = nullptr;
        }
    }
}

AIMPString::~AIMPString() {
    if (m_string)
        m_string->Release();
}
