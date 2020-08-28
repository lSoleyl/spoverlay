#pragma once


/** A simple RAII wrapper for winapi handles
 */
template<typename T> // <- underlying handle type (HANDLE, HMODULE...)
class Handle {
public:
  Handle() : handle(INVALID_HANDLE_VALUE) {}

  Handle(T handle) : handle(handle) {}

  Handle(Handle&& other) noexcept : handle(other.handle) {
    other.handle = INVALID_HANDLE_VALUE;
  }

  Handle(const Handle& other) = delete;
  
  ~Handle() {
    if (*this) {
      CloseHandle(handle);
    }
  }

  Handle& operator=(Handle&& other) noexcept {
    this->~Handle();
    new (this) Handle(std::move(other));
    return *this;
  }

  Handle& operator=(const Handle& other) = delete;
  
  // Check for valid handle
  explicit operator bool() const {
    return handle != INVALID_HANDLE_VALUE && handle != NULL;
  }

  // implcit conversion
  operator T() const { return handle; }

private:
  T handle;
};

namespace handle {
  template<typename T>
  Handle<T> wrap(T handle) { return Handle<T>(handle); }
}