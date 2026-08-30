#pragma once
#include <windows.h>
#include <winternl.h>

extern "C" {
    NTSTATUS NtCreateProcessEx(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, HANDLE ParentProcess, ULONG Flags, HANDLE SectionHandle, HANDLE DebugPort, HANDLE ExceptionPort, ULONG JobMemberLevel);
    NTSTATUS NtUnmapViewOfSection(HANDLE ProcessHandle, PVOID BaseAddress);
    NTSTATUS NtAllocateVirtualMemory(HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, PSIZE_T RegionSize, ULONG AllocationType, ULONG Protect);
    NTSTATUS NtWriteVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, SIZE_T NumberOfBytesToWrite, PSIZE_T NumberOfBytesWritten);
    NTSTATUS NtGetContextThread(HANDLE ThreadHandle, PCONTEXT Context);
    NTSTATUS NtSetContextThread(HANDLE ThreadHandle, PCONTEXT Context);
    NTSTATUS NtResumeThread(HANDLE ThreadHandle, PULONG PreviousSuspendCount);
    NTSTATUS NtCreateThreadEx(PHANDLE ThreadHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, HANDLE ProcessHandle, PVOID StartAddress, PVOID Parameter, ULONG CreateFlags, ULONG_PTR ZeroBits, SIZE_T StackSize, SIZE_T MaxStackSize, PVOID AttributeList);

}
