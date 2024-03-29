/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/dn280420(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGI_SWAPCHAIN2_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGI_SWAPCHAIN2_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/swapchain/idxgiswapchain1.h"
#include "sp/log.h"
#include "DXGI1_3.h"


__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGISwapChain2>
class swapchain2 : public sp::dx::dxgi::swapchain1<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    swapchain2() {}
    swapchain2(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] SwapChain created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~swapchain2() {}

    // Wrapped interface methods
    virtual HRESULT STDMETHODCALLTYPE SetSourceSize(UINT Width, UINT Height);
    virtual HRESULT STDMETHODCALLTYPE GetSourceSize(_Out_  UINT *pWidth, _Out_  UINT *pHeight);
    virtual HRESULT STDMETHODCALLTYPE SetMaximumFrameLatency(UINT MaxLatency);
    virtual HRESULT STDMETHODCALLTYPE GetMaximumFrameLatency(_Out_  UINT *pMaxLatency);
    virtual HANDLE  STDMETHODCALLTYPE GetFrameLatencyWaitableObject();
    virtual HRESULT STDMETHODCALLTYPE SetMatrixTransform(const DXGI_MATRIX_3X2_F *pMatrix);
    virtual HRESULT STDMETHODCALLTYPE GetMatrixTransform(_Out_  DXGI_MATRIX_3X2_F *pMatrix);

}; // class dxgi::swapchain2

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::swapchain2<IDXGISwapChain2>;


// Implementation
#include "sp/dx/dxgi/interface/swapchain/idxgiswapchain2_impl.hpp"


#endif // SP_DX_DXGI_IDXGI_SWAPCHAIN2_WRAPPER_CLASS_H_


#endif // _WIN32
