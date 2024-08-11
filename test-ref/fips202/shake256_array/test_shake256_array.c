#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"
#include "notrandombytes.c"
#include "print.c"

#ifndef OUTLEN
#define OUTLEN 1
#endif

#ifndef INLEN
#define INLEN 1
#endif

#ifndef TESTS
#define TESTS 100
#endif

#define shake256_jazz NAMESPACE2(shake256, OUTLEN, INLEN)

extern void shake256_jazz(uint8_t *out, const uint8_t *in);
extern void shake256(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen);

int main(void) {
    uint8_t out0[OUTLEN], out1[OUTLEN];
    uint8_t in0[INLEN], in1[INLEN];

    for (size_t t = 0; t < TESTS; t++) {
        randombytes(in0, INLEN);
        memcpy(in1, in0, INLEN);

        shake256_jazz(out0, in0);
        shake256(out1, OUTLEN, in1, INLEN);

        assert(memcmp(out0, out1, OUTLEN) == 0);
    }

    printf("PASS: shake256 = { outlen: %d, inlen: %d }\n", OUTLEN, INLEN);
    return 0;
}
