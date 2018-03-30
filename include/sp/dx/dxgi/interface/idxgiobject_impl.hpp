/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/bb174541(v=vs.85).aspx
*/
#ifdef _WIN32

#ifndef SP_DX_DXGI_IDXGIOBJECT_WRAPPER_CLASS_H_
#error Invalid header inclusion "dx/dxgi/interface/idxgiobject_impl.hpp" (Use "dx/dxgi/interface/idxgiobject.h" instead)
#endif

#include "sp/sp.h"


__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t>
HRESULT object<interface_t>::GetParent(REFIID riid, void **ppParent)
{
    return _interface->GetParent(riid, ppParent);
}


template <class interface_t>
HRESULT object<interface_t>::GetPrivateData(REFGUID Name, UINT *pDataSize, void *pData)
{
    return _interface->GetPrivateData(Name, pDataSize, pData);
}


template <class interface_t>
HRESULT object<interface_t>::SetPrivateData(REFGUID Name, UINT DataSize, const void *pData)
{
    return _interface->SetPrivateData(Name, DataSize, pData);
}


template <class interface_t>
HRESULT object<interface_t>::SetPrivateDataInterface(REFGUID Name, const IUnknown *pUnknown)
{
    return _interface->SetPrivateDataInterface(Name, pUnknown);
}


} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


#endif // _WIN32
