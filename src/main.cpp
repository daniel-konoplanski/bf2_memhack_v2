#include <cstdint>
#include <windows.h>
#include <constants/module_addresses.hpp>
#include <features/nametags/nametags_manager.hpp>

static constexpr uint32_t KEY_PRESS_SLEEP_VAL_MS{ 300 };

DWORD __stdcall cheatloop(LPVOID lpParam)
{
	bool nametag_enabled{ false };
    using namespace managers;

    NametagsManagerPtr nametagsManager{ std::make_unique<NametagsManager>() };

    // while (true)
    // {
	// 	if (GetAsyncKeyState(VK_F10))
	// 	{
	//     	if (nametag_enabled)
	// 		{
				nametagsManager->enable();
				nametag_enabled = !nametag_enabled;
				Sleep(KEY_PRESS_SLEEP_VAL_MS);
		// 	}
		// 	else
		// 	{
		// 		nametagsManager->disable();
		// 		nametag_enabled = !nametag_enabled;
		// 		Sleep(KEY_PRESS_SLEEP_VAL_MS);
		// 	}
		// }

		Sleep(100000);
    // }

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
