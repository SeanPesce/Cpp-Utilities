/*
    CONTRIBUTORS:
        Sean Pesce

*/

#pragma once

#ifndef SP_UTILITY_SYSTEM_ERROR_CODE_DEFINITIONS_H_
#define SP_UTILITY_SYSTEM_ERROR_CODE_DEFINITIONS_H_


#include "sp/sp.h"

#ifndef   _WIN32
#include <errno.h>
#endif // !_WIN32


#ifdef    _WIN32


#define SP_ERR_NONE                ERROR_SUCCESS
#define SP_ERR_SUCCESS             ERROR_SUCCESS

#define SP_ERR_ACCESS_DENIED       ERROR_ACCESS_DENIED
#define SP_ERR_BAD_FILE_TYPE       ERROR_BAD_FILE_TYPE
#define SP_ERR_BAD_FORMAT          ERROR_BAD_FORMAT
#define SP_ERR_BUFFER_OVERFLOW     ERROR_BUFFER_OVERFLOW
#define SP_ERR_EXISTS              ERROR_ALREADY_EXISTS
#define SP_ERR_FILE_EXISTS         ERROR_FILE_EXISTS
#define SP_ERR_FILE_NAME_TOO_LONG  ERROR_FILENAME_EXCED_RANGE
#define SP_ERR_FILE_NOT_FOUND      ERROR_FILE_NOT_FOUND
#define SP_ERR_INSUFFICIENT_BUFFER ERROR_INSUFFICIENT_BUFFER
#define SP_ERR_INVALID_ADDRESS     ERROR_INVALID_ADDRESS
#define SP_ERR_INVALID_DATA        ERROR_INVALID_DATA
#define SP_ERR_INVALID_DIR         ERROR_BAD_PATHNAME
#define SP_ERR_INVALID_NAME        ERROR_INVALID_NAME
#define SP_ERR_INVALID_PARAMETER   ERROR_INVALID_PARAMETER
#define SP_ERR_NOT_ENOUGH_MEMORY   ERROR_NOT_ENOUGH_MEMORY
#define SP_ERR_OUT_OF_MEMORY       ERROR_OUTOFMEMORY
#define SP_ERR_PATH_NOT_FOUND      ERROR_PATH_NOT_FOUND
#define SP_ERR_READ                ERROR_READ_FAULT
#define SP_ERR_TOO_MANY_LINKS      ERROR_TOO_MANY_LINKS
#define SP_ERR_USER_PROFILE        ERROR_USER_PROFILE_LOAD
#define SP_ERR_WRITE               ERROR_WRITE_FAULT


#else  // UNIX


#define SP_ERR_NONE                0
#define SP_ERR_SUCCESS             0

#define SP_ERR_ACCESS_DENIED       EACCES
#define SP_ERR_BAD_FILE_TYPE       EBADF
#define SP_ERR_BAD_FORMAT          EBFONT
#define SP_ERR_BUFFER_OVERFLOW     EOVERFLOW
#define SP_ERR_EXISTS              EEXIST
#define SP_ERR_FILE_EXISTS         EEXIST
#define SP_ERR_FILE_NAME_TOO_LONG  ENAMETOOLONG
#define SP_ERR_FILE_NOT_FOUND      ENOENT
#define SP_ERR_INSUFFICIENT_BUFFER ENOBUFS
#define SP_ERR_INVALID_ADDRESS     EFAULT
#define SP_ERR_INVALID_DATA        ENODATA
#define SP_ERR_INVALID_NAME        ENAMETOOLONG
#define SP_ERR_INVALID_PARAMETER   EINVAL
#define SP_ERR_NOT_ENOUGH_MEMORY   ENOMEM
#define SP_ERR_OUT_OF_MEMORY       ENOMEM
#define SP_ERR_PATH_NOT_FOUND      ENOENT
#define SP_ERR_READ                EIO
#define SP_ERR_TOO_MANY_LINKS      EMLINK
#define SP_ERR_USER_PROFILE        EUSERS
#define SP_ERR_WRITE               EIO


#endif // _WIN32



__SP_NAMESPACE
namespace err  {



} // namespace err
__SP_NAMESPACE_CLOSE // namespace sp

#endif // SP_UTILITY_SYSTEM_ERROR_CODE_DEFINITIONS_H_
