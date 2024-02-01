#include "wots.h"

#include <stdint.h>
#include <string.h>

#include "address.h"
#include "hash.h"
#include "params.h"
#include "thash.h"
#include "utils.h"
#include "utilsx1.h"
#include "wotsx1.h"

// TODO clarify address expectations, and make them more uniform.
// TODO i.e. do we expect types to be set already?
// TODO and do we expect modifications or copies?

/**
 * Computes the chaining function.
 * out and in have to be n-byte arrays.
 *
 * Interprets in as start-th value of the chain.
 * addr has to contain the address of the chain.
 */
static void gen_chain(unsigned char *out, const unsigned char *in, unsigned int start, unsigned int steps,
                      const spx_ctx *ctx, uint32_t addr[8]) {
    uint32_t i;

    /* Initialize out with the value at position 'start'. */
    memcpy(out, in, SPX_N);

    /* Iterate 'steps' calls to the hash function. */
    for (i = start; i < (start + steps) && i < SPX_WOTS_W; i++) {
#ifdef TEST_ADDRESS
        set_hash_addr_jazz(addr, i);
#else
        set_hash_addr(addr, i);
#endif

        thash(out, out, 1, ctx, addr);
    }
}

/**
 * base_w algorithm as described in draft.
 * Interprets an array of bytes as integers in base w.
 * This only works when log_w is a divisor of 8.
 */
static void base_w(unsigned int *output, const int out_len, const unsigned char *input) {
    int in = 0;
    int out = 0;
    unsigned char total;
    int bits = 0;
    int consumed;

    for (consumed = 0; consumed < out_len; consumed++) {
        if (bits == 0) {
            total = input[in];
            in++;
            bits += 8;
        }
        bits -= SPX_WOTS_LOGW;
        output[out] = (total >> bits) & (SPX_WOTS_W - 1);
        out++;
    }
}

/* Computes the WOTS+ checksum over a message (in base_w). */
static void wots_checksum(unsigned int *csum_base_w, const unsigned int *msg_base_w) {
    unsigned int csum = 0;
    unsigned char csum_bytes[(SPX_WOTS_LEN2 * SPX_WOTS_LOGW + 7) / 8];
    unsigned int i;

    /* Compute checksum. */
    for (i = 0; i < SPX_WOTS_LEN1; i++) {
        csum += SPX_WOTS_W - 1 - msg_base_w[i];
    }

    /* Convert checksum to base_w. */
    /* Make sure expected empty zero bits are the least significant bits. */
    csum = csum << ((8 - ((SPX_WOTS_LEN2 * SPX_WOTS_LOGW) % 8)) % 8);
    ull_to_bytes(csum_bytes, sizeof(csum_bytes), csum);

#ifdef TEST_WOTS_BASE_W
    base_w_jazz_out_WOTS_LEN2(csum_base_w, csum_bytes);
#else
    base_w(csum_base_w, SPX_WOTS_LEN2, csum_bytes);
#endif
}

/* Takes a message and derives the matching chain lengths. */
void chain_lengths(unsigned int *lengths, const unsigned char *msg) {
#ifdef TEST_WOTS_BASE_W
    base_w_jazz_out_WOTS_LEN1(lengths, msg);
#else
    base_w(lengths, SPX_WOTS_LEN1, msg);
#endif

#ifdef TEST_WOTS_CHECKSUM
    wots_checksum_jazz(lengths + SPX_WOTS_LEN1, lengths);
#else
    wots_checksum(lengths + SPX_WOTS_LEN1, lengths);
#endif
}

/**
 * Takes a WOTS signature and an n-byte message, computes a WOTS public key.
 *
 * Writes the computed public key to 'pk'.
 */
void wots_pk_from_sig(unsigned char *pk, const unsigned char *sig, const unsigned char *msg, const spx_ctx *ctx,
                      uint32_t addr[8]) {
    unsigned int lengths[SPX_WOTS_LEN];
    uint32_t i;

#ifdef TEST_WOTS_CHAIN_LENGTHS
    chain_lengths(lengths, msg);
#else
    chain_lengths(lengths, msg);
#endif

    for (i = 0; i < SPX_WOTS_LEN; i++) {
#ifdef TEST_ADDRESS
        set_chain_addr_jazz(addr, i);
#else
        set_chain_addr(addr, i);
#endif

#ifdef TEST_WOTS_GEN_CHAIN
        gen_chain_jazz(pk + i * SPX_N, sig + i * SPX_N, lengths[i], SPX_WOTS_W - 1 - lengths[i], ctx->pub_seed, addr);
#else
        gen_chain(pk + i * SPX_N, sig + i * SPX_N, lengths[i], SPX_WOTS_W - 1 - lengths[i], ctx, addr);
#endif
    }
}
