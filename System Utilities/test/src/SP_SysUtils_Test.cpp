// Made by Sean P

#include "SP_SysUtils_Test.hpp"


int main()
{
	SP_printIntro("Demo Program for SP_SysUtil Functions"); // Print the program name and author information

	#ifdef _WIN32
		std::cout << "WOW64 process? ";
		if(SP_IsWow64Process())
		{
			std::cout << "Yes" << std::endl;
		}
		else
		{
			std::cout << "No" << std::endl;
		}
	#endif // _WIN32
	std::cout << "Page size: " << getPageSize() << " bytes" << std::endl;
	std::cout << "Starting address of main() function: " << (void *)&main << std::endl;
	std::cout << "Base address of page containing main(): " << getPageBase((void *)&main) << std::endl;
	std::cout << "Memory protection settings:    R=" << MEM_PROTECT_R << "   X=" << MEM_PROTECT_X << "   RX=" << MEM_PROTECT_RX << "   RWX=" << MEM_PROTECT_RWX << std::endl;
	std::cout << "Setting protection to X at main..." << std::endl;
	setMemProtection((void *)&main, 1,  MEM_PROTECT_X);
	std::cout << "Memory protection at main(): " << getMemProtection((void *)&main) << std::endl;
	std::cout << "Setting protection to RWX at main()..." << std::endl;
	setMemProtection((void *)&main, 1,  MEM_PROTECT_RWX);
	std::cout << "New memory protection at main(): " << getMemProtection((void *)&main) << std::endl << std::endl;

	enterToContinue("Press enter to continue...", "\n");

	return 0;
}