#include "stdafx.h"

#include <Handle.h>
#include <DllInjection.h>

#include <utility/scope.h>

bool DllInjection::perform(HANDLE targetProcess, const std::wstring& pathToDll) {
  // Try to allocate memory in target process to write the dll path there
  auto pathBytes = (pathToDll.size() + 1) * sizeof(wchar_t);
  if (auto pathAddr = VirtualAllocEx(targetProcess, nullptr, pathBytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) {
    auto releaseMemory = utility::scope::on_exit([=]() { VirtualFreeEx(targetProcess, pathAddr, 0, MEM_RELEASE); });

    

    // Try to write the full unicode dll path there
    if (WriteProcessMemory(targetProcess, pathAddr, reinterpret_cast<const void*>(pathToDll.c_str()), pathBytes, nullptr)) {

      //TODO: locate the kernel32.dll inside the target process

      // Locate LoadLibraryW in our kernel32.dll process and hope that adress will be the same

      //TODO: add auto handle for LoadLibraray()->FreeLibrary()
      if (auto kernelDll = LoadLibraryW(L"kernel32.dll")) {
        if (auto loadLibAddr = GetProcAddress(kernelDll, "LoadLibraryW")) { // <- shouldn't fail anyway

          //FIXME: MSDN says this should be the thread handle, but the last time I received 0x90...
          if (CreateRemoteThreadEx(targetProcess, NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibAddr), pathAddr, 0, nullptr, NULL) != NULL) {

            //TODO: add some kind of error logging... (not into the console!)
            return true;
          }
        }
      }
    }
  }


  return false;
}

std::wstring DllInjection::dllPath() {
  wchar_t pathBuffer[512] = { 0 }; // though MAX_PATH is 260
  GetModuleFileNameW(THIS_DLL, pathBuffer, sizeof(pathBuffer) / sizeof(wchar_t));
  return std::wstring(pathBuffer);
}
