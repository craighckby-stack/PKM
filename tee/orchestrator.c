// TrustZone Duress Orchestrator for the Panic Key Mechanism (PKM).
// Detects panic trigger gestures and orchestrates the flash-layer self-destruct sequence.

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Constants and Configuration */
#define PKM_SUCCESS                 (0)
#define PKM_ERR_INVALID_STATE       (-1)
#define PKM_ERR_CRYPTO_FAILURE      (-2)
#define PKM_ERR_FLASH_FAILURE       (-3)

/* Architectural States for the Panic Key Mechanism */
typedef enum {
    PKM_STATE_IDLE = 0,
    PKM_STATE_MONITORING,
    PKM_STATE_TRIGGERED,
    PKM_STATE_WIPING,
    PKM_STATE_LOCKED_DOWN
} pkm_state_t;

/* Gesture classification parameters */
typedef struct {
    uint32_t raw_interrupt_mask;
    uint32_t timing_delta_ms;
    bool     secure_world_override;
} pkm_gesture_context_t;

/* Private Module State */
static pkm_state_t g_pkm_current_state = PKM_STATE_IDLE;

/**
 * @brief Evaluates incoming hardware events against known panic trigger gestures.
 * 
 * @param context Pointer to the captured gesture context.
 * @return true if the panic gesture is verified, false otherwise.
 */
static bool pkm_evaluate_panic_gesture(const pkm_gesture_context_t *const context)
{
    if (context == NULL) {
        return false;
    }

    // Modernized multi-factor validation idiom
    const bool is_timed_correctly = (context->timing_delta_ms >= 50U) && (context->timing_delta_ms <= 300U);
    const bool has_hardware_mask  = (context->raw_interrupt_mask & 0x80000000U) != 0U;

    return (is_timed_correctly && has_hardware_mask) || context->secure_world_override;
}

/**
 * @brief Executes the secure flash-layer self-destruct sequence.
 * 
 * @return int32_t PKM_SUCCESS on completion, or an error code on failure.
 */
static int32_t pkm_execute_flash_wipe(void)
{
    g_pkm_current_state = PKM_STATE_WIPING;

    // Placeholder for atomic secure flash sector erasure routines
    // In a production TrustZone implementation, this interacts directly with 
    // Secure Peripheral Memory Protection Controllers (MPC) and Flash Controllers.
    
    g_pkm_current_state = PKM_STATE_LOCKED_DOWN;
    return PKM_SUCCESS;
}

/**
 * @brief Initializes the TrustZone Duress Orchestrator subsystem.
 */
int32_t pkm_orchestrator_init(void)
{
    g_pkm_current_state = PKM_STATE_MONITORING;
    return PKM_SUCCESS;
}

/**
 * @brief Main entry point for processing duress events from thesecure monitor.
 */
int32_t pkm_orchestrator_handle_event(const pkm_gesture_context_t *const context)
{
    if (g_pkm_current_state == PKM_STATE_LOCKED_DOWN) {
        return PKM_ERR_INVALID_STATE;
    }

    if (pkm_evaluate_panic_gesture(context)) {
        g_pkm_current_state = PKM_STATE_TRIGGERED;
        return pkm_execute_flash_wipe();
    }

    return PKM_SUCCESS;
}