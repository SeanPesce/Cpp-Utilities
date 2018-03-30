/*
    CONTRIBUTORS:
        Sean Pesce

    Memory patch interface
*/

#pragma once

#ifndef SP_UTILITY_MEMORY_PATCH_INTERFACE_H_
#define SP_UTILITY_MEMORY_PATCH_INTERFACE_H_

#include "sp/sp.h"


__SP_NAMESPACE
namespace mem {


class patch {

protected:

    // Retrieve pointer to location in memory that will be patched
    virtual inline void* _resolve() const = 0;

public:

    // Constructors/destructors
    patch() {}
    virtual inline ~patch() {}

    // Apply patch data
    virtual inline bool apply() const = 0;

    // Restore original data
    virtual inline bool restore() const = 0;

}; // class patch


} // namespace mem
__SP_NAMESPACE_CLOSE // namespace sp

#endif // SP_UTILITY_MEMORY_PATCH_INTERFACE_H_
