// NSFShellView.h : Declaration of the CNSFShellView

#ifndef __NSFSHELLVIEW_H_
#define __NSFSHELLVIEW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "resource.h"       // main symbols
#include <atlwin.h>
#include "MyVirtualFolder.h"
#include "CNWSPidlMgr.h"

typedef enum
{
	MI_DELETE_INDEX=0,
	MI_PROPERTIES_INDEX,
	MI_REFRESH_INDEX,
	MI_HELPTOPICS_INDEX,
}NSF_MI_INDEX;

#define ID_HELP_HELPTOPICS   (FCIDM_SHVIEWFIRST + 0x702)

/////////////////////////////////////////////////////////////////////////////
// CNSFShellView
class ATL_NO_VTABLE CNSFShellView : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CNSFShellView, &CLSID_NSFShellView>,
	public IShellView
{
    enum { IDC_LISTVIEW = 123 };
    typedef enum {
       TBI_STD = 0,
       TBI_VIEW,
       TBI_LOCAL
    } TOOLBARITEM;

    typedef struct
    {
       TOOLBARITEM nType;
       TBBUTTON tb;
    } NS_TOOLBUTTONINFO, *PNS_TOOLBUTTONINFO;
    
public:
	HRESULT FinalConstruct();
	void FinalRelease();

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CNSFShellView)
    COM_INTERFACE_ENTRY_IID(IID_IShellView,IShellView)
    COM_INTERFACE_ENTRY(IOleWindow)
END_COM_MAP()


BEGIN_MSG_MAP(CNSFShellView)

    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
	MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
    MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
	MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)

    NOTIFY_CODE_HANDLER(NM_SETFOCUS, OnNotifySetFocus)
    NOTIFY_CODE_HANDLER(NM_KILLFOCUS, OnNotifyKillFocus)
    NOTIFY_CODE_HANDLER(LVN_ITEMACTIVATE, OnItemActivated)
    NOTIFY_CODE_HANDLER(HDN_ITEMCLICK, OnHeaderClick)
    COMMAND_ID_HANDLER(ID_VIEW_REFRESH, OnRefresh)  
END_MSG_MAP()

//////////////////////////////////////////////////////////////////////////////
//   INEFShellView

public:

	//È¡´úDECLARE_REGISTRY_RESOURCEID(IDR_NSFSHELLVIEW)
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister)
    {
		CComBSTR bstrDescription;

		CComBSTR bstrCLSID(CLSID_NSFShellView);
		bstrDescription.LoadString(IDS_DESCRIPTION);

		_ATL_REGMAP_ENTRY rm[] = { 
       	{ OLESTR("DESCRIPTION"), bstrDescription },
       	{ OLESTR("CLSID"), bstrCLSID },
       	{ NULL,NULL } };

		return _Module.UpdateRegistryFromResource(IDR_NSFSHELLVIEW, bRegister, rm);
    }
    
    //IOleWindow
    STDMETHOD(GetWindow)(HWND* phWnd);
    STDMETHOD(ContextSensitiveHelp)(BOOL);
    
    //IShellView
    STDMETHOD (TranslateAccelerator) (LPMSG);
    STDMETHOD (EnableModeless) (BOOL);
    STDMETHOD (UIActivate) (UINT);
    STDMETHOD (Refresh) (void);
    STDMETHOD (CreateViewWindow) (LPSHELLVIEW, LPCFOLDERSETTINGS, LPSHELLBROWSER, LPRECT, HWND*);
    STDMETHOD (DestroyViewWindow) (void);
    STDMETHOD (GetCurrentInfo) (LPFOLDERSETTINGS);
    STDMETHOD (AddPropertySheetPages) (DWORD, LPFNADDPROPSHEETPAGE, LPARAM);
    STDMETHOD (SaveViewState) (void);
    STDMETHOD (SelectItem) (LPCITEMIDLIST, UINT);
	STDMETHOD (GetItemObject) (UINT, REFIID, LPVOID*);
	//STDMETHOD(QueryContextMenu)(HMENU hMenu,
	//	UINT    iIndexMenu,
	//	UINT    idCmdFirst,
	//	UINT    idCmdLast,
	//	UINT    uFlags)
	//{       
	//	// add Menu item according to Explore's behavior

	//	if( NULL == m_pidl ) 
	//	{
	//		//no object is selected, context menu is created 

	//		//in Shell view and is belongs to current folder

	//		//enable to create new subfolder in current folder

	//		::InsertMenu(hMenu, iIndexMenu++, 
	//			MF_STRING | MF_BYPOSITION, 
	//			idCmdFirst + IDM_CREATE_FOLDER, 
	//			_TEXT("New NSE&Folder"));
	//		::InsertMenu(hMenu, iIndexMenu++, 
	//			MF_SEPARATOR | MF_STRING | MF_BYPOSITION, 
	//			0, _T(""));
	//		::InsertMenu(hMenu, iIndexMenu++, 
	//			MF_STRING | MF_BYPOSITION, 
	//			idCmdFirst + IDM_PROPERTIES, 
	//			_TEXT("&Properties"));
	//	}
	//	else
	//	{
	//		//one object is selected in Shell view or Tree view

	//		//add delete and properties two commands

	//		::InsertMenu(hMenu, iIndexMenu++, 
	//			MF_STRING | MF_BYPOSITION, 
	//			idCmdFirst + IDM_DELETE, 
	//			_TEXT("&Delete"));
	//		::InsertMenu(hMenu, iIndexMenu++, 
	//			MF_SEPARATOR | MF_STRING | MF_BYPOSITION, 
	//			0, _T(""));
	//		::InsertMenu(hMenu, iIndexMenu++, 
	//			MF_STRING | MF_BYPOSITION, 
	//			idCmdFirst + IDM_PROPERTIES, 
	//			_TEXT("&Properties"));
	//	}

	//	::SetMenuDefaultItem(hMenu, 0, TRUE);

	//	return MAKE_HRESULT(SEVERITY_SUCCESS, 0, IDM_LAST);
	//}

private:

    // Message handlers
    LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
    LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
    LRESULT OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
    LRESULT OnSettingChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnNotifySetFocus(UINT /*CtlID*/, LPNMHDR /*lpnmh*/, BOOL &/*bHandled*/);
    LRESULT OnNotifyKillFocus(UINT /*CtlID*/, LPNMHDR /*lpnmh*/, BOOL &/*bHandled*/);
    LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    
    LRESULT OnHeaderClick(UINT CtlID, LPNMHDR lpnmh, BOOL &bHandled);
    LRESULT OnItemActivated(UINT CtlID, LPNMHDR lpnmh, BOOL &bHandled);
    LRESULT OnRefresh(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

//////////////////////////////////////////////////////////////////////////////
//    assistant functions

private:
	void    _GetShellSettings(SHELLFLAGSTATE &sfs, DWORD dwMask);
	LRESULT _ViewActivate(UINT uState);
	LRESULT _ViewDeactivate(void);
	void    _UpdateShellSettings(void);
	BOOL    _CreateListView(void);

public:	
    HRESULT _Init(CMyVirtualFolder *pFolder,LPCITEMIDLIST pidlRoot);	
    BOOL    _InitListView();
    HRESULT _FillListView();

//////////////////////////////////////////////////////////////////////////////
//    data members
public:
	
    const MSG* m_pCurrentMsg;
    HWND m_hWnd;//right panel window handler
    HWND m_hwndList;//ListView handler
    DWORD m_dwListViewStyle;//ListView type

    //Contains folder view information. 
    FOLDERSETTINGS m_FolderSettings;

	//Contains a set of flags that indicate the current shell settings.
	//This structure is used with the SHGetSettings function. 
    SHELLFLAGSTATE m_ShellFlags; 

	//keep the IShellBrowser interface pointer 
	//to allow communication with the Windows Explorer window. 
    LPSHELLBROWSER m_pShellBrowser;

    UINT m_uState;//current window state

protected:

    CMyVirtualFolder *m_pFolder;    //pointer to current folder object
	LPITEMIDLIST  m_pidlRoot;
	HMENU m_hMenu;                //menu handler
	HACCEL m_hAccels;				//accel handler
	CNWSPidlMgr m_PidlMgr;
	BOOL       m_bForwardSort;   //current sort way
	USHORT     m_iSortedColumn;  //current sort column

};

#endif //__NSFSHELLVIEW_H_
