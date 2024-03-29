/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/bb174527(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGI_DEVICE_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGI_DEVICE_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/idxgiobject.h"
#include "sp/log.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIDevice>
class device : public sp::dx::dxgi::object<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    device() {}
    device(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] Device created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~device() {}

    // Wrapped interface methods
    virtual HRESULT STDMETHODCALLTYPE GetAdapter(_COM_Outptr_  IDXGIAdapter **pAdapter);
    virtual HRESULT STDMETHODCALLTYPE CreateSurface(_In_  const DXGI_SURFACE_DESC *pDesc, UINT NumSurfaces, DXGI_USAGE Usage, _In_opt_  const DXGI_SHARED_RESOURCE *pSharedResource, _COM_Outptr_  IDXGISurface **ppSurface);
    virtual HRESULT STDMETHODCALLTYPE QueryResourceResidency(_In_reads_(NumResources)  IUnknown *const *ppResources, _Out_writes_(NumResources)  DXGI_RESIDENCY *pResidencyStatus, UINT NumResources);
    virtual HRESULT STDMETHODCALLTYPE SetGPUThreadPriority(INT Priority);
    virtual HRESULT STDMETHODCALLTYPE GetGPUThreadPriority(_Out_  INT *pPriority);

}; // class dxgi::device

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::device<IDXGIDevice>;


// Implementation
#include "sp/dx/dxgi/interface/device/idxgidevice_impl.hpp"


#endif // SP_DX_DXGI_IDXGI_DEVICE_WRAPPER_CLASS_H_


#endif // _WIN32
