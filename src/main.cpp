#include <cstdint>
#include <windows.h>

#include <constants/constants.hpp>
#include <constants/module_addresses.hpp>
#include <features/nametags/nametags_manager.hpp>

DWORD __stdcall cheatloop(LPVOID lpParam)
{
    bool nametag_enabled{false};
    using namespace managers;

    NametagsManagerPtr nametagsManager{std::make_unique<NametagsManager>()};

    SHORT previousState{0};

    while (true)
    {
        SHORT currentState = GetAsyncKeyState(VK_F10);

        if ((currentState & 0x8000) && !(previousState & 0x8000))
        {
            nametag_enabled ^= true;

            if (nametag_enabled)
                nametagsManager->enable();
            else
                nametagsManager->disable();
        }

        previousState = currentState;

        Sleep(constants::KEY_POLL_VAL_MS);
    }

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        MessageBox(NULL, "bf2_memhack.dll successfully injected", "Injection Confirmation", MB_OK | MB_ICONINFORMATION);
        CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(cheatloop), NULL, 0, NULL);
    }

    return true;
}
