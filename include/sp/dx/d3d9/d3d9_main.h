/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        Microsoft documentation: https://msdn.microsoft.com/en-us/library/windows/desktop/bb219839(v=vs.85).aspx
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_D3D9_MAIN_H_
#define SP_DX_D3D9_MAIN_H_

#include "sp/sp.h"
#include "sp/dx/dx_main.h"
#include <string>

#define SP_D3D9_EXPORT_COUNT_ 15


__SP_NAMESPACE
namespace dx   {
namespace d3d9 {

enum EXPORT { D3DPERF_BeginEvent, D3DPERF_EndEvent, D3DPERF_GetStatus, D3DPERF_QueryRepeatFrame, D3DPERF_SetMarker, D3DPERF_SetOptions, D3DPERF_SetRegion, DebugSetLevel, DebugSetMute, Direct3D9EnableMaximizedWindowedModeShim, Direct3DCreate9, Direct3DCreate9Ex, Direct3DShaderValidatorCreate9, PSGPError, PSGPSampleTexture };

static const sp::dx::library library = sp::dx::library::d3d9;
extern const std::string filename;
extern const char* const export_names[SP_D3D9_EXPORT_COUNT_];


} // namespace d3d9
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


#endif // SP_DX_D3D9_MAIN_H_


#endif // _WIN32
