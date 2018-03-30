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
public:
    // Wrapped interface method
    UINT GetCreationFlags();
}; // class dxgi::factory3



} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


// Implementation
#include "sp/dx/dxgi/interface/factory/idxgifactory3_impl.hpp"


#endif // SP_DX_DXGI_IDXGIFACTORY3_WRAPPER_CLASS_H_


#endif // _WIN32
