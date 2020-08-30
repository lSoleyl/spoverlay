#include "stdafx.h"

#include <Process.hpp>

#include <iostream>
#include <thread>

HMODULE THIS_DLL = nullptr;

BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,  // handle to DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpReserved)  // reserved
{
    // Perform actions based on the reason for calling.
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      THIS_DLL = hinstDLL;


      if (!Process::IsInjectorProcess()) {
        // We are inside our target process...

        if (AllocConsole()) {
          FILE* fDummy;
          freopen_s(&fDummy, "CONIN$", "r", stdin);
          freopen_s(&fDummy, "CONOUT$", "w", stderr);
          freopen_s(&fDummy, "CONOUT$", "w", stdout);
        }


        std::cout << "soundpad_overlay.dll successfully injected into target process!" << std::endl;
      }

     // Initialize once for each new process.
     // Return FALSE to fail DLL load.
      break;

    case DLL_THREAD_ATTACH:
     // Do thread-specific initialization.
      break;

    case DLL_THREAD_DETACH:
     // Do thread-specific cleanup.
      break;

    case DLL_PROCESS_DETACH:
     // Perform any necessary cleanup.
      break;
  }
  return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
