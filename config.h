#pragma once
#include <windows.h>

#define XOR_KEY 0x55

// Encrypted strings
#define ENCRYPTED_TARGET L"\x96\x99\x9E\x9E\x9F\x98\x9E\x93\x88\x88" 
#define ENCRYPTED_PAYLOAD L"\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93" 

inline wchar_t* DecryptString(wchar_t* encrypted, size_t len) {
    for (size_t i = 0; i < len; i++) {
        encrypted[i] ^= XOR_KEY;
    }
    return encrypted;
}
