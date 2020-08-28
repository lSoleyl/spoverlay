#pragma once

class DLL DllInjection {
public:

  /** inject the soundpad_overlay.dll into the given target process.
   * 
   * @param targetProcess the process to inject into
   * @param pathToDll full path to the dll to inject
   *
   * @return true upon success
   */
  static bool perform(HANDLE targetProcess, const std::wstring& pathToDll);
  //TODO: Maybe establish a communication channel upon success...


  /** Returns the path to this dll
   */
  static std::wstring dllPath();

  
};