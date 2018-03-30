/*
    CONTRIBUTORS:
        Sean Pesce

    preferences.cpp: Defines settings preferences
*/
#include "sp/main/preferences.h"

#include "INIReader.h"

#include "sp/main.h"
#include "sp/config/ini_cfg.h"
#include "sp/environment.h"
#include "sp/file.h"
#include "sp/log.h"
#include "sp/string.h"


#define SP_CONFIG_FILE_ "dxgi_cfg.ini"

#define SP_PREFS_SECTION_DLL "DLL"
#define SP_PREFS_SECTION_STARTUP "Startup"


namespace prefs {

const std::string _config_file = SP_CONFIG_FILE_;
INIReader         _reader = INIReader(sp::file::path_abs<char>(_config_file.c_str()).c_str());

void initialize()
{
    sp::cfg::file = _config_file;

    if (!sp::file::exists(_config_file)) {
        if (sp::file::write_text(_config_file, "", false)) {
            // Error creating config file
        }
    }

    _reader = INIReader(sp::file::path_abs<char>(_config_file).c_str());

    if (_reader.ParseError() < 0) {
        // Error reading config file
        sp::log::write("ERROR: Failed to load settings from '" + _config_file + "'\n");
    } else {
        // DLL wrapper chain
        lib::type  = sp::dx::get_library_type_from_name(_reader.Get(SP_PREFS_SECTION_DLL, "Library", ""));
        lib::chain = _reader.Get(SP_PREFS_SECTION_DLL, "Chain", lib::chain);

        // cmd output window
        startup::open_cmd_out = _reader.GetBoolean(SP_PREFS_SECTION_STARTUP, "CmdOutput", startup::open_cmd_out);

        // Logging preferences
        startup::log_file = _reader.Get(SP_PREFS_SECTION_STARTUP, "LogFile", sp::file::path_abs<char>(startup::log_file));
        sp::log::set_cmd(&global::cmd_out);
        sp::log::set_output_file(startup::log_file);
        startup::log_file = sp::log::output_file(); // Fix output log file string if empty string was read from config file
        sp::log::initialize();
        if (startup::open_cmd_out) {
            global::cmd_out.start();
        }
    }
}



namespace lib {
sp::dx::LIBRARY type  = sp::dx::LIBRARY::unknown;
std::string     chain = "";
} // namespace lib



namespace startup {
#ifdef _DEBUG
bool open_cmd_out = true;
#else
bool open_cmd_out = false;
#endif
std::string log_file = sp::log::output_file();
} // namespace startup


} // namespace prefs


#undef SP_CONFIG_FILE_
