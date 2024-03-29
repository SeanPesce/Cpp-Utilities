/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/hh404556(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGIFACTORY2_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGIFACTORY2_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/factory/idxgifactory1.h"
#include "DXGI1_2.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIFactory2>
class factory2 : public sp::dx::dxgi::factory1<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    factory2() {}
    factory2(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] Factory created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~factory2() {}

    // Wrapped interface methods
    virtual HRESULT CreateSwapChainForComposition(IUnknown *pDevice, const DXGI_SWAP_CHAIN_DESC1 *pDesc, IDXGIOutput *pRestrictToOutput, IDXGISwapChain1 **ppSwapChain);
    virtual HRESULT CreateSwapChainForCoreWindow(IUnknown *pDevice, IUnknown *pWindow, const DXGI_SWAP_CHAIN_DESC1 *pDesc, IDXGIOutput *pRestrictToOutput, IDXGISwapChain1 **ppSwapChain);
    virtual HRESULT CreateSwapChainForHwnd(IUnknown *pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1 *pDesc, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc, IDXGIOutput *pRestrictToOutput, IDXGISwapChain1 **ppSwapChain);
    virtual HRESULT GetSharedResourceAdapterLuid(HANDLE hResource, LUID *pLuid);
    virtual BOOL    IsWindowedStereoEnabled();
    virtual HRESULT RegisterOcclusionStatusEvent(HANDLE hEvent, DWORD *pdwCookie);
    virtual HRESULT RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg, DWORD *pdwCookie);
    virtual HRESULT RegisterStereoStatusEvent(HANDLE hEvent, DWORD *pdwCookie);
    virtual HRESULT RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg, DWORD *pdwCookie);
    virtual void    UnregisterOcclusionStatus(DWORD dwCookie);
    virtual void    UnregisterStereoStatus(DWORD dwCookie);
}; // class dxgi::factory2



} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::factory2<IDXGIFactory2>;


// Implementation
#include "sp/dx/dxgi/interface/factory/idxgifactory2_impl.hpp"


#endif // SP_DX_DXGI_IDXGIFACTORY2_WRAPPER_CLASS_H_


#endif // _WIN32
