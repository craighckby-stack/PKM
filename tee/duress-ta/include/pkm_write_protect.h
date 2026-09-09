/**
 * @file pkm_write_protect.h
 * @brief Phase 4 — THE WALL: permanent write protection (fail-secure)
 *
 * CONTRACT (fail-secure, non-negotiable):
 *   pkm_wp_set_permanent() may report success ONLY IF a subsequent
 *   pkm_wp_query_permanent() CONFIRMS the locked state. Set-without-
 *   confirm = PKM_FAILED. This module NEVER simulates. A TODO here
 *   returns PKM_NOT_IMPLEMENTED — never success.
 *   (Post-mortem #4: the green-checkmark lie.)
 *
 *   Capability probe is step zero. UNSUPPORTED is expected — many UFS
 *   parts ship fWP disabled or absent. All README §7 graceful
 *   degradation flows from this module and only from here.
 *
 *   eMMC and UFS are different worlds: EXT_CSD permanent-WP fields +
 *   CMD28/CMD30/CMD31 vs. UFS logical-unit protection. One driver per
 *   bus. No shared pretend-API.
 *
 * GROUNDING:
 *   eMMC: JEDEC eMMC 5.1 — CMD28 (set WP), CMD30 (send WP status),
 *         CMD31 (interrupt), EXT_CSD power/permanent WP fields.
 *         Linux kernel drivers/mmc/core/.
 *   UFS:  JEDEC UFS spec — logical unit write protection,
 *         QUERY descriptor operations. Linux kernel drivers/ufs/.
 *
 * STATUS: STUB.
 */

#ifndef PKM_WRITE_PROTECT_H
#define PKM_WRITE_PROTECT_H

#include "pkm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t wlun;    /**< Well-known LUN / hw partition */
    uint8_t region;  /**< eMMC WP region ID where applicable */
} pkm_wp_target_t;

/**
 * @brief Step zero — REQUIRED before any set operation.
 * @param t Pointer to target structure
 * @param supported Pointer to boolean output indicating capability support
 * @return PKM_SUCCESS on successful probe execution
 */
pkm_result_t pkm_wp_probe(const pkm_wp_target_t *t, bool *supported);

/**
 * @brief Apply the permanent lock. Never trusts its own write.
 * @param t Pointer to target structure
 * @return PKM_SUCCESS only if verified via read-back confirmation
 */
pkm_result_t pkm_wp_set_permanent(const pkm_wp_target_t *t);

/**
 * @brief The load-bearing read-back. No success without locked == true.
 * @param t Pointer to target structure
 * @param locked Pointer to boolean output indicating locked status
 * @return PKM_SUCCESS on successful query execution
 */
pkm_result_t pkm_wp_query_permanent(const pkm_wp_target_t *t, bool *locked);

#ifdef __cplusplus
}
#endif

#endif /* PKM_WRITE_PROTECT_H */