// ExplorerWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "testtest444.h"
#include "ExplorerWnd.h"


// CExplorerWnd

IMPLEMENT_DYNAMIC(CExplorerWnd, CWnd)

CExplorerWnd::CExplorerWnd()
: m_pShellView(NULL)
, m_current_folder(NULL)
{

}

CExplorerWnd::~CExplorerWnd()
{
}


BEGIN_MESSAGE_MAP(CExplorerWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CExplorerWnd 메시지 처리기입니다.



int CExplorerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CExplorerWnd::OnDestroy()
{
	//__super::OnDestroy();

	if(m_pShellView != NULL)
	{
		m_pShellView->UIActivate(SVUIA_DEACTIVATE);
		m_pShellView->DestroyViewWindow();
		m_pShellView->Release();
		::PostMessage(m_hWnd, WM_QUIT, 0, 0);
	}
}

BOOL CExplorerWnd::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return __super::DestroyWindow();
}

int CExplorerWnd::update_view(IShellFolder* folder)
{
	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL,CSIDL_DESKTOP,&pidl);

	if( FAILED( ::SHGetSpecialFolderLocation(m_hWnd, CSIDL_DESKTOP, &pidl) ) ) 
		return 0;

	if(folder == NULL)
		::SHGetDesktopFolder(&folder);

	if(m_pShellView != NULL)
	{
		//m_pShellView->GetCurrentInfo(&fs);
		m_pShellView->UIActivate(SVUIA_DEACTIVATE);
		m_pShellView->DestroyViewWindow();
		m_pShellView->Release();
		m_pShellView = NULL;
	}
	

	HRESULT t = folder->CreateViewObject(m_hWnd,IID_IShellView, (void**)&m_pShellView);

	FOLDERSETTINGS fs;
	IShellView* pShellView2 = m_pShellView;
	t = pShellView2->GetCurrentInfo(&fs);

	//fs.ViewMode =  FVM_THUMBNAIL;

	CRect rect;
	GetClientRect(&rect);

	t = m_pShellView->CreateViewWindow(pShellView2, &fs, (IShellBrowser*)this, &rect, &m_encapwnd);

	AddRef();

	pShellView2->UIActivate(SVUIA_ACTIVATE_FOCUS);

	if(m_current_folder)
		m_current_folder->Release();

	t = folder->QueryInterface(__uuidof(IShellFolder),(void **)&m_current_folder);

	return 0;
}
