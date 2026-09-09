/**
 * @file pkm_purge.h
 * @brief Phase 2 — media-level physical erase (UFS Secure Purge / eMMC Sanitize)
 *
 * CONTRACT:
 *   Issue the REAL controller command and CONFIRM completion status
 *   before returning. No command path = PKM_NOT_IMPLEMENTED.
 *   Capability absent = PKM_UNSUPPORTED — a first-class, expected
 *   outcome (README §7). Never simulated.
 *
 *   PURPOSE: defeat remanence of TRIMMED blocks — cold-chip recovery
 *   of previously deleted data. Complements key zeroization; never
 *   replaces it.
 *
 * GROUNDING:
 *   UFS:  JEDEC UFS 2.x/3.x — Secure Purge (QUERY ops, purge status
 *         attributes). Linux kernel drivers/ufs/.
 *   eMMC: JEDEC eMMC 5.1 — Sanitize + TRIM, EXT_CSD fields.
 *         Linux kernel drivers/mmc/core/.
 *
 * STATUS: PRODUCTION.
 */

#ifndef PKM_PURGE_H
#define PKM_PURGE_H

#include <stdbool.h>
#include <stdint.h>
#include "pkm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Probe whether the attached device advertises purge/sanitize support.
 *
 * @param[out] supported Pointer to boolean flag receiving support status.
 * @return PKM_SUCCESS on successful query, or an appropriate error code.
 */
pkm_result_t pkm_purge_probe(bool *supported);

/**
 * @brief Initiate purge and BLOCK until the controller reports completion.
 *
 * May take seconds — acceptable: the device is offline from Phase 1.
 *
 * @return PKM_SUCCESS if purge completes and read-back verification succeeds,
 *         or an appropriate error code.
 */
pkm_result_t pkm_purge_execute(void);

#ifdef __cplusplus
}
#endif

#endif /* PKM_PURGE_H */