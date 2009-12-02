//========================================================================================
//
// Module:			Util.cpp
// Author:          Zeng Xi
// Creation Date:	10.15.2005
//
//========================================================================================

#include "stdafx.h"
#include "Util.h"
#include "CNWSPidlMgr.h"
#include "resource.h"

////////////////////////////////////////////
// Debug functions add here
#ifdef _WRITETRACEINFO_ON

TCHAR trString[MAX_STRLEN]=TEXT("");
FILE *fpDbg = NULL;
UINT trNum = 0;

void trInit() 
{
	fpDbg = _tfopen(_TEXT("C:\\NWS_trace.txt"),_TEXT("a+"));
	trNum=0;
}

void trPuts(LPTSTR str) 
{
	if(fpDbg == NULL)
		trInit();
	if(fpDbg) 
	{
		_ftprintf(fpDbg,_TEXT("No:%d -- %s\n\n"),trNum++,str);
		fflush(fpDbg);
	}
}

void trFinal() 
{
	if(fpDbg)
		fclose(fpDbg);
}

void trPrintHex(BYTE *input,int len,LPTSTR str)
{
	int i,j;
	i=0;
	j=0;
	for(i=0;i<len;i++)
	{
		j+=_stprintf(str+j,_TEXT("%02x "),input[i]);
	}
}

void trPrintFullPidl(LPCITEMIDLIST pidl,LPTSTR str)
{
	ATLASSERT(str != NULL);
	if(str==NULL)
	{
		MessageBox(NULL,_T("trPrintFullPidl() str=NULL"),_T("NSE"),MB_OK);
	}
	CNWSPidlMgr pidlMgr;
	TCHAR szTmp[MAX_PATH]=_TEXT("");

	if( pidl == NULL )
	{
		_tcscat( str, _TEXT("<NULL>"));
		return;
	}

	LPITEMIDLIST pidlTemp = (LPITEMIDLIST) pidl;

	while (pidlTemp->mkid.cb != 0)
	{
		_tcsnset(szTmp,0,MAX_PATH);
		trPrintHex((BYTE *)pidlTemp,(int)pidlTemp->mkid.cb,szTmp);
		_tcscat(str,szTmp);
		pidlTemp = pidlMgr.GetNextItem(pidlTemp);
	}  
	_tcscat(str,_TEXT("00 00"));
	return ;
}

#endif //_WRITETRACEINFO_ON

//////////////////////////////////////////////////////
// global tool functions
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

BOOL CALLBACK RefreshShellView( HWND hWnd, LPARAM lParam )
{
	if( hWnd ) 
	{
		TCHAR szClassName[MAX_PATH]=_T("");
		DWORD dwLen=MAX_PATH;
		GetClassName(hWnd,szClassName,dwLen);
		if( (_tcscmp(szClassName,_T("ExploreWClass"))==0) ||
			(_tcscmp(szClassName,_T("CabinetWClass"))==0) )
		{
			HWND hwndShellView = FindWindowEx(hWnd,NULL,_T("NSFViewClass"),NULL);
			if(hwndShellView !=NULL)
			{			
				HWND hwndExcept =(HWND)lParam;
				if( ( hwndExcept!=NULL && hwndExcept!=hwndShellView) ||
					( hwndExcept==NULL) )
					::SendMessage(hwndShellView,WM_COMMAND,ID_VIEW_REFRESH,0);
			}
		}

    }
	return( TRUE );
}

void RefreshShellViewWndsExcept(HWND hwndExcept)
{
	for(; !EnumWindows( (WNDENUMPROC) RefreshShellView,(LPARAM) hwndExcept ); ); // continue looping until done
}

void OpenDirInCfgFile(IN LPCITEMIDLIST pidlCurDir, 
                      OUT LPTSTR szPath,
                      OUT LPTSTR szCfgFile)         
{
	_tcscpy(szCfgFile,_Module.m_szInstallPath);
	_tcscat(szCfgFile,_T("\\NSExtAddDelFld.cfg"));

	CNWSPidlMgr pidlMgr;
	DWORD dwLen=MAX_PATH;
	if( (NULL==pidlCurDir)||(0 == pidlCurDir->mkid.cb))
		_tcscpy(szPath,_T("ROOT"));
	else
		pidlMgr.GetFullName(pidlCurDir,szPath,&dwLen);
}

HRESULT DeleteItemInCfgFile(IN LPCITEMIDLIST pidlCurDir, IN LPITEMIDLIST pidlItem)
{
	HRESULT Hr=S_OK;
	TCHAR szCfgFile[MAX_PATH]=_TEXT("");
	TCHAR szSection[MAX_PATH]=_TEXT("");
	OpenDirInCfgFile(pidlCurDir,szSection,szCfgFile);

    //3.Get the name of pidl
	CNWSPidlMgr pidlMgr;
    TCHAR szName[MAX_PATH]=TEXT("");
    HR(pidlMgr.GetName(pidlItem,szName));

    //4.Get the type of pidl
    ITEM_TYPE iItemType;
    iItemType = pidlMgr.GetItemType(pidlItem);
    
    DWORD dwCheck;
    TCHAR tmpStr[MAX_PATH]=TEXT("");
    DWORD dwLen=MAX_PATH;
    if( iItemType == NWS_FOLDER )
    {
        dwCheck = GetPrivateProfileString( szSection,_T("dir"),_T(""),tmpStr,dwLen, szCfgFile);
    }
    else if( iItemType == NWS_FILE )
    {
        dwCheck = GetPrivateProfileString( szSection,_T("file"),_T(""),tmpStr,dwLen, szCfgFile);
    }

    //5. delete the name of pidl if it exist in tmpStr and form a newstr
    TCHAR szNewStr[MAX_PATH]=_T("");
    if(_tcsstr(tmpStr,szName)==NULL) //basiclly filter
    {
        MessageBox(NULL,_T("_DoDelete: the pidl to be deleted can't be found in current folder!"),_T("Error"),MB_OK);
        return E_FAIL;
    }

    INT iNameLen= _tcslen(szName);
    INT iStrLen = _tcslen(tmpStr);
    INT iPos = 0;
    TCHAR *pSplitChr=NULL;
    BOOL bFound=FALSE;
    while(iPos + iNameLen <= iStrLen)
    {
        if( iPos + iNameLen == iStrLen) 
        {
            if(_tcsncmp(tmpStr+iPos,szName,iNameLen) == 0) //is the tail item
            {
                //iPos=0:is the only one item; iPos!=0:has more than one items
                if(iPos != 0) 
                    _tcsncpy(szNewStr,tmpStr,iPos-1);
                bFound=TRUE;
                break;
            }
        }
        else
        {
            pSplitChr=_tcschr(tmpStr+iPos,_T(';'));
            if(pSplitChr == NULL)
                break;
            if( ((pSplitChr-(tmpStr+iPos)) == iNameLen) && 
                (_tcsncmp(tmpStr+iPos,szName,iNameLen) == 0) )
            {
                _tcsncpy(szNewStr,tmpStr,iPos);
                _tcscat(szNewStr,pSplitChr+1);
                bFound=TRUE;
                break;
            }
            else
            {
                iPos=pSplitChr-tmpStr+1;
            }
        }
    }
    if(bFound == FALSE)
    {
        MessageBox(NULL,_T("_DoDelete: pidl ready to delete can't be find in current folder!"),_T("Error"),MB_OK);
        return E_FAIL;
    }

    //6. delete the old keyvalue and insert the new keyvalue
    if( iItemType == NWS_FOLDER )
    {
        if(_tcslen(szNewStr)!=0)
            dwCheck = WritePrivateProfileString( szSection,_T("dir"),szNewStr,szCfgFile);
        else
            dwCheck = WritePrivateProfileString( szSection,_T("dir"),NULL,szCfgFile);


        //Delete the deleted folder's corresponding section from cfg file
        //Notes:
        //To make this sample project perfect,indeed,we need to
        //recursively delete all the sub folders corresponding sections
        //from configuration file, to do this will spend lots.
        //
        //However, for our purpose is to describe how to delete/create folder,
        //for simplicity, we only delete the section which directly
        //referred by the deleted folder object from configuration file.

        if(_tcscmp(szSection,_T("ROOT"))==0)
        {
            _tcsnset(szSection,0,MAX_PATH);
            _tcscpy(szSection,szName);
        }
        else
        {
            _tcscat(szSection,_T("\\"));
            _tcscat(szSection,szName);
        }
        WritePrivateProfileString(szSection,NULL,NULL,szCfgFile);
    }
    else if( iItemType == NWS_FILE )
    {
        if(_tcslen(szNewStr)!=0)
            dwCheck = WritePrivateProfileString( szSection,_T("file"),szNewStr,szCfgFile);
        else
            dwCheck = WritePrivateProfileString( szSection,_T("file"),NULL,szCfgFile);
    }
    return Hr;
}

void CreateFolderInCfgFile(IN LPCITEMIDLIST pidlCurDir, OUT LPTSTR szTempNewName)
{

	TCHAR szCfgFile[MAX_PATH]=_T("");
	TCHAR szSection[MAX_PATH]=_T("");
	OpenDirInCfgFile(pidlCurDir,szSection,szCfgFile);

	TCHAR szDirKeyValue[MAX_PATH]=_T("");
	DWORD dwLen=MAX_PATH;
	GetPrivateProfileString(szSection,_T("dir"),_T(""),szDirKeyValue,dwLen, szCfgFile);

	TCHAR szTempName[MAX_PATH]=_T("");
	_tcscpy(szTempName,_T("New Folder"));
	UINT iIdx=1;
	TCHAR szKeyValue[MAX_PATH]=_T("");
	
	// Back up szDirKeyValue for _tcstok will change szDirKeyValue's value
	_tcscpy(szKeyValue,szDirKeyValue); 
	while(NotUniqueName(szTempName,szKeyValue)==TRUE)
	{
		iIdx++;
		_stprintf(szTempName,_T("New Folder(%d)"),iIdx);
		_tcscpy(szKeyValue,szDirKeyValue);
	}

	if(_tcslen(szDirKeyValue) == 0)
		_tcscpy(szDirKeyValue,szTempName);
	else
	{
		_tcscat(szDirKeyValue,_T(";"));
		_tcscat(szDirKeyValue,szTempName);
	}

	WritePrivateProfileString(szSection,_T("dir"),szDirKeyValue,szCfgFile);
	::CopyMemory(szTempNewName,szTempName,_tcslen(szTempName)*sizeof(TCHAR));
}

HRESULT ReplaceNameInCfgFile(IN LPCITEMIDLIST pidlCurDir, IN LPTSTR szNewName)
{
	TCHAR szCfgFile[MAX_PATH]=_TEXT("");
	TCHAR szSection[MAX_PATH]=_TEXT("");
	OpenDirInCfgFile(pidlCurDir,szSection,szCfgFile);

	TCHAR szKeyValue[MAX_PATH]=_TEXT("");
	DWORD dwLen=MAX_PATH;
	GetPrivateProfileString(szSection,_T("dir"),_T(""),szKeyValue,dwLen,szCfgFile);
	if(_tcslen(szKeyValue)==0)
		return E_FAIL;
	
	//last item in keyvalue is the new created item.
	TCHAR szNewKeyValue[MAX_PATH]=_TEXT("");
	TCHAR *pNewFolder=_tcsrchr(szKeyValue,_T(';'));
	if(pNewFolder==NULL) //current folder has only one item
		_tcscpy(szNewKeyValue,szNewName);
	else
	{	
		_tcsncpy(szNewKeyValue,szKeyValue,pNewFolder-szKeyValue+1);
		_tcscat(szNewKeyValue,szNewName);
	}
	WritePrivateProfileString(szSection,_T("dir"),szNewKeyValue,szCfgFile);

	return S_OK;
}
BOOL NotUniqueName(IN LPTSTR szObjName, IN LPTSTR szKeyValue)
{
    TCHAR seps[]= ";";
    TCHAR *token;    

    /* Establish string and get the first token: */
    token = _tcstok( szKeyValue, seps );
    while( token != NULL )
    {
        /* While there are tokens in "string" */
        //printf( " %s\n", token );
		if(_tcscmp(token,szObjName)==0)
		{
			return TRUE;
		}
        /* Get next token: */
        token = _tcstok( NULL, seps );
    }
	return FALSE;
}
