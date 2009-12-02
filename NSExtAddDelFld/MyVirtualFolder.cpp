//========================================================================================
//
// Module:			MyVirturalFolder.cpp
// Author:          Zeng Xi
// Creation Date:	10.15.2005
//
//========================================================================================

#include "stdafx.h"
#include "MyVirtualFolder.h"
#include "CPidlEnum.h"
#include "NSFShellView.h"
#include "CExtractIcon.h"
#include "CContextMenu.h"

/////////////////////////////////////////////////////////////////////////////
// CMyVirtualFolder
//
//
HRESULT CMyVirtualFolder::FinalConstruct()
{
	m_pidlPath = NULL;
    return S_OK;
}

void CMyVirtualFolder::FinalRelease()
{
	ATLTRACE(_T("MyVirtualFolder::FinalRelease\n"));

	if(m_pidlPath != NULL)
	{
		m_PidlMgr.Delete(m_pidlPath);
		m_pidlPath = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////////
//interfaces functions

STDMETHODIMP CMyVirtualFolder::GetClassID(LPCLSID pClsid)
{

	if ( NULL == pClsid )
		return E_POINTER;

	// Return our GUID to the shell.
	*pClsid = CLSID_MyVirtualFolder;

	return S_OK;
}

STDMETHODIMP CMyVirtualFolder::Initialize(LPCITEMIDLIST pidl)
{

	ATLTRACE(_T("CMyVirtualFolder(0x%08x)::Initialize() \n"), this);

	if(pidl && (!_Module.m_pidlNSFROOT))
	{
		_Module.m_pidlNSFROOT = m_PidlMgr.Copy(pidl);

		HRESULT Hr;
		HR(GetGlobalSettings());
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////
//
// IShellFolder Implementation
//

//BindToObject() is called when a folder in our part of the namespace is being browsed.
//Retrieves an IShellFolder object for a subfolder. 
STDMETHODIMP CMyVirtualFolder::BindToObject(LPCITEMIDLIST pidl,
											LPBC pbcReserved, 
											REFIID riid, 
											LPVOID *ppRetVal)
{

	*ppRetVal = NULL;

	HRESULT hr=S_OK;

	if( riid != IID_IShellFolder) 
		return E_NOINTERFACE;

	CComObject<CMyVirtualFolder> *pVFObj=0;

	hr=CComObject<CMyVirtualFolder>::CreateInstance(&pVFObj);
	if(FAILED(hr))
		return hr;

	pVFObj->AddRef();

	LPITEMIDLIST pidlNew = m_PidlMgr.Concatenate(m_pidlPath,pidl);
	pVFObj->m_pidlPath = m_PidlMgr.Copy(pidlNew);	
	m_PidlMgr.Delete(pidlNew);

	hr=pVFObj->QueryInterface(riid, ppRetVal);

	ATLASSERT(pVFObj);
	pVFObj->Release();

	return hr;
}
STDMETHODIMP CMyVirtualFolder::BindToStorage(	LPCITEMIDLIST pidl, 
												LPBC pbcReserved, 
												REFIID riid, 
												LPVOID *ppvOut)
{
	*ppvOut = NULL;
	return E_NOTIMPL;
}

int CMyVirtualFolder::_CompareIDs(LPARAM lParam, LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
	HRESULT Hr;

    CListSortInfo* pSort = (CListSortInfo*) lParam;
    if(NULL==lParam)
    {
        CListSortInfo sort = { this, COL_NAME, TRUE };
        pSort=&sort;
    }
    else
    {
        pSort = (CListSortInfo*) lParam;
        _ASSERT(NULL != lParam);
    }


	BOOL bIsEmpty1 = ( ( NULL==pidl1 )||( 0 == pidl1->mkid.cb) );
	BOOL bIsEmpty2 = ( ( NULL==pidl2 )||( 0 == pidl2->mkid.cb) );

	if( bIsEmpty1 && bIsEmpty2 ) 
		return 0;

	if( bIsEmpty1 ) 		
	{
		Hr=-1;
		return pSort->bForwardSort? Hr: Hr*(-1);
	}


	if( bIsEmpty2 ) 
	{
		Hr=1;
		return pSort->bForwardSort? Hr: Hr*(-1);
	}

	TCHAR szTemp1[MAX_PATH]=_TEXT("");
	TCHAR szTemp2[MAX_PATH]=_TEXT("");

	switch (pSort->iSortColumn)
	{
	case COL_NAME:
	case COL_TYPE:

		if( m_PidlMgr.GetItemType(pidl1)!=m_PidlMgr.GetItemType(pidl2) ) 
		{
			Hr = (m_PidlMgr.GetItemType(pidl1)==NWS_FOLDER) ? -1 : 1;
			return pSort->bForwardSort? Hr: Hr*(-1);
		}

		if(COL_NAME == pSort->iSortColumn) //COL_NAME:
		{
			DWORD dwLen1=MAX_PATH;
			DWORD dwLen2=MAX_PATH;

			HR(m_PidlMgr.GetFullName(pidl1,szTemp1,&dwLen1));
			HR(m_PidlMgr.GetFullName(pidl2,szTemp2,&dwLen2));


			Hr= lstrcmpi(szTemp1, szTemp2);
			return pSort->bForwardSort? Hr: Hr*(-1);
		}
		else//COL_TYPE:
		{
			HR(m_PidlMgr.GetItemAttributes(pidl1,ATTR_TYPE,szTemp1));
			HR(m_PidlMgr.GetItemAttributes(pidl2,ATTR_TYPE,szTemp2));

			Hr= lstrcmpi(szTemp1, szTemp2);
			return pSort->bForwardSort? Hr: Hr*(-1);
		}
		break;
	}
	return 0;
}

STDMETHODIMP CMyVirtualFolder::CompareIDs(LPARAM lParam, 
										  LPCITEMIDLIST pidl1, 
										  LPCITEMIDLIST pidl2)
{
	TCHAR szName1[MAX_PATH]=_T("");
	TCHAR szName2[MAX_PATH]=_T("");
	DWORD dwLen1=MAX_PATH;
	DWORD dwLen2=MAX_PATH;

	m_PidlMgr.GetFullName(pidl1,szName1,&dwLen1);
	m_PidlMgr.GetFullName(pidl2,szName2,&dwLen2);

	ITEM_TYPE it1,it2;

	it1=m_PidlMgr.GetItemType(pidl1);
	it2=m_PidlMgr.GetItemType(pidl2);

	if( it1 == it2 )
		return _tcscmp(szName1, szName2);
	else
	{
		if( NWS_FOLDER == it1)//pidl1 correspond a folder object
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
}

STDMETHODIMP CMyVirtualFolder::CreateViewObject(HWND hWnd, 
												REFIID riid, 
												LPVOID* ppRetVal)
{

	VALIDATE_OUT_POINTER(ppRetVal);

	HRESULT Hr;
	if( riid == IID_IShellView ) 
	{

		CComObject<CNSFShellView> *pView;

		Hr= CComObject<CNSFShellView>::CreateInstance(&pView);
		if(FAILED(Hr))
		{
			return Hr;
		}

		pView->AddRef();

		pView->_Init(this,m_pidlPath) ;

		Hr=pView->QueryInterface(IID_IShellView, ppRetVal);
		
		pView->Release();

		return Hr;
	}

	return E_NOINTERFACE;
}

// EnumObjects() creates a COM object that implements IEnumIDList.
STDMETHODIMP CMyVirtualFolder::EnumObjects(HWND hWnd, 
								  DWORD dwFlags, 
								  LPENUMIDLIST* ppEnumIDList)
{
	ATLTRACE(_T("CMyVirtualFolder(0x%08x)::EnumObjects(dwFlags=0x%04x)\n"), this, dwFlags);

	VALIDATE_OUT_POINTER(ppEnumIDList);

	HRESULT Hr;

    // Create an enumerator with CComEnumOnCArray<> and our copy policy class.
	CComObject<CPidlEnum> *pEnum;
	Hr = CComObject<CPidlEnum>::CreateInstance(&pEnum);
	if (SUCCEEDED(Hr))
	{
		// AddRef() the object while we're using it.
		pEnum->AddRef();

		// Init the enumerator.  Init() will AddRef() our IUnknown (obtained with
		// GetUnknown()) so this object will stay alive as long as the enumerator 
		// needs access to the collection m_NEFileArray.
		Hr = pEnum->_Init(m_pidlPath, dwFlags);

		// Return an IEnumIDList interface to the caller.
		if (SUCCEEDED(Hr))
			Hr = pEnum->QueryInterface(IID_IEnumIDList, (void**)ppEnumIDList);

		pEnum->Release();
	}

	return Hr;
}
STDMETHODIMP CMyVirtualFolder::GetAttributesOf(UINT uCount, 
									  LPCITEMIDLIST pidls[], 
									  LPDWORD pdwAttribs)
{
	*pdwAttribs = (DWORD)-1;

	if(( uCount==0 )||(pidls[0]->mkid.cb == 0)) 
	{
		// Can happen on Win95. Queries the view folder.
		DWORD dwAttr;
		dwAttr |= SFGAO_FOLDER | SFGAO_HASSUBFOLDER |SFGAO_HASPROPSHEET |SFGAO_DROPTARGET;
		*pdwAttribs &= dwAttr;
	}
	else
	{
		for( UINT i=0; i<uCount; i++ ) 
		{
			DWORD dwAttr = 0;
			dwAttr |= 0;

			switch( m_PidlMgr.GetItemType(pidls[i]) ) 
    		{
			case NWS_FOLDER:

				dwAttr |=SFGAO_FOLDER;

				dwAttr |=SFGAO_CANRENAME;

				//get the full pidl
				LPITEMIDLIST   tmpPidl;
				tmpPidl = m_PidlMgr.Concatenate(m_pidlPath, pidls[i]);				

				//decide whether current folder object has subfolder
				if( TRUE == m_PidlMgr.HasSubFolder(tmpPidl) )
				{ 
					dwAttr |= SFGAO_HASSUBFOLDER;
				}
				m_PidlMgr.Delete(tmpPidl);

    			break;
    		}
			*pdwAttribs &= dwAttr;
		}
	}
	
	return S_OK;
}

#define GET_SHGDN_FOR(dwFlags)         ((DWORD)dwFlags & (DWORD)0x0000FF00)
#define GET_SHGDN_RELATION(dwFlags)    ((DWORD)dwFlags & (DWORD)0x000000FF)

//GetDisplayNameOf() Retrieves the display name for the specified file object 
//                   or subfolder, returning it in a STRRET structure. 

STDMETHODIMP CMyVirtualFolder::GetDisplayNameOf(LPCITEMIDLIST pidl, 
												DWORD dwFlags, 
												LPSTRRET lpName)
{
	ATLTRACE(_T("CMyVirtualFolder(0x%08x)::GetDisplayNameOf(uFlags=0x%04x) \n"), this, dwFlags);

	HRESULT Hr;
	TCHAR szText[MAX_PATH]=_TEXT("");
	TCHAR szTmp[MAX_PATH]=_TEXT("");
	DWORD dwLen=MAX_PATH;

	if (NULL==pidl || pidl->mkid.cb == 0)//root folder
	{
		::LoadString(_Module.GetResourceInstance(),IDS_VFNAME,szText,MAX_PATH);
	}
	else
	{
		switch(GET_SHGDN_FOR(dwFlags))
		{
		case SHGDN_FORADDRESSBAR:
		case SHGDN_NORMAL:
			switch(GET_SHGDN_RELATION(dwFlags))
			{
			case SHGDN_INFOLDER:
				m_PidlMgr.GetName(pidl,szText);
				HR(_tcslen(szText)>0);
				break;
			case SHGDN_NORMAL:
				::LoadString(_Module.GetResourceInstance(),IDS_VFNAME,szText,MAX_PATH);
				_tcscat(szText,_TEXT("\\"));
				//get the full pidl
				LPITEMIDLIST   tmpPidl;

				tmpPidl = m_PidlMgr.Concatenate(m_pidlPath, pidl);    
				HR(m_PidlMgr.GetFullName(tmpPidl,szTmp,&dwLen));
				if( dwLen > 0 )
				{
					_tcscat(szText,szTmp);
				}
				m_PidlMgr.Delete(tmpPidl);
				break;

			default:
				return E_INVALIDARG;
			}
			break;
		default:
			return E_INVALIDARG;
		}
	}

	USES_CONVERSION;
#ifdef _UNICODE
	// Allocate the wide character string 
	int cchOleStr = _tcslen(szText) + 1;  
	lpName->pOleStr = (LPWSTR)_Module.m_Allocator.Alloc(cchOleStr * sizeof(OLECHAR)); 

	if( lpName->pOleStr==NULL ) 
		return E_OUTOFMEMORY;  

	::ZeroMemory(lpName->pOleStr,cchOleStr * sizeof(OLECHAR));

	lpName->uType = STRRET_WSTR;
	_tcscpy( lpName->pOleStr, T2CW(szText));
#else
	lpName->uType = STRRET_CSTR;
	_tcscpy(lpName->cStr, T2CA(szText)); //cStr had allocated space
#endif
	return S_OK;
}

STDMETHODIMP CMyVirtualFolder::GetUIObjectOf(HWND hWnd, 
									UINT nCount, 
									LPCITEMIDLIST* pidls, 
									REFIID riid, LPUINT, 
									LPVOID* ppRetVal)
{
	//ATLTRACE(_T("CMyVirtualFolder::GetUIObjectOf <%s> #%ld '%s'...\n"), DbgGetIID(riid), nCount, PidlToString(pidls==NULL ? NULL : pidls[0]));
	VALIDATE_OUT_POINTER(ppRetVal);

	ATLASSERT(nCount>0);
	if( nCount == 0 ) 
	{
		MessageBox(NULL,_T("CMyVirtualFolder::GetUIObjectOf() nCount=0"),_T("NSF"),MB_OK);
		return E_INVALIDARG;
	}

	HRESULT Hr;

    if( riid == IID_IExtractIcon ) 
    {
	    CComObject< CExtractIcon > *pExtractIcon;
	    HR( CComObject< CExtractIcon >::CreateInstance(&pExtractIcon) );

		pExtractIcon->AddRef();

	    pExtractIcon->_Init(*pidls);
	    Hr=pExtractIcon->QueryInterface(IID_IExtractIcon, ppRetVal);

		pExtractIcon->Release();
		return Hr;
    }

    if( riid == IID_IContextMenu ) 
    {
	    if( nCount!=1 ) 
			return E_FAIL;

	    CComObject<CContextMenu>* pContextMenu;
	    HR( CComObject<CContextMenu>::CreateInstance(&pContextMenu) );

		pContextMenu->AddRef();

	    HR( pContextMenu->_Init(this, hWnd, *pidls) );
		Hr=pContextMenu->QueryInterface(IID_IContextMenu, ppRetVal);

		pContextMenu->Release();

	    return Hr;
    }

    return E_NOINTERFACE;
}


STDMETHODIMP CMyVirtualFolder::ParseDisplayName( HWND hwndOwner, 
                                             LPBC pbcReserved, 
                                             LPOLESTR lpDisplayName, 
                                             LPDWORD pdwEaten, 
                                             LPITEMIDLIST *ppidl, 
                                             LPDWORD pdwAttributes)
{
	ATLTRACENOTIMPL(_T("CMyVirtualFolder::ParseDisplayName"));
}

STDMETHODIMP CMyVirtualFolder::SetNameOf(HWND, 
								LPCITEMIDLIST pidlOld, 
								LPCOLESTR pstrName, 
								DWORD, 
								LPITEMIDLIST* ppidlOut)
{
	ATLTRACE(_T("CMyVirtualFolder::SetNameOf\n"));

    USES_CONVERSION;
    if( ppidlOut!=NULL ) 
		*ppidlOut=NULL;
 
    DWORD dwAttr = SFGAO_CANRENAME;
    GetAttributesOf(1, &pidlOld, &dwAttr);
    if( (dwAttr & SFGAO_CANRENAME)==0 ) 
		return E_FAIL;
 
    TCHAR szNewName[MAX_PATH+1];
    if( wcslen(pstrName)>MAX_PATH ) 
		return E_FAIL;
    _tcscpy( szNewName, OLE2CT(pstrName));

	//1.Create new PIDL for the new display name
	LPITEMIDLIST  pidlNew = NULL;
	ITEM_TYPE iItemType = NWS_FOLDER;
	pidlNew=m_PidlMgr.Create(iItemType,szNewName);
	if(!pidlNew)
		return E_FAIL;

	//2.Replace "New Folder" to szNewName in Configuration File
	HRESULT Hr;
	HR(ReplaceNameInCfgFile(m_pidlPath,szNewName));

	//3.Notify Shell so it can rename the item in the Explorer tree
    LPITEMIDLIST pidlFullPath;
    pidlFullPath=m_PidlMgr.Concatenate(_Module.m_pidlNSFROOT,m_pidlPath);

	LPITEMIDLIST pidlFullOld,pidlFullNew;
    pidlFullOld=m_PidlMgr.Concatenate(pidlFullPath,pidlOld);
	pidlFullNew=m_PidlMgr.Concatenate(pidlFullPath,pidlNew);

	::SHChangeNotify(SHCNE_RENAMEFOLDER, SHCNF_IDLIST |SHCNF_FLUSH, pidlFullOld, pidlFullNew);
	::SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_IDLIST | SHCNF_FLUSH, pidlFullPath, NULL);
	
	m_PidlMgr.Delete(pidlFullPath);
	m_PidlMgr.Delete(pidlFullOld);
	m_PidlMgr.Delete(pidlFullNew);

	*ppidlOut = pidlNew;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// IQueryInfo
//

//GetInfoFlags() Retrieves the information flags for an item. 
//				 This method is not currently used. 
STDMETHODIMP CMyVirtualFolder::GetInfoFlags(DWORD* pdwFlags)
{
	ATLTRACENOTIMPL(_T("CMyVirtualFolder::GetInfoFlags"));
}

//GetInfoTip() Retrieves the info tip text information for an item. 
STDMETHODIMP CMyVirtualFolder::GetInfoTip(DWORD, LPWSTR* ppwszTip)
{
   VALIDATE_OUT_POINTER(ppwszTip);
   CComBSTR bstr;
   bstr.LoadString(IDS_INFOTIP);
   *ppwszTip = (LPWSTR)_Module.m_Allocator.Alloc((bstr.Length()+1) * sizeof(WCHAR)); 
   if( *ppwszTip==NULL ) 
	   return E_OUTOFMEMORY;  

   ::ZeroMemory(*ppwszTip,(bstr.Length()+1) * sizeof(WCHAR));

   wcscpy(*ppwszTip, bstr);
   return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// User defined functions

/////////////////////////////////////////////////////////////////////////////
// Menu related functions
// _DoDelete
// pidl: a simple PIDL
HRESULT CMyVirtualFolder::_DoDelete(LPITEMIDLIST pidl)
{
	HRESULT Hr;
    int ret;

    ret=MessageBox(NULL,_TEXT("This Delete operation will direct to the deleted item can't be recover,\nAre you sure to delete the selected item?"),
    					_TEXT("Notice"),MB_OKCANCEL|MB_ICONWARNING);
    if(ret==IDOK)
    {
		//1.Delete corresponding item from configuration file
		HR(DeleteItemInCfgFile(m_pidlPath,pidl));
        
		//2. Refresh the Tree view
		LPITEMIDLIST tmpPidl1,tmpPidl2;
		tmpPidl1=m_PidlMgr.Concatenate(_Module.m_pidlNSFROOT,m_pidlPath);
		tmpPidl2=m_PidlMgr.Concatenate(tmpPidl1,pidl);

		if( NWS_FOLDER == (m_PidlMgr.GetItemType(pidl)) )

			::SHChangeNotify( SHCNE_RMDIR , SHCNF_IDLIST | SHCNF_FLUSH, tmpPidl2, NULL);
		else
			::SHChangeNotify( SHCNE_DELETE , SHCNF_IDLIST | SHCNF_FLUSH, tmpPidl2, NULL);

		// Ask Shell to refresh current directory
		::SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_IDLIST | SHCNF_FLUSH, tmpPidl1, NULL);

		m_PidlMgr.Delete(tmpPidl1);
		m_PidlMgr.Delete(tmpPidl2);
		
		//3. Refresh Shell view Exist in All Explorer Windows.
		RefreshShellViewWndsExcept( NULL);

	}
	return S_OK;
}

// Check if the Window handle belongs to Shell view.
// We can do this because we know the Windows classname of the view window.
BOOL CMyVirtualFolder::_IsViewWindow(HWND hWnd)
{
   if( !::IsWindow(hWnd) ) 
	   return FALSE;

    TCHAR szViewClassName[64] = TEXT("");
    ::LoadString(_Module.GetResourceInstance(), IDS_CLASSNAME, szViewClassName, 64);

	TCHAR szClassName[64] = { 0 };
	::GetClassName(hWnd, szClassName, lengthof(szClassName) - 1);

	return( _tcsicmp(szClassName, szViewClassName)==0 );
}
BOOL CMyVirtualFolder::_IsNSFROOT()
{
	return ( m_pidlPath == NULL );
}
