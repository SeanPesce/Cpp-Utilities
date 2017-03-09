// Made by Sean P

#include "SP_AoB_Scan_Example.hpp"


int main()
{
    sp_print_intro("\nAoB Scan Demo Program");

    uint8_t buff[255];
    bool mask[255];
    size_t len = 0;

    reset_aob_scan_settings();

    if(is_64bit()) // this is a 64-bit process
    {
        // Define array to search for based on architecture and OS:
        #ifdef _WIN32
            bool m[10] = { true, true, true, true, true, true, true, true, true, true };  // Mask array
            uint8_t aob[10] = { 0xbf, 0x60, 0x52, 0x60, 0x00, 0xe8, 0x99, 0xda, 0xff, 0xff }; // starts at &dummy_function() + 104
            const char *str_aob = "bf 60 52 60 00 e8 99 da ff ff";
            char str_aob2[30] = { 'b', 'f', ' ', '6', '0', ' ', '5', '2', ' ', '6', '0', ' ', '0', '0', ' ', 'e', '8', ' ', '9', '9', ' ', 'd', 'a', ' ', 'f', 'f', ' ', 'f', 'f', '\0' };
            std::string str_aob3("bf 60 52 60 00 e8 99 da ff ff");
        #else
            bool m[10] = { true, true, true, true, true, true, true, true, true, true };  // Mask array
            uint8_t aob[10] = { 0xbf, 0x60, 0x52, 0x60, 0x00, 0xe8, 0x96, 0xd9, 0xff, 0xff }; // starts at &dummy_function() + 104
            const char *str_aob = "bf 60 52 60 00 e8 96 d9 ff ff";
            char str_aob2[30] = { 'b', 'f', ' ', '6', '0', ' ', '5', '2', ' ', '6', '0', ' ', '0', '0', ' ', 'e', '8', ' ', '9', '6', ' ', 'd', '9', ' ', 'f', 'f', ' ', 'f', 'f', '\0' };
            std::string str_aob3("bf 60 52 60 00 e8 96 d9 ff ff");
        #endif // _WIN32


        // Test aob_scan using byte array:
        print_aob(aob, (size_t)10);
        std::cout << "   found at   " << aob_scan(aob, 10, NULL) << std::endl;

        // Test aob_scan using strings:
        std::cout << str_aob << "   found at   " << aob_scan(str_aob) << std::endl;
        std::cout << str_aob2 << "   found at   " << aob_scan(str_aob2) << std::endl;
        std::cout << str_aob3 << "   found at   " << aob_scan(&str_aob3) << std::endl;
    }
    else // this is a 32-bit process
    {
        // Define array to search for based on architecture and OS:
        #ifdef _WIN32
            bool m[10] = { true, true, true, true, true, true, true, true, true, true };  // Mask array
            uint8_t aob[10] = { 0xe8, 0x39, 0xe0, 0xff, 0xff, 0x83, 0xc4, 0x10, 0x83, 0x45 }; // starts at &dummy_function() + 104
            const char *str_aob = "e8 39 e0 ff ff 83 c4 10 83 45";
            char str_aob2[30] = { 'e', '8', ' ', '3', '9', ' ', 'e', '0', ' ', 'f', 'f', ' ', 'f', 'f', ' ', '8', '3', ' ', 'c', '4', ' ', '1', '0', ' ', '8', '3', ' ', '4', '5', '\0' };
            std::string str_aob3("e8 39 e0 ff ff 83 c4 10 83 45");
        #else
            bool m[10] = { true, true, true, true, true, true, true, true, true, true };  // Mask array
            uint8_t aob[10] = { 0xe8, 0x6b, 0xdf, 0xff, 0xff, 0x83, 0xc4, 0x10, 0x83, 0x45 }; // starts at &dummy_function() + 104
            const char *str_aob = "e8 6b df ff ff 83 c4 10 83 45";
            char str_aob2[30] = { 'e', '8', ' ', '6', 'b', ' ', 'd', 'f', ' ', 'f', 'f', ' ', 'f', 'f', ' ', '8', '3', ' ', 'c', '4', ' ', '1', '0', ' ', '8', '3', ' ', '4', '5', '\0' };
            std::string str_aob3("e8 6b df ff ff 83 c4 10 83 45");
        #endif // _WIN32


        // Test aob_scan using byte array:
        print_aob(aob, (size_t)10);
        std::cout << "   found at   " << aob_scan(aob, 10, NULL) << std::endl;

        // Test aob_scan using strings:
        std::cout << str_aob << "   found at   " << aob_scan(str_aob) << std::endl;
        std::cout << str_aob2 << "   found at   " << aob_scan(str_aob2) << std::endl;
        std::cout << str_aob3 << "   found at   " << aob_scan(&str_aob3) << std::endl;
    }

    return 0;
}

// Prints a formatted byte array with masked bytes:
void print_aob(uint8_t *aob, bool *mask, size_t len)
{
    for(int b = 0; b < len; b++)
    {
        if(mask[b])
        {
            char cb[5] = { '\0', '\0', '\0', '\0', '\0' };
            sprintf(cb, "%02x", aob[b]);
            std::cout << cb;
        }
        else
        {
            std::cout << "??";
        }
        if(b != len-1)
        {
            std::cout << " ";
        }
    }
}

// Prints a formatted byte array (without mask):
void print_aob(uint8_t *aob, size_t len)
{
    for(int b = 0; b < len; b++)
    {
        char cb[5] = { '\0', '\0', '\0', '\0', '\0' };
        sprintf(cb, "%02x", aob[b]);
        std::cout << cb;
        if(b != len-1)
        {
            std::cout << " ";
        }
    }
}

// Prints formatted byte mask:
void print_mask(bool *mask, size_t len)
{
    for(int b = 0; b < len; b++)
    {
        if(mask[b])
        {
            std::cout << "bb";
        }
        else
        {
            std::cout << "??";
        }
        if(b != len-1)
        {
            std::cout << " ";
        }
    }
}


// This function isn't called; it's just used to test AoB Scanning:
void dummy_function()
{
    std::cout << "This is a dummy function. It should never be called." << std::endl;
    for(int i = 1; i <= 20; i++)
    {
        std::cout << "i = " << i << std::endl;
    }
    std::cout << "We have counted to 20. Congratulations." << std::endl;
}
