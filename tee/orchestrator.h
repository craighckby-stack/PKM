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
 * @brief Execution states for the duress self-destruct lifecycle.
 */
typedef enum {
    TEE_DESTRUCT_STATE_IDLE = 0,
    TEE_DESTRUCT_STATE_TRIGGERED,
    TEE_DESTRUCT_STATE_WIPING_KEYS,
    TEE_DESTRUCT_STATE_WIPING_STORAGE,
    TEE_DESTRUCT_STATE_COMPLETE,
    TEE_DESTRUCT_STATE_FAILED
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
 * @return true if the self-destruct sequence was successfully initiated.
 */
bool tee_orchestrator_handle_panic_gesture(void);

/**
 * @brief Query the current status of the self-destruct sequence.
 * 
 * @return Current lifecycle state of type tee_destruct_state_t.
 */
tee_destruct_state_t tee_orchestrator_get_status(void);

#ifdef __cplusplus
}
#endif

#endif // TEE_ORCHESTRATOR_H