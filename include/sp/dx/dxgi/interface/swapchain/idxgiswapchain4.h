/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/mt732707(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGI_SWAPCHAIN4_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGI_SWAPCHAIN4_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/swapchain/idxgiswapchain3.h"
#include "sp/log.h"
#include "DXGI1_5.h"


__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGISwapChain4>
class swapchain4 : public sp::dx::dxgi::swapchain3<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    swapchain4() {}
    swapchain4(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] SwapChain created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~swapchain4() {}

    // Wrapped interface methods
    virtual HRESULT STDMETHODCALLTYPE SetHDRMetaData(_In_  DXGI_HDR_METADATA_TYPE Type, _In_  UINT Size, _In_reads_opt_(Size)  void *pMetaData);

}; // class dxgi::swapchain4

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::swapchain4<IDXGISwapChain4>;


// Implementation
#include "sp/dx/dxgi/interface/swapchain/idxgiswapchain4_impl.hpp"


#endif // SP_DX_DXGI_IDXGI_SWAPCHAIN4_WRAPPER_CLASS_H_


#endif // _WIN32
