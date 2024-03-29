/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/hh404543(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGI_DEVICE2_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGI_DEVICE2_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/device/idxgidevice1.h"
#include "sp/log.h"
#include "DXGI1_2.h"


__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIDevice2>
class device2 : public sp::dx::dxgi::device1<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    device2() {}
    device2(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] Device created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~device2() {}

    // Wrapped interface methods
    virtual HRESULT STDMETHODCALLTYPE OfferResources(_In_  UINT NumResources, _In_reads_(NumResources)  IDXGIResource *const *ppResources, _In_  DXGI_OFFER_RESOURCE_PRIORITY Priority);
    virtual HRESULT STDMETHODCALLTYPE ReclaimResources(_In_  UINT NumResources, _In_reads_(NumResources)  IDXGIResource *const *ppResources, _Out_writes_all_opt_(NumResources)  BOOL *pDiscarded);
    virtual HRESULT STDMETHODCALLTYPE EnqueueSetEvent(_In_  HANDLE hEvent);

}; // class dxgi::device2

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::device2<IDXGIDevice2>;


// Implementation
#include "sp/dx/dxgi/interface/device/idxgidevice2_impl.hpp"


#endif // SP_DX_DXGI_IDXGI_DEVICE2_WRAPPER_CLASS_H_


#endif // _WIN32
