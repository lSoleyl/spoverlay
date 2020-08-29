#define DLL __declspec(dllimport)

#include <Windows.h>
#include <TlHelp32.h>
#include <string>

#include <Handle.h>
#include <DllInjection.h>

#include <iostream>

using namespace std;

const wchar_t* PROCESS_NAME = L"RocketLeague.exe";
int procNr = 2;

//TODO: move this somewhere else
DWORD FindProcess() {
  // Find rocketleague.exe process
  Handle snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
  if (!snap) {
    throw std::exception("Failed to create snapshot of process list");
  }

  PROCESSENTRY32 procEntry = { 0 };
  procEntry.dwSize = sizeof(procEntry);


  for (BOOL hasValue = Process32First(snap, &procEntry); hasValue; hasValue = Process32Next(snap, &procEntry)) {
    if (_wcsicmp(procEntry.szExeFile, PROCESS_NAME) == 0 && --procNr == 0) {
      return procEntry.th32ProcessID;
    }
  }

  return NULL;
}


int main() {
  cout << "Hello :)" << endl;

  if (auto pid = FindProcess()) {
    if (Handle procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid)) {
      cout << "Process found, performing dll injection" << endl;
      //TODO: select proper dll bitness depending on process
      cout << "Dll-Injection successful: " << std::boolalpha << DllInjection::perform(procHandle, DllInjection::dllPath()) << endl;
    }
  } else {
    cout << "Process not found!" << endl;
  }


}