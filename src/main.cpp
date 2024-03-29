/*
    CONTRIBUTORS:
        Sean Pesce

*/
#include "sp/main.h"
#include "sp/gui/message_box.h"
#include "sp/os/windows/dll.h"
#include "sp/log.h"
#include "sp/main/preferences.h"
#include "sp/dx/d3d9/d3d9_hooks.h"
#include "sp/dx/dxgi/dxgi_hooks.h"


#define SP_LIBRARY_NOT_YET_SUPPORTED sp::gui::open_dialog<char>("ERROR: Unsupported library (DirectX Overlay)", sp::str::format("Wrapping %s is not a feature yet, but future support is planned.", (sp::dx::get_library_name_from_type(global::lib::type) + ".dll").c_str())); ExitProcess(0);


namespace global {


namespace lib    {

// Wrapped library type
sp::dx::library type = sp::dx::library::unknown;
// Starting address of the wrapped library
HMODULE chain = NULL;
// Filename of the wrapped library
std::string chain_filename = "";

// Loads chained library (if one was specified)
bool load_chain()
{
    chain_filename = prefs::lib::chain;
    if (chain_filename.length() > 0) {
        global::lib::chain = LoadLibrary(chain_filename.c_str());
        if (!global::lib::chain) {
            SP_LOG("ERROR: Failed to load chained library wrapper: %s", chain_filename.c_str());
            chain_filename = "";
            sp::err::set(SP_ERR_BAD_FORMAT);
            return false;
        } else {
            SP_LOG_DBG_ELSE_PRINT("Loaded chained library wrapper: %s", chain_filename.c_str());
            return true;
        }
    }
    SP_LOG_DBG_ELSE_PRINT("No library chain specified; shared library will be loaded from system directory.");
    return false;
}

// Loads shared system copy of the wrapped library
bool load_shared_library()
{
    using sp::str::to_lowercase;
    using sp::str::format;
    bool retry = false, success = false;
    // Get DLL filename using DirectX library type (dxgi, d3d9, d3d10, etc)
    switch (global::lib::type)
    {
        case sp::dx::library::unknown:
            chain_filename = to_lowercase(sp::env::lib_name());
            break;
        default:
            chain_filename = sp::dx::get_library_name_from_type(global::lib::type) + ".dll";
            if (to_lowercase(chain_filename) != to_lowercase(sp::env::lib_name())) {
                retry = true;
            }
            break;
    } // switch (global::lib::type)

    std::string result_msg = "";
    // Attempt to load chained library
    chain = (HMODULE)sp::lib::load_shared(chain_filename);
    if (!chain) {
        result_msg = format("ERROR: Failed to load wrapped library (%s).", chain_filename.c_str());
        if (retry) {
            chain_filename = sp::env::lib_name();
            result_msg += format(" Retrying with current filename instead (%s)...\n", chain_filename.c_str());
            chain = (HMODULE)sp::lib::load_shared(chain_filename);
            if (chain) {
                // Successfully loaded shared library instead of the specified library to chain
                result_msg += format("SUCCESS: %s was loaded.", chain_filename.c_str());
                success = true;
            } else {
                // Failed to load shared library
                result_msg += format("FAILED: Unable to load %s", chain_filename.c_str());
                success = false;
            }
        }
    } else {
        result_msg = format("Loaded wrapped library: %s", chain_filename.c_str());
        success = true;
    }

    if (success) {
        SP_LOG_DBG_ELSE_PRINT(result_msg);
    } else {
        SP_LOG(result_msg);
    }
    return success;
}


bool hook_exports()
{
    switch (global::lib::type)
    {
        case sp::dx::library::d3d8:
            SP_LIBRARY_NOT_YET_SUPPORTED // @TODO
            break;
        case sp::dx::library::d3d9:
            sp::dx::d3d9::hook_exports();
            break;
        case sp::dx::library::d3d10:
            SP_LIBRARY_NOT_YET_SUPPORTED // @TODO
            break;
        case sp::dx::library::d3d11:
            SP_LIBRARY_NOT_YET_SUPPORTED // @TODO
            break;
        case sp::dx::library::d3d12:
            SP_LIBRARY_NOT_YET_SUPPORTED // @TODO
            break;
        case sp::dx::library::dinput8:
            SP_LIBRARY_NOT_YET_SUPPORTED // @TODO
            break;
        case sp::dx::library::dxgi:
            sp::dx::dxgi::hook_exports();
            break;
        case sp::dx::library::unknown:
            // @TODO: Attempt to determine what library this DLL is being loaded as a wrapper for
            SP_LIBRARY_NOT_YET_SUPPORTED
            return false;
            break;
        default:
            SP_LIBRARY_NOT_YET_SUPPORTED
            return false;
            break;
    } // switch (global::lib::type)
    return true;
}

} // namespace lib


sp::io::ps_ostream& cmd_out = sp::log::ostream();


void initialize()
{
    prefs::initialize();
    lib::type = sp::dx::get_library_type(prefs::lib::type);

    if (!lib::load_chain()) {
        lib::load_shared_library();
    }

    if (!lib::chain) {
        // Failed to load a wrapped library
        sp::gui::open_dialog<char>("ERROR (DirectX Overlay)", "Unable to load wrapped library. Possible fixes:\n\n"
            " - Rename the overlay DLL to the same file name as the original shared library\n"
            " - Set the library type in the config file (\"Library=_NAME_\")\n"
            " - Specify the chained library to load in the config file (\"Chain=_DLL_PATH_\")");
        ExitProcess(0);
    }

    if (!lib::hook_exports()) {
        // Error hooking exported functions
    }
}


} // namespace global


#undef SP_LIBRARY_NOT_YET_SUPPORTED
