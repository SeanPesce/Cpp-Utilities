/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/hh404631%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGI_SWAPCHAIN1_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGI_SWAPCHAIN1_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/swapchain/idxgiswapchain.h"
#include "sp/log.h"
#include "DXGI1_2.h"


__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGISwapChain1>
class swapchain1 : public sp::dx::dxgi::swapchain<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    swapchain1() {}
    swapchain1(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] SwapChain created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~swapchain1() {}

    // Wrapped interface methods
    virtual HRESULT STDMETHODCALLTYPE GetDesc1(_Out_  DXGI_SWAP_CHAIN_DESC1 *pDesc);
    virtual HRESULT STDMETHODCALLTYPE GetFullscreenDesc(_Out_  DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pDesc);
    virtual HRESULT STDMETHODCALLTYPE GetHwnd(_Out_  HWND *pHwnd);
    virtual HRESULT STDMETHODCALLTYPE GetCoreWindow(_In_  REFIID refiid, _COM_Outptr_  void **ppUnk);
    virtual HRESULT STDMETHODCALLTYPE Present1(UINT SyncInterval, UINT PresentFlags, _In_  const DXGI_PRESENT_PARAMETERS *pPresentParameters);
    virtual BOOL    STDMETHODCALLTYPE IsTemporaryMonoSupported();
    virtual HRESULT STDMETHODCALLTYPE GetRestrictToOutput(_Out_  IDXGIOutput **ppRestrictToOutput);
    virtual HRESULT STDMETHODCALLTYPE SetBackgroundColor(_In_  const DXGI_RGBA *pColor);
    virtual HRESULT STDMETHODCALLTYPE GetBackgroundColor(_Out_  DXGI_RGBA *pColor);
    virtual HRESULT STDMETHODCALLTYPE SetRotation(_In_  DXGI_MODE_ROTATION Rotation);
    virtual HRESULT STDMETHODCALLTYPE GetRotation(_Out_  DXGI_MODE_ROTATION *pRotation);

}; // class dxgi::swapchain1

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::swapchain1<IDXGISwapChain1>;


// Implementation
#include "sp/dx/dxgi/interface/swapchain/idxgiswapchain1_impl.hpp"


#endif // SP_DX_DXGI_IDXGI_SWAPCHAIN1_WRAPPER_CLASS_H_


#endif // _WIN32
