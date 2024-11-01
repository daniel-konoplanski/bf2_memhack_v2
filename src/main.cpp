#include <windows.h>
#include <constants/module_addresses.hpp>

void WINAPI cheatloop()
{

}

BOOL WINAPI DllMain([[maybe_unused]] HINSTANCE hinstDLL,
                    [[maybe_unused]] DWORD fdwReason,
                    [[maybe_unused]] LPVOID lpvReserved)
{
    constants::modules::initialize_module_addresses();

    CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(cheatloop), NULL, 0, NULL);
    
    return EXIT_SUCCESS;
}
