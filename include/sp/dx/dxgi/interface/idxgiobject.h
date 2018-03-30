/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/bb174541(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGIOBJECT_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGIOBJECT_WRAPPER_CLASS_H_

#include "windows/com/interface/iunknown.h"
#include "DXGI.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIObject>
class object : public sp::os::win::com::iunknown<interface_t> {
public:
    // Wrapped interface methods
    HRESULT GetParent(REFIID riid, void **ppParent);
    HRESULT GetPrivateData(REFGUID Name, UINT *pDataSize, void *pData);
    HRESULT SetPrivateData(REFGUID Name, UINT DataSize, const void *pData);
    HRESULT SetPrivateDataInterface(REFGUID Name, const IUnknown *pUnknown);
}; // class dxgi::object


} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


// Implementation
#include "sp/dx/dxgi/interface/idxgiobject_impl.hpp"


#endif // SP_DX_DXGI_IDXGIOBJECT_WRAPPER_CLASS_H_


#endif // _WIN32
