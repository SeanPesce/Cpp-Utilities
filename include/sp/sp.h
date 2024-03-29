/*
    CONTRIBUTORS:
        Sean Pesce

*/

#pragma once

#ifndef SEAN_PESCE_H_
#define SEAN_PESCE_H_


#ifndef   __NO_SP_NAMESPACE
#define   __USE_SP_NAMESPACE
#endif // __NO_SP_NAMESPACE


#ifdef __USE_SP_NAMESPACE
#define __SP_NAMESPACE namespace sp {
#define __SP_NAMESPACE_CLOSE }
#define sp sp
#else
#define __SP_NAMESPACE
#define __SP_NAMESPACE_CLOSE
#define sp
#endif // __USE_SP_NAMESPACE



__SP_NAMESPACE
extern const char*    author;
extern const wchar_t* author_w;
extern const char*    build;
extern const wchar_t* build_w;
__SP_NAMESPACE_CLOSE



#ifdef     _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else  // !_WIN32

#endif //  _WIN32


#if (defined(_DEBUG) | !defined(NDEBUG))
#define SP_DEBUG_BUILD true
#else
#define SP_DEBUG_BUILD false
#endif // DEBUG checks


#include "sp/error.h"


#endif // SEAN_PESCE_H_
