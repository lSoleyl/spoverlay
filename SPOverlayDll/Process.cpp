#include "stdafx.h"
#include <Process.hpp>

#include <TlHelp32.h>

Handle Process::FindProcess(const wchar_t* processName, int nthProcess) {
  // Create a snapshot of all processes
  if (Handle snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL)) {
    PROCESSENTRY32W procEntry = { 0 };
    procEntry.dwSize = sizeof(procEntry);


    for (BOOL hasValue = Process32FirstW(snap, &procEntry); hasValue; hasValue = Process32NextW(snap, &procEntry)) {
      if (_wcsicmp(procEntry.szExeFile, processName) == 0 && --nthProcess == 0) {
        return OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry.th32ProcessID);
      }
    }
  }  

  return Handle();
}

bool Process::IsInjectorProcess() {
  wchar_t moduleName[MAX_PATH];
  GetModuleFileNameW(NULL/*this process*/, moduleName, MAX_PATH);

  // Find the last '\' and ignore the prefix
  std::wstring_view moduleNameRef(moduleName);
  moduleNameRef = moduleNameRef.substr(moduleNameRef.rfind(L'\\')+1);

  return _wcsicmp(moduleNameRef.data(), L"SPOverlay.exe") == 0;
}
