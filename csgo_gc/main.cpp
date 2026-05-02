#include "stdafx.h"
#include "platform.h"
#include "steam_hook.h"
#include "gc_client.h" // Add this to access g_gc_client
#include <thread>      // Add this for the background thread

#if defined(_MSC_VER)
#define DLL_EXPORT extern "C" __declspec(dllexport)
#elif defined(__GNUC__)
#define DLL_EXPORT extern "C" __attribute__((visibility("default")))
#else
#error
#endif

// This function runs in the background
void KeyListener() 
{
    while (true) 
    {
        // Check if F8 is pressed
        if (GetAsyncKeyState(VK_F8) & 0x8000) 
        {
            // Call the function we will create in gc_client
            // Assuming g_gc_client is the global instance
            extern gc_client g_gc_client; 
            g_gc_client.trigger_fake_accept();

            // Prevent multiple triggers from one tap
            Sleep(1000); 
        }
        Sleep(10); // Don't hog the CPU
    }
}

DLL_EXPORT void InstallGC(bool dedicated)
{
    Platform::Initialize();
    SteamHookInstall(dedicated);

    // Start our key listener thread if we aren't a dedicated server
    if (!dedicated) {
        std::thread(KeyListener).detach();
    }
}