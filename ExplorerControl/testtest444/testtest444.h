// testtest444.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Ctesttest444App:
// �� Ŭ������ ������ ���ؼ��� testtest444.cpp�� �����Ͻʽÿ�.
//

class Ctesttest444App : public CWinApp
{
public:
	Ctesttest444App();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern Ctesttest444App theApp;