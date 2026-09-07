@@@START
/**
 * @file duress_applet.h
 * @brief TEE Secure-World Duress Applet Interface
 * 
 * Provides cryptographic and hardware-isolated handling for detecting duress 
 * trigger gestures and orchestrating OS-independent emergency self-destruct 
 * sequences within the Trusted Execution Environment.
 */

#ifndef TEE_DURESS_APPLET_H
#define TEE_DURESS_APPLET_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @brief Return status codes for duress applet operations. 
 */
typedef enum {
    TEE_DURESS_STATUS_SUCCESS         = 0x00000000,
    TEE_DURESS_STATUS_INVALID_PARAM   = 0x80000001,
    TEE_DURESS_STATUS_NOT_INITIALIZED = 0x80000002,
    TEE_DURESS_STATUS_TRIGGERED       = 0x80000003,
    TEE_DURESS_STATUS_HW_FAILURE      = 0x80000004
} tee_duress_status_t;

/** 
 * @brief Configuration parameters for duress gesture detection. 
 */
typedef struct {
    uint32_t sensitivity_threshold;
    uint32_t timeout_ms;
    bool     require_hardware_token;
} tee_duress_config_t;

/**
 * @brief Initialize the duress applet secure-world handler.
 * 
 * @param[in] config Pointer to applet configuration parameters.
 * @return tee_duress_status_t TEE_DURESS_STATUS_SUCCESS on success, or an appropriate error code.
 */
tee_duress_status_t tee_duress_init(const tee_duress_config_t *config);

/**
 * @brief Process incoming sensor or hardware events to detect duress gestures.
 * 
 * @param[in] event_data Pointer to raw event buffer from the untrusted world.
 * @param[in] data_size  Size of the event buffer in bytes.
 * @return tee_duress_status_t TEE_DURESS_STATUS_TRIGGERED if a duress gesture is verified.
 */
tee_duress_status_t tee_duress_evaluate_gesture(const void *event_data, size_t data_size);

/**
 * @brief Orchestrate the OS-independent emergency self-destruct sequence.
 * 
 * @note This operation is irreversible and purges all secure storage keys.
 * @return tee_duress_status_t Does not return on success.
 */
tee_duress_status_t tee_duress_execute_self_destruct(void);

#ifdef __cplusplus
}
#endif

#endif /* TEE_DURESS_APPLET_H */
@@@
@@@SUMMARY
Refined the TEE duress applet interface header with enhanced clarity, rigorous compliance standards, and clean, modern C idioms while preserving full API and binary compatibility.