// Made by Sean P

#include "SP_SysUtils.hpp"




///////////// SYSTEM UTILITIES /////////////


#ifdef _WIN32
// Wrapper function for IsWow64Process that provides compatibility on a wider range
//	of systems:
bool SP_IsWow64Process()
{
	bool isWow64Proc = false;

	// From Microsoft documentation:
	//	"IsWow64Process is not available on all supported versions of Windows.
	//	 Use GetModuleHandle to get a handle to the DLL that contains the function
	//	 and GetProcAddress to get a pointer to the function if available."
	LPFN_ISWOW64PROCESS funcIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	if(NULL != funcIsWow64Process)
    {
        if (!funcIsWow64Process(GetCurrentProcess(), (PBOOL)&isWow64Proc))
        {
            // IsWow64Proccess() call failed; assume the process isn't running under WOW64? @TODO: handle this better?
        }
    }

    return isWow64Proc;
}
#endif // _WIN32


// Get the base adress of the currrent process:
void *getProcessBase()
{
    #ifdef _WIN32
        return GetModuleHandle(NULL); // From MS documentation, HANDLE == void *
    #else
        // Resolve /proc/self/maps path (/proc/$PID/maps):
        char memMapFilePath[32];
		sprintf(memMapFilePath, MEM_MAP_FILE_DIR_FORMAT, (int)getpid());
        std::ifstream inFile(memMapFilePath); // Open file input stream
        void *processBase = NULL; // Will hold the base address of the process
        std::string line; // Will hold the first line of /proc/self/maps, which contains the base address

        if(std::getline(inFile, line))
        {
            sscanf(line.c_str(), "%p-", &processBase);
            return processBase;
        }
        else
        {
            // getline() failed
            return NULL;
        }

    #endif // _WIN32
}


// Determine whether the current process is running in 32-bit mode or 64-bit mode:
bool is32Bit()
{
    return (sizeof(void *) == sizeof(uint32_t));
}


// Determine whether the current process is running in 32-bit mode or 64-bit mode:
bool is64Bit()
{
    return (sizeof(void *) == sizeof(uint64_t));
}



///////////// MEMORY UTILITIES /////////////


// Set the memory protection permissions for a given section of memory:
int setMemProtection(void *address, size_t size, uint32_t newProtection, uint32_t *oldProtection)
{
    #ifdef _WIN32
        // Windows (use VirtualProtect)
        if(oldProtection == NULL){
            uint32_t oldProt; // If the user passes NULL for oldProtection, use &oldProt (otherwise VirtualProtect fails)
            return !VirtualProtect(address, size, (DWORD)newProtection, (DWORD*)&oldProt);
        } // Else...
        return !VirtualProtect(address, size, (DWORD)newProtection, (DWORD*)oldProtection);
    
    #else
        // Unix (use mprotect)
        *oldProtection = getMemProtection(address); // Create backup of old memory permissions
        return mprotect(getPageBase(address), size, (int)newProtection); // getMemPage is called to obtain a page-aligned address

    #endif // _WIN32
}


// Set the memory protection permissions for a given section of memory:
int setMemProtection(void *address, size_t size, uint32_t newProtection)
{
    #ifdef _WIN32
        uint32_t oldProt; // Must use &oldProt, otherwise VirtualProtect fails
        return !VirtualProtect(address, size, (DWORD)newProtection, (DWORD*)&oldProt);
    
    #else
        // Unix (use mprotect)
        return mprotect(getPageBase(address), size, (int)newProtection); // getMemPage is called to obtain a page-aligned address

    #endif // _WIN32
}


// On Windows, this a wrapper function for VirtualQuery (no functional changes).
// On Linux, this function attempts to emulate the usage of the VirtualQuery function from the Windows API.
size_t SP_VirtualQuery(void *address, MEMORY_BASIC_INFORMATION *buff, size_t length)
{
    #ifdef _WIN32
        return VirtualQuery((LPCVOID)address, buff, length);
    #else
        // Resolve /proc/self/maps path (/proc/$PID/maps):
        char memMapFilePath[32];
		sprintf(memMapFilePath, MEM_MAP_FILE_DIR_FORMAT, (int)getpid());

        std::ifstream inFile(memMapFilePath); // Open file input stream

        MEMORY_BASIC_INFORMATION region; // Memory info struct
        std::string line; // Holds each line of data from /proc/self/maps file as they are parsed
        size_t total = length / sizeof(MEMORY_BASIC_INFORMATION); // Get the number of regions to parse
        size_t count = 0; // Number of regions that have been parsed thus far
        
        // Parse each memory region to find the requested starting region:
        while(std::getline(inFile, line) && count != total)
        {
            parseMemMapRegion(line.c_str(), &region);
            if(region.BaseAddress <= address && (uint8_t *)region.BaseAddress+region.RegionSize > address)
            {
                // Starting region has been found
                memcpy(buff+count, &region, sizeof(MEMORY_BASIC_INFORMATION)); // Copy first region's info to buffer
                count++;
                break;
            }
        }
        // If buffer room still exists, get info for subsequent regions:
        while(std::getline(inFile, line) && count != total)
        {
            memcpy(buff+count, &region, sizeof(MEMORY_BASIC_INFORMATION));
            count++;
        }
        inFile.close();
        return count * sizeof(MEMORY_BASIC_INFORMATION);
    #endif // _WIN32
}


// Get the current memory protection permissions at a given address in memory (for THIS process):
uint32_t getMemProtection(void *address)
{
    MEMORY_BASIC_INFORMATION memInfo;
    if(SP_VirtualQuery(address, &memInfo, sizeof(memInfo)) < sizeof(memInfo))
    {
        // SP_VirtualQuery failed
        return -1;
    }
    return (uint32_t)memInfo.Protect;
}


// Parses a line from /proc/self/maps and stores the info in a MEMORY_BASIC_INFORMATION struct:
#ifndef _WIN32
void parseMemMapRegion(const char *mapsEntry, MEMORY_BASIC_INFORMATION *memInfo)
{
    void *start, *end; // Starting and ending addresses of the current region being parsed
    char permissionFlags[4] = { '\0', '\0', '\0', '\0' }, // Read/Write/Execute flags
         sharedFlag,    // "Shared" or "Private" flag character
         mappedDev[6] = { '\0', '\0', '\0', '\0', '\0', '\0' }; // If region is mapped to a file, this is the device number where the file resides

    // Parse the entry text:
    sscanf(mapsEntry, "%p-%p %3c%c %*x %5c", &start, &end, permissionFlags, &sharedFlag, mappedDev);

    // Set the region base address and size:
    memInfo->BaseAddress = (void*)start;
    memInfo->RegionSize = (size_t)((uint8_t*)end-(uint8_t*)start);

    // Set the permissions flags:
    memInfo->Protect = 0;
    if(strcmp(permissionFlags, "---") == 0)
    {
        memInfo->Protect = MEM_PROTECT_NONE;
    }
    else
    {
        if(permissionFlags[0] == 'r')
        {
            memInfo->Protect |= MEM_PROTECT_R;
        }
        if(permissionFlags[1] == 'w')
        {
            memInfo->Protect |= MEM_PROTECT_W;
        }
        if(permissionFlags[2] == 'x')
        {
            memInfo->Protect |= MEM_PROTECT_X;
        }
    }
    
    // Set the region type (Private/Mapped/Image):
    memInfo->Type = 0;
    if(sharedFlag == 'p')
    {
        memInfo->Type = MEM_PRIVATE;
    }
    else if(strcmp(mappedDev, "00:00") != 0)
    {
        memInfo->Type = MEM_MAPPED;
    }
    // @TODO: determine if regions are MEM_IMAGE using /proc/$PID/exe simlink
}
#endif // !_WIN32


// Obtain the starting address of the page of memory that contains the given memory address:
#ifdef _MSC_VER
void* __vectorcall getPageBase(void *address) // This function must use __vectorcall to avoid crashes in 32-bit mode
#else
void *getPageBase(void *address)
#endif // _MSC_VER
{
    if(is32Bit())
    { // 32-bit process
        // Calculate and return the base address of the page:
        return (void*)((uint32_t)(uint64_t)address - ((uint32_t)(uint64_t)address % getPageSize())); // Double cast to subdue compiler warnings
    }
    else
    { // 64-bit process
        // Calculate and return the base address of the page:
        return (void*)((uint64_t)address - ((uint64_t)address % getPageSize()));
    }
}


// Obtain the system page size:
size_t getPageSize()
{
	// Determine the OS:
    #ifdef _WIN32 // Windows:
        SYSTEM_INFO sysInfo;
		if(SP_IsWow64Process())
		{
			GetNativeSystemInfo(&sysInfo);
		}
		else
		{
			GetSystemInfo(&sysInfo);
		}

        return (size_t)sysInfo.dwPageSize;
    #else // Unix:
        return (size_t)sysconf(_SC_PAGESIZE);
    #endif // _WIN32
}
