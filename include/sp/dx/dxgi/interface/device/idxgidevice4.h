/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/mt732704(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGI_DEVICE4_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGI_DEVICE4_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/device/idxgidevice3.h"
#include "sp/log.h"
#include "DXGI1_5.h"


__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIDevice4>
class device4 : public sp::dx::dxgi::device3<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    device4() {}
    device4(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] Device created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~device4() {}

    // Wrapped interface methods
    virtual HRESULT STDMETHODCALLTYPE OfferResources1(_In_  UINT NumResources, _In_reads_(NumResources)  IDXGIResource *const *ppResources, _In_  DXGI_OFFER_RESOURCE_PRIORITY Priority, _In_  UINT Flags);
    virtual HRESULT STDMETHODCALLTYPE ReclaimResources1(_In_  UINT NumResources, _In_reads_(NumResources)  IDXGIResource *const *ppResources, _Out_writes_all_(NumResources)  DXGI_RECLAIM_RESOURCE_RESULTS *pResults);

}; // class dxgi::device4

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::device4<IDXGIDevice4>;


// Implementation
#include "sp/dx/dxgi/interface/device/idxgidevice4_impl.hpp"


#endif // SP_DX_DXGI_IDXGI_DEVICE4_WRAPPER_CLASS_H_


#endif // _WIN32
