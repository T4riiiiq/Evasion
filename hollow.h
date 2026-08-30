#pragma once
#include <windows.h>

// Declare the missing utility routines
void PrintError(const char* msg);
BOOL LoadShellcode(const wchar_t* path, PBYTE* ppBuffer, SIZE_T* pSize);
BOOL HollowProcess(const wchar_t* target, PBYTE pShellcode, SIZE_T shellcodeSize);
