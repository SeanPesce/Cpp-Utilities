/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/mt722566(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_IDXGIFACTORY5_WRAPPER_CLASS_H_
#define SP_DX_DXGI_IDXGIFACTORY5_WRAPPER_CLASS_H_

#include "sp/dx/dxgi/interface/factory/idxgifactory4.h"
#include "DXGI1_5.h"

__SP_NAMESPACE
namespace dx   {
namespace dxgi {


template <class interface_t = IDXGIFactory5>
class factory5 : public sp::dx::dxgi::factory4<interface_t> {
public:
    // Wrapped interface method
    HRESULT CheckFeatureSupport(DXGI_FEATURE Feature, void *pFeatureSupportData, UINT FeatureSupportDataSize);
}; // class dxgi::factory5



} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


// Implementation
#include "sp/dx/dxgi/interface/factory/idxgifactory5_impl.hpp"


#endif // SP_DX_DXGI_IDXGIFACTORY5_WRAPPER_CLASS_H_


#endif // _WIN32
