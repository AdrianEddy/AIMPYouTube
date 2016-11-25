#include "AimpMenu.h"

#include "AIMPString.h"
#include "SDK/apiMenu.h"
#include <functional>

IAIMPCore *AimpMenu::m_core = nullptr;
IAIMPServiceMenuManager *AimpMenu::m_menuManager = nullptr;

extern HINSTANCE g_hInst;

AimpMenu::AimpMenu(IAIMPMenuItem *item) : m_menuItem(item) {

}

AimpMenu::~AimpMenu() {
    if (m_menuItem)
        m_menuItem->Release();
}

IAIMPMenuItem *AimpMenu::Add(const std::wstring &name, CallbackFunc action, UINT icon, CallbackFunc showAction, const std::wstring &id) {
    IAIMPMenuItem *newItem = nullptr;
    if (SUCCEEDED(m_core->CreateObject(IID_IAIMPMenuItem, reinterpret_cast<void **>(&newItem)))) {
        newItem->SetValueAsObject(AIMP_MENUITEM_PROPID_PARENT, m_menuItem);
        newItem->SetValueAsObject(AIMP_MENUITEM_PROPID_ID, AIMPString(L"AIMPYouTube" + (id.empty() ? name : id)));

        if (action) {
            IAIMPServiceActionManager *manager = nullptr;
            IAIMPAction *newAction = nullptr;

            AIMPString actionName;
            if (m_menuItem) {
                IAIMPString *parentName;
                if (SUCCEEDED(m_menuItem->GetValueAsObject(AIMP_MENUITEM_PROPID_NAME, IID_IAIMPString, reinterpret_cast<void **>(&parentName)))) {
                    actionName->Add(parentName);
                    parentName->Release();
                    actionName->Add2(L" -> ", 4);
                }
            }
            actionName->Add(AIMPString(name));

            if (SUCCEEDED(m_core->QueryInterface(IID_IAIMPServiceActionManager, reinterpret_cast<void **>(&manager)))) {
                AIMPString actionID(L"AIMPYouTubeAction" + (id.empty() ? name : id));
                actionID->Add(actionName);
                manager->GetByID(actionID, &newAction);
                if (newAction) {
                    newItem->SetValueAsObject(AIMP_MENUITEM_PROPID_ACTION, newAction);
                    newItem->SetValueAsObject(AIMP_MENUITEM_PROPID_NAME, AIMPString(name));
                } else {
                    if (SUCCEEDED(m_core->CreateObject(IID_IAIMPAction, reinterpret_cast<void **>(&newAction)))) {
                        newAction->SetValueAsObject(AIMP_ACTION_PROPID_ID, actionID);
                        newAction->SetValueAsObject(AIMP_ACTION_PROPID_GROUPNAME, AIMPString(L"YouTube"));

                        newAction->SetValueAsObject(AIMP_ACTION_PROPID_NAME, actionName);
                        newAction->SetValueAsObject(AIMP_ACTION_PROPID_EVENT, new ClickHandler(action, newItem));
                        newAction->SetValueAsInt32(AIMP_ACTION_PROPID_ENABLED, true);

                        m_core->RegisterExtension(IID_IAIMPServiceActionManager, newAction);
                        newItem->SetValueAsObject(AIMP_MENUITEM_PROPID_ACTION, newAction);
                        newItem->SetValueAsObject(AIMP_MENUITEM_PROPID_NAME, AIMPString(name));

                        newAction->Release();
                    }
                }
            }
        } else {
            newItem->SetValueAsObject(AIMP_MENUITEM_PROPID_ID, AIMPString(L"AIMPYouTube" + (id.empty() ? name : id)));
            newItem->SetValueAsObject(AIMP_MENUITEM_PROPID_NAME, AIMPString(name));
            newItem->SetValueAsInt32(AIMP_MENUITEM_PROPID_ENABLED, true);
        }

        if (showAction) {
            newItem->SetValueAsObject(AIMP_MENUITEM_PROPID_EVENT_ONSHOW, new ClickHandler(showAction, newItem));
        }

        if (icon > 0) {
            SetIcon(newItem, icon);
        }

        newItem->SetValueAsInt32(AIMP_MENUITEM_PROPID_STYLE, AIMP_MENUITEM_STYLE_NORMAL);
        newItem->SetValueAsInt32(AIMP_MENUITEM_PROPID_VISIBLE, true);

        m_core->RegisterExtension(IID_IAIMPServiceMenuManager, newItem);
        return newItem;
    }
    return nullptr;
}

bool AimpMenu::SetIcon(IAIMPMenuItem *item, UINT icon) {
    bool ok = false;
    if (icon <= 0) {
        item->SetValueAsObject(AIMP_MENUITEM_PROPID_GLYPH, nullptr);
        return true;
    }

    IAIMPImage *img = nullptr;
    if (SUCCEEDED(m_core->CreateObject(IID_IAIMPImage, reinterpret_cast<void **>(&img)))) {
        IAIMPStream *imgStream = nullptr;
        if (SUCCEEDED(m_core->CreateObject(IID_IAIMPMemoryStream, reinterpret_cast<void **>(&imgStream)))) {
            if (HRSRC hResource = ::FindResource(g_hInst, MAKEINTRESOURCE(icon), L"PNG")) {
                if (DWORD imageSize = ::SizeofResource(g_hInst, hResource)) {
                    if (void *pResourceData = ::LockResource(::LoadResource(g_hInst, hResource))) {
                        unsigned int written = 0;
                        if (SUCCEEDED(imgStream->Write(reinterpret_cast<unsigned char *>(pResourceData), imageSize, &written)) && written > 0) {
                            if (SUCCEEDED(img->LoadFromStream(imgStream))) {
                                item->SetValueAsObject(AIMP_MENUITEM_PROPID_GLYPH, img);
                                ok = true;
                            }
                            imgStream->Release();
                        }
                    }
                }
            }
        }
        img->Release();
    }
    return ok;
}

AimpMenu *AimpMenu::Get(int id) {
    IAIMPMenuItem *menuItem = nullptr;
    if (SUCCEEDED(m_menuManager->GetBuiltIn(id, &menuItem))) {
        return new AimpMenu(menuItem);
    }
    return nullptr;
}

AimpMenu *AimpMenu::Get(const std::wstring &id) {
    IAIMPMenuItem *menuItem = nullptr;
    if (SUCCEEDED(m_menuManager->GetByID(AIMPString(L"AIMPYouTube" + id), &menuItem))) {
        return new AimpMenu(menuItem);
    }
    return nullptr;
}

void AimpMenu::Clear() {
    if (m_menuItem) {
        m_menuItem->DeleteChildren();
    }
}

bool AimpMenu::Init(IAIMPCore *Core) {
    m_core = Core;

    return SUCCEEDED(m_core->QueryInterface(IID_IAIMPServiceMenuManager, reinterpret_cast<void **>(&m_menuManager)));
}

void AimpMenu::Deinit() {
    if (m_menuManager)
        m_menuManager->Release();
}
