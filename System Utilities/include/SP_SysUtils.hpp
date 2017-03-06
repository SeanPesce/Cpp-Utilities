// Made by Sean P

#pragma once

#ifndef SP_SYSUTILS_HPP
	#define SP_SYSUTILS_HPP


#include <cstdint> // uint32_t
#include <cstring> // size_t, strcmp(), memcpy()

#ifdef _WIN32
	#include <Windows.h> // GetSystemInfo(), GetNativeSystemInfo(), GetProcAddress(), GetCurrentProcess(), IsWow64Process(), VirtualProtect()
#else
	#include <stdio.h>      // sprintf(), sscanf()
	#include <unistd.h>     // sysconf(_SC_PAGESIZE), getpid()
	#include <sys/mman.h>   // mprotect()
    #include <fstream>      // ifstream
    #include <errno.h>      // EINVAL
#endif // _WIN32


// Portable memory protection/info setting constants:
#ifdef _WIN32
    // Windows memory protection settings:
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
    #define SP_VQ_ERROR_INVALID_PARAMETER ERROR_INVALID_PARAMETER
#else
    /* Linux memory protection settings:
     *
     * Extra info:
     *  "-The Execute access right always implies the Read access right.
     *   -The Write access right always implies the Read access right."
     *      Source: Understanding the Linux Kernel (3rd Edition) by Daniel Bovet; Section 9.3.2 - Memory Region Access Rights
     *
     */
	#define MEM_PROTECT_NONE PROT_NONE
    #define MEM_PROTECT_R PROT_READ		// PROT_READ == VM_READ
    #define MEM_PROTECT_W PROT_WRITE	// PROT_WRITE == VM_WRITE
    #define MEM_PROTECT_X PROT_EXEC		// PROT_EXEC == VM_EXEC
    #define MEM_PROTECT_RW (PROT_READ|PROT_WRITE)
    #define MEM_PROTECT_RX (PROT_READ|PROT_EXEC)
    #define MEM_PROTECT_WX (PROT_WRITE|PROT_EXEC)
    #define MEM_PROTECT_RWX (PROT_READ|PROT_WRITE|PROT_EXEC)
    #define SP_VQ_ERROR_INVALID_PARAMETER EINVAL
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
 *  NOTE: This function is Windows-only, due to the way 32-bit processes are handled on 64-bit
 *        versions of Windows.
 *
 *	If GetProcAddress returns NULL, we know that the current system does not support IsWow64Process().
 */
 #ifdef _WIN32
bool SP_IsWow64Process();
#endif // _WIN32


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
 * A portable function for querying properties of memory regions.
 *  On Windows, this is merely a wrapper function for VirtualQuery (no extra
 *      functionality).
 *  On Unix, this function aims to emulate VirtualQuery by parsing /proc/self/maps.
 *
 *  @param address  An address that resides within the first memory region to be
 *                  parsed.
 *  @param buff     The buffer for storing memory information for each region.
 *  @param length   The size of the buffer (in bytes). Optimally this should be
 *                  a multiple of sizeof(MEMORY_BASIC_INFORMATION).
 */
size_t SP_VirtualQuery(void *address, MEMORY_BASIC_INFORMATION *buff, size_t length);

/*getMemProtection(void *)
 * Obtains the current memory protection permissions at a given address in memory (for this process).
 *  
 *  @param address The address for which this function obtains access the current access permissions
 */
uint32_t getMemProtection(void *address);


/*parseMemMapRegion(const char *, MEMORY_BASIC_INFORMATION *)
 * Parses a line of text from /proc/self/maps which describes a region of memory
 *  and the properties that apply to it (address range, access protection, etc).
 *  NOTE: This function is for Unix systems only; the memInfo struct aims to
 *        emulate the MEMORY_BASIC_INFORMATION struct returned by VirtualQuery on
 *        Windows operating systems.
 *
 *  @param mapsEntry The line of text from /proc/self/maps file.
 *  @param memInfo The memory information struct where the region's properties will
 *         be stored.
 */
 #ifndef _WIN32
void parseMemMapRegion(const char *mapsEntry, MEMORY_BASIC_INFORMATION *memInfo);
#endif // !_WIN32


/*nextMemRegion(MEMORY_BASIC_INFORMATION *, MEMORY_BASIC_INFORMATION *)
 * Obtains the memory information for the first memory region following the region specified
 *  by the "current" argument. If "current" is NULL, this function obtains the first
 *  memory region for the process.
 *
 *  @param current  The current region of memory.
 *  @param next     Buffer to hold the MEMORY_BASIC_INFORMATION struct for the next region.
 *
 *  @return The base address of the next region, or NULL if the function fails.
 */
void *nextMemRegion(MEMORY_BASIC_INFORMATION *current, MEMORY_BASIC_INFORMATION *next);


/*getPageBase(void *)
 * Obtains the base address of a process page that contains the given memory adress.
 *
 * @param address	The memory address that resides within the page.
 */
#ifdef _MSC_VER
void* __vectorcall getPageBase(void *address); // This function must use __vectorcall to avoid crashes in 32-bit mode
#else
void *getPageBase(void *address);
#endif // _MSC_VER;


/*getPageSize()
 * Obtains the system page size.
 */
size_t getPageSize();


#endif // SP_SYSUTILS_HPP
