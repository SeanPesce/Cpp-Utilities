/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/bb174569(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGI_SWAPCHAIN_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGI_SWAPCHAIN_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/idxgisubobject.h"
#include "sp/log.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGISwapChain>
class swapchain : public sp::dx::dxgi::subobject<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    swapchain() {}
    swapchain(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] SwapChain created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~swapchain() {}

    // Wrapped interface methods
    virtual HRESULT STDMETHODCALLTYPE Present(UINT SyncInterval, UINT Flags);
    virtual HRESULT STDMETHODCALLTYPE GetBuffer(UINT Buffer, _In_  REFIID riid, _COM_Outptr_  void **ppSurface);
    virtual HRESULT STDMETHODCALLTYPE SetFullscreenState(BOOL Fullscreen, _In_opt_  IDXGIOutput *pTarget);
    virtual HRESULT STDMETHODCALLTYPE GetFullscreenState(_Out_opt_  BOOL *pFullscreen, _COM_Outptr_opt_result_maybenull_  IDXGIOutput **ppTarget);
    virtual HRESULT STDMETHODCALLTYPE GetDesc(_Out_  DXGI_SWAP_CHAIN_DESC *pDesc);
    virtual HRESULT STDMETHODCALLTYPE ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
    virtual HRESULT STDMETHODCALLTYPE ResizeTarget(_In_  const DXGI_MODE_DESC *pNewTargetParameters);
    virtual HRESULT STDMETHODCALLTYPE GetContainingOutput(_COM_Outptr_  IDXGIOutput **ppOutput);
    virtual HRESULT STDMETHODCALLTYPE GetFrameStatistics(_Out_  DXGI_FRAME_STATISTICS *pStats);
    virtual HRESULT STDMETHODCALLTYPE GetLastPresentCount(_Out_  UINT *pLastPresentCount);

}; // class dxgi::swapchain

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::swapchain<IDXGISwapChain>;


// Implementation
#include "sp/dx/dxgi/interface/swapchain/idxgiswapchain_impl.hpp"


#endif // SP_DX_DXGI_IDXGI_SWAPCHAIN_WRAPPER_CLASS_H_


#endif // _WIN32
