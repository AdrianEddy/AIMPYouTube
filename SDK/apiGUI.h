/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v4.00 build 1660               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2015                 */
/*                 www.aimp.ru                  */
/*                                              */
/*            Mail: support@aimp.ru             */
/*                                              */
/************************************************/

#ifndef apiGUIH
#define apiGUIH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"
#include "apiMenu.h"

static const GUID IID_IAIMPServiceUI = {0x41494D50, 0x5365, 0x7276, 0x69, 0x63, 0x65, 0x55, 0x49, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIBaseButtonnedEdit = {0x61756942, 0x6173, 0x6542, 0x74, 0x6E, 0x45, 0x64, 0x69, 0x74, 0x00, 0x00};
static const GUID IID_IAIMPUIBaseComboBox = {0x61756942, 0x6173, 0x6543, 0x6F, 0x6D, 0x62, 0x6F, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIBaseEdit = {0x61756942, 0x6173, 0x6545, 0x64, 0x69, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIBBCBox = {0x61756942, 0x4243, 0x426F, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIBevel = {0x61756942, 0x6576, 0x656C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIBrandBox = {0x61756942, 0x7261, 0x6E64, 0x42, 0x6F, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIBrowseFolderDialog = {0x61756942, 0x7277, 0x7346, 0x6C, 0x64, 0x72, 0x44, 0x6C, 0x67, 0x00, 0x00};
static const GUID IID_IAIMPUIButton = {0x61756942, 0x7574, 0x746F, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUICategory = {0x61756943, 0x6174, 0x6567, 0x6F, 0x72, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIChangeEvents = {0x61756945, 0x766E, 0x7443, 0x68, 0x61, 0x6E, 0x67, 0x65, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUICheckBox = {0x61756943, 0x6865, 0x636B, 0x42, 0x6F, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUICheckComboBox = {0x61756943, 0x6865, 0x636B, 0x65, 0x64, 0x43, 0x6F, 0x6D, 0x62, 0x6F, 0x00};
static const GUID IID_IAIMPUIComboBox = {0x61756943, 0x6F6D, 0x626F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIControl = {0x61756943, 0x6F6E, 0x7472, 0x6F, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIDrawEvents = {0x61756945, 0x766E, 0x7444, 0x72, 0x61, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIEdit = {0x61756945, 0x6469, 0x7400, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIEditButton = {0x61756945, 0x6469, 0x7442, 0x74, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIFileDialogs = {0x61756946, 0x696C, 0x6544, 0x6C, 0x67, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIForm = {0x61756946, 0x6F72, 0x6D00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIFormEvents = {0x61756946, 0x6F72, 0x6D45, 0x76, 0x65, 0x6E, 0x74, 0x73, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIGroupBox = {0x61756947, 0x726F, 0x7570, 0x42, 0x6F, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIImage = {0x61756949, 0x6D61, 0x6765, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIImageComboBox = {0x61756949, 0x6D61, 0x6765, 0x43, 0x6F, 0x6D, 0x62, 0x6F, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIImageList = {0x61756949, 0x6D67, 0x4C69, 0x73, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIInputDialog = {0x61756949, 0x6E70, 0x7574, 0x44, 0x6C, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIInputDialogEvents = {0x61756949, 0x6E70, 0x7574, 0x44, 0x6C, 0x67, 0x45, 0x76, 0x6E, 0x74, 0x00};
static const GUID IID_IAIMPUIKeyboardEvents = {0x61756945, 0x766E, 0x744B, 0x65, 0x79, 0x62, 0x6F, 0x61, 0x72, 0x64, 0x00};
static const GUID IID_IAIMPUILabel = {0x6175694C, 0x6162, 0x656C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIMemo = {0x6175694D, 0x656D, 0x6F00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIMenuItem = {0x6175694D, 0x656E, 0x7549, 0x74, 0x65, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIMessageDialog = {0x6175694D, 0x7367, 0x446C, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIMouseEvents = {0x61756945, 0x766E, 0x744D, 0x6F, 0x75, 0x73, 0x65, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIMouseWheelEvents = {0x61756945, 0x766E, 0x744D, 0x6F, 0x75, 0x73, 0x65, 0x57, 0x68, 0x6C, 0x00};
static const GUID IID_IAIMPUIPageControl = {0x61756950, 0x6167, 0x6543, 0x74, 0x72, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIPageControlEvents = {0x61756950, 0x6167, 0x6543, 0x74, 0x72, 0x6C, 0x45, 0x76, 0x6E, 0x74, 0x00};
static const GUID IID_IAIMPUIPaintBox = {0x61756950, 0x6169, 0x6E74, 0x42, 0x6F, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIPanel = {0x61756950, 0x616E, 0x656C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIPlacementEvents = {0x61756945, 0x766E, 0x7442, 0x6F, 0x75, 0x6E, 0x64, 0x73, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIPopupMenu = {0x61756950, 0x6F70, 0x7570, 0x4D, 0x65, 0x6E, 0x75, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIPopupMenuEvents = {0x61756945, 0x766E, 0x7450, 0x6F, 0x70, 0x75, 0x70, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIProgressBar = {0x61756950, 0x726F, 0x6772, 0x65, 0x73, 0x73, 0x42, 0x61, 0x72, 0x00, 0x00};
static const GUID IID_IAIMPUIProgressDialog = {0x61756950, 0x726F, 0x6772, 0x65, 0x73, 0x73, 0x44, 0x6C, 0x67, 0x00, 0x00};
static const GUID IID_IAIMPUIProgressDialogEvents = {0x61756950, 0x7267, 0x7273, 0x44, 0x6C, 0x67, 0x45, 0x76, 0x6E, 0x74, 0x00};
static const GUID IID_IAIMPUIRadioBox = {0x61756952, 0x6164, 0x696F, 0x42, 0x6F, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIScrollBox = {0x61756953, 0x6372, 0x6F6C, 0x6C, 0x42, 0x6F, 0x78, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUISlider = {0x61756953, 0x6C69, 0x6465, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUISpinEdit = {0x61756953, 0x7069, 0x6E45, 0x64, 0x69, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUISplitter = {0x61756953, 0x706C, 0x6974, 0x74, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUITabControl = {0x61756954, 0x6162, 0x4374, 0x72, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUITabControlEvents = {0x61756954, 0x6162, 0x4374, 0x72, 0x6C, 0x45, 0x76, 0x6E, 0x74, 0x73, 0x00};
static const GUID IID_IAIMPUITabSheet = {0x61756954, 0x6162, 0x5368, 0x65, 0x65, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUITimeEdit = {0x61756954, 0x696D, 0x6545, 0x64, 0x69, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUITreeList = {0x61756954, 0x4C00, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUITreeListColumn = {0x61756954, 0x4C43, 0x6F6C, 0x75, 0x6D, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUITreeListCustomDrawEvents = {0x61756954, 0x4C44, 0x7261, 0x77, 0x45, 0x76, 0x6E, 0x74, 0x73, 0x00, 0x00};
static const GUID IID_IAIMPUITreeListDragSortingEvents = {0x6169544C, 0x4472, 0x6167, 0x53, 0x6F, 0x72, 0x74, 0x45, 0x76, 0x6E, 0x74};
static const GUID IID_IAIMPUITreeListEvents = {0x61756954, 0x4C45, 0x7665, 0x6E, 0x74, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUITreeListGroup = {0x61756954, 0x4C47, 0x726F, 0x75, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUITreeListInplaceEditingEvents = {0x61756954, 0x4C45, 0x6474, 0x45, 0x76, 0x6E, 0x74, 0x73, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUITreeListNode = {0x61756954, 0x4C4E, 0x6F64, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIValidationLabel = {0x61756956, 0x616C, 0x6964, 0x4C, 0x61, 0x62, 0x65, 0x6C, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIWinControl = {0x61756957, 0x696E, 0x4374, 0x72, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPUIWndProcEvents = {0x61756957, 0x6E64, 0x5072, 0x6F, 0x63, 0x45, 0x76, 0x6E, 0x74, 0x73, 0x00};

/*----------------------------------------------------------------------------------------------------------------------*/
/* Flags																												   */
/*----------------------------------------------------------------------------------------------------------------------*/

  // Modifiers Flags
  const int AIMPUI_FLAGS_MOD_ALT    = 1;
  const int AIMPUI_FLAGS_MOD_CTRL   = 2;
  const int AIMPUI_FLAGS_MOD_SHIFT  = 4;

  // Borders Flags
  const int AIMPUI_FLAGS_BORDER_LEFT   = 1;
  const int AIMPUI_FLAGS_BORDER_TOP    = 2;
  const int AIMPUI_FLAGS_BORDER_RIGHT  = 4;
  const int AIMPUI_FLAGS_BORDER_BOTTOM = 8;

  // Font Style Flags
  const int AIMPUI_FLAGS_FONT_BOLD      = 1;
  const int AIMPUI_FLAGS_FONT_ITALIC    = 2;
  const int AIMPUI_FLAGS_FONT_UNDERLINE = 4;
  const int AIMPUI_FLAGS_FONT_STRIKEOUT = 8;

  // Modal Result Flags
  const int AIMPUI_FLAGS_MODALRESULT_NONE     = 0;
  const int AIMPUI_FLAGS_MODALRESULT_OK       = 1;
  const int AIMPUI_FLAGS_MODALRESULT_CANCEL   = 2;
  const int AIMPUI_FLAGS_MODALRESULT_ABORT    = 3;
  const int AIMPUI_FLAGS_MODALRESULT_RETRY    = 4;
  const int AIMPUI_FLAGS_MODALRESULT_IGNORE   = 5;
  const int AIMPUI_FLAGS_MODALRESULT_YES      = 6;
  const int AIMPUI_FLAGS_MODALRESULT_NO       = 7;
  const int AIMPUI_FLAGS_MODALRESULT_CLOSE    = 8;
  const int AIMPUI_FLAGS_MODALRESULT_HELP     = 9;
  const int AIMPUI_FLAGS_MODALRESULT_TRYAGAIN = 10;
  const int AIMPUI_FLAGS_MODALRESULT_CONTINUE = 11;

  // Edit Mask Flags
  const int AIMPUI_FLAGS_EDITMASK_TEXT    = 0;
  const int AIMPUI_FLAGS_EDITMASK_INTEGER = 1;
  const int AIMPUI_FLAGS_EDITMASK_FLOAT   = 2;

  // Form's BorderStyle
  const int AIMPUI_FLAGS_BORDERSTYLE_SIZEABLE          = 0;
  const int AIMPUI_FLAGS_BORDERSTYLE_SINGLE            = 1;
  const int AIMPUI_FLAGS_BORDERSTYLE_DIALOG            = 2;
  const int AIMPUI_FLAGS_BORDERSTYLE_TOOLWINDOW        = 3;
  const int AIMPUI_FLAGS_BORDERSTYLE_TOOLWINDOWSIZABLE = 4;
  const int AIMPUI_FLAGS_BORDERSTYLE_NONE              = 5;

  // Form's BorderIcons
  const int AIMPUI_FLAGS_BORDERICON_SYSTEMMENU         = 1;
  const int AIMPUI_FLAGS_BORDERICON_MINIMIZE           = 2;
  const int AIMPUI_FLAGS_BORDERICON_MAXIMIZE           = 4;

  // Flags for IAIMPUIBrowseFolderDialog
  const int AIMPUI_FLAGS_BROWSEFOLDER_CUSTOMPATHS = 1;
  const int AIMPUI_FLAGS_BROWSEFOLDER_MULTISELECT = 2;

  // Button Styles
  const int AIMPUI_FLAGS_BUTTON_STYLE_NORMAL          = 0;
  const int AIMPUI_FLAGS_BUTTON_STYLE_DROPDOWN        = 1;
  const int AIMPUI_FLAGS_BUTTON_STYLE_DROPDOWNBUTTON  = 2;

  // Flags for IAIMPUITreeListNode.Get
  const int AIMPUI_FLAGS_TL_NODE_GET_PARENT      = -1;
  const int AIMPUI_FLAGS_TL_NODE_GET_NEXTSIBLING = -2;
  const int AIMPUI_FLAGS_TL_NODE_GET_PREVSIBLING = -3;

  // Flags for IAIMPUITreeList.SortBy
  const int AIMPUI_FLAGS_TL_SORTBY_FLAG_AUTO       = 0;
  const int AIMPUI_FLAGS_TL_SORTBY_FLAG_ASCENDING  = 1;
  const int AIMPUI_FLAGS_TL_SORTBY_FLAG_DESCENDING = 2;

  // Flags for InsertTo for IAIMPUITreeListDragDropEvents
  const int AIMPUI_FLAGS_TL_INSERTTO_AFTER  = 0;
  const int AIMPUI_FLAGS_TL_INSERTTO_BEFORE = 1;
  const int AIMPUI_FLAGS_TL_INSERTTO_INTO   = 2;

  // Flags for AIMPUI_TL_PROPID_GRID_LINES property
  const int AIMPUI_FLAGS_TL_GRIDLINE_VERTICAL   = 1;
  const int AIMPUI_FLAGS_TL_GRIDLINE_HORIZONTAL = 2;

/*----------------------------------------------------------------------------------------------------------------------*/
/* Property IDs																											   */
/*----------------------------------------------------------------------------------------------------------------------*/

  // PropID for IAIMPUIControl
  const int AIMPUI_CONTROL_PROPID_CUSTOM     = 0;
  const int AIMPUI_CONTROL_PROPID_ENABLED    = 1;
  const int AIMPUI_CONTROL_PROPID_HINT       = 2;
  const int AIMPUI_CONTROL_PROPID_NAME       = 3;
  const int AIMPUI_CONTROL_PROPID_PARENT     = 4;
  const int AIMPUI_CONTROL_PROPID_POPUPMENU  = 5;
  const int AIMPUI_CONTROL_PROPID_VISIBLE    = 6;
  const int AIMPUI_CONTROL_MAX_PROPID        = 20;

  // PropID for IAIMPUIWinControl
  const int AIMPUI_WINCONTROL_PROPID_FOCUSED  = AIMPUI_CONTROL_MAX_PROPID + 1;
  const int AIMPUI_WINCONTROL_PROPID_TABORDER = AIMPUI_CONTROL_MAX_PROPID + 2;
  const int AIMPUI_WINCONTROL_MAX_PROPID      = AIMPUI_CONTROL_MAX_PROPID + 10;

  // PropID for IAIMPUIBaseEdit
  const int AIMPUI_BASEEDIT_PROPID_BORDERS   = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_BASEEDIT_PROPID_MAXLENGTH = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_BASEEDIT_PROPID_READONLY  = AIMPUI_WINCONTROL_MAX_PROPID + 3;
  const int AIMPUI_BASEEDIT_PROPID_SELLENGTH = AIMPUI_WINCONTROL_MAX_PROPID + 4;
  const int AIMPUI_BASEEDIT_PROPID_SELSTART  = AIMPUI_WINCONTROL_MAX_PROPID + 5;
  const int AIMPUI_BASEEDIT_PROPID_SELTEXT   = AIMPUI_WINCONTROL_MAX_PROPID + 6;
  const int AIMPUI_BASEEDIT_PROPID_TEXT      = AIMPUI_WINCONTROL_MAX_PROPID + 7;
  const int AIMPUI_BASEEDIT_MAX_PROPID       = AIMPUI_WINCONTROL_MAX_PROPID + 10;

  // PropID for IAIMPUIBBCBox
  const int AIMPUI_BBCBOX_PROPID_BORDERS     = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_BBCBOX_PROPID_TEXT        = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_BBCBOX_PROPID_TRANSPARENT = AIMPUI_WINCONTROL_MAX_PROPID + 3;
  const int AIMPUI_BBCBOX_PROPID_WORDWRAP    = AIMPUI_WINCONTROL_MAX_PROPID + 4;

  // PropID for IAIMPUIBevel
  const int AIMPUI_BEVEL_PROPID_BORDERS = AIMPUI_CONTROL_MAX_PROPID + 1;

  // PropID for IAIMPUIBrandBox
  const int AIMPUI_BRANDBOX_PROPID_CAPTION = AIMPUI_WINCONTROL_MAX_PROPID + 1;

  // PropID for IAIMPUIButton
  const int AIMPUI_BUTTON_PROPID_CAPTION      = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_BUTTON_PROPID_FOCUSONCLICK = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_BUTTON_PROPID_DEFAULT      = AIMPUI_WINCONTROL_MAX_PROPID + 3;
  const int AIMPUI_BUTTON_PROPID_DROPDOWNMENU = AIMPUI_WINCONTROL_MAX_PROPID + 4;
  const int AIMPUI_BUTTON_PROPID_IMAGEINDEX   = AIMPUI_WINCONTROL_MAX_PROPID + 5;
  const int AIMPUI_BUTTON_PROPID_IMAGELIST    = AIMPUI_WINCONTROL_MAX_PROPID + 6;
  const int AIMPUI_BUTTON_PROPID_MODALRESULT  = AIMPUI_WINCONTROL_MAX_PROPID + 7;
  const int AIMPUI_BUTTON_PROPID_STYLE        = AIMPUI_WINCONTROL_MAX_PROPID + 8;

  // PropID for IAIMPUIBaseButtonnedEdit
  const int AIMPUI_BUTTONEDEDIT_PROPID_BUTTONSIMAGES = AIMPUI_BASEEDIT_MAX_PROPID + 1;
  const int AIMPUI_BUTTONEDEDIT_MAX_PROPID           = AIMPUI_BASEEDIT_MAX_PROPID + 10;

  // PropID for IAIMPUIEditButton
  const int AIMPUI_EDITBUTTON_PROPID_CUSTOM     = 0;
  const int AIMPUI_EDITBUTTON_PROPID_CAPTION    = 1;
  const int AIMPUI_EDITBUTTON_PROPID_ENABLED    = 2;
  const int AIMPUI_EDITBUTTON_PROPID_HINT       = 3;
  const int AIMPUI_EDITBUTTON_PROPID_INDEX      = 4;
  const int AIMPUI_EDITBUTTON_PROPID_IMAGEINDEX = 5;
  const int AIMPUI_EDITBUTTON_PROPID_VISIBLE    = 6;
  const int AIMPUI_EDITBUTTON_PROPID_WIDTH      = 7;

  // PropID for IAIMPUICategory
  const int AIMPUI_CATEGORY_PROPID_AUTOSIZE = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_CATEGORY_PROPID_CAPTION  = AIMPUI_WINCONTROL_MAX_PROPID + 2;

  // PropID for IAIMPUICheckBox and IAIMPUIRadioBox
  const int AIMPUI_CHECKBOX_PROPID_AUTOSIZE = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_CHECKBOX_PROPID_CAPTION  = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_CHECKBOX_PROPID_STATE    = AIMPUI_WINCONTROL_MAX_PROPID + 3;
  const int AIMPUI_CHECKBOX_PROPID_WORDWRAP = AIMPUI_WINCONTROL_MAX_PROPID + 4;

  // PropID for IAIMPUIComboBox
  const int AIMPUI_COMBOBOX_PROPID_AUTOCOMPLETE   = AIMPUI_BUTTONEDEDIT_MAX_PROPID + 1;
  const int AIMPUI_COMBOBOX_PROPID_ITEMINDEX      = AIMPUI_BUTTONEDEDIT_MAX_PROPID + 2;
  const int AIMPUI_COMBOBOX_PROPID_ITEMOBJECT     = AIMPUI_BUTTONEDEDIT_MAX_PROPID + 3;
  const int AIMPUI_COMBOBOX_PROPID_TEXT           = AIMPUI_BUTTONEDEDIT_MAX_PROPID + 4;
  const int AIMPUI_COMBOBOX_PROPID_STYLE          = AIMPUI_BUTTONEDEDIT_MAX_PROPID + 5;

  // PropID for IAIMPUICheckComboBox
  const int AIMPUI_CHECKCOMBO_PROPID_TEXT      = AIMPUI_BUTTONEDEDIT_MAX_PROPID + 1;

  // PropID for IAIMPUIEdit
  const int AIMPUI_EDIT_PROPID_PASSWORDCHAR    = AIMPUI_BUTTONEDEDIT_MAX_PROPID + 2;
  const int AIMPUI_EDIT_PROPID_TEXTHINT        = AIMPUI_BUTTONEDEDIT_MAX_PROPID + 3;

  // PropID for IAIMPUIGroupBox
  const int AIMPUI_GROUPBOX_PROPID_AUTOSIZE    = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_GROUPBOX_PROPID_BORDERS     = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_GROUPBOX_PROPID_TRANSPARENT = AIMPUI_WINCONTROL_MAX_PROPID + 3;
  const int AIMPUI_GROUPBOX_PROPID_CHECKMODE   = AIMPUI_WINCONTROL_MAX_PROPID + 4;
  const int AIMPUI_GROUPBOX_PROPID_CHECKED     = AIMPUI_WINCONTROL_MAX_PROPID + 5;
  const int AIMPUI_GROUPBOX_PROPID_CAPTION     = AIMPUI_WINCONTROL_MAX_PROPID + 6;

  // PropID for IAIMPUIImage
  const int AIMPUI_IMAGE_PROPID_IMAGE            = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_IMAGE_PROPID_IMAGESTRETCHMODE = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_IMAGE_PROPID_IMAGEINDEX       = AIMPUI_WINCONTROL_MAX_PROPID + 3;
  const int AIMPUI_IMAGE_PROPID_IMAGELIST        = AIMPUI_WINCONTROL_MAX_PROPID + 4;

  // PropID for IAIMPUIImageComboBox
  const int AIMPUI_IMAGECOMBOBOX_PROPID_IMAGELIST = AIMPUI_BUTTONEDEDIT_MAX_PROPID + 1;
  const int AIMPUI_IMAGECOMBOBOX_PROPID_ITEMINDEX = AIMPUI_BUTTONEDEDIT_MAX_PROPID + 2;

  // PropID for IAIMPUILabel
  const int AIMPUI_LABEL_PROPID_AUTOSIZE       = AIMPUI_CONTROL_MAX_PROPID + 1;
  const int AIMPUI_LABEL_PROPID_LINE           = AIMPUI_CONTROL_MAX_PROPID + 2;
  const int AIMPUI_LABEL_PROPID_TEXT           = AIMPUI_CONTROL_MAX_PROPID + 3;
  const int AIMPUI_LABEL_PROPID_TEXTALIGN      = AIMPUI_CONTROL_MAX_PROPID + 4; 
  const int AIMPUI_LABEL_PROPID_TEXTALIGNVERT  = AIMPUI_CONTROL_MAX_PROPID + 5; 
  const int AIMPUI_LABEL_PROPID_TEXTCOLOR      = AIMPUI_CONTROL_MAX_PROPID + 6;
  const int AIMPUI_LABEL_PROPID_TEXTSTYLE      = AIMPUI_CONTROL_MAX_PROPID + 7;
  const int AIMPUI_LABEL_PROPID_TRANSPARENT    = AIMPUI_CONTROL_MAX_PROPID + 8;
  const int AIMPUI_LABEL_PROPID_URL            = AIMPUI_CONTROL_MAX_PROPID + 9;
  const int AIMPUI_LABEL_PROPID_WORDWRAP       = AIMPUI_CONTROL_MAX_PROPID + 10;
  const int AIMPUI_LABEL_MAX_PROPID            = AIMPUI_CONTROL_MAX_PROPID + 20;

  // PropID for IAIMPUIMemo
  const int AIMPUI_MEMO_PROPID_CARET_XY = AIMPUI_BASEEDIT_MAX_PROPID + 1;

  // PropID for IAIMPUITabSheet
  const int AIMPUI_TABSHEET_PROPID_CAPTION   = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_TABSHEET_PROPID_INDEX     = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_TABSHEET_PROPID_VISIBLE   = AIMPUI_WINCONTROL_MAX_PROPID + 3;

  // PropID for IAIMPUIPageControl
  const int AIMPUI_PAGECONTROL_PROPID_ACTIVE = AIMPUI_WINCONTROL_MAX_PROPID + 1;

  // PropID for IAIMPUIPanel
  const int AIMPUI_PANEL_PROPID_AUTOSIZE    = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_PANEL_PROPID_BORDERS     = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_PANEL_PROPID_TRANSPARENT = AIMPUI_WINCONTROL_MAX_PROPID + 3;
  const int AIMPUI_PANEL_PROPID_PADDING     = AIMPUI_WINCONTROL_MAX_PROPID + 4;

  // PropID for IAIMPUIProgressBar
  const int AIMPUI_PROGRESSBAR_PROPID_INDETERMINATE = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_PROGRESSBAR_PROPID_MAX           = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_PROGRESSBAR_PROPID_MIN           = AIMPUI_WINCONTROL_MAX_PROPID + 3;
  const int AIMPUI_PROGRESSBAR_PROPID_PROGRESS      = AIMPUI_WINCONTROL_MAX_PROPID + 4;

  // PropID for IAIMPUIScrollBox
  const int AIMPUI_SCROLLBOX_PROPID_BORDERS    = AIMPUI_WINCONTROL_MAX_PROPID + 1;

  // PropID for IAIMPUISlider
  const int AIMPUI_SLIDER_PROPID_HORIZONTAL    = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_SLIDER_PROPID_MARKS         = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_SLIDER_PROPID_PAGESIZE      = AIMPUI_WINCONTROL_MAX_PROPID + 3;
  const int AIMPUI_SLIDER_PROPID_TRANSPARENT   = AIMPUI_WINCONTROL_MAX_PROPID + 4;
  const int AIMPUI_SLIDER_PROPID_VALUE         = AIMPUI_WINCONTROL_MAX_PROPID + 5;
  const int AIMPUI_SLIDER_PROPID_VALUEDEFAULT  = AIMPUI_WINCONTROL_MAX_PROPID + 6;
  const int AIMPUI_SLIDER_PROPID_VALUEMAX      = AIMPUI_WINCONTROL_MAX_PROPID + 7;
  const int AIMPUI_SLIDER_PROPID_VALUEMIN      = AIMPUI_WINCONTROL_MAX_PROPID + 8;

  // PropID for IAIMPUISplitter
  const int AIMPUI_SPLITTER_PROPID_CANHIDE     = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_SPLITTER_PROPID_CONTROL     = AIMPUI_WINCONTROL_MAX_PROPID + 2;

  // PropID for IAIMPUISpinEdit
  const int AIMPUI_SPINEDIT_PROPID_DISPLAYMASK = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_SPINEDIT_PROPID_INCREMENT   = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_SPINEDIT_PROPID_MAXVALUE    = AIMPUI_WINCONTROL_MAX_PROPID + 3;
  const int AIMPUI_SPINEDIT_PROPID_MINVALUE    = AIMPUI_WINCONTROL_MAX_PROPID + 4;
  const int AIMPUI_SPINEDIT_PROPID_VALUE       = AIMPUI_WINCONTROL_MAX_PROPID + 5;
  const int AIMPUI_SPINEDIT_PROPID_VALUETYPE   = AIMPUI_WINCONTROL_MAX_PROPID + 6;

  // PropID for IAIMPUITabControl
  const int AIMPUI_TABCONTROL_PROPID_ACTIVETABINDEX = AIMPUI_WINCONTROL_MAX_PROPID + 1;

  // PropID for IAIMPUITimeEdit
  const int AIMPUI_TIMEDIT_PROPID_VALUE = AIMPUI_WINCONTROL_MAX_PROPID + 1;

  // PropID for IAIMPUITreeListColumn
  const int AIMPUI_TL_COLUMN_PROPID_CAN_RESIZE      = 1;
  const int AIMPUI_TL_COLUMN_PROPID_CAPTION         = 2;
  const int AIMPUI_TL_COLUMN_PROPID_DRAWINDEX       = 3;
  const int AIMPUI_TL_COLUMN_PROPID_IMAGEINDEX      = 4;
  const int AIMPUI_TL_COLUMN_PROPID_INDEX           = 5;
  const int AIMPUI_TL_COLUMN_PROPID_TEXT_ALIGNMENT  = 6;
  const int AIMPUI_TL_COLUMN_PROPID_TEXT_VISIBLE    = 7;
  const int AIMPUI_TL_COLUMN_PROPID_VISIBLE         = 8;
  const int AIMPUI_TL_COLUMN_PROPID_WIDTH           = 9;

  // PropID for IAIMPUITreeListGroup
  const int AIMPUI_TL_GROUP_PROPID_CAPTION      = 1;
  const int AIMPUI_TL_GROUP_PROPID_CHECKSTATE   = 2;
  const int AIMPUI_TL_GROUP_PROPID_EXPANDED     = 3;
  const int AIMPUI_TL_GROUP_PROPID_INDEX        = 4;
  const int AIMPUI_TL_GROUP_PROPID_SELECTED     = 5;

  // PropID for IAIMPUITreeListNode
  const int AIMPUI_TL_NODE_PROPID_ABS_VISIBLE_INDEX     = 0;
  const int AIMPUI_TL_NODE_PROPID_CHECK_ENABLED         = 1;
  const int AIMPUI_TL_NODE_PROPID_CHECKED               = 2;
  const int AIMPUI_TL_NODE_PROPID_CHILDREN_CHECK_STATE  = 3;
  const int AIMPUI_TL_NODE_PROPID_EXPANDED              = 4;
  const int AIMPUI_TL_NODE_PROPID_IMAGEINDEX            = 5;
  const int AIMPUI_TL_NODE_PROPID_INDEX                 = 6;
  const int AIMPUI_TL_NODE_PROPID_LEVEL                 = 7;
  const int AIMPUI_TL_NODE_PROPID_SELECTED              = 8;
  const int AIMPUI_TL_NODE_PROPID_TAG                   = 9;

  // PropID for IAIMPUITreeList
  const int AIMPUI_TL_PROPID_ALLOW_DELETING             = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_TL_PROPID_ALLOW_EDITING              = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_TL_PROPID_ALLOW_FOCUS_CELLS          = AIMPUI_WINCONTROL_MAX_PROPID + 3;
  const int AIMPUI_TL_PROPID_ALLOW_MULTISELECT          = AIMPUI_WINCONTROL_MAX_PROPID + 4;
  const int AIMPUI_TL_PROPID_ALLOW_REORDER_COLUMNS      = AIMPUI_WINCONTROL_MAX_PROPID + 5;
  const int AIMPUI_TL_PROPID_ALLOW_SHOWWHIDE_COLUMNS    = AIMPUI_WINCONTROL_MAX_PROPID + 6;
  const int AIMPUI_TL_PROPID_AUTOCHECK_SUBNODES         = AIMPUI_WINCONTROL_MAX_PROPID + 7;
  const int AIMPUI_TL_PROPID_BORDERS                    = AIMPUI_WINCONTROL_MAX_PROPID + 8;
  const int AIMPUI_TL_PROPID_CELL_HINTS                 = AIMPUI_WINCONTROL_MAX_PROPID + 9;
  const int AIMPUI_TL_PROPID_CHECKBOXES                 = AIMPUI_WINCONTROL_MAX_PROPID + 10;
  const int AIMPUI_TL_PROPID_COLUMN_AUTOWIDTH           = AIMPUI_WINCONTROL_MAX_PROPID + 11;
  const int AIMPUI_TL_PROPID_COLUMN_HEIGHT              = AIMPUI_WINCONTROL_MAX_PROPID + 12;
  const int AIMPUI_TL_PROPID_COLUMN_IMAGES              = AIMPUI_WINCONTROL_MAX_PROPID + 13;
  const int AIMPUI_TL_PROPID_COLUMN_VISIBLE             = AIMPUI_WINCONTROL_MAX_PROPID + 14;
  const int AIMPUI_TL_PROPID_DRAG_SORTING               = AIMPUI_WINCONTROL_MAX_PROPID + 15;
  const int AIMPUI_TL_PROPID_DRAG_SORTING_CHANGE_LEVEL  = AIMPUI_WINCONTROL_MAX_PROPID + 16;
  const int AIMPUI_TL_PROPID_GRID_LINES                 = AIMPUI_WINCONTROL_MAX_PROPID + 20;
  const int AIMPUI_TL_PROPID_GROUP_HEIGHT               = AIMPUI_WINCONTROL_MAX_PROPID + 21;
  const int AIMPUI_TL_PROPID_GROUPS                     = AIMPUI_WINCONTROL_MAX_PROPID + 22;
  const int AIMPUI_TL_PROPID_GROUPS_ALLOW_COLLAPSE      = AIMPUI_WINCONTROL_MAX_PROPID + 23;
  const int AIMPUI_TL_PROPID_GROUPS_FOCUS_ON_CLICK      = AIMPUI_WINCONTROL_MAX_PROPID + 24;
  const int AIMPUI_TL_PROPID_HOT_TRACK                  = AIMPUI_WINCONTROL_MAX_PROPID + 25;
  const int AIMPUI_TL_PROPID_INCSEARCH_COLUMN_INDEX     = AIMPUI_WINCONTROL_MAX_PROPID + 26;
  const int AIMPUI_TL_PROPID_NODE_HEIGHT                = AIMPUI_WINCONTROL_MAX_PROPID + 27;
  const int AIMPUI_TL_PROPID_NODE_IMAGE_ALIGNMENT       = AIMPUI_WINCONTROL_MAX_PROPID + 28;
  const int AIMPUI_TL_PROPID_NODE_IMAGES                = AIMPUI_WINCONTROL_MAX_PROPID + 29;
  const int AIMPUI_TL_PROPID_SORTING_MODE               = AIMPUI_WINCONTROL_MAX_PROPID + 30;

  // PropID for IAIMPUIValidationLabel
  const int AIMPUI_VALIDATIONLABEL_PROPID_GLYPH = AIMPUI_LABEL_MAX_PROPID + 1;

  // PropID for IAIMPUIForm
  const int AIMPUI_FORM_PROPID_BORDERICONS   = AIMPUI_WINCONTROL_MAX_PROPID + 1;
  const int AIMPUI_FORM_PROPID_BORDERSTYLE   = AIMPUI_WINCONTROL_MAX_PROPID + 2;
  const int AIMPUI_FORM_PROPID_CAPTION       = AIMPUI_WINCONTROL_MAX_PROPID + 3;
  const int AIMPUI_FORM_PROPID_CLOSEBYESCAPE = AIMPUI_WINCONTROL_MAX_PROPID + 4;
  const int AIMPUI_FORM_PROPID_ICON          = AIMPUI_WINCONTROL_MAX_PROPID + 5;
  const int AIMPUI_FORM_PROPID_PADDING       = AIMPUI_WINCONTROL_MAX_PROPID + 6;
  const int AIMPUI_FORM_PROPID_SHOWONTASKBAR = AIMPUI_WINCONTROL_MAX_PROPID + 7;

  // PropID for IAIMPUIProgressDialog
  const int AIMPUI_PROGRESSDLG_PROPID_CAPTION                  = 1;
  const int AIMPUI_PROGRESSDLG_PROPID_MESSAGE                  = 2;
  const int AIMPUI_PROGRESSDLG_PROPID_SHOW_PROGRESS_ON_TASKBAR = 3;

/*----------------------------------------------------------------------------------------------------------------------*/
/* Basic Events Interfaces																								   */
/*----------------------------------------------------------------------------------------------------------------------*/

  enum TAIMPUITextAlignment
  {
		utaLeftJustify = 0, 
		utaRightJustify = 1, 
		utaCenter = 2
  };

  enum TAIMPUITextVerticalAlignment
  {
		utvaTop = 0, 
		utvaBottom = 1, 
		utvaCenter = 2
  };

  enum TAIMPUIMouseButton
  {
		umbLeft = 0, 
		umbRight = 1, 
		umbMiddle = 2
  };
  
  /* IAIMPUIChangeEvents */

  class IAIMPUIChangeEvents: public IUnknown
  {
		public:
			virtual void OnChanged(IUnknown* Sender) = 0;
  };

  /* IAIMPUIDrawEvents */

  class IAIMPUIDrawEvents: public IUnknown
  {
		public:
			virtual void WINAPI OnDraw(IUnknown* Sender, HDC DC, const RECT R) = 0;
  };

  /* IAIMPUIKeyboardEvents */

  class IAIMPUIKeyboardEvents: public IUnknown
  {
	  public:
		virtual void WINAPI OnEnter(IUnknown* Sender) = 0;
		virtual void WINAPI OnExit(IUnknown* Sender) = 0;
		virtual void WINAPI OnKeyDown(IUnknown* Sender, WORD* Key, WORD Modifiers) = 0;
		virtual void WINAPI OnKeyPress(IUnknown* Sender, WCHAR* Key) = 0;
		virtual void WINAPI OnKeyUp(IUnknown* Sender, WORD* Key, WORD Modifiers) = 0;
  };

  /* IAIMPUIPopupMenuEvents */

  class IAIMPUIPopupMenuEvents: public IUnknown
  {
	  public:
		virtual BOOL WINAPI OnContextPopup(IUnknown* Sender, int X, int Y);
  };

  /* IAIMPUIMouseEvents */

  class IAIMPUIMouseEvents: public IUnknown
  {
	  public:
		virtual void WINAPI OnMouseDoubleClick(IUnknown* Sender, TAIMPUIMouseButton Button, int X, int Y, WORD Modifiers) = 0;
		virtual void WINAPI OnMouseDown(IUnknown* Sender, TAIMPUIMouseButton Button, int X, int Y, WORD Modifiers) = 0;
		virtual void WINAPI OnMouseLeave(IUnknown* Sender) = 0;
		virtual void WINAPI OnMouseMove(IUnknown* Sender, int X, int Y, WORD Modifiers) = 0;
		virtual void WINAPI OnMouseUp(IUnknown* Sender, TAIMPUIMouseButton Button, int X, int Y, WORD Modifiers) = 0;
  };

  /* IAIMPUIMouseWheelEvents */

  class IAIMPUIMouseWheelEvents: public IUnknown
  {
	  public:
		virtual BOOL WINAPI OnMouseWheel(IUnknown* Sender, int WheelDelta, int X, int Y, WORD Modifiers) = 0;
  };

  /* IAIMPUIPlacementEvents */

  class IAIMPUIPlacementEvents: public IUnknown
  {
	  public:
		virtual void WINAPI OnBoundsChanged(IUnknown* Sender) = 0;
  };

/*----------------------------------------------------------------------------------------------------------------------*/
/* Basic Controls Interfaces  																						       */
/*----------------------------------------------------------------------------------------------------------------------*/

  /* TAIMPUIControlPlacement */

  enum TAIMPUIControlAlignment 
  {
	  ualNone = 0, 
	  ualTop = 1, 
	  ualBottom = 2, 
	  ualLeft = 3, 
	  ualRight = 4, 
	  ualClient = 5
  };

  #pragma pack(push, 1)
  struct TAIMPUIControlPlacement
  {
		TAIMPUIControlAlignment Alignment;
		RECT AlignmentMargins;
		RECT Anchors;
		RECT Bounds;
  };

  /* TAIMPUIControlPlacementConstraints */

  struct TAIMPUIControlPlacementConstraints
  {
		int MaxHeight;
		int MaxWidth;
		int MinHeight;
		int MinWidth;
  };
  #pragma pack(pop)

  /* IAIMPUIControl */

  class IAIMPUIControl: public IAIMPPropertyList
  {
	  public:
			virtual HRESULT WINAPI GetPlacement(TAIMPUIControlPlacement** Placement) = 0;
			virtual HRESULT WINAPI GetPlacementConstraints(TAIMPUIControlPlacementConstraints** Constraints) = 0;
			virtual HRESULT WINAPI SetPlacement(TAIMPUIControlPlacement* Placement) = 0;
			virtual HRESULT WINAPI SetPlacementConstraints(TAIMPUIControlPlacementConstraints* Constraints) = 0;

			// Coords Translation
			virtual HRESULT WINAPI ClientToScreen(POINT* P) = 0;
			virtual HRESULT WINAPI ScreenToClient(POINT* P) = 0;

			// Drawing
			virtual HRESULT WINAPI PaintTo(HDC DC, int X, int Y) = 0;
			virtual HRESULT WINAPI Invalidate() = 0;
  };


  /* IAIMPUIWinControl */

  class IAIMPUIWinControl: public IAIMPUIControl
  {
	  public:
			virtual HRESULT WINAPI GetControl(int Index, REFIID IID, void** Obj) = 0;
			virtual int WINAPI GetControlCount() = 0;
			virtual HWND WINAPI GetHandle() = 0;
			virtual BOOL WINAPI HasHandle() = 0;
			virtual HRESULT WINAPI SetFocus() = 0;
  };

  /* IAIMPUIWndProcEvents */

  class IAIMPUIWndProcEvents: public IUnknown
  {
	  public:
			virtual BOOL WINAPI OnBeforeWndProc(DWORD Message, WPARAM ParamW, LPARAM ParamL, LRESULT* Result) = 0;
			virtual void WINAPI OnAfterWndProc(DWORD Message, WPARAM ParamW, LPARAM ParamL, LRESULT* Result) = 0;
  };

/*----------------------------------------------------------------------------------------------------------------------*/
/* Non-Visual Components Interfaces																					       */
/*----------------------------------------------------------------------------------------------------------------------*/

  /* IAIMPUIImageList */

  class IAIMPUIImageList: public IUnknown
  {
	  public:
			virtual HRESULT WINAPI Add(IAIMPImage* Image) = 0;
			virtual HRESULT WINAPI Clear() = 0;
			virtual HRESULT WINAPI Delete(int Index) = 0; 
			virtual HRESULT WINAPI Draw(HDC DC, int Index, int X, int Y, BOOL Enabled) = 0;
			virtual HRESULT WINAPI LoadFromResource(HMODULE Instance, WCHAR* ResName, WCHAR* ResType) = 0;
			virtual int WINAPI GetCount() = 0;
			virtual HRESULT WINAPI GetSize(SIZE* Size) = 0;
			virtual HRESULT WINAPI SetSize(SIZE Size) = 0;
  };

  /* IAIMPUIMenuItem */

  class IAIMPUIMenuItem: public IAIMPMenuItem
  {
		public:
			virtual HRESULT WINAPI Add(IAIMPString* ID, IAIMPUIMenuItem** MenuItem) = 0;
			virtual HRESULT WINAPI Delete(int Index) = 0;
			virtual HRESULT WINAPI Get(int Index, REFIID IID, void** Obj) = 0;
			virtual int WINAPI GetCount() = 0;
  };

  /* IAIMPUIPopupMenu */

  class IAIMPUIPopupMenu: public IUnknown
  {
	public:
		virtual HRESULT WINAPI Add(IAIMPString* ID, IAIMPUIMenuItem** MenuItem) = 0;
		virtual HRESULT WINAPI Delete(int Index) = 0;
		virtual HRESULT WINAPI DeleteChildren() = 0;
		virtual HRESULT WINAPI Get(int index, REFIID IID, void** MenuItem) = 0;
		virtual int WINAPI GetCount() = 0;
		virtual HRESULT WINAPI Popup(POINT ScreenPoint) = 0;
		virtual HRESULT WINAPI Popup2(RECT ScreenRect) = 0;
  };

/*----------------------------------------------------------------------------------------------------------------------*/
/* Special Controls and Events Interfaces																			       */
/*----------------------------------------------------------------------------------------------------------------------*/

  /* IAIMPUIBaseEdit */

  class IAIMPUIBaseEdit: public IAIMPUIWinControl
  {
	public:
		virtual HRESULT WINAPI CopyToClipboard() = 0;
		virtual HRESULT WINAPI CutToClipboard() = 0;
		virtual HRESULT WINAPI PasteFromClipboard() = 0;
		virtual HRESULT WINAPI SelectAll() = 0;
		virtual HRESULT WINAPI SelectNone() = 0;
  };

  /* IAIMPUIBBCBox */

  class IAIMPUIBBCBox: public IAIMPUIWinControl
  {
  };

  /* IAIMPUIBevel */

  class IAIMPUIBevel: public IAIMPUIControl
  {
  };

  /* IAIMPUIButton */

  class IAIMPUIButton: public IAIMPUIWinControl
  {
	public:
		virtual HRESULT WINAPI ShowDropDownMenu() = 0;
  };

  /* IAIMPUIEditButton */

  class IAIMPUIEditButton: public IAIMPPropertyList
  {
  };

  /* IAIMPUIBaseButtonnedEdit */

  class IAIMPUIBaseButtonnedEdit: public IAIMPUIBaseEdit
  {
	public:
		virtual HRESULT WINAPI AddButton(IUnknown* EventsHandler, IAIMPUIEditButton** Button) = 0;
		virtual HRESULT WINAPI DeleteButton(int Index) = 0;
		virtual HRESULT WINAPI DeleteButton2(IAIMPUIEditButton* Button) = 0;
		virtual HRESULT WINAPI GetButton(int Index, IAIMPUIEditButton** Button) = 0;
		virtual int WINAPI GetButtonCount() = 0;
  };

  /* IAIMPUIBaseComboBox */

  class IAIMPUIBaseComboBox: public IAIMPUIBaseButtonnedEdit
  {
	public:
		virtual HRESULT WINAPI Add(IUnknown* Obj, int ExtraData) = 0;
		virtual HRESULT WINAPI Add2(IAIMPObjectList* List) = 0;
		virtual HRESULT WINAPI Clear() = 0;
		virtual HRESULT WINAPI Delete(int Index) = 0;
		virtual HRESULT WINAPI GetItem(int Index, REFIID IID, void** Obj) = 0;
		virtual INT WINAPI GetItemCount() = 0;
		virtual HRESULT WINAPI SetItem(int Index, IUnknown* Obj) = 0;
  };

  /* IAIMPUIBrandBox */

  class IAIMPUIBrandBox: public IAIMPUIWinControl
  {
  };

  /* IAIMPUICategory */

  class IAIMPUICategory: public IAIMPUIWinControl
  {
  };

  /* IAIMPUICheckBox */

  class IAIMPUICheckBox: public IAIMPUIWinControl
  {
  };

  /* IAIMPUIComboBox */

  class IAIMPUIComboBox: public IAIMPUIBaseComboBox
  {
  };

  /* IAIMPUICheckComboBox */

  class IAIMPUICheckComboBox: public IAIMPUIBaseComboBox
  {
	  public:
			virtual BOOL WINAPI GetChecked(int Index) = 0;
			virtual HRESULT WINAPI SetChecked(int Index, BOOL Value) = 0;
  };

  /* IAIMPUIEdit */

  class IAIMPUIEdit: public IAIMPUIBaseButtonnedEdit
  {
  };

  /* IAIMPUIGroupBox */

  class IAIMPUIGroupBox: public IAIMPUIWinControl
  {
  };

  /* IAIMPUIImage */

  class IAIMPUIImage: public IAIMPUIControl
  {
  };

  /* IAIMPUIImageComboBox */

  class IAIMPUIImageComboBox: public IAIMPUIBaseComboBox
  {
	public:
		virtual int WINAPI GetImageIndex(int Index) = 0;
		virtual HRESULT WINAPI SetImageIndex(int Index, int Value) = 0;
  };

  /* IAIMPUILabel */

  class IAIMPUILabel: public IAIMPUIControl
  {
  };

  /* IAIMPUIMemo */

  class IAIMPUIMemo: public IAIMPUIBaseEdit
  {
	public:
		virtual HRESULT WINAPI AddLine(IAIMPString* S) = 0;
		virtual HRESULT WINAPI Clear() = 0;
		virtual HRESULT WINAPI DeleteLine(int Index) = 0;
		virtual HRESULT WINAPI InsertLine(int Index, IAIMPString* S) = 0;
		virtual HRESULT WINAPI GetLine(int Index, IAIMPString* S) = 0;
		virtual int WINAPI GetLineCount() = 0;
		virtual HRESULT WINAPI SetLine(int Index, IAIMPString* ) = 0;
		// I/O
		virtual HRESULT WINAPI LoadFromFile(IAIMPString* FileName) = 0;
		virtual HRESULT WINAPI LoadFromStream(IAIMPStream* Stream) = 0;
		virtual HRESULT WINAPI SaveToFile(IAIMPString* FileName) = 0;
		virtual HRESULT WINAPI SaveToStream(IAIMPStream* Stream) = 0;
  };

  /* IAIMPUIPaintBox */

  class IAIMPUIPaintBox: public IAIMPUIControl
  {
  };

  /* IAIMPUITabSheet */

  class IAIMPUITabSheet: public IAIMPUIWinControl
  {
  };

  /* IAIMPUIPageControl */

  class IAIMPUIPageControl: public IAIMPUIWinControl
  {
	public:
		virtual HRESULT WINAPI Add(IAIMPString* Name, IAIMPUITabSheet** Page) = 0;
		virtual HRESULT WINAPI Delete(int Index) = 0;
		virtual HRESULT WINAPI Delete2(IAIMPUITabSheet* Page) = 0;
		virtual HRESULT WINAPI Get(int Index, IAIMPUITabSheet** Page) = 0;
		virtual int WINAPI GetCount() = 0;
  };

  /* IAIMPUIPageControlEvents */

  class IAIMPUIPageControlEvents: public IUnknown
  {
	public:
		virtual void WINAPI OnActivating(IAIMPUIPageControl* Sender, IAIMPUITabSheet* Page, BOOL* Allow) = 0;
		virtual void WINAPI OnActivated(IAIMPUIPageControl* Sender, IAIMPUITabSheet* Page) = 0;
  };

  /* IAIMPUIPanel */

  class IAIMPUIPanel: public IAIMPUIWinControl
  {
  };

  /* IAIMPUIProgressBar */

  class IAIMPUIProgressBar: public IAIMPUIControl
  {
  };

  /* IAIMPUIScrollBox */

  class IAIMPUIScrollBox: public IAIMPUIWinControl
  {
	public:
		virtual HRESULT WINAPI MakeVisible(IAIMPUIControl* Control) = 0;
  };

  /* IAIMPUISlider */

  class IAIMPUISlider: public IAIMPUIWinControl
  {
  };

  /* IAIMPUISplitter */

  class IAIMPUISplitter: public IAIMPUIControl
  {
  };

  /* IAIMPUISpinEdit */

  class IAIMPUISpinEdit: public IAIMPUIWinControl
  {
  };

  /* IAIMPUITabControl */

  class IAIMPUITabControl: public IAIMPUIWinControl
  {
	public:
		virtual HRESULT WINAPI Add(IAIMPString* S) = 0;
	    virtual HRESULT WINAPI Delete(int Index) = 0;
		virtual HRESULT WINAPI Get(int Index, IAIMPString** Tab) = 0;
		virtual int WINAPI GetCount() = 0;
  };

  /* IAIMPUITabControlEvents */

  class IAIMPUITabControlEvents: public IAIMPUIChangeEvents
  {
	public:
		virtual void WINAPI OnActivating(IAIMPUITabControl* Sender, int TabIndex, BOOL* Allow) = 0;
		virtual void WINAPI OnActivated(IAIMPUITabControl* Sender, int TabIndex) = 0;
  };

  /* IAIMPUITimeEdit */

  class IAIMPUITimeEdit: public IAIMPUIWinControl
  {
  };

  /* IAIMPUITreeListColumn */

  class IAIMPUITreeListColumn: public IAIMPPropertyList
  {
  };

  /* IAIMPUITreeListGroup */

  class IAIMPUITreeListGroup: public IAIMPPropertyList
  {
		public:
			virtual HRESULT WINAPI Get(int Index, REFIID IID, void** Obj) = 0;
			virtual int WINAPI GetCount() = 0;
  };

  /* IAIMPUITreeListNode */

  class IAIMPUITreeListNode: public IAIMPPropertyList
  {
	  public:
			// Nodes
			virtual HRESULT WINAPI Add(IAIMPUITreeListNode** Node) = 0;
			virtual HRESULT WINAPI ClearChildren() = 0;
			virtual HRESULT WINAPI FindByTag(DWORD_PTR Tag, BOOL Recursive, REFIID IID, void** Node) = 0;
			virtual HRESULT WINAPI FindByValue(int ColumnIndex, IAIMPString* Value, BOOL Recursive, REFIID IID, void** Node) = 0;
			virtual HRESULT WINAPI Get(int Index, REFIID IID, void** Obj) = 0;
			virtual int WINAPI GetCount() = 0;

			// Values
			virtual HRESULT WINAPI ClearValues() = 0;
			virtual HRESULT WINAPI GetValue(int Index, IAIMPString** Value) = 0;
			virtual HRESULT WINAPI SetValue(int Index, IAIMPString* Value) = 0;

			// Groups
			virtual HRESULT WINAPI GetGroup(REFIID IID, void** Group) = 0;
  };

  /* IAIMPUITreeList */

  class IAIMPUITreeList: public IAIMPUIWinControl
  {
	  public:
			// Columns
			virtual HRESULT WINAPI AddColumn(REFIID IID, void** Obj) = 0;
			virtual HRESULT WINAPI ClearColumns() = 0;
			virtual HRESULT WINAPI DeleteColumn(int Index) = 0;
			virtual HRESULT WINAPI GetColumn(int Index, REFIID IID, void** Obj) = 0;
			virtual int WINAPI GetColumnCount() = 0;

			// Nodes
			virtual HRESULT WINAPI Clear() = 0;
			virtual HRESULT WINAPI Delete(IAIMPUITreeListNode* Node) = 0;
			virtual HRESULT WINAPI GetPath(IAIMPUITreeListNode* Node, IAIMPString** S) = 0;
			virtual HRESULT WINAPI GetRootNode(REFIID IID, void** Obj) = 0;
			virtual HRESULT WINAPI MakeTop(IAIMPUITreeListNode* Node) = 0;
			virtual HRESULT WINAPI MakeVisible(IAIMPUITreeListNode* Node) = 0;
			virtual HRESULT WINAPI SetPath(IAIMPString* S) = 0;

			// Nodes - Absolute List
			virtual HRESULT WINAPI GetAbsoluteVisibleNode(int Index, REFIID IID, void** Obj) = 0;
			virtual int WINAPI GetAbsoluteVisibleNodeCount() = 0;

			// Nodes - Selection
			virtual HRESULT WINAPI DeleteSelected() = 0;
			virtual HRESULT WINAPI SelectAll() = 0;
			virtual HRESULT WINAPI SelectNone() = 0;
			virtual HRESULT WINAPI GetFocused(REFIID IID, void** Obj) = 0;
			virtual HRESULT WINAPI SetFocused(IUnknown* Obj) = 0;
			virtual HRESULT WINAPI GetSelected(int Index, REFIID IID, void** Obj) = 0;
			virtual int WINAPI GetSelectedCount() = 0;

			// Inplace Editing
			virtual HRESULT WINAPI GetEditingCell(int* ColumnIndex, int* RowIndex) = 0;
			virtual HRESULT WINAPI StartEditing(IAIMPUITreeListColumn* Column) = 0;
			virtual HRESULT WINAPI StopEditing() = 0;

			// Grouping
			virtual HRESULT WINAPI GroupBy(IAIMPUITreeListColumn* Column, BOOL ResetPrevGroupingParams) = 0;
			virtual HRESULT WINAPI GetGroup(int Index, REFIID IID, void** Obj) = 0;
			virtual int WINAPI GetGroupCount() = 0;
			virtual HRESULT WINAPI Regroup() = 0;
			virtual HRESULT WINAPI ResetGrouppingParams() = 0;

			// Sorting
			virtual HRESULT WINAPI ResetSortingParams() = 0;
			virtual HRESULT WINAPI Resort() = 0;
			virtual HRESULT WINAPI SortBy(IAIMPUITreeListColumn* Column, DWORD Flags, BOOL ResetPrevSortingParams) = 0;

			// Customized Settings
			virtual HRESULT WINAPI ConfigLoad(IAIMPConfig* Config, IAIMPString* Key) = 0;
			virtual HRESULT WINAPI ConfigSave(IAIMPConfig* Config, IAIMPString* Key) = 0;
  };

  /* IAIMPUITreeListDragSortingEvents */

  class IAIMPUITreeListDragSortingEvents: public IUnknown
  {
		public:
			virtual void WINAPI OnDragSorting(IAIMPUITreeList* Sender) = 0;
			virtual void WINAPI OnDragSortingNodeOver(IAIMPUITreeList* Sender, IAIMPUITreeListNode* Node, DWORD Flags, BOOL* Handled) = 0;
  };

  /* IAIMPUITreeListCustomDrawEvents */

  class IAIMPUITreeListCustomDrawEvents: public IUnknown
  {
		public:
			virtual void WINAPI OnCustomDrawNode(IAIMPUITreeList* Sender, HDC DC, RECT R, IAIMPUITreeListNode* Node, BOOL* Handled) = 0;
			virtual void WINAPI OnCustomDrawNodeCell(IAIMPUITreeList* Sender, HDC DC, RECT R, IAIMPUITreeListNode* Node, IAIMPUITreeListColumn* Column, BOOL* Handled) = 0;
			virtual void WINAPI OnGetNodeBackground(IAIMPUITreeList* Sender, IAIMPUITreeListNode* Node, DWORD* Color) = 0;
  };

  /* IAIMPUITreeListInplaceEditingEvents */

  class IAIMPUITreeListInplaceEditingEvents: public IUnknown
  {
		public:
			virtual void WINAPI OnEditing(IAIMPUITreeList* Sender, IAIMPUITreeListNode* Node, int ColumnIndex, BOOL* Allow) = 0;
		    virtual void WINAPI OnEdited(IAIMPUITreeList* Sender, IAIMPUITreeListNode* Node, int ColumnIndex, IAIMPString* Value) = 0;
  };

  /* IAIMPUITreeListEvents */

  class IAIMPUITreeListEvents: public IUnknown
  {
		public:
			virtual void WINAPI OnColumnClick(IAIMPUITreeList* Sender, int ColumnIndex) = 0;
			virtual void WINAPI OnFocusedColumnChanged(IAIMPUITreeList* Sender) = 0;
			virtual void WINAPI OnFocusedNodeChanged(IAIMPUITreeList* Sender) = 0;
			virtual void WINAPI OnNodeChecked(IAIMPUITreeList* Sender, IAIMPUITreeListNode* Node) = 0;
			virtual void WINAPI OnNodeDblClicked(IAIMPUITreeList* Sender, IAIMPUITreeListNode* Node) = 0;
			virtual void WINAPI OnSelectionChanged(IAIMPUITreeList* Sender) = 0;
			virtual void WINAPI OnSorted(IAIMPUITreeList* Sender) = 0;
			virtual void WINAPI OnStructChanged(IAIMPUITreeList* Sender) = 0;
  };

  /* IAIMPUIRadioBox */

  class IAIMPUIRadioBox: public IAIMPUICheckBox
  {
  };

  /* IAIMPUIValidationLabel */

  class IAIMPUIValidationLabel: public IAIMPUILabel
  {
  };

/*----------------------------------------------------------------------------------------------------------------------*/
/* Top-Level Window Interfaces																						       */
/*----------------------------------------------------------------------------------------------------------------------*/
  
  /* IAIMPUIForm */

  class IAIMPUIForm: public IAIMPUIWinControl
  {
		public:
			virtual HRESULT WINAPI Close() = 0;
			virtual HRESULT WINAPI GetFocusedControl(IAIMPUIWinControl** Control) = 0;
			virtual HRESULT WINAPI Localize() = 0;
			virtual HRESULT WINAPI Release(BOOL Postponed) = 0;
			virtual int WINAPI ShowModal() = 0;
  };

  /* IAIMPUIFormEvents */

  class IAIMPUIFormEvents: public IUnknown
  {
		public:
			virtual void WINAPI OnActivated(IAIMPUIForm* Sender) = 0;
			virtual void WINAPI OnDeactivated(IAIMPUIForm* Sender) = 0;
			virtual void WINAPI OnCreated(IAIMPUIForm* Sender) = 0;
			virtual void WINAPI OnDestroyed(IAIMPUIForm* Sender) = 0;
			virtual void WINAPI OnCloseQuery(IAIMPUIForm* Sende, BOOL* CanClose) = 0;
			virtual void WINAPI OnLocalize(IAIMPUIForm* Sender) = 0;
			virtual void WINAPI OnShortCut(IAIMPUIForm* Sender, WORD Key, WORD Modifiers, BOOL* Handled) = 0;
  };

/*----------------------------------------------------------------------------------------------------------------------*/
/* Dialogs																											       */
/*----------------------------------------------------------------------------------------------------------------------*/

  /* IAIMPUIBrowseFolderDialog */

  class IAIMPUIBrowseFolderDialog: public IUnknown
  {
		public:
			virtual HRESULT WINAPI Execute(HWND OwnerWnd, DWORD Flags, IAIMPString* DefaultPath, IAIMPObjectList** Selection) = 0;
  };

  /* IAIMPUIFileDialogs */

  class IAIMPUIFileDialogs: public IUnknown
  {
		public:
			virtual HRESULT WINAPI ExecuteOpenDialog(HWND OwnerWnd, IAIMPString* Caption, IAIMPString* Filter, IAIMPString** FileName) = 0;
			virtual HRESULT WINAPI ExecuteOpenDialog2(HWND OwnerWnd, IAIMPString* Caption, IAIMPString* Filter, IAIMPObjectList** Files) = 0;
			virtual HRESULT WINAPI ExecuteSaveDialog(HWND OwnerWnd, IAIMPString* Caption, IAIMPString* Filter, IAIMPString** FileName, int* FilterIndex) = 0;
  };

  /* IAIMPUIInputDialog */

  class IAIMPUIInputDialog: public IUnknown
  {
		public:
			virtual HRESULT WINAPI Execute(HWND OwnerWnd, IAIMPString* Caption, 
				IUnknown* EventsHandler, IAIMPString* Text, VARIANT* Value) = 0;
			virtual HRESULT WINAPI Execute2(HWND OwnerWnd, IAIMPString* Caption,
				IUnknown* EventsHandler, IAIMPObjectList* TextForValues, VARIANT* Values, int ValueCount) = 0;
  };

  /* IAIMPUIInputDialogEvents */

  class IAIMPUIInputDialogEvents: public IUnknown
  {
		public:
			virtual HRESULT WINAPI OnValidate(const VARIANT Value, int ValueIndex) = 0;
  };

  /* IAIMPUIMessageDialog */

  class IAIMPUIMessageDialog: public IUnknown
  {
		public:
		    virtual HRESULT WINAPI Execute(HWND OwnerWnd, IAIMPString* Caption, IAIMPString* Text, DWORD Flags) = 0;
  };

  /* IAIMPUIProgressDialog */

  class IAIMPUIProgressDialog: public IAIMPPropertyList
  {
		public:
			virtual HRESULT WINAPI Finished() = 0;
			virtual HRESULT WINAPI Progress(const INT64 Position, INT64 Total, IAIMPString* Text) = 0;
			virtual HRESULT WINAPI Started() = 0;
  };

  /* IAIMPUIProgressDialogEvents */

  class IAIMPUIProgressDialogEvents: public IUnknown
  {
		public:
		    virtual void WINAPI OnCanceled() = 0;
  };

/*----------------------------------------------------------------------------------------------------------------------*/
/* GUI Service																										       */
/*----------------------------------------------------------------------------------------------------------------------*/

const int AIMPUI_SERVICE_CREATEFORM_FLAGS_CHILD = 1;

  /* IAIMPServiceUI */

  class IAIMPServiceUI: public IUnknown
  {
		public:
			virtual HRESULT WINAPI CreateControl(IAIMPUIForm* Owner, IAIMPUIWinControl* Parent, 
				IAIMPString* Name, IUnknown* EventsHandler, REFIID IID, void** Control) = 0;
			virtual HRESULT WINAPI CreateForm(HWND OwnerWindow, DWORD Flags, 
				IAIMPString* Name, IUnknown* EventsHandler, IAIMPUIForm** Form) = 0;
			virtual HRESULT WINAPI CreateObject(IAIMPUIForm* Owner, IUnknown* EventsHandler, REFIID IID, void** Obj) = 0;
  };

#endif // !apiGUIH