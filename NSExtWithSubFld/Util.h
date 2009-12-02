#ifndef __MYUTIL_H_
#define __MYUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <crtdbg.h>
#include <stdio.h>
#include "MyVirtualFolder.h"


// Neat macros someone always leaves out
#ifndef lengthof
   #define lengthof(x) (sizeof(x)/sizeof(*x))
#endif

#ifndef offsetof
  #define offsetof(type, field) ((int)&((type*)0)->field)
#endif

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#ifdef _DEBUG
#define HR(expr) { if(FAILED(Hr=expr)) { _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr); _CrtDbgBreak(); return Hr; } }
#define VALIDATE_POINTER(p) ATLASSERT(p); if(!p) return E_POINTER
#define VALIDATE_OUT_POINTER(p) ATLASSERT(p); if(!p) return E_POINTER; else *p=NULL
#else
#define HR(expr) if(FAILED(Hr=expr)) return Hr
#define VALIDATE_POINTER(p) if(!p) return E_POINTER
#define VALIDATE_OUT_POINTER(p) if(!p) return E_POINTER; else *p=NULL
#endif//_DEBUG

#define DbgGetIID
#define PidlToString


typedef enum
{
	ATTR_TYPE =0,
}NEF_ATTRIBUTES;


// Constants that tell us the order of
// columns in the ListView control.
typedef enum 
{
   COL_NAME = 0,
   COL_TYPE,
} LISTVIEW_COLUMNS;

// Constants defining where in the shared
// ImageList control the icons are placed.
typedef enum
{
   ICON_INDEX_NSFROOT = 0,
   ICON_INDEX_FOLDER,
   ICON_INDEX_FOLDER_OPEN,
   ICON_INDEX_FILE,
} ICONINDEX;

struct CListSortInfo
{
    CMyVirtualFolder*  pNSFolder;       // parent folder of the view
	USHORT            iSortColumn;            //which column to sort by
    BOOL              bForwardSort;     // which direction to sort
};

HRESULT GetGlobalSettings();
BOOL CreateImageLists();
int CALLBACK ListViewSortFuncCB(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
 
int WideCharToLocal(LPTSTR, LPWSTR, DWORD);
int LocalToWideChar(LPWSTR, LPTSTR, DWORD);
BOOL LocaleIsChinese();
void RefreshExplorerWindow();

#endif //__MYUTIL_H_