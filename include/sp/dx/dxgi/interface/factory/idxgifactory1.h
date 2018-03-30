/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/ff471335(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGIFACTORY1_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGIFACTORY1_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/factory/idxgifactory.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIFactory1>
class factory1 : public sp::dx::dxgi::factory<interface_t> {
public:
    // Wrapped interface methods
    HRESULT EnumAdapters1(UINT Adapter, IDXGIAdapter1 **ppAdapter);
    BOOL    IsCurrent();
}; // class dxgi::factory1



} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


// Implementation
#include "sp/dx/dxgi/interface/factory/idxgifactory1_impl.hpp"


#endif // SP_DX_DXGI_IDXGIFACTORY1_WRAPPER_CLASS_H_


#endif // _WIN32
