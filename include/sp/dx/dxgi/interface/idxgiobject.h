/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/bb174541(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGIOBJECT_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGIOBJECT_WRAPPER_CLASS_H_

#include "sp/os/windows/com/interface/iunknown.h"
#include "DXGI.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIObject>
class object : public sp::os::win::com::iunknown<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    object() {}
    object(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] Object created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~object() {}

    // Wrapped interface methods
    virtual HRESULT GetParent(REFIID riid, void **ppParent);
    virtual HRESULT GetPrivateData(REFGUID Name, UINT *pDataSize, void *pData);
    virtual HRESULT SetPrivateData(REFGUID Name, UINT DataSize, const void *pData);
    virtual HRESULT SetPrivateDataInterface(REFGUID Name, const IUnknown *pUnknown);
}; // class dxgi::object


} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::object<IDXGIObject>;


// Implementation
#include "sp/dx/dxgi/interface/idxgiobject_impl.hpp"


#endif // SP_DX_DXGI_IDXGIOBJECT_WRAPPER_CLASS_H_


#endif // _WIN32
