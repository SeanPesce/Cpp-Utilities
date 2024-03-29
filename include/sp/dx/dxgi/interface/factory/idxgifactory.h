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
#include "sp/log.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIFactory>
class factory : public sp::dx::dxgi::object<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    factory() {}
    factory(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] Factory created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~factory() {}

    // Wrapped interface methods
    virtual HRESULT CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter **ppAdapter);
    virtual HRESULT CreateSwapChain(IUnknown *pDevice, DXGI_SWAP_CHAIN_DESC *pDesc, IDXGISwapChain **ppSwapChain);
    virtual HRESULT EnumAdapters(UINT Adapter, IDXGIAdapter **ppAdapter);
    virtual HRESULT GetWindowAssociation(HWND *pWindowHandle);
    virtual HRESULT MakeWindowAssociation(HWND WindowHandle, UINT Flags);
}; // class dxgi::factory

typedef factory<IDXGIFactory> factory0;

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::factory<IDXGIFactory>;


// Implementation
#include "sp/dx/dxgi/interface/factory/idxgifactory_impl.hpp"


#endif // SP_DX_DXGI_IDXGIFACTORY_WRAPPER_CLASS_H_


#endif // _WIN32
