/**
 * @file key_zeroize.c
 * @brief Enclave Hardware Key Zeroization Engine
 *
 * Implements secure, deterministic zeroization and immediate purging of 
 * hardware-derived cryptographic keys within the trusted secure enclave 
 * upon execution of the Panic Key Mechanism (PKM) protocol.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * Enclave hardware key management status codes.
 */
typedef enum {
    ENCLAVE_SUCCESS = 0,
    ENCLAVE_ERROR_NULL_POINTER = -1,
    ENCLAVE_ERROR_SECURE_PURGE_FAILED = -2
} EnclaveStatus_t;

/**
 * @brief Securely overwrites sensitive memory regions using volatile semantics
 *        to prevent compiler optimization bypasses.
 * 
 * @param destination Pointer to the sensitive memory buffer to zeroize.
 * @param length_bytes Size of the memory region in bytes.
 */
static inline void secure_memset_volatile(volatile uint8_t *destination, size_t length_bytes)
{
    if (destination == NULL) {
        return;
    }

    volatile uint8_t *cursor = destination;
    while (length_bytes--) {
        *cursor++ = 0x00;
    }
    
    // Ensure memory barrier/synchronization point to prevent instruction reordering
    __asm__ volatile("" ::: "memory");
}

/**
 * @brief Executes immediate purging of hardware-derived data keys in the secure enclave.
 * 
 * Triggered by the Panic Key Mechanism protocol to neutralize active cryptographic 
 * material and prevent unauthorized access or cold-boot extraction.
 *
 * @param key_buffer Pointer to the hardware key material residing in secure memory.
 * @param buffer_size Size of the key buffer in bytes.
 * @return EnclaveStatus_t Returns ENCLAVE_SUCCESS on successful zeroization.
 */
EnclaveStatus_t enclave_purge_hardware_keys(uint8_t *key_buffer, size_t buffer_size)
{
    if (key_buffer == NULL && buffer_size > 0) {
        return ENCLAVE_ERROR_NULL_POINTER;
    }

    if (buffer_size == 0) {
        return ENCLAVE_SUCCESS;
    }

    // Execute compiler-safe zeroization sequence via volatile memory operations
    secure_memset_volatile((volatile uint8_t *)key_buffer, buffer_size);

    return ENCLAVE_SUCCESS;
}

#ifdef __cplusplus
}
#endif