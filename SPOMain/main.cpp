#define DLL __declspec(dllimport)

#include <Windows.h>
#include <string>

#include <DllInjection.hpp>
#include <Process.hpp>

#include <iostream>


using namespace std;

const wchar_t* PROCESS_NAME = L"notepad.exe";
int procNr = 1;



int main() {
  cout << "Hello :)" << endl;

  if (auto processHandle = Process::FindProcess(PROCESS_NAME, procNr)) {
    cout << "Process found, performing dll injection" << endl;
    //TODO: select proper dll bitness depending on process
    cout << "Dll-Injection successful: " << std::boolalpha << DllInjection::Perform(processHandle, DllInjection::DllPath()) << endl;
  } else {
    cout << "Process not found!" << endl;
  }


}