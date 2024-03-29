/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/dn280345(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGI_DEVICE3_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGI_DEVICE3_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/device/idxgidevice2.h"
#include "sp/log.h"
#include "DXGI1_3.h"


__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIDevice3>
class device3 : public sp::dx::dxgi::device2<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    device3() {}
    device3(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] Device created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~device3() {}

    // Wrapped interface methods
    virtual void STDMETHODCALLTYPE Trim();

}; // class dxgi::device3

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::device3<IDXGIDevice3>;


// Implementation
#include "sp/dx/dxgi/interface/device/idxgidevice3_impl.hpp"


#endif // SP_DX_DXGI_IDXGI_DEVICE3_WRAPPER_CLASS_H_


#endif // _WIN32
