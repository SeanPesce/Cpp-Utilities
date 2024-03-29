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
#include "sp/os/windows/dll.h"



#define SP_PREFS_SECTION_DLL "DLL"
#define SP_PREFS_SECTION_STARTUP "Startup"


std::string prefs::_file = prefs::defaults::file;
INIReader prefs::_reader = INIReader(sp::file::path_abs<char>(prefs::file().c_str()).c_str());


sp::dx::library prefs::lib::type  = sp::dx::library::unknown;
std::string     prefs::lib::chain = "";


bool prefs::_initialized = false;


// If config file is missing, this private function creates a new one before initialization.
void _prefs_pre_init(const char* filename = SP_CONFIG_FILE_NAME_)
{
    if (!sp::log::initialized())
    {
        sp::log::initialize();
    }

    if (!sp::file::exists(filename))
    {
        long err = sp::file::write_text(filename, std::string());

        if (err == SP_ERR_SUCCESS)
        {
            SP_LOG("[%s %s] Auto-generated configuration file: %s\n",
                sp::str::get_date(sp::util::YYYYMMDD).c_str(),
                sp::str::get_time().c_str(),
                filename);
        }
        else
        {
            // Failed to create config file
            SP_LOG("[%s %s] ERROR: Failed to generate configuration file (%s)\n",
                sp::str::get_date(sp::util::YYYYMMDD).c_str(),
                sp::str::get_time().c_str(),
                filename);
        }
    }
}



bool prefs::initialize(const char* filename)
{
    if (!sp::log::initialized())
    {
        sp::log::initialize();
    }
    if (!prefs::_initialized)
    {
        if (filename == NULL)
        {
            filename = prefs::defaults::file;
        }
        prefs::_file = filename;
        _prefs_pre_init(filename);
    }

    prefs::_reader = INIReader(sp::file::path_abs<char>(prefs::file().c_str()).c_str());

    if (prefs::_reader.ParseError() < 0)
    {
        // Error reading config file
        sp::log::write("ERROR: Failed to load settings from '" + sp::file::path_abs<char>(prefs::file().c_str()) + "'\n\n");
        return false;
    }
    else
    {
        // Logging preferences
        //sp::log::set_file(
        //        prefs::_reader.Get(SP_PREFS_SECTION_STARTUP, "LogFile", sp::file::path_abs<char>(sp::log::defaults::file)), // Default log file name
        //        true, // Reinitialize log
        //        prefs::_reader.GetBoolean(SP_PREFS_SECTION_STARTUP, "CmdOutput", SP_DEBUG_BUILD) // Open log output window?
        //    );
        if (user_setting<bool>("Console", "ExternalOutputWindow", SP_DEBUG_BUILD).read()) // Open log output window?
        {
            sp::log::show();
        }



        // DLL wrapper chain
        prefs::lib::type = sp::dx::get_library_type_from_name(user_setting<std::string>(SP_PREFS_SECTION_DLL, "Library", "").read());
        prefs::lib::chain = user_setting<std::string>(SP_PREFS_SECTION_DLL, "Chain", prefs::lib::chain).read();

        sp::lib::load_user_specified();
    }
    prefs::_initialized = true;
    return true;
}


const INIReader& user_setting_base_internal::reader()
{
    return prefs::reader();
}


#undef SP_CONFIG_FILE_
