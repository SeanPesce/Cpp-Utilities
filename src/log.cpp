/*
    CONTRIBUTORS:
        Sean Pesce

*/

#include "sp/log.h"
#include "sp/system/process.h"
#include "sp/error.h"
#include "sp/file.h"
#include "sp/environment.h"
#include <iostream>



__SP_NAMESPACE
namespace log {


bool _enable_cmd = false;
sp::io::cmd_ostream __cmd;
sp::io::cmd_ostream* _cmd = &__cmd;
std::string _output_file = "SP_log.log";


// If true, log file is not overwritten during initialization
bool append = false;

inline bool _init_file();


void initialize()
{
    _init_file();
    if (_enable_cmd) {
        _cmd->start();
    }
}


void write(const std::string &message, bool append_new_line)
{
    std::string append = append_new_line ? "\n" : "";
    std::cout << (message + append);
    file::append_text(_output_file, message + append);
    print_cmd(message, append_new_line);
}


inline bool _init_file()
{
    long e = SP_ERR_SUCCESS;
    if (append) {
        e = file::append_text(_output_file, std::string(""));
    } else {
        e = file::write_text(_output_file, std::string(""));
    }
    return (e == SP_ERR_SUCCESS);
}


// Records message, but only in the log file
bool write_file(const std::string &message, bool append_new_line, bool overwrite)
{
    long e = SP_ERR_SUCCESS;
    std::string append = append_new_line ? sp::str::endl<char>() : "";
    if (overwrite) {
        e = file::write_text(_output_file, message + append);
    } else {
        e = file::append_text(_output_file, message + append);
    }
    return (e == SP_ERR_SUCCESS);
}


std::string output_file()
{
    return _output_file;
}


void set_output_file(const std::string &file)
{
    if (file.length() > 0) {
        _output_file = file;
    } else {
        // Log file can't be empty string
        sp::err::set(SP_ERR_INVALID_PARAMETER);
    }
}


bool print_cmd(const std::string &message, bool append_new_line)
{
    return _cmd->print(message, append_new_line);
}


void set_cmd(sp::io::cmd_ostream* cmd)
{
    if (cmd == NULL) {
        sp::err::set(SP_ERR_INVALID_PARAMETER);
        return;
    }
    bool running = _cmd->running();
    if (running) {
        _cmd->terminate();
    }
    _cmd = cmd;
    if (running) {
        _cmd->start();
    }
}


void reset_cmd()
{
    bool running = _cmd->running();
    if (running) {
        _cmd->terminate();
    }
    _cmd = &__cmd;
    if (_cmd->running()) {
        _cmd->terminate();
    }
    if (running) {
        _cmd->start();
    }
}


bool open_cmd()
{
    return _cmd->start();
}


bool close_cmd()
{
    return _cmd->terminate();
}


} // namespace log
__SP_NAMESPACE_CLOSE // namespace sp
