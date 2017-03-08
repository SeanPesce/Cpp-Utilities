// Made by Sean P

/*  
    CREDITS:
    
    The overarching scan procedure is inspired by the algorithm written by SteveAndrews
    on the Cheat Engine forums. His profile can be found here:
    http://forum.cheatengine.org/profile.php?mode=viewprofile&u=332503&sid=d456d53e8550ae5efe3c04f60d4a081e

        The specific thread where SteveAndrews' code was posted:
        http://forum.cheatengine.org/viewtopic.php?p=5582229&sid=c47e69a1330187cd8035016ab56c736d
    
    

    The find_aob algorithm is heavily modified from an algorithm written by the user fdsasdf
    on the UnKnoWnCheaTs forum. His profile can be found here:
    https://www.unknowncheats.me/forum/members/645501.html

        The specific post where fdsasdf's code can be found:
        https://www.unknowncheats.me/forum/1064093-post3.html
    


    SEE ALSO:

    A github repository comparing benchmarks of various FindPattern algorithms (including fdsasdf's):
    https://github.com/learn-more/findpattern-bench/
*/

#include "SP_AoB_Scan.hpp"


///////////// Global Flags & Variables /////////////

bool AOBS_SKIP_EXECUTABLE_MEM = false;   // If true, memory with "Executable" flag will NOT be searched
bool AOBS_SKIP_MAPPED_MEM = true;        // If true, MEM_MAPPED memory will NOT be searched




//////////////////// Functions /////////////////////


// Scans all accessible regions of current process memory for an array of
//  bytes (AoB), beginning at the given starting address:
void *aob_scan(uint8_t *aob, bool *mask, size_t length, void *start)
{
    MEMORY_BASIC_INFORMATION region;
    void *result = NULL;

    if(aob == NULL || length <= 0)
    {
        set_error(SP_ERROR_INVALID_PARAMETER);
        return NULL;
    }
    else if(length > SP_AOB_MAX_LENGTH)
    {
        set_error(SP_ERROR_INSUFFICIENT_BUFFER); // AoB is too long
        return NULL;
    }

    if(start != NULL)
    {
        // Get the region of the starting address, if it was specified:
        if(virtual_query(start, &region, sizeof(region)) < sizeof(region))
        {
            // Invalid address; set error number
            set_error(SP_ERROR_INVALID_ADDRESS);
            return NULL;
        }
        else 
        {
            // Adjust region base and size to account for not starting at the base address:
            region.RegionSize -= (size_t)((uint8_t*)start - (uint8_t*)region.BaseAddress);
            region.BaseAddress = start;
        }
    }
    else
    {
        region.BaseAddress = NULL; // First call to next_mem_region(&region, &region) gets process base if region.BaseAddress == NULL
        region.RegionSize = 0;
        region.Protect = MEM_PROTECT_NONE;
    }

    do
    {
        if(is_aob_scannable(&region) && region.RegionSize >= length)
        {
            result = find_aob((uint8_t*)region.BaseAddress, region.RegionSize, aob, mask, length);

            if(result != NULL)
            {
                return result;
            }
            
        }
    }while(next_mem_region(&region, &region) != NULL);

    return NULL;
}


// Convenient override functions for aob_scan:
void *aob_scan(uint8_t *aob, bool *mask, size_t length)
{
    return aob_scan(aob, mask, length, NULL); // If no starting address was specified, use NULL and scan will begin at process base
}

void *aob_scan(uint8_t *aob, size_t length, void *start)
{
    // If no mask was specified, pass NULL to indicate all bytes must match
    return aob_scan(aob, NULL, length, start);
}

void *aob_scan(uint8_t *aob, size_t length)
{
    // If no mask was specified, pass NULL to indicate all bytes must match
    return aob_scan(aob, NULL, length, NULL); // If no starting address was specified, use NULL and scan will begin at process base
}

void *aob_scan(const char *str_aob, void *start)
{
    size_t str_aob_len = std::char_traits<char>::length(str_aob); // Length of str_aob != length of final byte array
    uint8_t aob[str_aob_len];
    bool mask[str_aob_len];
    size_t length = string_to_aob(str_aob, aob, mask);
    return aob_scan(aob, mask, length, start);
}

void *aob_scan(char *str_aob, void *start)
{
    size_t str_aob_len = std::char_traits<char>::length(str_aob); // Length of str_aob != length of final byte array
    uint8_t aob[str_aob_len];
    bool mask[str_aob_len];
    size_t length = string_to_aob((const char *)str_aob, aob, mask);
    return aob_scan(aob, mask, length, start);
}

void *aob_scan(std::string *str_aob, void *start)
{
    size_t str_aob_len = std::char_traits<char>::length(str_aob->c_str()); // Length of str_aob != length of final byte array
    uint8_t aob[str_aob_len];
    bool mask[str_aob_len];
    size_t length = string_to_aob(str_aob->c_str(), aob, mask);
    return aob_scan(aob, mask, length, start);
}

void *aob_scan(const char *str_aob)
{
    size_t str_aob_len = std::char_traits<char>::length(str_aob); // Length of str_aob != length of final byte array
    uint8_t aob[str_aob_len];
    bool mask[str_aob_len];
    size_t length = string_to_aob(str_aob, aob, mask);
    return aob_scan(aob, mask, length, NULL);
}

void *aob_scan(char *str_aob)
{
    size_t str_aob_len = std::char_traits<char>::length(str_aob); // Length of str_aob != length of final byte array
    uint8_t aob[str_aob_len];
    bool mask[str_aob_len];
    size_t length = string_to_aob((const char *)str_aob, aob, mask);
    return aob_scan(aob, mask, length, NULL);
}

void *aob_scan(std::string *str_aob)
{
    size_t str_aob_len = std::char_traits<char>::length(str_aob->c_str()); // Length of str_aob != length of final byte array
    uint8_t aob[str_aob_len];
    bool mask[str_aob_len];
    size_t length = string_to_aob(str_aob->c_str(), aob, mask);
    return aob_scan(aob, mask, length, NULL);
}


// Searches for an AoB within a given section of memory:
void *find_aob(uint8_t *base, size_t region_size, uint8_t *aob, bool *mask, size_t length)
{
    uint8_t *end = base + region_size - length;

    for(; base <= end; ++base)
    {
        if(mask != NULL && compare_byte_arrays(base, aob, mask, length))
        {
            return base;
        }
        else if(compare_byte_arrays(base, aob, length)) // No mask
        {
            return base;
        }
    }
    return NULL;
}


// Returns true if both byte arrays are identical (excluding wildcard bytes):
bool compare_byte_arrays(uint8_t *mem, uint8_t *aob, bool *mask, size_t length)
{
    uint8_t *end = aob + length;
    for(; aob < end; mem++, aob++, mask++)
    {
        if(*mask && *mem != *aob)
        {
            return false;
        }
    }
    return true;
}


// Returns true if both byte arrays are identical:
bool compare_byte_arrays(uint8_t *mem, uint8_t *aob, size_t length) // No mask
{
    uint8_t *end = aob + length;
    for(; aob < end; mem++, aob++)
    {
        if(*mem != *aob)
        {
            return false;
        }
    }
    return true;
}


// Checks if a region of memory should be scanned with the current scan settings:
bool is_aob_scannable(MEMORY_BASIC_INFORMATION *mem_info)
{
    return (
        mem_info->Protect != MEM_PROTECT_NONE &&         // Check if memory is non-accessible.
        !(AOBS_SKIP_MAPPED_MEM && mem_info->Type == MEM_MAPPED) &&    // Check if memory is mapped and skipping mapped memory is on.
        !(AOBS_SKIP_EXECUTABLE_MEM && ( // Check if executable memory should be skipped.
            mem_info->Protect == MEM_PROTECT_X ||
            mem_info->Protect == MEM_PROTECT_RX ||
            mem_info->Protect == MEM_PROTECT_WX ||
            mem_info->Protect == MEM_PROTECT_RWX
        ))
    );
}


// Converts a C string-formatted, null-terminated string representing an AoB to a true byte
//  array and generates a corresponding mask for determining wildcards.
size_t string_to_aob(const char *str_aob, uint8_t *aob, bool *mask)
{
    size_t str_aob_len = std::char_traits<char>::length(str_aob); // Length of str_aob != length of final byte array
    size_t pos = 0; // Position in final byte array

    for(int c = 0; c < str_aob_len; c++ /* lol */)
    {
        if(str_aob[c] == ' ') // Skip spaces
        {
            while(str_aob[c+1] == ' ') // Allow multiple spaces (very forgiving)
            {
                c++;
            }
            continue;
        }
        else if(str_aob[c] == '?' || str_aob[c] == 'x' || str_aob[c] == 'X') // Check for wildcard
        { // Allow "?", "??", "x", "xx", "X", or "XX" to specify wildcard
            // Allow wildcard specification to be 1 or 2 REPEATED wildcard characters:
            if(str_aob[c] == '?' && str_aob[c+1] == '?')  
            {
                c++;
            }
            else if(str_aob[c] == 'x' && str_aob[c+1] == 'x')
            {
                c++;
            }
            else if(str_aob[c] == 'X' && str_aob[c+1] == 'X')
            {
                c++;
            }
            mask[pos] = false; // Set pattern mask to wildcard
            aob[pos] = 0;
            pos++;
            continue;
        }
        else
        {
            const char byte_chars[3] = { str_aob[c], str_aob[c+1], '\0' };

            sscanf(byte_chars, "%2x", (unsigned int*)&aob[pos]); // Parse next 2 characters as a hex byte and store in aob

            mask[pos] = true; // Set pattern mask to non-wildcard
            pos++;
            c++; // 2 characters were read
        }
    }

    return pos;
}


// Overload of string_to_aob using a non-constant char *:
size_t string_to_aob(char *str_aob, uint8_t *aob, bool *mask)
{
    return string_to_aob((const char *)str_aob, aob, mask);
}


// Overload of string_to_aob using a C++ string *:
size_t string_to_aob(std::string *str_aob, uint8_t *aob, bool *mask)
{
    return string_to_aob(str_aob->c_str(), aob, mask);
}


// Sets global search flags to their default values:
void reset_aob_scan_settings()
{
    AOBS_SKIP_EXECUTABLE_MEM = false;
    AOBS_SKIP_MAPPED_MEM = true;
}

