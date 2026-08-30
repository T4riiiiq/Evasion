#include "evasion.h"
#include <windows.h>

BOOL IsNtdllHooked() {
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (!hNtdll) return FALSE;
    FARPROC pNtCreateFile = GetProcAddress(hNtdll, "NtCreateFile");
    if (!pNtCreateFile) return FALSE;
    BYTE* pByte = (BYTE*)pNtCreateFile;
    if (pByte[0] == 0xE9 || pByte[0] == 0xEB) return TRUE;
    return FALSE;
}