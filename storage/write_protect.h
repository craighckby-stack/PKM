/**
 * @file write_protect.h
 * @brief PKM Permanent Write Protection Interface
 * 
 * Hardware storage controller (JEDEC UFS/eMMC) permanent write-lock definitions
 * utilized during critical panic execution sequences to irreversibly secure and
 * isolate corrupted boot partitions.
 */

#ifndef STORAGE_WRITE_PROTECT_H
#define STORAGE_WRITE_PROTECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Storage controller hardware target types for write-protection.
 */
typedef enum {
    STORAGE_TARGET_UFS = 0,
    STORAGE_TARGET_EMMC
} StorageTargetType;

/**
 * @brief Write-protection operation status codes.
 */
typedef enum {
    WRITE_PROTECT_STATUS_SUCCESS = 0,
    WRITE_PROTECT_STATUS_ERROR_INVALID_TARGET,
    WRITE_PROTECT_STATUS_ERROR_HARDWARE_BUSY,
    WRITE_PROTECT_STATUS_ERROR_UNSUPPORTED,
    WRITE_PROTECT_STATUS_ERROR_PERMANENT_LOCKED
} WriteProtectStatus;

/**
 * @brief Permanently enables hardware write-protection on the specified storage partition.
 * 
 * This routine interacts directly with the storage controller to set permanent 
 * write-lock configuration descriptors. Once executed, this operation is irreversible 
 * and intended exclusively for panic or severe corruption mitigation workflows.
 *
 * @param target_type The underlying storage controller architecture (UFS or eMMC).
 * @param partition_id Identifier of the target boot partition to lock.
 * @return WriteProtectStatus Execution status indicating success or failure reason.
 */
WriteProtectStatus storage_enable_permanent_write_protection(
    StorageTargetType target_type, 
    uint8_t partition_id
);

/**
 * @brief Verifies if the specified storage partition is permanently write-protected.
 *
 * @param target_type The underlying storage controller architecture (UFS or eMMC).
 * @param partition_id Identifier of the target boot partition to query.
 * @return true if the partition is permanently locked, false otherwise.
 */
bool storage_is_permanent_write_protected(
    StorageTargetType target_type, 
    uint8_t partition_id
);

#ifdef __cplusplus
}
#endif

#endif /* STORAGE_WRITE_PROTECT_H */