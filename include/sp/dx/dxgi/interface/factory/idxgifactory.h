/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/bb174535(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGIFACTORY_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGIFACTORY_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/idxgiobject.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIFactory>
class factory : public sp::dx::dxgi::object<interface_t> {
public:
    // Wrapped interface methods
    HRESULT CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter **ppAdapter);
    HRESULT CreateSwapChain(IUnknown *pDevice, DXGI_SWAP_CHAIN_DESC *pDesc, IDXGISwapChain **ppSwapChain);
    HRESULT EnumAdapters(UINT Adapter, IDXGIAdapter **ppAdapter);
    HRESULT GetWindowAssociation(HWND *pWindowHandle);
    HRESULT MakeWindowAssociation(HWND WindowHandle, UINT Flags);
}; // class dxgi::factory

typedef factory<IDXGIFactory> factory0;

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


// Implementation
#include "sp/dx/dxgi/interface/factory/idxgifactory_impl.hpp"


#endif // SP_DX_DXGI_IDXGIFACTORY_WRAPPER_CLASS_H_


#endif // _WIN32
