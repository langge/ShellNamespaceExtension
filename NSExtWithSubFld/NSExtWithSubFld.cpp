// NSExtWithSubFld.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f NSExtWithSubFldps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
//#include "NSExtWithSubFld.h"


CNWSModule _Module;
//5fcd4425-ca3a-48f4-a57c-b8a75c32acb1
const CLSID CLSID_MyVirtualFolder =  {0x5fcd4425,0xca3a,0x48f4,{0xa5,0x7c,0xb8,0xa7,0x5c,0x32,0xac,0xb1}};
//9ae7d328-3bbf-471a-9439-adda0f16390f
const CLSID CLSID_NSFShellView =     {0x9ae7d328,0x3bbf,0x471a,{0x94,0x39,0xad,0xda,0x0f,0x16,0x39,0x0f}};

#include "MyVirtualFolder.h"
#include "NSFShellView.h"

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_MyVirtualFolder, CMyVirtualFolder)
OBJECT_ENTRY(CLSID_NSFShellView, CNSFShellView)
OBJECT_ENTRY(CLSID_NSFShellView, CNSFShellView)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
		HRESULT Hr;

		_Module.Init (ObjectMap,hInstance);
		_tcscpy(_Module.m_szDN_NSFROOT,_T("iShoot"));
		CreateImageLists();

        DisableThreadLibraryCalls(hInstance);

#ifdef _ATL_DEBUG_INTERFACES
		_Module.m_nIndexBreakAt = 1;
#endif

    }
    else if (dwReason == DLL_PROCESS_DETACH)
	{
        _Module.Term();
	}
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	if(_Module.GetLockCount()==0)
	{
		//Refresh the Desktop
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
    // registers object, typelib and all interfaces in typelib
	HRESULT Hr;
	Hr=_Module.RegisterServer(FALSE);


	//Refresh Desktop
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	return Hr;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	HRESULT Hr;
	Hr= _Module.UnregisterServer(FALSE);

	//Refresh Desktop
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

	::CoFreeUnusedLibraries();

	return Hr;
}
