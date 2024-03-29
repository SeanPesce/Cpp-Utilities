/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/dn457942(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGIFACTORY3_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGIFACTORY3_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/factory/idxgifactory2.h"
#include "DXGI1_3.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIFactory3>
class factory3 : public sp::dx::dxgi::factory2<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    factory3() {}
    factory3(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] Factory created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~factory3() {}

    // Wrapped interface method
    virtual UINT GetCreationFlags();
}; // class dxgi::factory3



} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::factory3<IDXGIFactory3>;


// Implementation
#include "sp/dx/dxgi/interface/factory/idxgifactory3_impl.hpp"


#endif // SP_DX_DXGI_IDXGIFACTORY3_WRAPPER_CLASS_H_


#endif // _WIN32
