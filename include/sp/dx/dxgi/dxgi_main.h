/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        Microsoft documentation: https://msdn.microsoft.com/en-us/library/windows/desktop/bb205169%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
        ReShade source:          https://github.com/crosire/reshade
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_MAIN_H_
#define SP_DX_DXGI_MAIN_H_

#include "sp/sp.h"
#include "sp/dx/dx_main.h"
#include <string>

#define SP_DXGI_EXPORT_COUNT_ 20


__SP_NAMESPACE
namespace dx   {
namespace dxgi {

enum EXPORT { ApplyCompatResolutionQuirking, CompatString, CompatValue, CreateDXGIFactory, CreateDXGIFactory1, CreateDXGIFactory2, DXGID3D10CreateDevice, DXGID3D10CreateLayeredDevice, DXGID3D10ETWRundown, DXGID3D10GetLayeredDeviceSize, DXGID3D10RegisterLayers, DXGIDumpJournal, DXGIGetDebugInterface1, DXGIReportAdapterConfiguration, DXGIRevertToSxS, PIXBeginCapture, PIXEndCapture, PIXGetCaptureState, SetAppCompatStringPointer, UpdateHMDEmulationStatus };

static const sp::dx::library library = sp::dx::library::dxgi;
extern const std::string filename;
extern const char* const export_names[SP_DXGI_EXPORT_COUNT_];


} // namespace dxgi
} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


#endif // SP_DX_DXGI_MAIN_H_


#endif // _WIN32
