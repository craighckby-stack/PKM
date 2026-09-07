/**
 * @file pkm_types.h
 * @brief PKM shared types — Panic Key Mechanism
 *
 * PKM is "Panic Key Mechanism". No other expansion is valid in this repo.
 * Any other expansion appearing in generated code is a hallucination marker.
 */

#ifndef PKM_TYPES_H
#define PKM_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum {
    PKM_OK = 0,
    PKM_NOT_IMPLEMENTED,   /* stubs return this — it is honest */
    PKM_FAILED,            /* attempted, verification did not pass */
    PKM_UNSUPPORTED,       /* hardware lacks the capability (README §7) */
    PKM_INVALID_STATE,     /* called outside the §3 contract */
    PKM_HW_FAULT           /* hardware reported an error */
} pkm_result_t;

/* README §3 phase order. The orchestrator enforces this mechanically. */
typedef enum {
    PKM_PHASE_IDLE = 0,
    PKM_PHASE_KEYS,        /* 1. zeroize keystore + derivation material   */
    PKM_PHASE_PURGE,       /* 2. UFS Secure Purge / eMMC Sanitize         */
    PKM_PHASE_BOOT,        /* 3. corrupt all redundant boot targets       */
    PKM_PHASE_WP,          /* 4. enable + VERIFY permanent write protect  */
    PKM_PHASE_HALT         /* 5. zeroize volatile RAM, power off          */
} pkm_phase_t;

/*
 * POWER-CUT SAFETY (the reason the order exists — README §3):
 *   Interrupt before KEYS completes -> data intact.   Acceptable.
 *   Interrupt after  KEYS completes -> data dead.     Acceptable.
 *   There is no third state. This is the entire design.
 */

/* Per-device storage target. Populated ONLY from docs/HARDWARE.md.
 * Hardcoding LUNs, offsets, or partition names anywhere else is a
 * contract violation. */
typedef struct {
    const char *label;    /* e.g. "bLUN0", "bLUN1", "xbl", "xblbak",
                             "abl", "boot_a", "boot_b" */
    uint8_t     wlun;     /* UFS well-known LUN id / eMMC hw partition */
    uint64_t    offset;   /* bytes, from the device partition map */
    uint64_t    length;   /* FULL extent — partial coverage is a coin flip */
} pkm_storage_target_t;

#endif /* PKM_TYPES_H */
