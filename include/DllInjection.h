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

private:

  /** Looks up the address of the given function in the specified dll name inside another process to be usable
   *  as thread proc for StartRemoteThreadEx(). This specified dll must also be loaded inside this process for it to work.
   */
  static void* GetDllFunctionAddress(HANDLE targetProcess, const wchar_t* dllName, const char* functionName);


  /** Returns the base address at which the given dll is loaded in the target process or nullptr if it isn't loaded.
   */
  static void* GetDllBaseAddress(HANDLE targetProcess, const wchar_t* dllName);
};