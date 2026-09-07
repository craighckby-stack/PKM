/**
 * @file boot_corrupt.h
 * @brief PKM-Soft Bootloader Partition Corruption Engine
 * 
 * Manages controlled data degradation and entropy injection into bootloader 
 * partitions and LUNs to deliberately trigger permanent Boot ROM verification 
 * failures upon panic activation.
 */

#ifndef PKM_SOFT_BOOT_CORRUPT_H
#define PKM_SOFT_CORRUPT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Operational status codes for boot corruption routines. */
typedef enum {
    BOOT_CORRUPT_STATUS_SUCCESS = 0,
    BOOT_CORRUPT_STATUS_ERROR_INVALID_TARGET = -1,
    BOOT_CORRUPT_STATUS_ERROR_IO_FAILURE = -2,
    BOOT_CORRUPT_STATUS_ERROR_LOCKED = -3
} boot_corrupt_status_t;

/** @brief Target partition specifiers for corruption injection. */
typedef enum {
    BOOT_CORRUPT_TARGET_PRIMARY_LUN = 0,
    BOOT_CORRUPT_TARGET_SECONDARY_LUN,
    BOOT_CORRUPT_TARGET_BOOT_HEADER,
    BOOT_CORRUPT_TARGET_ALL
} boot_corrupt_target_t;

/**
 * @brief Initializes the boot corruption engine subsystem.
 * @return boot_corrupt_status_t BOOT_CORRUPT_STATUS_SUCCESS on readiness.
 */
boot_corrupt_status_t boot_corrupt_init(void);

/**
 * @brief Injects garbage data and corrupts integrity checks on a specified target.
 * 
 * @param target The specific bootloader partition or LUN to compromise.
 * @return boot_corrupt_status_t Status of the corruption operation.
 */
boot_corrupt_status_t boot_corrupt_inject(boot_corrupt_target_t target);

#ifdef __cplusplus
}
#endif

#endif /* PKM_SOFT_BOOT_CORRUPT_H */