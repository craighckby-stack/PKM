/**
 * @file pkm_keys.h
 * @brief Phase 1 — cryptographic key destruction (THE MANDATORY STEP)
 *
 * CONTRACT:
 *   Zeroize targets the REAL keystore: RPMB key blobs, keymaster keys,
 *   TEE Secure Storage, key derivation material. Wiping self-allocated
 *   buffers is fiction. (Post-mortem #1: the 64-byte stack array that
 *   wiped itself.)
 *
 *   This is §3 step 1 and the ONLY mandatory phase. Every later phase
 *   is degradation-tolerant. This one is not: the orchestrator must
 *   HALT — not continue — if this phase does not complete OK. A brick
 *   with keys intact is the one forbidden end state.
 *
 * GROUNDING:
 *   AOSP keymaster / keystore2 key deletion paths; OP-TEE secure
 *   storage (REE FS + RPMB) deletion; TEE crypto key management.
 *
 * STATUS: STUB — each function is a distinct hardware integration
 * with its own source of truth. No shared pretend-API.
 */

#ifndef PKM_KEYS_H
#define PKM_KEYS_H

#include "pkm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Delete all FBE/file-content key blobs: RPMB + TEE Secure Storage,
 *        all users/profiles, plus the master wraps above them.
 * @return pkm_result_t PKM_SUCCESS on absolute completion, error code otherwise.
 */
[[nodiscard]] pkm_result_t pkm_keys_zeroize_fbe(void);

/**
 * @brief Delete hardware keystore / keymaster root + attestation material.
 * @return pkm_result_t PKM_SUCCESS on absolute completion, error code otherwise.
 */
[[nodiscard]] pkm_result_t pkm_keys_zeroize_keymaster(void);

/**
 * @brief Delete key derivation material (the wrapper hierarchy).
 * @return pkm_result_t PKM_SUCCESS on absolute completion, error code otherwise.
 */
[[nodiscard]] pkm_result_t pkm_keys_zeroize_kdf(void);

/**
 * @brief Overwrite TEE volatile memory holding unwrapped key copies:
 *        TA heap, crypto-primitive caches, scratch. Runs in Phase 5 too —
 *        last writes before power-off.
 * @return pkm_result_t PKM_SUCCESS on absolute completion, error code otherwise.
 */
[[nodiscard]] pkm_result_t pkm_keys_zeroize_volatile(void);

#ifdef __cplusplus
}
#endif

#endif /* PKM_KEYS_H */
