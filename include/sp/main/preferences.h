/*
    CONTRIBUTORS:
        Sean Pesce

*/

#pragma once

#ifndef SP_UTIL_GLOBAL_PREFERENCES_H_
#define SP_UTIL_GLOBAL_PREFERENCES_H_

#include "sp/sp.h"
#include "sp/log.h"
#include "sp/preferences/user_setting.h"
#include "sp/dx/dxgi/dxgi_main.h"

#include <string>



#ifndef SP_CONFIG_FILE_NAME_
#define SP_CONFIG_FILE_NAME_ "config.ini"
//#ifdef _WIN32
//#pragma message("Runtime configuration file name defaulting to \"" SP_CONFIG_FILE_NAME_ "\" (Use #define SP_CONFIG_FILE_NAME_ to set a custom file)")
//#else // UNIX
//#warning "Runtime configuration file name defaulting to \"" SP_CONFIG_FILE_NAME_ "\" (Use #define SP_CONFIG_FILE_NAME_ to set a custom file)"
//#endif // _WIN32
#endif




class prefs {
private:
    prefs() = delete;

    static bool _initialized;
    static std::string _file; // Configuration file
    static INIReader _reader; // File reader


public:
    static bool initialize(const char* filename = SP_CONFIG_FILE_NAME_);

    static inline bool initialized() { return prefs::_initialized; }
    static inline const std::string& file() { return prefs::_file; }

    static inline const INIReader& reader()
    {
        if (prefs::_reader.ParseError() < 0)
        {
            prefs::_reader = INIReader(sp::file::path_abs<char>(prefs::file().c_str()).c_str());
        }
        return prefs::_reader;
    }


    class lib {
    public:
        static sp::dx::library type;
        static std::string chain;
    private:
        lib() = delete;
    };


    class defaults {
    public:
        static constexpr const char* file = SP_CONFIG_FILE_NAME_;
    private:
        defaults() = delete;
    };
};



#endif // SP_UTIL_GLOBAL_PREFERENCES_H_
