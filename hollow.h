#pragma once
#include <windows.h>


void PrintError(const char* msg);
BOOL LoadShellcode(const wchar_t* path, PBYTE* ppBuffer, SIZE_T* pSize);
BOOL HollowProcess(const wchar_t* target, PBYTE pShellcode, SIZE_T shellcodeSize);
