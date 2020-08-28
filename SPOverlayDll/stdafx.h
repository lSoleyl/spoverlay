#pragma once

#define DLL __declspec(dllexport)


#include <Windows.h>

// May be used throughout this dll to reference the dll's module
extern HMODULE THIS_DLL;

#include <string>
#include <vector>
#include <list>
