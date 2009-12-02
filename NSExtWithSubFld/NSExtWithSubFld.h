/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Nov 22 17:52:23 2004
 */
/* Compiler settings for E:\zengxi\Netting\NettCryptFile\dvp_src\VC\NSExtWithSubFld\NSExtWithSubFld.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __NSExtWithSubFld_h__
#define __NSExtWithSubFld_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IMyVirtualFolder_FWD_DEFINED__
#define __IMyVirtualFolder_FWD_DEFINED__
typedef interface IMyVirtualFolder IMyVirtualFolder;
#endif 	/* __IMyVirtualFolder_FWD_DEFINED__ */


#ifndef __INSFShellView_FWD_DEFINED__
#define __INSFShellView_FWD_DEFINED__
typedef interface INSFShellView INSFShellView;
#endif 	/* __INSFShellView_FWD_DEFINED__ */


#ifndef __MyVirtualFolder_FWD_DEFINED__
#define __MyVirtualFolder_FWD_DEFINED__

#ifdef __cplusplus
typedef class MyVirtualFolder MyVirtualFolder;
#else
typedef struct MyVirtualFolder MyVirtualFolder;
#endif /* __cplusplus */

#endif 	/* __MyVirtualFolder_FWD_DEFINED__ */


#ifndef __NSFShellView_FWD_DEFINED__
#define __NSFShellView_FWD_DEFINED__

#ifdef __cplusplus
typedef class NSFShellView NSFShellView;
#else
typedef struct NSFShellView NSFShellView;
#endif /* __cplusplus */

#endif 	/* __NSFShellView_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IMyVirtualFolder_INTERFACE_DEFINED__
#define __IMyVirtualFolder_INTERFACE_DEFINED__

/* interface IMyVirtualFolder */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMyVirtualFolder;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A9C116F8-4318-443D-98DC-AAECC9421155")
    IMyVirtualFolder : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IMyVirtualFolderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMyVirtualFolder __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMyVirtualFolder __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMyVirtualFolder __RPC_FAR * This);
        
        END_INTERFACE
    } IMyVirtualFolderVtbl;

    interface IMyVirtualFolder
    {
        CONST_VTBL struct IMyVirtualFolderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMyVirtualFolder_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMyVirtualFolder_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMyVirtualFolder_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMyVirtualFolder_INTERFACE_DEFINED__ */


#ifndef __INSFShellView_INTERFACE_DEFINED__
#define __INSFShellView_INTERFACE_DEFINED__

/* interface INSFShellView */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_INSFShellView;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BDA27638-CAE8-4424-8D09-722B8F9C883E")
    INSFShellView : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct INSFShellViewVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            INSFShellView __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            INSFShellView __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            INSFShellView __RPC_FAR * This);
        
        END_INTERFACE
    } INSFShellViewVtbl;

    interface INSFShellView
    {
        CONST_VTBL struct INSFShellViewVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INSFShellView_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define INSFShellView_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define INSFShellView_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __INSFShellView_INTERFACE_DEFINED__ */



#ifndef __NSExtWithSubFldLib_LIBRARY_DEFINED__
#define __NSExtWithSubFldLib_LIBRARY_DEFINED__

/* library NSExtWithSubFldLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_NSExtWithSubFldLib;

EXTERN_C const CLSID CLSID_MyVirtualFolder;

#ifdef __cplusplus

class DECLSPEC_UUID("C7AAC159-D8D7-4CC3-9496-A07887E63664")
MyVirtualFolder;
#endif

EXTERN_C const CLSID CLSID_NSFShellView;

#ifdef __cplusplus

class DECLSPEC_UUID("AD38E3F1-D732-48FB-9749-98C0CF905A3A")
NSFShellView;
#endif
#endif /* __NSExtWithSubFldLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
