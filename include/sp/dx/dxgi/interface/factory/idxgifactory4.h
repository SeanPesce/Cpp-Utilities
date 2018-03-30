/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/mt427785(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGIFACTORY4_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGIFACTORY4_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/factory/idxgifactory3.h"
#include "DXGI1_4.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIFactory4>
class factory4 : public sp::dx::dxgi::factory3<interface_t> {
public:
    // Wrapped interface methods
    HRESULT EnumAdapterByLuid(LUID AdapterLuid, REFIID riid, void **ppvAdapter);
    HRESULT EnumWarpAdapter(REFIID riid, void **ppvAdapter);
}; // class dxgi::factory4



} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


// Implementation
#include "sp/dx/dxgi/interface/factory/idxgifactory4_impl.hpp"


#endif // SP_DX_DXGI_IDXGIFACTORY4_WRAPPER_CLASS_H_


#endif // _WIN32
