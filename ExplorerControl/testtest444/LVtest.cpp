// LVtest.cpp : ���� �����Դϴ�.
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


// CLVtest �����Դϴ�.

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


// CLVtest �޽��� ó�����Դϴ�.
