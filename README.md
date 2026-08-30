# Direct Syscall Remote Thread Injector

An educational adversary simulation utility and low-level Win32 research template demonstrating direct system call execution and dynamic memory protection state transitions.

## Technical Architecture Flow

1. **User-Mode Hook Subversion:** Bypasses standard ntdll.dll API monitoring hooks natively by executing memory allocation operations via direct inline assembly system calls (NtAllocateVirtualMemory, NtWriteVirtualMemory).
2. **Dynamic Protection Permutations:** Minimizes static in-memory signatures by avoiding persistent RWX allocation blocks. Memory is mapped as safe Read/Write (PAGE_READWRITE), written to, and subsequently transitioned to Execute/Read (PAGE_EXECUTE_READ) permissions right before execution.
3. **Independent Runtime Worker Threads:** Evades thread-hijacking and registry modification heuristics by leaving target suspended primary application threads completely untouched. Code execution is safely handled via an independent, isolated runtime thread invoked via NtCreateThreadEx.

## Native Compilation (MinGW-w64)

g++ -o hollow.exe hollow.cpp evasion.cpp syscalls.cpp utils.cpp -municode -Wl,-subsystem,console -O2 -std=c++17 -static -fno-exceptions -fno-rtti

*Disclaimer: This repository is intended strictly for professional security research, educational telemetry validation, and academic laboratory tracking purposes.*