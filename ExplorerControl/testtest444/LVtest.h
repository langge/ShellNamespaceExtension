#pragma once


// CLVtest ���Դϴ�.

class CLVtest : public CListView
{
	DECLARE_DYNCREATE(CLVtest)

protected:
	CLVtest();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CLVtest();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


