#pragma once


#ifndef WM_GETISHELLBROWSER
    #define WM_GETISHELLBROWSER (WM_USER+7)
#endif
   
class IShellBrowserImpl : public IShellBrowser, public ICommDlgBrowser
{
	DWORD m_dwRef;
public:
	IShellBrowserImpl():m_dwRef(0){}

	STDMETHOD(QueryInterface)(REFIID iid, void **ppvObject)
	{
		if(ppvObject == NULL)
			return E_POINTER;

		*ppvObject = NULL;

		if(iid == IID_IUnknown)
			*ppvObject = (IUnknown*)(IShellBrowser*) this;
		else if(iid == IID_IOleWindow)
			*ppvObject = (IOleWindow*) this;			
		else if(iid == IID_IShellBrowser)
			*ppvObject = (IShellBrowser*) this;
		else if(iid == IID_ICommDlgBrowser)
			*ppvObject = (ICommDlgBrowser*) this;
		else
			return E_NOINTERFACE;
		((IUnknown*)(*ppvObject))->AddRef();
		return S_OK;
	}

	STDMETHOD_(ULONG, AddRef)() { return ++m_dwRef; }
	STDMETHOD_(ULONG, Release)(){ return --m_dwRef; }  //not heap based	
    // *** IOleWindow methods ***
    STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode){return E_NOTIMPL;}	
	// *** ICommDlgBrowser methods ***
    STDMETHOD(OnDefaultCommand) (THIS_ struct IShellView * ppshv)
	{	//handle double click and ENTER key if needed
		return E_NOTIMPL; 
	}
    STDMETHOD(OnStateChange) (THIS_ struct IShellView * ppshv,ULONG uChange)
	{	//handle selection, rename, focus if needed
		return E_NOTIMPL; 
	}
    STDMETHOD(IncludeObject) (THIS_ struct IShellView * ppshv,LPCITEMIDLIST pidl)
	{	//filter files if needed
		return S_OK;
	}
    // *** IShellBrowser methods *** (same as IOleInPlaceFrame)
    STDMETHOD(InsertMenusSB) (HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths) {return E_NOTIMPL;}
    STDMETHOD(SetMenuSB) (HMENU hmenuShared, HOLEMENU holemenuReserved,HWND hwndActiveObject){return E_NOTIMPL;}
    STDMETHOD(RemoveMenusSB) (HMENU hmenuShared){return E_NOTIMPL;}
    STDMETHOD(SetStatusTextSB) (LPCOLESTR lpszStatusText){return E_NOTIMPL;}
    STDMETHOD(EnableModelessSB) (BOOL fEnable){return E_NOTIMPL;}
	STDMETHOD(BrowseObject)(LPCITEMIDLIST pidl, UINT wFlags){return E_NOTIMPL; }
	STDMETHOD(GetViewStateStream)(DWORD grfMode,LPSTREAM  *ppStrm){return E_NOTIMPL;}	
	STDMETHOD(OnViewWindowActive)(struct IShellView *ppshv){return E_NOTIMPL;}
	STDMETHOD(SetToolbarItems)(LPTBBUTTON lpButtons, UINT nButtons,UINT uFlags){return E_NOTIMPL;}
	STDMETHOD(TranslateAcceleratorSB) (LPMSG lpmsg, WORD wID) { return S_OK; }
};


struct TREEITEMINFO
{
	TREEITEMINFO() {memset(this, 0, sizeof(TREEITEMINFO));}
	ITEMIDLIST*   pidlSelf;
	ITEMIDLIST*   pidlFullyQual;
	IShellFolder* pParentFolder;
	DWORD		  dwFlags;
};

// CExplorerWnd

class CExplorerWnd : public CWnd, public IShellBrowserImpl
{
	DECLARE_DYNAMIC(CExplorerWnd)

public:
	CExplorerWnd();
	virtual ~CExplorerWnd();

	IShellView*			m_pShellView;
	IShellFolder*		m_current_folder;
	HWND				m_encapwnd;

protected:
	DECLARE_MESSAGE_MAP()

public:
	STDMETHOD(GetWindow)(HWND * lphwnd)
	{ 
		*lphwnd = m_hWnd; 
		return S_OK; 
	}
	//IShellBrowser
	STDMETHOD(QueryActiveShellView)(struct IShellView ** ppshv)
	{
		m_pShellView->AddRef();
		*ppshv = m_pShellView;
		return S_OK; 
	}
	STDMETHOD(GetControlWindow)(UINT id, HWND * lphwnd)
	{
		//return E_NOTIMPL;
		if(lphwnd == NULL)
			return E_POINTER;
		if(FCW_STATUS == id)
		{
			*lphwnd = m_hWnd;
			return S_OK;
		}
		/*else if(FCW_TREE == id)
		{
			*lphwnd = m_hWnd;
			return S_OK;
		}*/
		return E_NOTIMPL;
	}

	STDMETHOD(SendControlMsg)(UINT id, UINT uMsg, WPARAM wParam,LPARAM lParam, LRESULT *pret)
	{
		//return E_NOTIMPL;
		if(pret == NULL)
			return E_POINTER;
		if(FCW_STATUS == id || FCW_TREE == id)
		{
			*pret = ::SendMessage(m_hWnd, uMsg, wParam, lParam);
			return S_OK;
		}
	
		return E_NOTIMPL;
	}

	STDMETHOD(OnDefaultCommand) (THIS_ struct IShellView * ppshv)
	{	//handle double click and ENTER key if needed

		QueryActiveShellView(&ppshv);

		IShellFolder *pParentFolder = NULL;//, *desktop = NULL;
		//SHGetDesktopFolder(&desktop);


		LPITEMIDLIST    pIDL = NULL;

		IDataObject *data;
		ppshv->GetItemObject(SVGIO_SELECTION, IID_IDataObject, (void **)&data);

		FORMATETC   fm_etc = {(CLIPFORMAT)RegisterClipboardFormat(CFSTR_SHELLIDLIST), NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
		STGMEDIUM   stg_medium;
		data->GetData(&fm_etc, &stg_medium);
		LPIDA   toppos = reinterpret_cast<LPIDA>(GlobalLock(stg_medium.hGlobal));
		pIDL = reinterpret_cast<LPITEMIDLIST>(reinterpret_cast<BYTE *>(toppos) + toppos->aoffset[1]);
		ReleaseStgMedium(&stg_medium);
		if(SUCCEEDED(m_current_folder->BindToObject(pIDL, NULL, IID_IShellFolder, (void**)&pParentFolder)))
		{
			update_view(pParentFolder);
		}


		//return E_NOTIMPL; 
		return S_OK; 
	}

public:
	LRESULT OnGetIShellBrowser(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		//AddRef(); //not addrefed
		//SetWindowLong(DWL_MSGRESULT, (LONG)(IShellBrowser*)this); //use this if dialog
		return (LRESULT)(IShellBrowser*)this;
	}

	/*LRESULT OnTreeGetDispInfo(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		LPNMTVDISPINFO lpdi = (LPNMTVDISPINFO)pnmh;
		TREEITEMINFO* pItemInfo = (TREEITEMINFO*)lpdi->item.lParam;
		SHFILEINFO     sfi;

		if(lpdi->item.mask & TVIF_TEXT)
		{
			if(SHGetFileInfo((LPCTSTR)pItemInfo->pidlFullyQual, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))
				lstrcpy(lpdi->item.pszText, sfi.szDisplayName);
		}

		if(lpdi->item.mask & TVIF_IMAGE)
		{
			if(SHGetFileInfo((LPCTSTR)pItemInfo->pidlFullyQual, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_LINKOVERLAY))
				lpdi->item.iImage = sfi.iIcon;
		}
		if(lpdi->item.mask & TVIF_SELECTEDIMAGE)
		{
			if(SHGetFileInfo((LPCTSTR)pItemInfo->pidlFullyQual, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON))
				lpdi->item.iSelectedImage = sfi.iIcon;
		}
	}*/
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual BOOL DestroyWindow();
	int update_view(IShellFolder* folder = NULL);
};


