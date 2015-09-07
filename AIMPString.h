#pragma once

#include "IUnknownInterfaceImpl.h"
#include <string>

#include "SDK/apiCore.h"
#include "SDK/apiObjects.h"
#include "rapidjson/document.h"

class AIMPString {
public:
    AIMPString();
    AIMPString(const std::wstring &string);
    AIMPString(const wchar_t *string);
    AIMPString(const rapidjson::Value &val);
    ~AIMPString();

    operator IAIMPString *() { return m_string; }
    IAIMPString *operator ->() { return m_string; }

    static inline void Init(IAIMPCore *core) { m_core = core; }

private:
    static IAIMPCore *m_core;
    IAIMPString *m_string;
};
