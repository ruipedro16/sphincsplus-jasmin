#ifndef SPX_FORS_H
#define SPX_FORS_H

#include <stdint.h>

#include "params.h"
#include "context.h"

/**
 * Signs a message m, deriving the secret key from sk_seed and the FTS address.
 * Assumes m contains at least SPX_FORS_HEIGHT * SPX_FORS_TREES bits.
 */
#define fors_sign SPX_NAMESPACE(fors_sign)
void fors_sign(unsigned char *sig, unsigned char *pk,
               const unsigned char *m,
               const spx_ctx* ctx,
               const uint32_t fors_addr[8]);

/**
 * Derives the FORS public key from a signature.
 * This can be used for verification by comparing to a known public key, or to
 * subsequently verify a signature on the derived public key. The latter is the
 * typical use-case when used as an FTS below an OTS in a hypertree.
 * Assumes m contains at least SPX_FORS_HEIGHT * SPX_FORS_TREES bits.
 */
#define fors_pk_from_sig SPX_NAMESPACE(fors_pk_from_sig)
void fors_pk_from_sig(unsigned char *pk,
                      const unsigned char *sig, const unsigned char *m,
                      const spx_ctx* ctx,
                      const uint32_t fors_addr[8]);

void fors_gen_leafx1(unsigned char *leaf,
                            const spx_ctx *ctx,
                            uint32_t addr_idx, void *info);

#endif
