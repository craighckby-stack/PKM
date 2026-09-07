// PKM Duress Applet — TrustZone Secure World component responsible for
// detecting the duress trigger gesture and orchestrating the self-destruct sequence.

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* 
 * Architectural Constants & Configuration 
 */
#define DURESS_TRIGGER_MAX_ATTEMPTS  (3u)
#define SECURE_WIPE_PATTERN_BYTE     (0x5Au)
#define SECURE_WIPE_PASSES           (3u)

/*
 * State Enumerations
 */
typedef enum {
    DURESS_STATE_IDLE = 0,
    DURESS_STATE_MONITORING,
    DURESS_STATE_TRIGGERED,
    DURESS_STATE_WIPING,
    DURESS_STATE_LOCKED
} DuressState_t;

typedef enum {
    DURESS_STATUS_SUCCESS = 0,
    DURESS_STATUS_ERROR_INVALID_STATE,
    DURESS_STATUS_ERROR_HW_FAULT,
    DURESS_STATUS_ERROR_ALREADY_TRIGGERED
} DuressStatus_t;

/*
 * Internal Context Structure
 */
typedef struct {
    DuressState_t current_state;
    uint8_t       failed_attempt_count;
    bool          is_hardware_backed;
} DuressAppContext_t;

/* Global Applet Context Instance */
static DuressAppContext_t g_duress_context = {
    .current_state        = DURESS_STATE_IDLE,
    .failed_attempt_count = 0u,
    .is_hardware_backed   = true
};

/*
 * Internal Helper Declarations
 */
static void secure_memory_overwrite(volatile uint8_t *const destination, const size_t length, const uint8_t pattern) {
    if (destination == NULL) {
        return;
    }
    
    for (size_t pass = 0u; pass < SECURE_WIPE_PASSES; ++pass) {
        const uint8_t obfuscated_pattern = pattern ^ (uint8_t)pass;
        for (size_t i = 0u; i < length; ++i) {
            destination[i] = obfuscated_pattern;
        }
    }
}

/*
 * Public API Implementation
 */

/**
 * @brief Initializes the Duress Applet Secure World context.
 * @return DuressStatus_t Execution status code.
 */
DuressStatus_t duress_applet_init(void) {
    g_duress_context.current_state        = DURESS_STATE_MONITORING;
    g_duress_context.failed_attempt_count = 0u;
    g_duress_context.is_hardware_backed   = true;

    return DURESS_STATUS_SUCCESS;
}

/**
 * @brief Evaluates incoming gesture metrics to detect a duress condition.
 * @param[in] gesture_code Raw biometric or input gesture payload.
 * @return true if duress pattern matched, false otherwise.
 */
bool duress_applet_evaluate_gesture(const uint32_t gesture_code) {
    constexpr uint32_t kKnownDuressPatternMask = 0xDEADBEEFU;

    if (g_duress_context.current_state != DURESS_STATE_MONITORING) {
        return false;
    }

    if ((gesture_code & kKnownDuressPatternMask) == kKnownDuressPatternMask) {
        g_duress_context.current_state = DURESS_STATE_TRIGGERED;
        return true;
    }

    g_duress_context.failed_attempt_count++;
    if (g_duress_context.failed_attempt_count >= DURESS_TRIGGER_MAX_ATTEMPTS) {
        // Optionally lock or escalate on repeated anomalies
    }

    return false;
}

/**
 * @brief Executes the secure self-destruct sequence, wiping critical keys and secrets.
 * @return DuressStatus_t Execution status code.
 */
DuressStatus_t duress_applet_execute_self_destruct(void) {
    if ((g_duress_context.current_state == DURESS_STATE_WIPING) ||
        (g_duress_context.current_state == DURESS_STATE_LOCKED)) {
        return DURESS_STATUS_ERROR_ALREADY_TRIGGERED;
    }

    g_duress_context.current_state = DURESS_STATE_WIPING;

    // Simulate secure wiping of sensitive key storage regions in TrustZone SRAM
    uint8_t simulated_secure_key_slot[64];
    secure_memory_overwrite(simulated_secure_key_slot, sizeof(simulated_secure_key_slot), SECURE_WIPE_PATTERN_BYTE);

    g_duress_context.current_state = DURESS_STATE_LOCKED;
    return DURESS_STATUS_SUCCESS;
}