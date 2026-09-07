/**
 * @file key_zeroize.h
 * @brief TrustZone hardware keystore zeroization and key purge operations.
 * 
 * Responsible for securely erasing derived per-boot cryptographic keys 
 * during the PKM duress execution sequence.
 */

#ifndef STORAGE_KEY_ZEROIZE_H
#define STORAGE_KEY_ZEROIZE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Purges and securely zeroizes all active per-boot cryptographic keys
 *        from the TrustZone hardware keystore.
 *
 * @note Executed as a critical security response during the PKM duress sequence.
 * @return true if zeroization and hardware verification succeed, false otherwise.
 */
bool keystore_zeroize_per_boot_keys(void);

#ifdef __cplusplus
}
#endif

#endif /* STORAGE_KEY_ZEROIZE_H */