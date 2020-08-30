#include "stdafx.h"

#include <Handle.hpp>
#include <DllInjection.hpp>

#include <utility/scope.hpp>

#include <Psapi.h>
#include <TlHelp32.h>

bool DllInjection::Perform(HANDLE targetProcess, const std::wstring& pathToDll) {
  // Try to allocate memory in target process to write the dll path there
  auto pathBytes = (pathToDll.size() + 1) * sizeof(wchar_t);
  if (auto pathAddr = VirtualAllocEx(targetProcess, nullptr, pathBytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) {
    //auto releaseMemory = utility::scope::on_exit([=]() { VirtualFreeEx(targetProcess, pathAddr, 0, MEM_RELEASE); });
    //FIXME: Don't release the memory like this, because it we start a new thread in the target process and the memory is read delayed and
    //       if we free the memory before the thread has started, we will only crash the target process.
    //       This wouldn't happen however if I wait for the thread to finish...
    

    // Try to write the full unicode dll path there
    if (WriteProcessMemory(targetProcess, pathAddr, reinterpret_cast<const void*>(pathToDll.c_str()), pathBytes, nullptr)) {

      if (auto remoteLoadLibAddr = GetDllFunctionAddress(targetProcess, L"kernel32.dll", "LoadLibraryW")) {
        //FIXME: MSDN says this should be the thread handle, but the last time I received 0x90...
        if (CreateRemoteThreadEx(targetProcess, NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(remoteLoadLibAddr), pathAddr, 0, nullptr, NULL) != NULL) {

          //TODO: add some kind of error logging... (not into the console!)
          return true;
        }
      }
    }
  }


  return false;
}

std::wstring DllInjection::DllPath() {
  wchar_t pathBuffer[512] = { 0 }; // though MAX_PATH is 260
  GetModuleFileNameW(THIS_DLL, pathBuffer, sizeof(pathBuffer) / sizeof(wchar_t));
  return std::wstring(pathBuffer);
}

void* DllInjection::GetDllFunctionAddress(HANDLE targetProcess, const wchar_t* dllName, const char* functionName) {
  // First load the proc adress inside our own process

  if (auto localDll = LoadLibraryW(dllName)) {
    // Make sure to restore ref count of the specified dll in our process
    auto freeDll = utility::scope::on_exit([localDll]() { FreeLibrary(localDll);  });

    if (auto funAddr = GetProcAddress(localDll, functionName)) {
      auto relativeAddr = reinterpret_cast<char*>(funAddr) - reinterpret_cast<char*>(localDll);

      // Now find the dll in the remote process and get it's base address
      if (auto remoteDll = GetDllBaseAddress(targetProcess, dllName)) {
        // Add offset to remote dll base address and we are done
        return static_cast<void*>(static_cast<char*>(remoteDll) + relativeAddr);
      }
    }
  }

  // Otherwise return null
  return nullptr;
}

void* DllInjection::GetDllBaseAddress(HANDLE targetProcess, const wchar_t* dllName) {
  // Create a snapshot of the target process to inspect it's module list
  if (Handle snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(targetProcess))) {
    MODULEENTRY32W moduleEntry = { 0 };
    moduleEntry.dwSize = sizeof(MODULEENTRY32W);

    for (BOOL hasModule = Module32FirstW(snapshot, &moduleEntry); hasModule; hasModule = Module32NextW(snapshot, &moduleEntry)) {
      if (_wcsicmp(dllName, moduleEntry.szModule) == 0) {
        // found the dll we were looking for, return it's base address
         return static_cast<void*>(moduleEntry.modBaseAddr);
      }
    }
  }

  return nullptr;
}
