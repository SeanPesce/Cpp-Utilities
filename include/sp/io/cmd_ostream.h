/*
    CONTRIBUTORS:
        Sean Pesce

*/
#ifdef _WIN32
#pragma once

#ifndef SP_INPUT_OUTPUT_CMD_OUTPUT_STREAM_H_
#define SP_INPUT_OUTPUT_CMD_OUTPUT_STREAM_H_

#include "sp/sp.h"
#include "sp/string.h"
#include "sp/environment.h"
#include "sp/system/process/child.h"

#include <string>


#define SP_IO_CMD_OUT_LAUNCH_CMD_ "cmd /V:ON /C \"@ECHO OFF&TITLE " + _title + "&SET \"sp_log_output_line= \"&FOR /l %x IN (1, 0, 1) DO (SET /p sp_log_output_line=\"\"" \
                                  " & ECHO:!sp_log_output_line! & SET \"sp_log_output_line= \" & TASKLIST /FI \"IMAGENAME EQ " + sp::env::exe_name() +             \
                                  "\" 2>NUL | FIND /I /N \"" + sp::env::exe_name() + "\">NUL & IF \"!ERRORLEVEL!\"==\"1\" EXIT 0)\""


__SP_NAMESPACE
namespace io   {


// Class for printing messages to external cmd window
class cmd_ostream : public sp::sys::proc::child {
private:
    // Window title
    std::string _title;

public:
    cmd_ostream(const std::string &title = "CMD Output")
            : sp::sys::proc::child("", "", true, 0)
    {
        _title = title;
        _command = SP_IO_CMD_OUT_LAUNCH_CMD_;
    }

    cmd_ostream(const sp::io::cmd_ostream&) = delete; // Disable copy constructor
    ~cmd_ostream() { terminate(); }

    // @TODO: Overload operator<< for printing

    bool print(const std::string &message = "", bool append_line_feed = true)
    {
        uint32_t bytes_written = 0;
        std::string line_feed = "";
        if (append_line_feed) {
            line_feed = "\n";
        }
        if (running()) {
            if (!WriteFile(io.stdin_write,
                            std::string(message).append(line_feed).c_str(),
                            static_cast<DWORD>(message.length() + line_feed.length()),
                            reinterpret_cast<DWORD*>(&bytes_written),
                            NULL)) {
                // Handle error
                return false;
            }
        } else {
            return false;
        }
        return true;
    }


    inline std::string title() const
    {
        return _title;
    }


    inline void set_title(const std::string &new_title)
    {
        if (_title != new_title) {
            _title = new_title;
            restart();
        }
    }

}; // class cmd_ostream


} // namespace io
__SP_NAMESPACE_CLOSE // namespace sp

#undef SP_IO_CMD_OUT_LAUNCH_CMD_

#endif // SP_INPUT_OUTPUT_CMD_OUTPUT_STREAM_H_


#endif // _WIN32
