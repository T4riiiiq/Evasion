#include <windows.h>
#include <winternl.h>

// Direct external linkage signatures to block name scrambling
extern "C" {

    NTSTATUS NtCreateProcessEx(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, HANDLE ParentProcess, ULONG Flags, HANDLE SectionHandle, HANDLE DebugPort, HANDLE ExceptionPort, ULONG JobMemberLevel) {
        NTSTATUS status;
        __asm__ volatile (
            ".intel_syntax noprefix \n\t"
            "mov r10, rcx \n\t"
            "mov eax, 0x00B9 \n\t"
            "syscall \n\t"
            ".att_syntax prefix \n\t"
            : "=a"(status)
        );
        return status;
    }

    NTSTATUS NtUnmapViewOfSection(HANDLE ProcessHandle, PVOID BaseAddress) {
        NTSTATUS status;
        __asm__ volatile (
            ".intel_syntax noprefix \n\t"
            "mov r10, rcx \n\t"
            "mov eax, 0x002A \n\t"
            "syscall \n\t"
            ".att_syntax prefix \n\t"
            : "=a"(status)
        );
        return status;
    }

    NTSTATUS NtAllocateVirtualMemory(HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, PSIZE_T RegionSize, ULONG AllocationType, ULONG Protect) {
        NTSTATUS status;
        __asm__ volatile (
            ".intel_syntax noprefix \n\t"
            "mov r10, rcx \n\t"
            "mov eax, 0x0018 \n\t"
            "syscall \n\t"
            ".att_syntax prefix \n\t"
            : "=a"(status)
        );
        return status;
    }

    NTSTATUS NtWriteVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, SIZE_T NumberOfBytesToWrite, PSIZE_T NumberOfBytesWritten) {
        NTSTATUS status;
        __asm__ volatile (
            ".intel_syntax noprefix \n\t"
            "mov r10, rcx \n\t"
            "mov eax, 0x003A \n\t"
            "syscall \n\t"
            ".att_syntax prefix \n\t"
            : "=a"(status)
        );
        return status;
    }

    NTSTATUS NtGetContextThread(HANDLE ThreadHandle, PCONTEXT Context) {
        NTSTATUS status;
        __asm__ volatile (
            ".intel_syntax noprefix \n\t"
            "mov r10, rcx \n\t"
            "mov eax, 0x00F2 \n\t"
            "syscall \n\t"
            ".att_syntax prefix \n\t"
            : "=a"(status)
        );
        return status;
    }

    NTSTATUS NtSetContextThread(HANDLE ThreadHandle, PCONTEXT Context) {
        NTSTATUS status;
        __asm__ volatile (
            ".intel_syntax noprefix \n\t"
            "mov r10, rcx \n\t"
            "mov eax, 0x00F3 \n\t"
            "syscall \n\t"
            ".att_syntax prefix \n\t"
            : "=a"(status)
        );
        return status;
    }

    NTSTATUS NtResumeThread(HANDLE ThreadHandle, PULONG PreviousSuspendCount) {
        NTSTATUS status;
        __asm__ volatile (
            ".intel_syntax noprefix \n\t"
            "mov r10, rcx \n\t"
            "mov eax, 0x0052 \n\t"
            "syscall \n\t"
            ".att_syntax prefix \n\t"
            : "=a"(status)
        );
        return status;
    }

    NTSTATUS NtCreateThreadEx(PHANDLE ThreadHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, HANDLE ProcessHandle, PVOID StartAddress, PVOID Parameter, ULONG CreateFlags, ULONG_PTR ZeroBits, SIZE_T StackSize, SIZE_T MaxStackSize, PVOID AttributeList) {
        NTSTATUS status;
        __asm__ volatile (
            ".intel_syntax noprefix \n\t"
            "mov r10, rcx \n\t"
            "mov eax, 0x00C2 \n\t"
            "syscall \n\t"
            ".att_syntax prefix \n\t"
            : "=a"(status)
        );
        return status;
    }

}
