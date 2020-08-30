#pragma once

#include "Handle.hpp"

class DLL Process {
public:
  /** Searches all running processes and returns a handle to the process with the specified process name
   *  if found. The process name is case insensitive.
   *
   * @param processName the process to search for (only the name of the executable, not the whole path).
   * @param nthProcess to search for the second process with the same name supply a 2 here.
   *
   * @return a handle to the found process, or an invalid/empty handle if no such process was found.
   *         This is a handle returned by OpenProcess() with PROCESS_ALL_ACCESS specified
   */
  static Handle FindProcess(const wchar_t* processName, int nthProcess = 1);


  /** Returns true if this process is the injector process (SPOverlay.exe) and thus the DLL should not try to inject
   *  itself into the DirectX-Queue.
   */
  static bool IsInjectorProcess();
};
