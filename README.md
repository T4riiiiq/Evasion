# Direct Syscall Remote Thread Injector

An advanced, low-level offensive security development template and process injection framework optimized for multi-process operations in environments monitored by Endpoint Detection and Response (EDR) agents. This utility demonstrates the subversion of universal user-mode EDR hook telemetry and the elimination of persistent memory-scanning signatures through direct kernel-layer interaction.

---

## ⚡ Core Architectural Mechanics

The framework coordinates four primary engineering primitives to subvert standard host-based behavioral and memory heuristics:

* **User-Mode EDR Hook Subversion (Direct Syscalls)**
  The loader completely bypasses user-mode monitoring hooks deployed by modern EDR solutions within `ntdll.dll`. By leveraging direct inline assembly stubs wrapped inside an explicit `extern "C"` linkage block, execution requests skip user-mode API logging and route directly to the kernel interface layer (`NtAllocateVirtualMemory`, `NtWriteVirtualMemory`).

* **Dynamic Page Protection Permutations**
  To break automated memory hunting loops and memory scanning signature tracking, the tool completely avoids persistent Read/Write/Execute (`RWX`) allocations. Target pages are initialized strictly with safe Read/Write (`PAGE_READWRITE`) permissions. The ciphertext payload is streamed into the buffer, and the page memory state is transitioned dynamically to Execute/Read (`PAGE_EXECUTE_READ`) via `VirtualProtectEx` immediately prior to thread invocation.

* **Independent Background Thread Allocation**
  The architecture explicitly drops traditional process hollowing and context-hijacking routines (`NtGetContextThread` / `NtSetContextThread` / `NtResumeThread`), which modern OS kernels and EDR heuristics flag as anomalous. The primary suspended thread of the target process remains completely untouched. Execution is achieved by spawning an isolated, independent runtime background thread natively via `NtCreateThreadEx`.

* **In-Memory Armoring & Stream Parsing**
  The loader reads a pre-encrypted stageless payload blob from the command-line interface. It handles on-disk static signature evasion through asymmetric obfuscation, parsing the byte array into private RAM and stripping away the protection armor via an inline XOR decoding loop right before injection occurs.

---

## 🛠️ Environment Requirements

To compile and execute this framework without environmental errors, your development platform must satisfy the following constraints:

* **Compiler Toolchain:** MinGW-w64 / GCC Version 16.1.0+ (Enforces strict modern type checking)
* **Development Shell:** MSYS2 UCRT64 Environment (Essential for proper UCRT output stream flashing)
* **Language Standard:** ISO C++17 Standard (`-std=c++17`)
* **Target OS Architecture:** Windows 10 / 11 x64 Natively

---

## 🚀 Deployment & Operational Guide

Follow this step-by-step checklist to clone, configure, compile, and execute the tool from scratch:

### 1. Clone the Codebase
Open your terminal emulator and clone the remote repository path onto your local attack box or development machine:
```bash
git clone https://github.com
cd Evasion
```

### 2. Prepare Your Encrypted Payload
The loader's internal decoding engine expects a stageless payload blob encrypted with a static `0xAA` XOR byte key. You can generate a compatible file using this rapid Python automation string:
```bash
python3 -c "import sys; data = open('raw_shellcode.bin', 'rb').read(); enc = bytearray(b ^ 0xAA for b in data); open('shellcode_enc.bin', 'wb').write(enc)"
```

### 3. Open the Native Compilation Shell
* On your target Windows compilation machine, launch the **MSYS2 UCRT64 Shell** environment.
* Navigate into your cloned project directory:
```bash
cd /c/Users/YOUR_USER/Desktop/Projects/Evasion
```

### 4. Execute Native Compilation
Run the following compilation command string inside the UCRT64 shell to bind your multi-file project modules cleanly without triggering linker discard faults or C++ name-mangling link blocks:
```bash
g++ -o hollow.exe hollow.cpp evasion.cpp syscalls.cpp utils.cpp -municode -Wl,-subsystem,console -O2 -std=c++17 -static -fno-exceptions -fno-rtti
```

### 5. Launch the Framework
Execute the compiled binary by passing your target runner process path and your encrypted payload path explicitly via the command-line argument array:
```powershell
.\hollow.exe --target C:\Windows\System32\notepad.exe --payload shellcode_enc.bin
```

---

> **Disclaimer:** *This repository is intended strictly for professional security research, educational telemetry validation, and authorized academic laboratory tracking purposes.*
