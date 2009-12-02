#include "stdafx.h"
#include "Util.h"
#include "CNWSPidlMgr.h"
#include "resource.h"

////////////////////////////////////////////
// Debug functions add here

//////////////////////////////////////////////////////
// Other functions
HRESULT GetGlobalSettings()
{
	HRESULT hr = S_OK;

    static TCHAR szValue[MAX_PATH];
	//TCHAR szValue[MAX_PATH]=TEXT("");

	USES_CONVERSION;
	LPOLESTR wszNSECLSID = NULL;
	hr = ::StringFromCLSID(CLSID_MyVirtualFolder, &wszNSECLSID);

    CRegKey keyRoot,key;
	DWORD dwType, dw = sizeof(szValue);
    keyRoot.Open(HKEY_CLASSES_ROOT, _T("CLSID"), KEY_READ);

	TCHAR szSubKey[MAX_PATH]=TEXT("");
	_tcscpy(szSubKey,OLE2T(wszNSECLSID));
	_tcscat(szSubKey,_T("\\InProcServer32"));

    if( key.Open(keyRoot, szSubKey, KEY_READ) == S_OK ) 
	{
        *szValue = _T('\0');
        if(::RegQueryValueEx(key.m_hKey, NULL, \
			NULL, &dwType, (LPBYTE)szValue, &dw) == ERROR_SUCCESS)
		{
			//_tprintf("szValue=%s\n",szValue);
			TCHAR* pch = _tcsrchr(szValue, '\\');
			::CopyMemory(_Module.m_szInstallPath,szValue,pch-szValue);
			
			CoTaskMemFree(wszNSECLSID);
			key.Close();
			keyRoot.Close();
			return S_OK;
		}
		else
		{
			CoTaskMemFree(wszNSECLSID);
			key.Close();
			keyRoot.Close();
			return E_FAIL;
		}
    }
	else
	{
		CoTaskMemFree(wszNSECLSID);
		keyRoot.Close();
		return E_FAIL;
	}
	
}
BOOL CreateImageLists()
{
   // Set the small image list
   int nSmallCx = ::GetSystemMetrics(SM_CXSMICON);
   int nSmallCy = ::GetSystemMetrics(SM_CYSMICON);

   HIMAGELIST imgSmall = ImageList_Create(nSmallCx, nSmallCy, ILC_COLOR32 | ILC_MASK, 4, 0);
   if( imgSmall==NULL ) return FALSE;

   // Set the large image list
   int nLargeCx = ::GetSystemMetrics(SM_CXICON);
   int nLargeCy = ::GetSystemMetrics(SM_CYICON);
   HIMAGELIST imgLarge = ImageList_Create(nLargeCx, nLargeCy, ILC_COLOR32 | ILC_MASK, 4, 0);
   if( imgLarge==NULL ) return FALSE;

   HICON hIcon;

   // Load NSF Root icon
   hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
                              MAKEINTRESOURCE(IDI_NSFROOT),
                              IMAGE_ICON, 
                              nSmallCx, nSmallCy, 
                              LR_DEFAULTCOLOR);
   ImageList_AddIcon(imgSmall, hIcon);
   hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
                              MAKEINTRESOURCE(IDI_NSFROOT),
                              IMAGE_ICON, 
                              nLargeCx, nLargeCy, 
                              LR_DEFAULTCOLOR);
   ImageList_AddIcon(imgLarge, hIcon);

   // Load Folder icon
   hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
                              MAKEINTRESOURCE(IDI_FOLDER),
                              IMAGE_ICON, 
                              nSmallCx, nSmallCy, 
                              LR_DEFAULTCOLOR);
   ImageList_AddIcon(imgSmall, hIcon);
   hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
                              MAKEINTRESOURCE(IDI_FOLDER),
                              IMAGE_ICON, 
                              nLargeCx, nLargeCy, 
                              LR_DEFAULTCOLOR);
   ImageList_AddIcon(imgLarge, hIcon);

   // Load the Open Folder icon

   hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
                              MAKEINTRESOURCE(IDI_FOLDER_OPEN),
                              IMAGE_ICON, 
                              nSmallCx, nSmallCy, 
                              LR_DEFAULTCOLOR);
   ImageList_AddIcon(imgSmall, hIcon);
   hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
                              MAKEINTRESOURCE(IDI_FOLDER_OPEN),
                              IMAGE_ICON, 
                              nLargeCx, nLargeCy, 
                              LR_DEFAULTCOLOR);
   ImageList_AddIcon(imgLarge, hIcon);

   // Load File icon (use the icon for this DLL)
   hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
                              MAKEINTRESOURCE(IDI_FILE),
                              IMAGE_ICON, 
                              nSmallCx, nSmallCy, 
                              LR_DEFAULTCOLOR);
   ImageList_AddIcon(imgSmall, hIcon);
   hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), 
                              MAKEINTRESOURCE(IDI_FILE),
                              IMAGE_ICON, 
                              nLargeCx, nLargeCy, 
                              LR_DEFAULTCOLOR);
   ImageList_AddIcon(imgLarge, hIcon);

   //save the created imagelist
   _Module.m_ImageLists.m_hImageListSmall = imgSmall;
   _Module.m_ImageLists.m_hImageListLarge = imgLarge;

   return TRUE;
}
// Sort List View item callback function.
int CALLBACK ListViewSortFuncCB(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    //return CMyVirtualFolder::_CompareIDs(lParamSort, (LPCITEMIDLIST)lParam1, (LPCITEMIDLIST)lParam2);  
    CListSortInfo* pSort = (CListSortInfo*) lParamSort;

    _ASSERT(NULL != pSort);

    return (int) pSort->pNSFolder->CompareIDs( lParamSort, (LPITEMIDLIST) lParam1,
                                                   (LPITEMIDLIST) lParam2 );
}
int WideCharToLocal(LPTSTR pLocal, LPWSTR pWide, DWORD dwChars)
{
*pLocal = 0;

#ifdef UNICODE
lstrcpyn(pLocal, pWide, dwChars);
#else
WideCharToMultiByte( CP_ACP, 
                     0, 
                     pWide, 
                     -1, 
                     pLocal, 
							dwChars, 
                     NULL, 
                     NULL);
#endif

return lstrlen(pLocal);
}


int LocalToWideChar(LPWSTR pWide, LPTSTR pLocal, DWORD dwChars)
{
*pWide = 0;

#ifdef UNICODE
lstrcpyn(pWide, pLocal, dwChars);
#else
MultiByteToWideChar( CP_ACP, 
                     0, 
                     pLocal, 
                     -1, 
                     pWide, 
                     dwChars); 
#endif

return lstrlenW(pWide);
}

BOOL LocaleIsChinese()
{
	LANGID LanguageId=GetSystemDefaultLangID();//=0x804ÊÇÖÐÎÄ
	if(LanguageId==0x0804)
		return TRUE;
	if(LanguageId==0x0404)
		return TRUE;
	if(LanguageId==0x0c04)
		return TRUE;
	if(LanguageId==0x1004)
		return TRUE;
	return FALSE;
}

void RefreshExplorerWindow()
{
	HWND hwndShell=::FindWindow(_TEXT("ExploreWClass"),NULL);
	//ATLASSERT(hwndShell);
	if(hwndShell!=NULL)
	{
		PostMessage(hwndShell,WM_KEYDOWN,VK_F5,0);
		PostMessage(hwndShell,WM_KEYUP,VK_F5,0xC0000001);
	}
	else
	{


	}
}