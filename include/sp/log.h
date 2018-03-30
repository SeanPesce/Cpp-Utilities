/*
    CONTRIBUTORS:
        Sean Pesce

*/

#pragma once

#ifndef SP_UTILITY_LOG_H_
#define SP_UTILITY_LOG_H_

#include "sp/sp.h"
#include "sp/io/cmd_ostream.h"
#include <string>

#ifdef _DEBUG
#define SP_LOG_DBG(format_str, ...) sp::log::write(sp::str::format(format_str, __VA_ARGS__))
#define SP_LOG_DBG_ELSE_PRINT(format_str, ...) sp::log::write(sp::str::format(format_str, __VA_ARGS__)) // Writes log in debug builds. If release build, only prints to console instead
#else
#define SP_LOG_DBG(format_str, ...)
#define SP_LOG_DBG_ELSE_PRINT(format_str, ...) sp::log::print_cmd(sp::str::format(format_str, __VA_ARGS__))
#endif // _DEBUG

#define SP_LOG(format_str, ...) sp::log::write(sp::str::format(format_str, __VA_ARGS__))


__SP_NAMESPACE
namespace log {

extern bool append; // If true, log file is not overwritten during initialization

void initialize();
void write(const std::string &message, bool append_new_line = true);
bool write_file(const std::string &message, bool append_new_line = true, bool overwrite = false);
std::string output_file();
void set_output_file(const std::string &file);
bool print_cmd(const std::string &message, bool append_new_line = true);
void set_cmd(sp::io::cmd_ostream* cmd);
void reset_cmd();
bool open_cmd();
bool close_cmd();


} // namespace log
__SP_NAMESPACE_CLOSE // namespace sp

#endif // SP_UTILITY_LOG_H_
