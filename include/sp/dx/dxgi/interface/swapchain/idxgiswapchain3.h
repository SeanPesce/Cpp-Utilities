/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/dn903673(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGI_SWAPCHAIN3_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGI_SWAPCHAIN3_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/swapchain/idxgiswapchain2.h"
#include "sp/log.h"
#include "DXGI1_4.h"


__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGISwapChain3>
class swapchain3 : public sp::dx::dxgi::swapchain2<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    swapchain3() {}
    swapchain3(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] SwapChain created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~swapchain3() {}

    // Wrapped interface methods
    virtual UINT STDMETHODCALLTYPE GetCurrentBackBufferIndex();
    virtual HRESULT STDMETHODCALLTYPE CheckColorSpaceSupport(_In_  DXGI_COLOR_SPACE_TYPE ColorSpace, _Out_  UINT *pColorSpaceSupport);
    virtual HRESULT STDMETHODCALLTYPE SetColorSpace1(_In_  DXGI_COLOR_SPACE_TYPE ColorSpace);
    virtual HRESULT STDMETHODCALLTYPE ResizeBuffers1(_In_  UINT BufferCount, _In_  UINT Width, _In_  UINT Height, _In_  DXGI_FORMAT Format, _In_  UINT SwapChainFlags, _In_reads_(BufferCount)  const UINT *pCreationNodeMask, _In_reads_(BufferCount)  IUnknown *const *ppPresentQueue);

}; // class dxgi::swapchain3

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::swapchain3<IDXGISwapChain3>;


// Implementation
#include "sp/dx/dxgi/interface/swapchain/idxgiswapchain3_impl.hpp"


#endif // SP_DX_DXGI_IDXGI_SWAPCHAIN3_WRAPPER_CLASS_H_


#endif // _WIN32
