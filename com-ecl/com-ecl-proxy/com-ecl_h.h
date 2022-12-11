

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for com-ecl.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __com2Decl_h_h__
#define __com2Decl_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAcc_FWD_DEFINED__
#define __IAcc_FWD_DEFINED__
typedef interface IAcc IAcc;

#endif 	/* __IAcc_FWD_DEFINED__ */


#ifndef __ComEcl_FWD_DEFINED__
#define __ComEcl_FWD_DEFINED__

#ifdef __cplusplus
typedef class ComEcl ComEcl;
#else
typedef struct ComEcl ComEcl;
#endif /* __cplusplus */

#endif 	/* __ComEcl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IAcc_INTERFACE_DEFINED__
#define __IAcc_INTERFACE_DEFINED__

/* interface IAcc */
/* [object][version][uuid] */ 


EXTERN_C const IID IID_IAcc;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e0959149-f389-4b42-be22-6278a9ff70af")
    IAcc : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Inc( 
            LONGLONG val) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Value( 
            /* [out] */ LONGLONG *val) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAccVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAcc * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAcc * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAcc * This);
        
        HRESULT ( STDMETHODCALLTYPE *Inc )( 
            IAcc * This,
            LONGLONG val);
        
        HRESULT ( STDMETHODCALLTYPE *Value )( 
            IAcc * This,
            /* [out] */ LONGLONG *val);
        
        END_INTERFACE
    } IAccVtbl;

    interface IAcc
    {
        CONST_VTBL struct IAccVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAcc_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAcc_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAcc_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAcc_Inc(This,val)	\
    ( (This)->lpVtbl -> Inc(This,val) ) 

#define IAcc_Value(This,val)	\
    ( (This)->lpVtbl -> Value(This,val) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAcc_INTERFACE_DEFINED__ */



#ifndef __ComEclLibrary_LIBRARY_DEFINED__
#define __ComEclLibrary_LIBRARY_DEFINED__

/* library ComEclLibrary */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ComEclLibrary;

EXTERN_C const CLSID CLSID_ComEcl;

#ifdef __cplusplus

class DECLSPEC_UUID("4e6665df-5646-4815-b0e9-81d6af1c51eb")
ComEcl;
#endif
#endif /* __ComEclLibrary_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


