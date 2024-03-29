/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        Microsoft documentation: https://msdn.microsoft.com/en-us/library/windows/desktop/bb219839(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_D3D9_EXPORTS_H_
#define SP_DX_D3D9_EXPORTS_H_

#include "sp/sp.h"
#include "D3D9.h"



/*! Creates an IDirect3D9 object and returns an interface to it.

    \param SDKVersion  The value of this parameter should be D3D_SDK_VERSION.

    \return If successful, this function returns a pointer to an IDirect3D9 interface; otherwise, a NULL pointer is returned.
*/
IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion);
typedef IDirect3D9*(WINAPI *Direct3DCreate9_t)(UINT);


/*! Creates an IDirect3D9Ex object and returns an interface to it.

    \param[in]  SDKVersion  The value of this parameter should be D3D_SDK_VERSION.
    \param[out] ppD3D       Address of a pointer to an IDirect3D9Ex interface, representing the created IDirect3D9Ex object. If the function fails, NULL is inserted here.

    \return D3DERR_NOTAVAILABLE if Direct3DEx features are not supported (no WDDM driver is installed) or if the SDKVersion does not match the version of the DLL.

    \return D3DERR_OUTOFMEMORY if out-of-memory conditions are detected when creating the enumerator object.

    \return S_OK if the creation of the enumerator object is successful.
*/
HRESULT WINAPI Direct3DCreate9Ex(UINT SDKVersion, IDirect3D9Ex **ppD3D);
typedef HRESULT(WINAPI *Direct3DCreate9Ex_t)(UINT, IDirect3D9Ex**);


/*! This function is internal and not intended to be used by software developers.
*/
void WINAPI DebugSetMute();
typedef void(WINAPI *DebugSetMute_t)();


/*! This function is internal and not intended to be used by software developers.
*/
void WINAPI Direct3DShaderValidatorCreate9();
typedef void(WINAPI *Direct3DShaderValidatorCreate9_t)();





#endif // SP_DX_D3D9_EXPORTS_H_


#endif // _WIN32
