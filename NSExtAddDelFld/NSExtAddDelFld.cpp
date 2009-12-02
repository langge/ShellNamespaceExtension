// NSExtAddDelFld.cpp : Namespace Extension implemented create and delete object.
//
// Written by ZengXi (zxlet@163.com)
// Copyright (c) 2004-2005 ZengXi.
//
// Beware of bugs.
//
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//

// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f NSExtAddDelFldps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>


CNWSModule _Module;
const CLSID CLSID_MyVirtualFolder =  {0xb9af185e,0x1276,0x4bc7,{0x93,0x6b,0x69,0x2e,0x16,0xb1,0xad,0x4a}};
const CLSID CLSID_NSFShellView =     {0x0eb8e89d,0xa6ee,0x40b3,{0xa9,0xda,0x92,0xef,0xc2,0x08,0xa6,0x7e}};

#include "MyVirtualFolder.h"
#include "NSFShellView.h"

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_MyVirtualFolder, CMyVirtualFolder)
OBJECT_ENTRY(CLSID_NSFShellView, CNSFShellView)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
#ifdef _WRITETRACEINFO_ON
	trInit();
#endif
		_Module.Init (ObjectMap,hInstance);
		_tcscpy(_Module.m_szDN_NSFROOT,_T("NSE_WithSubFld"));
		
		CreateImageLists();

        DisableThreadLibraryCalls(hInstance);

#ifdef _ATL_DEBUG_INTERFACES
		_Module.m_nIndexBreakAt = 1;
#endif

    }
    else if (dwReason == DLL_PROCESS_DETACH)
	{
        _Module.Term();

#ifdef _WRITETRACEINFO_ON
		trFinal();
#endif
	}
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	if(_Module.GetLockCount()==0)
	{
		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, not include typelib and all interfaces in typelib
	HRESULT Hr;
	Hr=_Module.RegisterServer(FALSE);

    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

	if(_Module.m_wWinMajor == 5 && _Module.m_wWinMinor == 1) //XP can't auto refresh explore
	{
		RefreshShellViewWndsExcept(NULL);
	}
	return Hr;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	HRESULT Hr;
	Hr= _Module.UnregisterServer(FALSE);

    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	if(_Module.m_wWinMajor == 5 && _Module.m_wWinMinor == 1) //XP can't auto refresh explore
	{
		::SHChangeNotify( SHCNE_RMDIR , SHCNF_IDLIST | SHCNF_FLUSH, _Module.m_pidlNSFROOT, NULL);
	}

	::CoFreeUnusedLibraries();

	return Hr;
}