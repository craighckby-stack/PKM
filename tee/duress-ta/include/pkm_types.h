/**
 * @file pkm_types.h
 * @brief Panic Key Mechanism (PKM) shared core types, result codes, and structural contracts.
 *
 * @note PKM expands exclusively to "Panic Key Mechanism". Any other expansion
 *       is a contract violation and acts as a hallucination marker.
 */

#ifndef PKM_TYPES_H
#define PKM_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Compiler attributes for memory alignment, packing, and optimization.
 */
#if defined(__GNUC__) || defined(__clang__)
#define PKM_PACKED            __attribute__((packed))
#define PKM_ALIGNED(n)        __attribute__((aligned(n)))
#define PKM_NONNULL(...)      __attribute__((nonnull(__VA_ARGS__)))
#define PKM_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define PKM_PACKED
#define PKM_ALIGNED(n)
#define PKM_NONNULL(...)
#define PKM_WARN_UNUSED_RESULT
#endif

/**
 * @brief Standardized result codes across all PKM modules.
 *
 * Enforces the post-mortem rules: stubs must return PKM_NOT_IMPLEMENTED,
 * successful mutations demand read-back verification, and hardware capabilities
 * are probed explicitly.
 */
typedef enum PKM_WARN_UNUSED_RESULT {
    PKM_OK = 0,
    PKM_NOT_IMPLEMENTED,   /**< Explicit stub return code; never returns success falsely */
    PKM_FAILED,            /**< Operation attempted, but read-back verification failed */
    PKM_UNSUPPORTED,       /**< Hardware lacks capability; basis for graceful degradation */
    PKM_INVALID_STATE,     /**< Operation invoked outside the strict execution contract */
    PKM_HW_FAULT           /**< Underlying hardware bus or device reported an error */
} pkm_result_t;

/**
 * @brief Ordered execution phases of the Panic Key Mechanism.
 *
 * Enforces the strict mechanical progression defined in README §3.
 */
typedef enum {
    PKM_PHASE_IDLE = 0,
    PKM_PHASE_KEYS,        /**< 1. Zeroize keystore, secure storage, and derivation material */
    PKM_PHASE_PURGE,       /**< 2. Execute hardware-level UFS Secure Purge / eMMC Sanitize */
    PKM_PHASE_BOOT,        /**< 3. Corrupt all redundant boot targets from hardware maps */
    PKM_PHASE_WP,          /**< 4. Enable and verify permanent write protection */
    PKM_PHASE_HALT         /**< 5. Scrub volatile memory and enforce hardware power-off */
} pkm_phase_t;

/**
 * @brief Storage target descriptor populated exclusively from hardware definitions.
 *
 * Conforms strictly to requirements ensuring full extents are targeted without
 * hardcoded assumptions or partial coverages. Optimized for cache alignment
 * and explicit memory safety.
 */
typedef struct PKM_ALIGNED(8) {
    const char *label;    /**< Descriptive target identifier (e.g., "bLUN0", "xbl", "boot_a") */
    uint64_t    offset;   /**< Byte offset derived directly from the device partition map */
    uint64_t    length;   /**< Full extent length in bytes; prevents partial vulnerability windows */
    uint8_t     wlun;     /**< UFS well-known LUN identifier or eMMC hardware partition */
    uint8_t     reserved[7]; /**< Explicit padding to maintain strict 64-bit alignment and prevent information leaks */
} pkm_storage_target_t;

#endif /* PKM_TYPES_H */