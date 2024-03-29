/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/ff471335(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGIFACTORY1_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGIFACTORY1_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/factory/idxgifactory.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIFactory1>
class factory1 : public sp::dx::dxgi::factory<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    factory1() {}
    factory1(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] Factory created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~factory1() {}

    // Wrapped interface methods
    virtual HRESULT EnumAdapters1(UINT Adapter, IDXGIAdapter1 **ppAdapter);
    virtual BOOL    IsCurrent();
}; // class dxgi::factory1



} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::factory1<IDXGIFactory1>;


// Implementation
#include "sp/dx/dxgi/interface/factory/idxgifactory1_impl.hpp"


#endif // SP_DX_DXGI_IDXGIFACTORY1_WRAPPER_CLASS_H_


#endif // _WIN32
