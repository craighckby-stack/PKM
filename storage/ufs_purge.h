/**
 * @file ufs_purge.h
 * @brief PKM UFS Secure Purge and Storage Sanitize Interface
 * 
 * Provides low-level storage controller media-erase and secure purge 
 * declarations required for the system duress protocol.
 */

#ifndef PKM_STORAGE_UFS_PURGE_H
#define PKM_STORAGE_UFS_PURGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief UFS Sanitize Operation Status Codes
 */
typedef enum {
    UFS_SANITIZE_STATUS_SUCCESS = 0,
    UFS_SANITIZE_STATUS_BUSY    = 1,
    UFS_SANITIZE_STATUS_FAILED  = 2,
    UFS_SANITIZE_STATUS_INVALID = 3
} ufs_sanitize_status_t;

/**
 * @brief UFS Purge Method Types
 */
typedef enum {
    UFS_PURGE_METHOD_FLUSH_CACHE = 0,
    UFS_PURGE_METHOD_BLOCK_ERASE = 1,
    UFS_PURGE_METHOD_CRYPTO_SCRUB = 2
} ufs_purge_method_t;

/**
 * @brief Configuration parameters for UFS secure purge execution.
 */
typedef struct {
    ufs_purge_method_t method;
    uint32_t           timeout_ms;
    bool               verify_after_purge;
} ufs_purge_config_t;

/**
 * @brief Initializes the UFS secure purge subsystem.
 * 
 * @return true if initialization succeeded, false otherwise.
 */
bool ufs_purge_init(void);

/**
 * @brief Executes a low-level storage controller media-erase and secure purge.
 * 
 * @param config Pointer to the purge configuration structure.
 * @return ufs_sanitize_status_t Result status of the sanitize operation.
 */
ufs_sanitize_status_t ufs_execute_secure_purge(const ufs_purge_config_t *config);

#ifdef __cplusplus
}
#endif

#endif /* PKM_STORAGE_UFS_PURGE_H */