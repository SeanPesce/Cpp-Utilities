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
public:
    // Wrapped interface methods
    HRESULT CreateSwapChainForComposition(IUnknown *pDevice, const DXGI_SWAP_CHAIN_DESC1 *pDesc, IDXGIOutput *pRestrictToOutput, IDXGISwapChain1 **ppSwapChain);
    HRESULT CreateSwapChainForCoreWindow(IUnknown *pDevice, IUnknown *pWindow, const DXGI_SWAP_CHAIN_DESC1 *pDesc, IDXGIOutput *pRestrictToOutput, IDXGISwapChain1 **ppSwapChain);
    HRESULT CreateSwapChainForHwnd(IUnknown *pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1 *pDesc, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc, IDXGIOutput *pRestrictToOutput, IDXGISwapChain1 **ppSwapChain);
    HRESULT GetSharedResourceAdapterLuid(HANDLE hResource, LUID *pLuid);
    BOOL    IsWindowedStereoEnabled();
    HRESULT RegisterOcclusionStatusEvent(HANDLE hEvent, DWORD *pdwCookie);
    HRESULT RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg, DWORD *pdwCookie);
    HRESULT RegisterStereoStatusEvent(HANDLE hEvent, DWORD *pdwCookie);
    HRESULT RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg, DWORD *pdwCookie);
    void    UnregisterOcclusionStatus(DWORD dwCookie);
    void    UnregisterStereoStatus(DWORD dwCookie);
}; // class dxgi::factory2



} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


// Implementation
#include "sp/dx/dxgi/interface/factory/idxgifactory2_impl.hpp"


#endif // SP_DX_DXGI_IDXGIFACTORY2_WRAPPER_CLASS_H_


#endif // _WIN32
