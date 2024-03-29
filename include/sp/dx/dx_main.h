/*
    CONTRIBUTORS:
        Sean Pesce

*/
#ifdef _WIN32
#pragma once

#ifndef SP_DX_MAIN_H_
#define SP_DX_MAIN_H_

#include "sp/sp.h"
#include <string>


#define SP_DX_LIB_TYPE_COUNT_ 8
#define SP_DX_LIB_NAME_UNKNOWN_ "_UNKNOWN_"
#define SP_DX_LIB_NAME_DINPUT8_ "dinput8"
#define SP_DX_LIB_NAME_D3D8_    "d3d8"
#define SP_DX_LIB_NAME_D3D9_    "d3d9"
#define SP_DX_LIB_NAME_D3D10_   "d3d10"
#define SP_DX_LIB_NAME_D3D11_   "d3d11"
#define SP_DX_LIB_NAME_D3D12_   "d3d12"
#define SP_DX_LIB_NAME_DXGI_    "dxgi"


__SP_NAMESPACE
namespace dx {


enum class library { unknown, dinput8, d3d8, d3d9, d3d10, d3d11, d3d12, dxgi };
extern const std::string library_name[SP_DX_LIB_TYPE_COUNT_];


sp::dx::library get_library_type(sp::dx::library default_type = sp::dx::library::unknown);
std::string     get_library_name();
sp::dx::library get_library_type_from_name(const std::string &name, bool remove_file_ext = true);
std::string     get_library_name_from_type(sp::dx::library type);


} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp


#endif // SP_DX_MAIN_H_


#endif // _WIN32
