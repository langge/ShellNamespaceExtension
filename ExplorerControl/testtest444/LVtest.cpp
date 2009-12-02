// LVtest.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "testtest444.h"
#include "LVtest.h"


// CLVtest

IMPLEMENT_DYNCREATE(CLVtest, CListView)

CLVtest::CLVtest()
{

}

CLVtest::~CLVtest()
{
}

BEGIN_MESSAGE_MAP(CLVtest, CListView)
END_MESSAGE_MAP()


// CLVtest 진단입니다.

#ifdef _DEBUG
void CLVtest::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CLVtest::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLVtest 메시지 처리기입니다.
