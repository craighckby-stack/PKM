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
 * STATUS: STUB.
 */

#ifndef PKM_PURGE_H
#define PKM_PURGE_H

#include "pkm_types.h"

/* Probe whether the attached device advertises purge/sanitize support. */
pkm_result_t pkm_purge_probe(bool *supported);

/* Initiate purge and BLOCK until the controller reports completion.
 * May take seconds — acceptable: the device is offline from Phase 1. */
pkm_result_t pkm_purge_execute(void);

#endif /* PKM_PURGE_H */
