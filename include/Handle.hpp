#pragma once


/** A simple RAII wrapper for winapi handles
 */
class DLL Handle {
public:
  Handle() noexcept;
  Handle(HANDLE handle) noexcept;
  Handle(Handle&& other) noexcept;
  Handle(const Handle& other) = delete;
  ~Handle();

  Handle& operator=(Handle&& other) noexcept;
  Handle& operator=(const Handle& other) = delete;
  
  // Check for valid handle
  explicit operator bool() const;

  // implcit conversion
  operator HANDLE() const;

private:
  HANDLE handle;
};
