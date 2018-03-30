/*
    CONTRIBUTORS:
        Sean Pesce

*/

#pragma once

#ifndef SP_DX_OVERLAY_PREFERENCES_H_
#define SP_DX_OVERLAY_PREFERENCES_H_

#include "sp/sp.h"
#include "sp/dx/dxgi/dxgi_main.h"

#include <string>


namespace prefs   {


void initialize();

namespace lib     {
extern sp::dx::LIBRARY type;
extern std::string     chain;
} // namespace lib

namespace startup {
extern bool open_cmd_out;
extern std::string log_file;
} // namespace startup


} // namespace prefs



#endif // SP_DX_OVERLAY_PREFERENCES_H_
