#include "stdafx.h"

#include <Handle.h>

Handle::Handle() noexcept : handle(INVALID_HANDLE_VALUE) {}
Handle::Handle(HANDLE handle) noexcept : handle(handle) {}

Handle::Handle(Handle&& other) noexcept : handle(other.handle) {
  other.handle = INVALID_HANDLE_VALUE;
}

Handle::~Handle() {
  if (*this) {
    CloseHandle(handle);
  }
}

Handle& Handle::operator=(Handle&& other) noexcept {
  this->~Handle();
  new (this) Handle(std::move(other));
  return *this;
}

Handle::operator bool() const {
  return handle != INVALID_HANDLE_VALUE && handle != NULL;
}

Handle::operator HANDLE() const {
  return handle;
}
