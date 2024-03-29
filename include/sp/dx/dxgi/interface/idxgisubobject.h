/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/bb174528(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGI_DEVICE_SUBOBJECT_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGI_DEVICE_SUBOBJECT_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/idxgiobject.h"
#include "sp/log.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIDeviceSubObject>
class subobject : public sp::dx::dxgi::object<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    subobject() {}
    subobject(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] SubObject created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~subobject() {}

    // Wrapped interface methods
    virtual HRESULT STDMETHODCALLTYPE GetDevice(_In_  REFIID riid, _COM_Outptr_  void **ppDevice);

}; // class dxgi::subobject

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::subobject<IDXGIDeviceSubObject>;


// Implementation
#include "sp/dx/dxgi/interface/idxgisubobject_impl.hpp"


#endif // SP_DX_DXGI_IDXGI_DEVICE_SUBOBJECT_WRAPPER_CLASS_H_


#endif // _WIN32
