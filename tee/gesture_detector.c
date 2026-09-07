@@@START
// Implements TrustZone (TEE) power button gesture detection for the PKM duress trigger.
// Monitors hardware power key events below the OS layer to verify the 3-press execution sequence.

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Status codes returned by the gesture detector */
#define TEE_GESTURE_SUCCESS       0
#define TEE_GESTURE_IN_PROGRESS   1
#define TEE_GESTURE_TIMEOUT      -1
#define TEE_GESTURE_INVALID_STATE -2

/* Protocol configuration constants */
#define PKM_REQUIRED_PRESS_COUNT  3
#define PKM_MAX_INTERVAL_MS       500U

typedef enum {
    POWER_KEY_EVENT_RELEASED = 0,
    POWER_KEY_EVENT_PRESSED  = 1
} power_key_event_t;

typedef struct {
    uint8_t  press_count;
    uint32_t last_press_timestamp_ms;
    bool     is_sequence_active;
} pkm_gesture_state_t;

static pkm_gesture_state_t g_pkm_state = {
    .press_count             = 0,
    .last_press_timestamp_ms = 0,
    .is_sequence_active      = false
};

/**
 * @brief Resets the PKM gesture detector state machine to its initial idle condition.
 */
static void pkm_state_reset(pkm_gesture_state_t *const state) {
    state->press_count             = 0;
    state->last_press_timestamp_ms = 0;
    state->is_sequence_active      = false;
}

/**
 * @brief Initializes a fresh gesture tracking sequence at the specified timestamp.
 */
static void pkm_state_start_new(pkm_gesture_state_t *const state, uint32_t timestamp_ms) {
    state->is_sequence_active      = true;
    state->press_count             = 1;
    state->last_press_timestamp_ms = timestamp_ms;
}

/**
 * @brief Evaluates whether the elapsed duration between presses has violated the allowed window.
 */
static bool pkm_is_interval_exceeded(uint32_t current_time_ms, uint32_t last_time_ms, uint32_t max_interval_ms) {
    return (current_time_ms - last_time_ms) > max_interval_ms;
}

/**
 * @brief Processes incoming power key events to detect the valid multi-press duress trigger.
 */
int tee_gesture_detector_update(power_key_event_t event, uint32_t current_timestamp_ms) {
    if (event != POWER_KEY_EVENT_PRESSED) {
        return g_pkm_state.is_sequence_active ? TEE_GESTURE_IN_PROGRESS : TEE_GESTURE_SUCCESS;
    }

    if (!g_pkm_state.is_sequence_active) {
        pkm_state_start_new(&g_pkm_state, current_timestamp_ms);
        return TEE_GESTURE_IN_PROGRESS;
    }

    if (pkm_is_interval_exceeded(current_timestamp_ms, g_pkm_state.last_press_timestamp_ms, PKM_MAX_INTERVAL_MS)) {
        pkm_state_reset(&g_pkm_state);
        pkm_state_start_new(&g_pkm_state, current_timestamp_ms);
        return TEE_GESTURE_TIMEOUT;
    }

    g_pkm_state.press_count++;
    g_pkm_state.last_press_timestamp_ms = current_timestamp_ms;

    if (g_pkm_state.press_count >= PKM_REQUIRED_PRESS_COUNT) {
        pkm_state_reset(&g_pkm_state);
        return TEE_GESTURE_SUCCESS;
    }

    return TEE_GESTURE_IN_PROGRESS;
}
@@@SUMMARY
Refactored the codebase for pristine modern idioms by introducing descriptive helper functions, improving structural alignment, and enhancing documentation clarity while preserving all functional interfaces and state contracts.