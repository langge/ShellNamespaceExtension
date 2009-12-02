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
	// Description      : 폴더 탐색을 지원한다.
	// Return type    : static int (IDOK or IDCANCEL)
	// Arguments    : 
	//      CString& sPath    : 초기 경로 & 선택 경로
	//      CString sTitle    : "Choose a folder:"를 대체할 제목
	//      HWND hParent    : 부모 핸들
	//      UINT uiFlag      : 설정 플래그
	//      LPITEMIDLIST pidlRoot:지정 경로를 루트로 삼는다.
	//      UINT uiSpecial    : 특수 폴더를 루트로 삼는다.pidlRoot보다 우선한다.
	//--------------------------------------------------------------------------------
	// uiFlag 종류
	//    BIF_RETURNONLYSDIRS    : 파일 시스템 디렉토리를 선택할 경우에만 
	//                  OK 버튼 활성화
	//    BIF_DONTGOBELOWDOMAIN  : 네트워크 폴더는 보여주지 않는다.
	//    BIF_STATUSTEXT      : 라벨을 활성화 시킨다.(여기서는 전체 경로 출력)
	//    BIF_EDITBOX        : 폴더를 수동으로 선택하는 에디터 박스 사용
	//                  (쉘 버젼 4.71이상)
	//    BIF_VALIDATE      : BIF_EDITBOX의 보충 역할.
	//                  잘못된 파일이나 폴더 이름을 에디터박스에 입력할
	//                  경우 통지를 받을수 있다.(쉘 버젼 4.71이상)
	//    BIF_BROWSEFORCOMPUTER  : 사용자가 컴퓨터 이름만 선택하게 한다.
	//    BIF_BROWSEFORPRINTER  : 프린터 이름만 선택하게 한다.
	//    BIF_BROWSEINCLUDEFILES  : 트리 뷰에 파일까지 나타난다.
	//    그 외에도 다음과 같은 플래그가 존재한다.(도움말 참조)
	//    BIF_RETURNFSANCESTORS, 
	//    BIF_NEWDIALOGSTYLE, BIF_USENEWUI, BIF_SHAREABLE, BIF_BROWSEINCLUDEURLS
	// uiSpecial 종류
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
	// Description      : 경로명을 ITEMIDLIST로 변환한다.
	// Return type    : static HRESULT 
	// Arguments    : 
	//      LPCTSTR szPath      : 경로명
	//      LPITEMIDLIST* ppidl    : 찾아낸 ITEMIDLIST
	//      LPSHELLFOLDER pFolder  : 루트 폴더 정보.. NULL이면 데스크 탑을 뒤진다.
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
	// Description      : 문자열을 SHELLFOLDER 형으로 변환한다.
	// Return type    : static void 
	// Arguments    : 
	//      LPCTSTR szPath        : 주어진 경로명
	//      LPSHELLFOLDER& pSubFolder  : 변환된 SHELLFOLDER
	static void PathToShellFolder(LPCTSTR szPath, LPSHELLFOLDER& pSubFolder)
	{
		LPITEMIDLIST pidl = NULL;
		LPSHELLFOLDER pFolder = NULL;

		// Get the memory allocator
		LPMALLOC pMalloc = NULL;
		SHGetMalloc(&pMalloc);

		// Get the IShellFolder interface for the desktop
		SHGetDesktopFolder(&pFolder);

		// 데스크 탑 폴더의 아이템 갯수를 획득한다.
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
	// Description      : 폴더 내부에 존재하는 아이템 갯수 혹은 찾은 대상까지의 갯수를 구한다.
	// Return type    : static int 
	// Arguments    : 
	//      LPSHELLFOLDER pFolder  : 검색 대상 루트 폴더
	//      FOLDERCONTENTPROC pfn  : User Defined CallBack Function
	//      DWORD dwData      : 외부에서 넘어오는 대상 인자 - CallBack 함수에서 사용하는 인자
	//      LPITEMIDLIST* ppidl    : 찾아낸 LPITEMIDLIST
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
	// Description      : STRRET형을 문자열로 변환한다.
	// Return type    : static void 
	// Arguments    : 
	//      LPITEMIDLIST pidl  : 현재 아이템에 대한 ItemIDList 값
	//      LPSTRRET pStr    : IShellFolder::GetDisplayNameOf() 함수를 통해서 획득한 STRRET 값
	//      LPSTR pszBuf    : 변환된 문자열이 담겨질 버퍼
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
				//캡션에서 Help 삭제
				DWORD dwStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
				SetWindowLong(hWnd, GWL_EXSTYLE, dwStyle & ~WS_EX_CONTEXTHELP);

				//지정된 폴더가 선택되도록 한다.
				SendMessage(hWnd, BFFM_SETSELECTION, TRUE, dwData);

				//대화상자가 중앙에 오도록 한다.
				RECT rc;
				GetClientRect(hWnd, &rc);
				SetWindowPos(hWnd, NULL,
					(GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left))/2,
					(GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top))/2,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);

				//Status text에 3D 테두리 첨가
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
	//인자로 넘어온 dwData와 현재 검색된 pszItem이 동일한 아이템인지 비교한 결과
	//pIdl은 CallBack 함수를 공통적으로 사용하기 위해서 넣은 무효인자이다.
	static BOOL CALLBACK SearchText(LPCSTR pszItem, LPITEMIDLIST pIdl, DWORD dwData)
	{
		return static_cast<BOOL>(lstrcmpi(pszItem, reinterpret_cast<LPCSTR>(dwData)));
	}
	//인자로 넘어온 dwData와 현재 검색된 pszItem이 동일한 아이템인지 비교한 결과
	//pIdl은 CallBack 함수를 공통적으로 사용하기 위해서 넣은 무효인자이다.
	BOOL CALLBACK SearchPidl(LPCSTR pszItem, LPITEMIDLIST pIdl, DWORD dwData)
	{
		LPITEMIDLIST PIDL = reinterpret_cast<LPITEMIDLIST>(dwData);
		return (pIdl == PIDL) ? 0:1;
	}
};

//#endif //__SHELLBROWSE_H__20020531