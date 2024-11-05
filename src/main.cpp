#include "features/minimap/minimap_manager.hpp"
#include <cstdint>
#include <windows.h>

#include <constants/constants.hpp>
#include <constants/module_addresses.hpp>
#include <features/nametags/nametags_manager.hpp>

DWORD __stdcall cheatloop(LPVOID lpParam)
{
    using namespace managers;

    NametagsManagerPtr nametagsManager{std::make_unique<NametagsManager>()};
    MinimapManagerPtr minimapManager{std::make_unique<MinimapManager>()};

    bool nametag_enabled{false};
    bool minimap_enabled{false};

    SHORT previous_state_f10{0};
    SHORT previous_state_f11{0};

    while (true)
    {
        SHORT current_state_f10 = GetAsyncKeyState(VK_F10);
        SHORT current_state_f11 = GetAsyncKeyState(VK_F11);

        if ((current_state_f10 & 0x8000) && !(previous_state_f10 & 0x8000))
        {
            nametag_enabled ^= true;

            if (nametag_enabled)
                nametagsManager->enable();
            else
                nametagsManager->disable();
        }

        if ((current_state_f11 & 0x8000) && !(previous_state_f11 & 0x8000))
        {
            minimap_enabled ^= true;

            if (minimap_enabled)
                minimapManager->enable();
            else
                minimapManager->disable();
        }

        previous_state_f10 = current_state_f10;
        previous_state_f11 = current_state_f11;

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
