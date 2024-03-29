/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/ff471331%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGI_DEVICE1_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGI_DEVICE1_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/device/idxgidevice.h"
#include "sp/log.h"
#include "DXGI.h"


__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIDevice1>
class device1 : public sp::dx::dxgi::device<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    device1() {}
    device1(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] Device created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~device1() {}

    // Wrapped interface methods
    virtual HRESULT STDMETHODCALLTYPE SetMaximumFrameLatency(/* [in] */ UINT MaxLatency);
    virtual HRESULT STDMETHODCALLTYPE GetMaximumFrameLatency(_Out_  UINT *pMaxLatency);

}; // class dxgi::device1

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::device1<IDXGIDevice1>;


// Implementation
#include "sp/dx/dxgi/interface/device/idxgidevice1_impl.hpp"


#endif // SP_DX_DXGI_IDXGI_DEVICE1_WRAPPER_CLASS_H_


#endif // _WIN32
