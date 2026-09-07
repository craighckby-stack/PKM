// PKM TrustZone Duress Orchestrator
// Header defining the secure-world interface for handling panic gestures and orchestrating the multi-stage self-destruct sequence.

#ifndef TEE_ORCHESTRATOR_H
#define TEE_ORCHESTRATOR_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Execution states governing the duress self-destruct lifecycle.
 */
typedef enum {
    TEE_DESTRUCT_STATE_IDLE           = 0,
    TEE_DESTRUCT_STATE_TRIGGERED      = 1,
    TEE_DESTRUCT_STATE_WIPING_KEYS    = 2,
    TEE_DESTRUCT_STATE_WIPING_STORAGE = 3,
    TEE_DESTRUCT_STATE_COMPLETE       = 4,
    TEE_DESTRUCT_STATE_FAILED         = 5
} tee_destruct_state_t;

/**
 * @brief Initialize the TrustZone duress orchestrator subsystem.
 * 
 * @return true if initialization succeeded, false otherwise.
 */
bool tee_orchestrator_init(void);

/**
 * @brief Handle an incoming panic gesture or duress signal.
 * 
 * @return true if the self-destruct sequence was successfully initiated, false otherwise.
 */
bool tee_orchestrator_handle_panic_gesture(void);

/**
 * @brief Query the current execution status of the self-destruct sequence.
 * 
 * @return The active lifecycle state represented as a tee_destruct_state_t.
 */
tee_destruct_state_t tee_orchestrator_get_status(void);

#ifdef __cplusplus
}
#endif

#endif // TEE_ORCHESTRATOR_H