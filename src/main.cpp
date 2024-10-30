#include <windows.h>

__stdcall void cheatloop()
{
    
}

BOOL WINAPI DllMain([[maybe_unused]] HINSTANCE hinstDLL,
                    [[maybe_unused]] DWORD fdwReason,
                    [[maybe_unused]] LPVOID lpvReserved)
{
    CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(cheatloop), NULL, 0, NULL);
    
    return EXIT_SUCCESS;
}