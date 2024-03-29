/*
    CONTRIBUTORS:
        Sean Pesce

    References:
        Microsoft documentation: https://msdn.microsoft.com/en-us/library/windows/desktop/bb205169%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
        ReShade source:          https://github.com/crosire/reshade
*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_DXGI_EXPORTS_H_
#define SP_DX_DXGI_EXPORTS_H_

#include "sp/sp.h"
#include "DXGI.h"
#include "Dxgi1_3.h"
//#include "Dxgi1_6.h"
#include "DXGIDebug.h"

#ifdef _WIN64
#define SP_DXGI_ALTCALL __fastcall
#else
#define SP_DXGI_ALTCALL WINAPI
#endif // __WIN64



/*  Creates a DXGI 1.0 factory that you can use to generate other DXGI objects. */
typedef HRESULT(WINAPI *CreateDXGIFactory_t)(REFIID, void**);
HRESULT WINAPI  CreateDXGIFactory(REFIID riid, void **ppFactory);


/*  Creates a DXGI 1.1 factory that you can use to generate other DXGI objects. */
typedef HRESULT(WINAPI *CreateDXGIFactory1_t)(REFIID, void**);
HRESULT WINAPI  CreateDXGIFactory1(REFIID riid, void **ppFactory);


/*
    Creates a DXGI 1.3 factory that you can use to generate other DXGI objects.

    In Windows 8, any DXGI factory created while DXGIDebug.dll was present on the system would
    load and use it. Starting in Windows 8.1, apps explicitly request that DXGIDebug.dll be
    loaded instead. Use CreateDXGIFactory2 and specify the DXGI_CREATE_FACTORY_DEBUG flag to
    request DXGIDebug.dll; the DLL will be loaded if it is present on the system.
*/
typedef HRESULT(WINAPI *CreateDXGIFactory2_t)(UINT, REFIID, void**);
HRESULT WINAPI  CreateDXGIFactory2(UINT Flags, REFIID riid, void **ppFactory);


/*  Allows a process to indicate that it's resilient to any of its graphics devices being removed. */
// NOTE: Not supported until next Windows update
typedef HRESULT(WINAPI *DXGIDeclareAdapterRemovalSupport_t)();
HRESULT WINAPI DXGIDeclareAdapterRemovalSupport();


/*  Retrieves a debugging interface. */
typedef HRESULT(WINAPI *DXGIGetDebugInterface_t)(REFIID, void**);
HRESULT WINAPI DXGIGetDebugInterface(REFIID riid, void **ppDebug);


/*  Retrieves an interface that Windows Store apps use for debugging the Microsoft DirectX Graphics Infrastructure (DXGI). */
typedef HRESULT(WINAPI *DXGIGetDebugInterface1_t)(UINT, REFIID, void**);
HRESULT WINAPI  DXGIGetDebugInterface1(UINT Flags, REFIID riid, void **ppDebug);


/*  Undocumented DXGI export */
typedef HRESULT(WINAPI *DXGID3D10CreateDevice_t)(HMODULE, IDXGIFactory*, IDXGIAdapter*, UINT, void*, void**);
HRESULT WINAPI DXGID3D10CreateDevice(HMODULE hModule, IDXGIFactory *pFactory, IDXGIAdapter *pAdapter, UINT Flags, void *pUnknown, void **ppDevice);

/*  Undocumented DXGI export */
typedef HRESULT(WINAPI *DXGID3D10CreateLayeredDevice_t)(void*, void*, void*, void*, void*);
HRESULT WINAPI DXGID3D10CreateLayeredDevice(void *pUnknown1, void *pUnknown2, void *pUnknown3, void *pUnknown4, void *pUnknown5);

/*  Undocumented DXGI export */
typedef SIZE_T(WINAPI *DXGID3D10GetLayeredDeviceSize_t)(const void*, UINT);
SIZE_T WINAPI DXGID3D10GetLayeredDeviceSize(const void *pLayers, UINT NumLayers);

/*  Undocumented DXGI export */
typedef HRESULT(WINAPI *DXGID3D10RegisterLayers_t)(const void*, UINT);
HRESULT WINAPI DXGID3D10RegisterLayers(const void *pLayers, UINT NumLayers);


/*  Undocumented DXGI export */
typedef void(SP_DXGI_ALTCALL *ApplyCompatResolutionQuirking_t)(void*, void*);
void SP_DXGI_ALTCALL ApplyCompatResolutionQuirking(void *pUnknown1, void *pUnknown2);

/*  Undocumented DXGI export */
typedef int(WINAPI *CompatString_t)(const char*, void**, char*, bool);
int WINAPI CompatString(const char *strUnknown1, void **ppUnknown, char *strUnknown2, bool bUnknown);

/*  Undocumented DXGI export */
typedef void*(SP_DXGI_ALTCALL *CompatValue_t)(const char*, void**);
void* SP_DXGI_ALTCALL CompatValue(const char *strUnknown, void **ppUnknown);

/*  Undocumented DXGI export */
typedef void(SP_DXGI_ALTCALL *DXGIDumpJournal_t)(void(WINAPI*)(const char*));
void SP_DXGI_ALTCALL DXGIDumpJournal(void(WINAPI *funcUnknown)(const char*));

/*  Undocumented DXGI export */
typedef void*(WINAPI *DXGIRevertToSxS_t)();
void* WINAPI DXGIRevertToSxS();

/*  Undocumented DXGI export */
typedef void(SP_DXGI_ALTCALL *SetAppCompatStringPointer_t)(void*, const char*);
void SP_DXGI_ALTCALL SetAppCompatStringPointer(void *pUnknown, const char *strUnknown);

/*  Undocumented DXGI export */
typedef long(SP_DXGI_ALTCALL *UpdateHMDEmulationStatus_t)(bool);
long SP_DXGI_ALTCALL UpdateHMDEmulationStatus(bool bUnknown);

// The following function prototypes only appear in IDA for the x86 binary:
//int     WINAPI DXGIReportAdapterConfiguration(int iUnknown);
//__int32 WINAPI PIXBeginCapture(unsigned int iUnknown, void *pUnknown);
//__int32 WINAPI PIXEndCapture(int iUnknown);


/*  Additional undocumented DXGI exports (hooked through ASM) */
//DXGID3D10ETWRundown
//DXGIReportAdapterConfiguration
//PIXBeginCapture
//PIXEndCapture
//PIXGetCaptureState


#endif // SP_DX_DXGI_EXPORTS_H_


#endif // _WIN32
