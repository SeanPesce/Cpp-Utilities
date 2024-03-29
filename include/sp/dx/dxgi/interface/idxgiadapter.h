/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/bb174523%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGI_ADAPTER_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGI_ADAPTER_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/idxgiobject.h"
#include "sp/log.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIAdapter>
class adapter : public sp::dx::dxgi::object<interface_t> {
protected:
    virtual inline void _wrap(interface_t **original) override
    {
        this->_interface = *original;
        *original = this;
    }

public:
    adapter() {}
    adapter(interface_t **original)
    {
        if (original && *original)
        {
            SP_LOG_DBG("[DXGI] Adapter created (UUID = %s)\n", sp::os::win::com::guid::to_string(__uuidof(**original)).c_str());
        }
        this->_wrap(original);
    }
    virtual ~adapter() {}

    // Wrapped interface methods
    virtual HRESULT STDMETHODCALLTYPE EnumOutputs(/* [in] */ UINT Output, _COM_Outptr_  IDXGIOutput **ppOutput);
    virtual HRESULT STDMETHODCALLTYPE GetDesc(_Out_  DXGI_ADAPTER_DESC *pDesc);
    virtual HRESULT STDMETHODCALLTYPE CheckInterfaceSupport(_In_  REFGUID InterfaceName, _Out_  LARGE_INTEGER *pUMDVersion);
}; // class dxgi::adapter

} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


template class sp::dx::dxgi::adapter<IDXGIAdapter>;


// Implementation
#include "sp/dx/dxgi/interface/idxgiadapter_impl.hpp"


#endif // SP_DX_DXGI_IDXGI_ADAPTER_WRAPPER_CLASS_H_


#endif // _WIN32
