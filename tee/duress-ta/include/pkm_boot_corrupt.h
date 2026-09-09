/**
 * @file pkm_boot_corrupt.h
 * @brief Phase 3 — boot chain destruction (ALL redundant targets)
 *
 * CONTRACT:
 *   Corrupt EVERY target in the per-device table loaded from
 *   docs/HARDWARE.md: both UFS boot W-LUNs, XBL + XBL backup, ABL
 *   variants, boot_a + boot_b. FULL extents. (Post-mortem #2: the
 *   single 4 KB token write against a redundant boot chain — a coin
 *   flip that sometimes corrupts a partition table instead.)
 *
 *   Per target: write -> flush -> READ-BACK verify. Any verification
 *   failure = PKM_FAILED for the phase. No verification = no success.
 *
 *   IRREVERSIBILITY DOES NOT COME FROM THIS MODULE. Deterministic
 *   garbage is correct here — the signature check needs invalid bytes,
 *   not random ones. Permanence is Phase 4's job alone.
 *
 * CALLING ORDER: strictly after Phases 1–2, strictly before Phase 4.
 *
 * STATUS: OPTIMIZED SOVEREIGN CORE.
 */

#ifndef PKM_BOOT_CORRUPT_H
#define PKM_BOOT_CORRUPT_H

#include <stddef.h>
#include "pkm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Load the per-device target table. Source of truth: docs/HARDWARE.md.
 * 
 * @param[out] targets Pointer to receive the target table array pointer.
 * @param[out] count Pointer to receive the number of targets in the table.
 * @return pkm_result_t PKM_SUCCESS on success, or appropriate error code.
 */
pkm_result_t pkm_boot_load_targets(const pkm_storage_target_t **targets,
                                   size_t *count)
    __attribute__((nonnull(1, 2)));

/**
 * @brief Corrupt, flush, and read-back-verify every target in the table.
 * 
 * @return pkm_result_t PKM_SUCCESS if all targets are fully verified corrupt.
 */
pkm_result_t pkm_boot_corrupt_execute(void);

#ifdef __cplusplus
}
#endif

#endif /* PKM_BOOT_CORRUPT_H */