#if	!defined(AFX_CONTEXTMENU_H__20041122_3BB7_5346_1433_0080AD509054__INCLUDED_)
#define	AFX_CONTEXTMENU_H__20041122_3BB7_5346_1433_0080AD509054__INCLUDED_

#pragma	once


//////////////////////////////////////////////////////////////////////////////
// CContextMenu
// The IContextMenu	implementation for folders in the 
// Explorer's Folder tree.
//
class ATL_NO_VTABLE	CContextMenu : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IContextMenu
{
public:

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CContextMenu)
	COM_INTERFACE_ENTRY_IID(IID_IContextMenu,IContextMenu)
END_COM_MAP()

// Attributes
public:

	CMyVirtualFolder *m_pFolder;
	HWND m_hWnd;
	LPITEMIDLIST m_pidl;
	CNWSPidlMgr	 m_PidlMgr;

	typedef	enum 
	{
		IDM_PROPERTIES =0,
		IDM_CREATE_FOLDER,
		//IDM_CREATE_FILE,
		IDM_DELETE,
		IDM_LAST,
	} MENUITEMS;

// CDropTarget
public:
	HRESULT	FinalConstruct()
	{
		m_hWnd = NULL;
		m_pFolder =	NULL;
		return S_OK;
	}

	void FinalRelease()
	{
		ATLASSERT(m_pFolder);
		if(m_pFolder==NULL)
		{
			MessageBox(NULL,_T("CContextMenu()::FinalRelease() m_pFolder==NULL"),_T("NSF"),MB_OK);
			return;
		}
		m_pFolder->Release();

		if(	m_pidl != NULL)
		{
			m_PidlMgr.Delete(m_pidl);
			m_pidl=NULL;
		}
		m_hWnd = NULL;
	}

	HRESULT	_Init(CMyVirtualFolder *pFolder, HWND hWnd,	LPCITEMIDLIST pidl)
	{
		ATLASSERT(pFolder);
		if(pFolder==NULL)
		{
			MessageBox(NULL,_T("CContextMenu()::_Init(pFolder==NULL)"),_T("NSF"),MB_OK);
			return E_FAIL;
		}
		m_pFolder =	pFolder;
		m_pFolder->AddRef();
		m_pidl = m_PidlMgr.Copy(pidl);
		m_hWnd = hWnd;

		return S_OK;
	}

// IContextMenu
public:
	STDMETHOD(QueryContextMenu)(HMENU hMenu,
								UINT	iIndexMenu,
								UINT	idCmdFirst,
								UINT	idCmdLast,
								UINT	uFlags)
	{
		ATLTRACE(_T("CContextMenu::QueryContextMenu\n"));
		ATLASSERT( idCmdLast > idCmdFirst +	IDM_LAST);
		if(!(idCmdLast > idCmdFirst + IDM_LAST))
		{
			MessageBox(NULL,_T("CContextMenu::QueryContextMenu() idCmdLast>idCmdFirst+IDM_LAST"),_T("NSF"),MB_OK);
			return E_FAIL;
		}

		// add Menu item according to Explore's behavior
		if(	NULL == m_pidl ) 
		{
			//no item selected in current list view, context menu relate to current folder
			//we enable to create new subfolder in current folder
			::InsertMenu(hMenu,	iIndexMenu++, MF_STRING	| MF_BYPOSITION, idCmdFirst	+ IDM_CREATE_FOLDER, _TEXT("New NSE&Folder"));
			::InsertMenu(hMenu, iIndexMenu++, MF_SEPARATOR | MF_STRING | MF_BYPOSITION, 0, _T(""));
			::InsertMenu(hMenu,	iIndexMenu++, MF_STRING	| MF_BYPOSITION, idCmdFirst	+ IDM_PROPERTIES, _TEXT("&Properties"));
		}
		else
		{
			::InsertMenu(hMenu,	iIndexMenu++, MF_STRING	| MF_BYPOSITION, idCmdFirst	+ IDM_DELETE, _TEXT("&Delete"));
			::InsertMenu(hMenu, iIndexMenu++, MF_SEPARATOR | MF_STRING | MF_BYPOSITION, 0, _T(""));
			::InsertMenu(hMenu,	iIndexMenu++, MF_STRING	| MF_BYPOSITION, idCmdFirst	+ IDM_PROPERTIES, _TEXT("&Properties"));
		}
		
		::SetMenuDefaultItem(hMenu,	0, TRUE);

		return MAKE_HRESULT(SEVERITY_SUCCESS, 0, IDM_LAST);
	}

    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO pcmi)
    {
        ATLTRACE(_T("CContextMenu::InvokeCommand\n"));
        ATLASSERT(m_pFolder);
        
        if(m_pFolder == NULL)
        {
            MessageBox(NULL,_T("CContextMenu()::InvokeCommand() m_pFolder==NULL"),_T("NSF"),MB_OK);
            return E_FAIL;
        }
       
        USES_CONVERSION;
        // The command is sent via a verb
        if( HIWORD(pcmi->lpVerb) ) 
        {
            LPCMINVOKECOMMANDINFOEX pcmix = NULL;
            if( pcmi->cbSize>=sizeof(CMINVOKECOMMANDINFOEX)-sizeof(POINT) ) 
                pcmix = (LPCMINVOKECOMMANDINFOEX)pcmi;
            
            LPCSTR pstr = pcmi->lpVerb;
            
            // If it's an UNICODE string, convert it to ANSI now
            // NOTE: No C++ block here because of W2CA stack-scope.
            if( (pcmix!=NULL) && 
				((pcmix->fMask & CMIC_MASK_UNICODE)!=0) && 
				(pcmix->lpVerbW!=NULL) )    
                pstr = W2CA(pcmix->lpVerbW);
            
            if( strcmp(pstr, "Delete")==0 ) 
				pcmi->lpVerb = (LPCSTR)IDM_DELETE;
            else if( strcmp(pstr, "New NSEFolder")==0 ) 
				pcmi->lpVerb = (LPCSTR)IDM_CREATE_FOLDER;
            else 
				return E_INVALIDARG;
        }

        // Check that it's a valid command
        if( LOWORD(pcmi->lpVerb)>IDM_LAST ) 
            return E_INVALIDARG;
        
        // Process our command
        switch( LOWORD(pcmi->lpVerb) ) 
        {
            case IDM_DELETE:
            {
                m_pFolder->_DoDelete(m_pidl);
            }
            break;
			case IDM_CREATE_FOLDER:
			{
                if(m_pFolder->_IsViewWindow(m_hWnd))
					::SendMessage(m_hWnd,WM_COMMAND,ID_NEWITEM_FOLDER,0);
                else
					MessageBox(NULL,_T("Don't support create new folder except in Shell View window"),_T("Error"),MB_OK);
			}
			break;            
            default:
                ::MessageBeep((UINT)-1);
                return E_INVALIDARG;
        }
        return S_OK;
    }

	STDMETHOD(GetCommandString)(UINT idCmd,	UINT uFlags, LPUINT, LPSTR pszName,	UINT cchMax)
	{
		ATLTRACE(_T("CContextMenu::GetCommandString\n"));
		HRESULT	hr = E_INVALIDARG;
		switch(	uFlags ) 
		{
		case GCS_VERBA:
		case GCS_VERBW:					 
			  {
					if( idCmd == IDM_DELETE ) 
					{
						if(	uFlags & GCS_UNICODE )
							wcscpy((LPOLESTR)pszName, L"Delete");
						else
							strcpy(pszName,"Delete");
						return S_OK;
					}	
					if( idCmd == IDM_CREATE_FOLDER ) 
					{
						if(	uFlags & GCS_UNICODE )
							wcscpy((LPOLESTR)pszName, L"Create NSEFolder");
						else
							strcpy(pszName,"Create NSEFolder");
						return S_OK;
					}					
					else 
						return	E_INVALIDARG;					
			  }
			  break;
		case GCS_HELPTEXTA:
		case GCS_HELPTEXTW:
			  {
					if( idCmd == IDM_DELETE ) 
					{
						if(	uFlags & GCS_UNICODE )
							wcscpy((LPOLESTR)pszName, L"Delete current object in NSExtAddDelFld NSE");
						else
							strcpy(pszName,"Delete current object in NSExtAddDelFld NSE");
						return S_OK;
					}
					if( idCmd == IDM_CREATE_FOLDER ) 
					{
						if(	uFlags & GCS_UNICODE )
							wcscpy((LPOLESTR)pszName, L"Create a new folder object in NSExtAddDelFld NSE");
						else
							strcpy(pszName,"Create a new folder object in NSExtAddDelFld NSE");
						return S_OK;
					}					
					else 
						return	E_INVALIDARG;					
			  }
			  break;			
		case GCS_VALIDATE:
			  hr = S_OK;
			  break;			
		}
		return hr;
	}
};


#endif // !defined(AFX_CONTEXTMENU_H__20041122_3BB7_5346_1433_0080AD509054__INCLUDED_)
