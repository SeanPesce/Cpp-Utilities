/*
    CONTRIBUTORS:
        Sean Pesce

*/

#pragma once

#ifndef SP_DYNAMIC_LINK_LIBRARY_H_
#define SP_DYNAMIC_LINK_LIBRARY_H_

#include "sp/sp.h"
#include "sp/environment.h"

#include <string>


__SP_NAMESPACE
namespace lib {


// If given a relative file path, loads a DLL from the default system directory
inline void* load_system_library(const std::string &filename)
{
    return LoadLibrary((sp::env::system_dir() + "\\" + filename).c_str());
}


} // namespace lib
__SP_NAMESPACE_CLOSE // namespace sp

#endif // SP_DYNAMIC_LINK_LIBRARY_H_
