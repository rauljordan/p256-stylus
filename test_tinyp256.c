#include <stdint.h>
#include "tinyp256.h"
#include "stylus.h"

// Keygen:
// openssl ecparam -name prime256v1 -genkey -out private.pem
// openssl ec -in private.pem -pubout > public.pem

// openssl ec -pubin -in public.pem -outform DER  2> /dev/null | xxd -i -s 27
uint8_t pubkey[64] = {
    0x36, 0x8e, 0xd4, 0x01, 0x0d, 0x11, 0x85, 0x0b,
    0xa6, 0x87, 0xb8, 0x87, 0x34, 0x5a, 0x6c, 0x26,
    0x98, 0x52, 0x59, 0xd7, 0xc2, 0x6f, 0x48, 0xab,
    0xde, 0xf7, 0x69, 0x3b, 0x5c, 0xf3, 0x4f, 0x19,
    0x84, 0xe9, 0x5b, 0x75, 0x28, 0xf0, 0x39, 0xdc,
    0x5a, 0x2a, 0x57, 0x49, 0x53, 0xbf, 0x1a, 0x93,
    0xc1, 0x07, 0x4e, 0x5d, 0xa1, 0xcb, 0x78, 0xca,
    0x12, 0x11, 0x45, 0xd4, 0x5e, 0x86, 0x45, 0x4c
};

// openssl dgst -sha256 -binary msg.bin | xxd -i
uint8_t digest[32] = {
    0x91, 0x75, 0x1c, 0xee, 0x0a, 0x1a, 0xb8, 0x41,
    0x44, 0x00, 0x23, 0x8a, 0x76, 0x14, 0x11, 0xda,
    0xa2, 0x96, 0x43, 0xab, 0x4b, 0x82, 0x43, 0xe9,
    0xa9, 0x16, 0x49, 0xe2, 0x5b, 0xe5, 0x3a, 0xda
};

// openssl dgst -sha256 -sign private.pem -out msg.sig.bin msg.bin
// openssl asn1parse -in msg.sig.bin -inform DER | awk -F':' '{print $4}' | awk 'NF > 0' | xxd -r -p | xxd -i
uint8_t signature[64] = {
    // r
    0x89, 0x68, 0xAB, 0x49, 0xC8, 0x2D, 0x2E, 0x8A,
    0x7A, 0x0F, 0xBB, 0xAD, 0xD8, 0xC5, 0x98, 0x3B,
    0x88, 0x6D, 0xEF, 0x0E, 0x8C, 0x47, 0x84, 0xD0,
    0xB4, 0x85, 0x9E, 0x03, 0xFF, 0x43, 0x90, 0xA5,
    // s
    0x85, 0xA0, 0xC4, 0x13, 0xD7, 0xDD, 0x4C, 0xCC,
    0x1A, 0x73, 0xED, 0xE5, 0xB8, 0x95, 0x5A, 0xB1,
    0xF8, 0x89, 0x85, 0x83, 0x14, 0xD6, 0x3B, 0x29,
    0x53, 0x92, 0x58, 0xA2, 0x4C, 0x13, 0x0B, 0xBD
};

ArbResult user_main(uint8_t * args, size_t args_len) {
    if (tinyp256_verify(pubkey, sizeof(pubkey), digest, sizeof(digest), signature, sizeof(signature)) == TINYP256_OK) {
        return (ArbResult) {
            .status = Success,
            .output = args,
            .output_len = sizeof(args)
        };
    } else {
        return (ArbResult) {
            .status = Failure,
            .output = args,
            .output_len = sizeof(args)
        };
    }
}

// sets user_main as the entrypoint
ENTRYPOINT(user_main);

void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = dest;
    const unsigned char* s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}