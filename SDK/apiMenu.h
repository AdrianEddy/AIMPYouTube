/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v4.00 build 1690               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2016                 */
/*                 www.aimp.ru                  */
/*              ICQ: 345-908-513                */
/*            Mail: support@aimp.ru             */
/*                                              */
/************************************************/

#ifndef apiMenuH
#define apiMenuH

#include <windows.h>
#include <unknwn.h>
#include "apiActions.h"
#include "apiObjects.h"

static const GUID IID_IAIMPMenuItem = {0x41494D50, 0x4D65, 0x6E75, 0x49, 0x74, 0x65, 0x6D, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPServiceMenuManager = {0x41494D50, 0x5372, 0x764D, 0x65, 0x6E, 0x75, 0x4D, 0x6E, 0x67, 0x72, 0x00};

// IAIMPMenuItem Properties
const int AIMP_MENUITEM_PROPID_CUSTOM       = 0;
const int AIMP_MENUITEM_PROPID_ACTION       = 1;
const int AIMP_MENUITEM_PROPID_ID           = 2;
const int AIMP_MENUITEM_PROPID_NAME         = 3;
const int AIMP_MENUITEM_PROPID_ENABLED      = 4;
const int AIMP_MENUITEM_PROPID_STYLE        = 5; // One of the AIMP_MENUITEM_STYLE_XXX
const int AIMP_MENUITEM_PROPID_EVENT        = 6;
const int AIMP_MENUITEM_PROPID_EVENT_ONSHOW = 7;
const int AIMP_MENUITEM_PROPID_GLYPH        = 10;
const int AIMP_MENUITEM_PROPID_PARENT       = 11;
const int AIMP_MENUITEM_PROPID_VISIBLE      = 12;
const int AIMP_MENUITEM_PROPID_CHECKED		= 13;
const int AIMP_MENUITEM_PROPID_DEFAULT      = 14;
const int AIMP_MENUITEM_PROPID_SHORTCUT     = 15;

// Styles for the AIMP_MENUITEM_PROPID_STYLE property
const int AIMP_MENUITEM_STYLE_NORMAL   = 0;
const int AIMP_MENUITEM_STYLE_CHECKBOX = 1;
const int AIMP_MENUITEM_STYLE_RADIOBOX = 2;

// Built-in menu ids
const int AIMP_MENUID_COMMON_UTILITIES                  = 0;
const int AIMP_MENUID_PLAYER_MAIN_FUNCTIONS             = 10;
const int AIMP_MENUID_PLAYER_MAIN_OPEN                  = 11;
const int AIMP_MENUID_PLAYER_MAIN_OPTIONS               = 12;
const int AIMP_MENUID_PLAYER_PLAYLIST_ADDING            = 20;
const int AIMP_MENUID_PLAYER_PLAYLIST_DELETION          = 21;
const int AIMP_MENUID_PLAYER_PLAYLIST_SORTING           = 22;
const int AIMP_MENUID_PLAYER_PLAYLIST_MISCELLANEOUS     = 23;
const int AIMP_MENUID_PLAYER_PLAYLIST_MANAGE            = 24;
const int AIMP_MENUID_PLAYER_PLAYLIST_CONTEXT_ADDING    = 30;
const int AIMP_MENUID_PLAYER_PLAYLIST_CONTEXT_QUEUE     = 31;
const int AIMP_MENUID_PLAYER_PLAYLIST_CONTEXT_FUNCTIONS = 32;
const int AIMP_MENUID_PLAYER_PLAYLIST_CONTEXT_SENDING   = 33;
const int AIMP_MENUID_PLAYER_PLAYLIST_CONTEXT_DELETION  = 34;  
const int AIMP_MENUID_PLAYER_TRAY                       = 40;
const int AIMP_MENUID_PLAYER_EQ_LIB                     = 41;
const int AIMP_MENUID_PLAYER_STOP_OPTIONS               = 42; // v4.00.1690
const int AIMP_MENUID_ML_MISCELLANEOUS                  = 50; // v4.10
const int AIMP_MENUID_ML_DELETION                       = 51; // v4.10
const int AIMP_MENUID_ML_MAIN_DB                        = 60; // v4.10
const int AIMP_MENUID_ML_MAIN_FUNCTIONS                 = 61; // v4.10
const int AIMP_MENUID_ML_MAIN_OPEN                      = 62; // v4.10
const int AIMP_MENUID_ML_MAIN_OPTIONS                   = 63; // v4.10
const int AIMP_MENUID_ML_TABLE_CONTEXT_ADDING           = 70; // v4.10
const int AIMP_MENUID_ML_TABLE_CONTEXT_FUNCTIONS        = 71; // v4.10
const int AIMP_MENUID_ML_TABLE_CONTEXT_SENDING          = 72; // v4.10
const int AIMP_MENUID_ML_TABLE_CONTEXT_DELETION         = 73; // v4.10
const int AIMP_MENUID_ML_TREE_CONTEXT_FUNCTIONS         = 80; // v4.10
const int AIMP_MENUID_ML_TREE_CONTEXT_DELETION          = 81; // v4.10

/* IAIMPMenuItem */

class IAIMPMenuItem: public IAIMPPropertyList
{
	public:
		virtual HRESULT WINAPI DeleteChildren() = 0;
};

/* IAIMPServiceMenuManager */

class IAIMPServiceMenuManager: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetBuiltIn(int ID, IAIMPMenuItem **MenuItem) = 0;
		virtual HRESULT WINAPI GetByID(IAIMPString *ID, IAIMPMenuItem **MenuItem) = 0;
};

#endif // !apiMenuH