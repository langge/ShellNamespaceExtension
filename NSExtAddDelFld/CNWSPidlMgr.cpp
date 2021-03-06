//========================================================================================
//
// Module:			CNWSPidlMgr.cpp
// Author:          Zeng Xi
// Creation Date:	11.15.2004
//
//========================================================================================

#include "stdafx.h"
#include "CNWSPidlMgr.h"
#include "Util.h"

//==================================================================
LPITEMIDLIST CNWSPidlMgr::Concatenate(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
LPITEMIDLIST   pidlNew;
UINT           cb1 = 0, 
               cb2 = 0;

	//are both of these NULL?
	if(!pidl1 && !pidl2)
	   return NULL;

	//if pidl1 is NULL, just return a copy of pidl2
	if(!pidl1)
	{
	   pidlNew = Copy(pidl2);
	   return pidlNew;
	}

	//if pidl2 is NULL, just return a copy of pidl1
	if(!pidl2)
	{
	   pidlNew = Copy(pidl1);
	   return pidlNew;
	}

	cb1 = GetByteSize(pidl1) - sizeof(ITEMIDLIST);
	cb2 = GetByteSize(pidl2);

	//create the new PIDL
	pidlNew = (LPITEMIDLIST)_Module.m_Allocator.Alloc(cb1 + cb2);
	if(pidlNew)
	{
		::ZeroMemory(pidlNew,cb1+cb2);

		//copy the first PIDL
		::CopyMemory(pidlNew, pidl1, cb1);
		//copy the second PIDL
		::CopyMemory(((LPBYTE)pidlNew) + cb1, pidl2, cb2);
	}
	return pidlNew;
}

void CNWSPidlMgr::Delete(LPITEMIDLIST pidl)
{
	if (pidl)
	{
		_Module.m_Allocator.Free(pidl);
	}
}

LPITEMIDLIST CNWSPidlMgr::GetNextItem(LPCITEMIDLIST pidl)
{
	ATLASSERT(pidl != NULL);
	if (!pidl)
		return NULL;
	return (LPITEMIDLIST)(LPBYTE)(((LPBYTE)pidl) + pidl->mkid.cb);
}

LPITEMIDLIST CNWSPidlMgr::GetLastItem(LPCITEMIDLIST pidl)
{
	LPITEMIDLIST pidlLast = NULL;

	//get the PIDL of the last item in the list
	if(pidl)
	while ( pidl->mkid.cb)
	{
		pidlLast = (LPITEMIDLIST)pidl;
		pidl = GetNextItem(pidl);
	}

	return pidlLast;
}

LPITEMIDLIST CNWSPidlMgr::Copy(LPCITEMIDLIST pidlSrc)
{
	LPITEMIDLIST pidlTarget = NULL;
	UINT Size = 0;

	if (pidlSrc == NULL)
		return NULL;

	// Allocate memory for the new PIDL.
	Size = GetByteSize(pidlSrc);
	pidlTarget = (LPITEMIDLIST) _Module.m_Allocator.Alloc(Size);

	if (pidlTarget == NULL)
		return NULL;

	// Copy the source PIDL to the target PIDL.
	::ZeroMemory(pidlTarget,Size);
	::CopyMemory(pidlTarget, pidlSrc, Size);

	return pidlTarget;
}

UINT CNWSPidlMgr::GetByteSize(LPCITEMIDLIST pidl)
{
	UINT Size = 0;
	LPITEMIDLIST pidlTemp = (LPITEMIDLIST) pidl;

	ATLASSERT(pidl != NULL);
	if (!pidl)
		return 0;

	while (pidlTemp->mkid.cb != 0)
	{
		Size += pidlTemp->mkid.cb;
		pidlTemp = GetNextItem(pidlTemp);
	}  

	// add the size of the NULL terminating ITEMIDLIST
	Size += sizeof(ITEMIDLIST);

	return Size;
}

bool CNWSPidlMgr::IsSingle(LPCITEMIDLIST pidl)
{
	LPITEMIDLIST pidlTemp = GetNextItem(pidl);
	return pidlTemp->mkid.cb == 0;
}

BOOL CNWSPidlMgr::HasSubFolder(LPCITEMIDLIST pidl)
{
    TCHAR szPath[MAX_PATH]=_TEXT("");
    DWORD dwLen=MAX_PATH;
    
    GetFullName(pidl,szPath,&dwLen);
	if(dwLen>0)
	{
		//return HasSubFolder(szPath);
		TCHAR szCfgFile[MAX_PATH]=_TEXT("");
		_tcscpy(szCfgFile,_Module.m_szInstallPath);
		_tcscat(szCfgFile,_T("\\NSExtAddDelFld.cfg"));

		TCHAR tmpStr[MAX_PATH]=_TEXT("");    
		GetPrivateProfileString(szPath,_T("dir"),_T("NotFound"),tmpStr,dwLen, szCfgFile);
    
		if( (_tcscmp(tmpStr,_T("NotFound"))==0 ) || (_tcslen(tmpStr)==0 ) )
		{
			return FALSE;       
		}
		else 
		{
			return TRUE;
		}
	}
	else
		return FALSE;
}

//get the pathname releative to NSF's root folder of folder object use its complex PIDL
HRESULT CNWSPidlMgr::GetFullName(LPCITEMIDLIST pidl,LPTSTR szFullName,DWORD *pdwLen)
{
	if(!pdwLen)
		return E_FAIL;

	*pdwLen=0;

	if(!pidl) //NSE's root folder
	{
		return S_OK;
	}

	LPITEMIDLIST pidlTemp = (LPITEMIDLIST) pidl;

	while (pidlTemp->mkid.cb != 0)
	{
		TCHAR szTemp[MAX_PATH]=_TEXT("");
		GetName(pidlTemp,szTemp);

		if( szFullName)
		{
			if(0 == *pdwLen)
			{
				_tcscpy(szFullName,szTemp);
			}
			else
			{
				_tcscat(szFullName,_TEXT("\\"));
				_tcscat(szFullName,szTemp);
			}
			*pdwLen =_tcslen(szFullName);
		}
		else
		{
			*pdwLen+=_tcslen(szTemp);
		}
		pidlTemp = GetNextItem(pidlTemp);
	}  

	*pdwLen += 1;

	return S_OK;
}


HRESULT CNWSPidlMgr::GetItemAttributes(LPCITEMIDLIST pidl,USHORT iAttrNum,LPTSTR pszAttrOut)
{
	TCHAR szTemp[MAX_PATH]=TEXT("");

	switch (iAttrNum)
	{
	case ATTR_TYPE :
		{
			LPITEMIDLIST pidlTemp;
			pidlTemp=GetLastItem(pidl);

			if( NWS_FOLDER == GetItemType(pidlTemp))
			{
				_tcscpy(pszAttrOut,_TEXT("Folder"));
			}
			else
			{
				GetName(pidlTemp,szTemp);

				SHFILEINFO sfi;  
				ZeroMemory(&sfi,sizeof(sfi));

				if (SHGetFileInfo(szTemp, 0, &sfi, sizeof(sfi),  \
						SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME ) ) 
					_tcscpy(pszAttrOut,sfi.szTypeName);
				else
				{
					_tcscpy(pszAttrOut,_TEXT("File"));
				}
			}
		}
		break;

	default :
		return E_INVALIDARG;
	}
	return S_OK;
}

//=====================================================================
LPPIDLDATA CNWSPidlMgr::GetDataPointer(LPCITEMIDLIST pidl)
{
	if(!pidl)
		return NULL;

	return (LPPIDLDATA)(pidl->mkid.abID);
}
LPITEMIDLIST CNWSPidlMgr::Create(ITEM_TYPE iItemType,LPTSTR pszName)
{
	USHORT TotalSize = sizeof(ITEMIDLIST) + sizeof(ITEM_TYPE) + (_tcslen(pszName)+1)*sizeof(TCHAR);

	// Also allocate memory for the final null SHITEMID.
	LPITEMIDLIST pidlNew = NULL;
	pidlNew = (LPITEMIDLIST) _Module.m_Allocator.Alloc(TotalSize + sizeof(ITEMIDLIST));
	if (pidlNew)
	{
		//::ZeroMemory(pidlNew,TotalSize + sizeof(USHORT));
		::ZeroMemory(pidlNew,TotalSize + sizeof(ITEMIDLIST));

		LPITEMIDLIST pidlTemp = pidlNew;

		// Prepares the PIDL to be filled with actual data
		pidlTemp->mkid.cb = (USHORT)TotalSize;

		LPPIDLDATA     pData;
		pData = GetDataPointer(pidlTemp);
		// Fill the PIDL
		pData->type = iItemType;
		::CopyMemory(pData->szName, pszName, (_tcslen(pszName)+1) * sizeof(TCHAR));

		// Set an empty PIDL at the end
		//set the NULL terminator to 0
		pidlTemp = GetNextItem(pidlTemp);
		pidlTemp->mkid.cb = 0;
		pidlTemp->mkid.abID[0] = 0;
	}
	return pidlNew;
}
ITEM_TYPE CNWSPidlMgr::GetItemType(LPCITEMIDLIST pidl)
{
	LPITEMIDLIST  pidlTemp = GetLastItem(pidl);
	LPPIDLDATA pData;
	pData = GetDataPointer(pidlTemp);
	return pData->type;
}

HRESULT CNWSPidlMgr::GetName(LPCITEMIDLIST pidl,LPTSTR pszName)
{
	LPPIDLDATA pData;
	pData = GetDataPointer(pidl);
	_tcscpy(pszName,pData->szName);
	return S_OK;
}
