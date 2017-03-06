// Made by Sean P

#pragma once

#ifndef SP_SYSUTILS_HPP
	#define SP_SYSUTILS_HPP


#include <cstdint> // uint32_t
#include <cstring> // size_t, strcmp(), memcpy()

#ifdef _WIN32
	#include <Windows.h> // GetSystemInfo(), GetNativeSystemInfo(), GetProcAddress(), GetCurrentProcess(), IsWow64Process(), VirtualProtect()
#else
	#include <stdio.h>      // sprintf()
	#include <unistd.h>     // sysconf(_SC_PAGESIZE), getpid()
	#include <sys/mman.h>   // mprotect()
    #include <fstream>      // ifstream
    #include <iostream>     // cout, endl //@TODO: Delete this line
#endif // _WIN32


// Portable memory protection/info setting constants:
#ifdef _WIN32
	// NOTE: Not all Windows protection settings are included. See Microsoft documentation for more info:
    // https://msdn.microsoft.com/en-us/library/windows/desktop/aa366786(v=vs.85).aspx
    #define MEM_PROTECT_NONE PAGE_NOACCESS
    #define MEM_PROTECT_R PAGE_READONLY
    #define MEM_PROTECT_W PAGE_WRITECOPY
    #define MEM_PROTECT_X PAGE_EXECUTE
    #define MEM_PROTECT_RW PAGE_READWRITE
    #define MEM_PROTECT_RX PAGE_EXECUTE_READ
    #define MEM_PROTECT_WX PAGE_EXECUTE_WRITECOPY
    #define MEM_PROTECT_RWX PAGE_EXECUTE_READWRITE
#else
	#define MEM_PROTECT_NONE PROT_NONE
    #define MEM_PROTECT_R PROT_READ
    #define MEM_PROTECT_W PROT_WRITE
    #define MEM_PROTECT_X PROT_EXEC
    #define MEM_PROTECT_RW (PROT_READ|PROT_WRITE)
    #define MEM_PROTECT_RX (PROT_READ|PROT_EXEC)
    #define MEM_PROTECT_WX (PROT_WRITE|PROT_EXEC)
    #define MEM_PROTECT_RWX (PROT_READ|PROT_WRITE|PROT_EXEC)
    /*MEM_IMAGE
     * MS documentation:
     *  "Indicates that the memory pages within the region are mapped into the view of an image section."
     * Non-MS documentation:
     *  "The page is used to store the module(EXE/DLL). It may share the physical storage with other
     *   process because shared DLL is loaded to memory only once and shared by multiple processes."
     *   Source: https://codesequoia.wordpress.com/2008/11/28/understand-process-address-space-usage/
     */
    #define MEM_IMAGE 0x1000000
    #define MEM_MAPPED 0x40000      // MS documentation: "Indicates that the memory pages within the region are mapped into the view of a section."
    #define MEM_PRIVATE 0x20000     // MS documentation: "Indicates that the memory pages within the region are private (that is, not shared by other processes)."
#endif // _WIN32


//Other constants & typedefs:
#ifdef _WIN32
    typedef bool (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);  // For compatibility with non-supporting Windows systems when calling IsWow64Process()
#else
    // Formatter string for resolving the memory map file of a process:
    #define MEM_MAP_FILE_DIR_FORMAT "/proc/%d/maps"	// The %d will be filled in with the process ID
    // Formatter string for resolving executable file simlink of a process:
    #define PROC_EXE_FILE_LINK_FORMAT "/proc/%d/exe" // The %d will be filled in with the process ID
    // Memory info struct to emulate the kind used in the Windows API, seen here:
    //  https://msdn.microsoft.com/en-us/library/windows/desktop/aa366775(v=vs.85).aspx
    typedef struct SP_MEMORY_BASIC_INFORMATION {
        void*       BaseAddress;
        void*       AllocationBase; // Unused for now
        void*       AllocationProtect; // Unused for now
        size_t      RegionSize;
        uint32_t    State; // Unused for now
        uint32_t    Protect;
        uint32_t    Type;
    } MEMORY_BASIC_INFORMATION;
#endif // _WIN32


///////////// Function prototypes /////////////


/*SP_IsWow64Process()
 * A wrapper function for the Windows API function IsWow64Process(). This wrapper function
 *	calls IsWow64Process() using GetProcAddress() to provide compatibility on a wider range
 *	of systems. This method is described in the Windows API documentation for IsWow64Process():
 *
 *		"IsWow64Process is not available on all supported versions of Windows.
 *		 Use GetModuleHandle to get a handle to the DLL that contains the function
 *		 and GetProcAddress to get a pointer to the function if available."
 *
 *	If GetProcAddress returns NULL, we know that the current system does not support IsWow64Process().
 */
bool SP_IsWow64Process();


/*getProcessBase
 * Obtains the base address of the current process.
 */
void *getProcessBase();


/*is32Bit()
 * Determines whether the current process is running in 32-bit mode or 64-bit mode.
 *
 *  @return true if the current process is running in 32-bit mode; false otherwise.
 */
bool is32Bit();


/*is64Bit()
 * Determines whether the current process is running in 32-bit mode or 64-bit mode.
 *
 *  @return true if the current process is running in 64-bit mode; false otherwise.
 */
bool is64Bit();


/* setMemProtection(void *, size_t, uint32_t, uint32_t *)
 *  Cross-platform wrapper function for changing memory protection settings.
 *      On Windows, VirtualProtect is called. On Unix, mprotect is called.
 *  @param address  The location in memory where the memory protection will be changed.
 *  @param size The size of the section of memory whose protection will be changed.
 *              In Windows, the size is measured in bytes.
 *              In Unix, the size is measured in pages.
 *  @param newProtection    The new memory protection setting for the section of memory.
 *  @param oldProtection    Saves the original memory protection settings on Windows.
 *                          NOTE: The oldProtection parameter is not used in Unix.
 *  @return 0 if successful; non-zero value on failure.
 */
int setMemProtection(void *address, size_t size, uint32_t newProtection, uint32_t *oldProtection);


/* setMemProtection(void *, size_t, uint32_t)
 *	Overloaded version of setMemProtection(void *, size_t, uint32_t, uint32_t *). This is the same
 *		as calling the original function with a NULL oldProtection argument.
 *  @params See @setMemProtection(void *, size_t, uint32_t, uint32_t *) documentation.
 *  @return 0 if successful; non-zero value on failure.
 */
int setMemProtection(void *address, size_t size, uint32_t newProtection);


/*SP_VirtualQuery(void *, MEMORY_BASIC_INFORMATION *, size_t)
 * @TODO: Documentation
 */
size_t SP_VirtualQuery(void *address, MEMORY_BASIC_INFORMATION *buff, size_t length);

/*getMemProtection(void *)
 * Obtains the current memory protection permissions at a given address in memory (for this process).
 *
 *  @param address the address for which this function obtains the permissions
 */
uint32_t getMemProtection(void *address);


/*parseMemMapRegion(const char *, MEMORY_BASIC_INFORMATION *)
 * @TODO: Documentation
 */
void parseMemMapRegion(const char *mapsEntry, MEMORY_BASIC_INFORMATION *memInfo);


/*getPageBase(void *)
 * Obtains the base address of a process page that contains the given memory adress.
 *
 * @param address	The memory address that resides within the page.
 */
void *getPageBase(void *address);


/*getPageSize()
 * Obtains the system page size.
 */
size_t getPageSize();


#endif // SP_SYSUTILS_HPP