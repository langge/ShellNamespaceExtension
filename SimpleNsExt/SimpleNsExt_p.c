

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Wed Dec 02 12:05:15 2009
 */
/* Compiler settings for .\SimpleNsExt.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "SimpleNsExt.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _SimpleNsExt_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } SimpleNsExt_MIDL_TYPE_FORMAT_STRING;

typedef struct _SimpleNsExt_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } SimpleNsExt_MIDL_PROC_FORMAT_STRING;

typedef struct _SimpleNsExt_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } SimpleNsExt_MIDL_EXPR_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const SimpleNsExt_MIDL_TYPE_FORMAT_STRING SimpleNsExt__MIDL_TypeFormatString;
extern const SimpleNsExt_MIDL_PROC_FORMAT_STRING SimpleNsExt__MIDL_ProcFormatString;
extern const SimpleNsExt_MIDL_EXPR_FORMAT_STRING SimpleNsExt__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IShellFolderImpl_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IShellFolderImpl_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IShellViewImpl_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IShellViewImpl_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need a Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const SimpleNsExt_MIDL_PROC_FORMAT_STRING SimpleNsExt__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const SimpleNsExt_MIDL_TYPE_FORMAT_STRING SimpleNsExt__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IShellFolderImpl, ver. 0.0,
   GUID={0x68DBA4F0,0xA70F,0x4BD2,{0x88,0x3C,0x8E,0x76,0x54,0x15,0xE1,0x36}} */

#pragma code_seg(".orpc")
static const unsigned short IShellFolderImpl_FormatStringOffsetTable[] =
    {
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IShellFolderImpl_ProxyInfo =
    {
    &Object_StubDesc,
    SimpleNsExt__MIDL_ProcFormatString.Format,
    &IShellFolderImpl_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IShellFolderImpl_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    SimpleNsExt__MIDL_ProcFormatString.Format,
    &IShellFolderImpl_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(3) _IShellFolderImplProxyVtbl = 
{
    0,
    &IID_IShellFolderImpl,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy
};

const CInterfaceStubVtbl _IShellFolderImplStubVtbl =
{
    &IID_IShellFolderImpl,
    &IShellFolderImpl_ServerInfo,
    3,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IShellViewImpl, ver. 0.0,
   GUID={0x0EA6B88C,0x6FC4,0x4DBC,{0xAE,0x9C,0x7D,0x3B,0xFE,0x2C,0x9F,0xF9}} */

#pragma code_seg(".orpc")
static const unsigned short IShellViewImpl_FormatStringOffsetTable[] =
    {
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IShellViewImpl_ProxyInfo =
    {
    &Object_StubDesc,
    SimpleNsExt__MIDL_ProcFormatString.Format,
    &IShellViewImpl_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IShellViewImpl_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    SimpleNsExt__MIDL_ProcFormatString.Format,
    &IShellViewImpl_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(3) _IShellViewImplProxyVtbl = 
{
    0,
    &IID_IShellViewImpl,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy
};

const CInterfaceStubVtbl _IShellViewImplStubVtbl =
{
    &IID_IShellViewImpl,
    &IShellViewImpl_ServerInfo,
    3,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    SimpleNsExt__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x70001f4, /* MIDL Version 7.0.500 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * _SimpleNsExt_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IShellViewImplProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IShellFolderImplProxyVtbl,
    0
};

const CInterfaceStubVtbl * _SimpleNsExt_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IShellViewImplStubVtbl,
    ( CInterfaceStubVtbl *) &_IShellFolderImplStubVtbl,
    0
};

PCInterfaceName const _SimpleNsExt_InterfaceNamesList[] = 
{
    "IShellViewImpl",
    "IShellFolderImpl",
    0
};


#define _SimpleNsExt_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _SimpleNsExt, pIID, n)

int __stdcall _SimpleNsExt_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _SimpleNsExt, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _SimpleNsExt, 2, *pIndex )
    
}

const ExtendedProxyFileInfo SimpleNsExt_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _SimpleNsExt_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _SimpleNsExt_StubVtblList,
    (const PCInterfaceName * ) & _SimpleNsExt_InterfaceNamesList,
    0, // no delegation
    & _SimpleNsExt_IID_Lookup, 
    2,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

