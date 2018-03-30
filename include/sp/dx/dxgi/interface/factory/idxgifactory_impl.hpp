/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/bb174535(v=vs.85).aspx
*/
#ifdef _WIN32

#ifndef SP_DX_DXGI_IDXGIFACTORY_WRAPPER_CLASS_H_
#error Invalid header inclusion "dx/dxgi/interface/factory/idxgifactory_impl.hpp" (Use "dx/dxgi/interface/factory/idxgifactory.h" instead)
#endif

#include "sp/sp.h"


__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t>
HRESULT factory<interface_t>::CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter **ppAdapter)
{
    return _interface->CreateSoftwareAdapter(Module, ppAdapter);
}


template <class interface_t>
HRESULT factory<interface_t>::CreateSwapChain(IUnknown *pDevice, DXGI_SWAP_CHAIN_DESC *pDesc, IDXGISwapChain **ppSwapChain)
{
    return _interface->CreateSwapChain(pDevice, pDesc, ppSwapChain);
}


template <class interface_t>
HRESULT factory<interface_t>::EnumAdapters(UINT Adapter, IDXGIAdapter **ppAdapter)
{
    return _interface->EnumAdapters(Adapter, ppAdapter);
}


template <class interface_t>
HRESULT factory<interface_t>::GetWindowAssociation(HWND *pWindowHandle)
{
    return _interface->GetWindowAssociation(pWindowHandle);
}


template <class interface_t>
HRESULT factory<interface_t>::MakeWindowAssociation(HWND WindowHandle, UINT Flags)
{
    return _interface->MakeWindowAssociation(WindowHandle, Flags);
}


} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


#endif // _WIN32
