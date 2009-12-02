//#ifndef __SHELLBROWSE_H__20020531
//#define __SHELLBROWSE_H__20020531
#pragma once

#include "Shlobj.h"

typedef BOOL (CALLBACK *FOLDERCONTENTPROC)(LPCSTR, LPITEMIDLIST, DWORD);

class CShell {
public:
	CShell() {}

	//////////////////////////////////////////////////////////////////////////////////
	// Function Name  : BrowseForFolder
	// Description      : ���� Ž���� �����Ѵ�.
	// Return type    : static int (IDOK or IDCANCEL)
	// Arguments    : 
	//      CString& sPath    : �ʱ� ��� & ���� ���
	//      CString sTitle    : "Choose a folder:"�� ��ü�� ����
	//      HWND hParent    : �θ� �ڵ�
	//      UINT uiFlag      : ���� �÷���
	//      LPITEMIDLIST pidlRoot:���� ��θ� ��Ʈ�� ��´�.
	//      UINT uiSpecial    : Ư�� ������ ��Ʈ�� ��´�.pidlRoot���� �켱�Ѵ�.
	//--------------------------------------------------------------------------------
	// uiFlag ����
	//    BIF_RETURNONLYSDIRS    : ���� �ý��� ���丮�� ������ ��쿡�� 
	//                  OK ��ư Ȱ��ȭ
	//    BIF_DONTGOBELOWDOMAIN  : ��Ʈ��ũ ������ �������� �ʴ´�.
	//    BIF_STATUSTEXT      : ���� Ȱ��ȭ ��Ų��.(���⼭�� ��ü ��� ���)
	//    BIF_EDITBOX        : ������ �������� �����ϴ� ������ �ڽ� ���
	//                  (�� ���� 4.71�̻�)
	//    BIF_VALIDATE      : BIF_EDITBOX�� ���� ����.
	//                  �߸��� �����̳� ���� �̸��� �����͹ڽ��� �Է���
	//                  ��� ������ ������ �ִ�.(�� ���� 4.71�̻�)
	//    BIF_BROWSEFORCOMPUTER  : ����ڰ� ��ǻ�� �̸��� �����ϰ� �Ѵ�.
	//    BIF_BROWSEFORPRINTER  : ������ �̸��� �����ϰ� �Ѵ�.
	//    BIF_BROWSEINCLUDEFILES  : Ʈ�� �信 ���ϱ��� ��Ÿ����.
	//    �� �ܿ��� ������ ���� �÷��װ� �����Ѵ�.(���� ����)
	//    BIF_RETURNFSANCESTORS, 
	//    BIF_NEWDIALOGSTYLE, BIF_USENEWUI, BIF_SHAREABLE, BIF_BROWSEINCLUDEURLS
	// uiSpecial ����
	//    CSIDL_FLAG_CREATE
	//    CSIDL_ADMINTOOLS 
	//    CSIDL_ALTSTARTUP 
	//    CSIDL_APPDATA 
	//    CSIDL_BITBUCKET 
	//    CSIDL_COMMON_ADMINTOOLS
	//    CSIDL_COMMON_ALTSTARTUP 
	//    CSIDL_COMMON_APPDATA
	//    CSIDL_COMMON_DESKTOPDIRECTORY 
	//    CSIDL_COMMON_DOCUMENTS 
	//    CSIDL_COMMON_FAVORITES 
	//    CSIDL_COMMON_PROGRAMS 
	//    CSIDL_COMMON_STARTMENU 
	//    CSIDL_COMMON_STARTUP 
	//    CSIDL_COMMON_TEMPLATES 
	//    CSIDL_CONTROLS 
	//    CSIDL_COOKIES
	//    CSIDL_DESKTOP 
	//    CSIDL_DESKTOPDIRECTORY 
	//    CSIDL_DRIVES 
	//    CSIDL_FAVORITES 
	//    CSIDL_FONTS 
	//    CSIDL_HISTORY 
	//    CSIDL_INTERNET 
	//    CSIDL_INTERNET_CACHE 
	//    CSIDL_LOCAL_APPDATA 
	//    CSIDL_MYMUSIC 
	//    CSIDL_MYPICTURES
	//    CSIDL_NETHOOD 
	//    CSIDL_NETWORK 
	//    CSIDL_PERSONAL 
	//    CSIDL_PRINTERS 
	//    CSIDL_PRINTHOOD 
	//    CSIDL_PROFILE 
	//    CSIDL_PROGRAM_FILES 
	//    CSIDL_PROGRAM_FILES_COMMON 
	//    CSIDL_PROGRAMS 
	//    CSIDL_RECENT 
	//    CSIDL_SENDTO 
	//    CSIDL_STARTMENU 
	//    CSIDL_STARTUP 
	//    CSIDL_SYSTEM
	//    CSIDL_TEMPLATES 
	//    CSIDL_WINDOWS
	static int BrowseForFolder(CString& sPath, CString sTitle, HWND hParent=NULL, 
		UINT uiFlag=0, UINT uiSpecial=0, LPITEMIDLIST pidlRoot=NULL)
	{
		BROWSEINFO bi;
		ZeroMemory(&bi, sizeof(BROWSEINFO));
		bi.hwndOwner = (hParent == NULL) ? AfxGetMainWnd()->GetSafeHwnd() : hParent;
		bi.pidlRoot = pidlRoot;
		char pszDisplayName[_MAX_PATH];
		bi.pszDisplayName = pszDisplayName;
		bi.lpszTitle = sTitle;
		bi.ulFlags = uiFlag;
		bi.lParam = (LPARAM)(LPCTSTR)sPath; // set lParam to point to path
		bi.lpfn = BrowseCallbackProc;  // set the callback procedure
		LPITEMIDLIST pidl=NULL;

		if (uiSpecial)
		{
			SHGetSpecialFolderLocation(NULL, uiSpecial, &pidl);
			bi.pidlRoot = pidl;
		}
		else
			bi.pidlRoot = pidlRoot;

		LPITEMIDLIST pidlFolder = SHBrowseForFolder(&bi);
		char szPath[_MAX_PATH]="";
		int nRet = IDCANCEL;
		if (pidlFolder && SHGetPathFromIDList (pidlFolder, szPath)) 
		{
			sPath.Format("%s", szPath);
			nRet = IDOK;
		}

		LPMALLOC pMalloc = NULL;
		SHGetMalloc(&pMalloc);
		pMalloc->Free(pidl);
		pMalloc->Free(pidlFolder);
		pMalloc->Release();

		return nRet;
	}

	//////////////////////////////////////////////////////////
	// PathToPidl
	// Function Name  : SHPathToPidl
	// Description      : ��θ��� ITEMIDLIST�� ��ȯ�Ѵ�.
	// Return type    : static HRESULT 
	// Arguments    : 
	//      LPCTSTR szPath      : ��θ�
	//      LPITEMIDLIST* ppidl    : ã�Ƴ� ITEMIDLIST
	//      LPSHELLFOLDER pFolder  : ��Ʈ ���� ����.. NULL�̸� ����ũ ž�� ������.
	static HRESULT PathToPidl(LPCTSTR szPath, LPITEMIDLIST* ppidl, 
		LPSHELLFOLDER pFolder=NULL)
	{
		OLECHAR wszPath[MAX_PATH] = {0};
		ULONG nCharsParsed = 0;
		LPSHELLFOLDER pShellFolder = NULL;
		BOOL bFreeOnExit = FALSE;

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, wszPath, MAX_PATH);

		// Use the desktop's IShellFolder by default
		if(pFolder == NULL)
		{
			SHGetDesktopFolder(&pShellFolder);
			bFreeOnExit = TRUE;
		} 
		else
			pShellFolder = pFolder;

		HRESULT hr = pShellFolder->ParseDisplayName(NULL, NULL, wszPath, &nCharsParsed, ppidl, NULL);

		if(bFreeOnExit)
			pShellFolder->Release();

		return hr;
	}


	// Function Name  : PathToShellFolder
	// Description      : ���ڿ��� SHELLFOLDER ������ ��ȯ�Ѵ�.
	// Return type    : static void 
	// Arguments    : 
	//      LPCTSTR szPath        : �־��� ��θ�
	//      LPSHELLFOLDER& pSubFolder  : ��ȯ�� SHELLFOLDER
	static void PathToShellFolder(LPCTSTR szPath, LPSHELLFOLDER& pSubFolder)
	{
		LPITEMIDLIST pidl = NULL;
		LPSHELLFOLDER pFolder = NULL;

		// Get the memory allocator
		LPMALLOC pMalloc = NULL;
		SHGetMalloc(&pMalloc);

		// Get the IShellFolder interface for the desktop
		SHGetDesktopFolder(&pFolder);

		// ����ũ ž ������ ������ ������ ȹ���Ѵ�.
		int iNumOfItems = EnumFolderContent(pFolder, NULL, 0, NULL);
		// Try to find a match under Desktop
		int rc = EnumFolderContent(pFolder, 
			SearchText, //CallBack Function
			reinterpret_cast<DWORD>(szPath), &pidl);

		// If not found, try under My Computer
		if(rc == iNumOfItems)
		{
			// Bind to My Computer
			LPITEMIDLIST pidlMyComp;
			SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &pidlMyComp);
			pFolder->BindToObject(pidlMyComp, NULL, IID_IShellFolder,
				reinterpret_cast<LPVOID*>(&pSubFolder));

			// Free the pointer to the desktop folder
			pFolder->Release();
			pMalloc->Free(pidlMyComp);
			pFolder = pSubFolder;

			// Scan My Computer
			iNumOfItems = EnumFolderContent(pFolder, NULL, 0, NULL);
			rc = EnumFolderContent(pFolder, 
				SearchText, //CallBack Function
				reinterpret_cast<DWORD>(szPath), &pidl);

			if(rc == iNumOfItems)
			{
				// Make the last attempt: is it a path name?
				HRESULT hr = PathToPidl(szPath, &pidl, pFolder); //Local Function
				if(FAILED(hr)) {
					pMalloc->Free(pidl);
					pFolder->Release();
					pSubFolder = NULL;
					return;
				}
			}
		}

		// If here, then:
		//   pidl points to the folder we need to bind to enumerate the content
		//   pFolder points to the IShellFolder of the pidl's parent folder

		// Bind to the subfolder we're searching for
		// pFolder can point to Desktop's or My Computer's IShellFolder
		pFolder->BindToObject(pidl, NULL, IID_IShellFolder,
			reinterpret_cast<LPVOID*>(&pSubFolder));

		// Clean up
		pFolder->Release();
		pMalloc->Free(pidl);
		pMalloc->Release();
	}

	// Function Name  : EnumFolderContent
	// Description      : ���� ���ο� �����ϴ� ������ ���� Ȥ�� ã�� �������� ������ ���Ѵ�.
	// Return type    : static int 
	// Arguments    : 
	//      LPSHELLFOLDER pFolder  : �˻� ��� ��Ʈ ����
	//      FOLDERCONTENTPROC pfn  : User Defined CallBack Function
	//      DWORD dwData      : �ܺο��� �Ѿ���� ��� ���� - CallBack �Լ����� ����ϴ� ����
	//      LPITEMIDLIST* ppidl    : ã�Ƴ� LPITEMIDLIST
	static int EnumFolderContent(LPSHELLFOLDER pFolder, FOLDERCONTENTPROC pfn, 
		DWORD dwData, LPITEMIDLIST* ppidl)
	{
		int iNumOfItems = 0;

		// Enumerates the content
		LPENUMIDLIST pEnumIDList = NULL;
		pFolder->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &pEnumIDList);

		ULONG ulFetched = 0;
		LPITEMIDLIST pItem = NULL;
		while(NOERROR == pEnumIDList->Next(1, &pItem, &ulFetched))
		{
			STRRET sName;
			TCHAR szBuf[MAX_PATH] = {0};
			pFolder->GetDisplayNameOf(pItem, 0, &sName);
			StrretToString(pItem, &sName, szBuf);

			// Invoke callback
			if(pfn)
			{
				if(!pfn(szBuf, pItem, dwData))
				{
					// Returns the current PIDL
					if(ppidl != NULL)
						*ppidl = pItem;
					break;
				}
			}
			++iNumOfItems;
		}
		return iNumOfItems;
	}

	// Function Name  : StrretToString
	// Description      : STRRET���� ���ڿ��� ��ȯ�Ѵ�.
	// Return type    : static void 
	// Arguments    : 
	//      LPITEMIDLIST pidl  : ���� �����ۿ� ���� ItemIDList ��
	//      LPSTRRET pStr    : IShellFolder::GetDisplayNameOf() �Լ��� ���ؼ� ȹ���� STRRET ��
	//      LPSTR pszBuf    : ��ȯ�� ���ڿ��� ����� ����
	static void StrretToString(LPITEMIDLIST pidl, LPSTRRET pStr, LPSTR pszBuf)
	{
		lstrcpy(pszBuf, "");

		switch(pStr->uType)
		{
		case STRRET_WSTR:              // Unicode string
			WideCharToMultiByte(CP_ACP, 0, pStr->pOleStr, -1, pszBuf, MAX_PATH, NULL, NULL);
			break;

		case STRRET_OFFSET:            // Offset
			lstrcpy(pszBuf, reinterpret_cast<LPSTR>(pidl) + pStr->uOffset);
			break;

		case STRRET_CSTR:              // ANSI string
			lstrcpy(pszBuf, pStr->cStr);
			break;
		}
	}
private:
	// This is the default callback procedure for the SHBrowseForFolder function.
	// It will set the current selection to the directory specified in the edit control
	static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM dwData)
	{
		switch (uMsg)
		{
		case BFFM_INITIALIZED:
			{
				//ĸ�ǿ��� Help ����
				DWORD dwStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
				SetWindowLong(hWnd, GWL_EXSTYLE, dwStyle & ~WS_EX_CONTEXTHELP);

				//������ ������ ���õǵ��� �Ѵ�.
				SendMessage(hWnd, BFFM_SETSELECTION, TRUE, dwData);

				//��ȭ���ڰ� �߾ӿ� ������ �Ѵ�.
				RECT rc;
				GetClientRect(hWnd, &rc);
				SetWindowPos(hWnd, NULL,
					(GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left))/2,
					(GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top))/2,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);

				//Status text�� 3D �׵θ� ÷��
				HWND hwndLabel = GetDlgItem(hWnd, 0x3743);
				if (IsWindow(hwndLabel))
				{
					TCHAR szClass[MAX_PATH] = {0};
					GetClassName(hwndLabel, szClass, MAX_PATH);
					if (lstrcmpi(szClass, __TEXT("static")))
						break;
				}
				else
					break;
				dwStyle = GetWindowLong(hwndLabel, GWL_EXSTYLE);
				SetWindowLong(hwndLabel, GWL_EXSTYLE, dwStyle | WS_EX_STATICEDGE);
				SetWindowPos(hwndLabel, NULL, 0, 0, 0, 0,
					SWP_NOSIZE | SWP_NOMOVE | SWP_DRAWFRAME);
			}
			break;
		case BFFM_SELCHANGED:
			{
				TCHAR szText[MAX_PATH] = {0};
				SHGetPathFromIDList(reinterpret_cast<LPITEMIDLIST>(lParam), szText);
				SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, reinterpret_cast<LPARAM>(szText));
			}
			break;
		case BFFM_VALIDATEFAILED:
			break;
		}
		return 0;
	}
	//���ڷ� �Ѿ�� dwData�� ���� �˻��� pszItem�� ������ ���������� ���� ���
	//pIdl�� CallBack �Լ��� ���������� ����ϱ� ���ؼ� ���� ��ȿ�����̴�.
	static BOOL CALLBACK SearchText(LPCSTR pszItem, LPITEMIDLIST pIdl, DWORD dwData)
	{
		return static_cast<BOOL>(lstrcmpi(pszItem, reinterpret_cast<LPCSTR>(dwData)));
	}
	//���ڷ� �Ѿ�� dwData�� ���� �˻��� pszItem�� ������ ���������� ���� ���
	//pIdl�� CallBack �Լ��� ���������� ����ϱ� ���ؼ� ���� ��ȿ�����̴�.
	BOOL CALLBACK SearchPidl(LPCSTR pszItem, LPITEMIDLIST pIdl, DWORD dwData)
	{
		LPITEMIDLIST PIDL = reinterpret_cast<LPITEMIDLIST>(dwData);
		return (pIdl == PIDL) ? 0:1;
	}
};

//#endif //__SHELLBROWSE_H__20020531