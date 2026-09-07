// Implements TrustZone (TEE) power button gesture detection for the PKM duress trigger.
// Monitors hardware power key events below the OS layer to verify the 3-press execution sequence.

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define TEE_GESTURE_SUCCESS       0
#define TEE_GESTURE_IN_PROGRESS   1
#define TEE_GESTURE_TIMEOUT      -1
#define TEE_GESTURE_INVALID_STATE -2

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
    .press_count = 0,
    .last_press_timestamp_ms = 0,
    .is_sequence_active = false
};

static void reset_gesture_state(pkm_gesture_state_t *const state) {
    state->press_count = 0;
    state->last_press_timestamp_ms = 0;
    state->is_sequence_active = false;
}

static bool is_interval_exceeded(uint32_t current_time_ms, uint32_t last_time_ms, uint32_t max_interval_ms) {
    return (current_time_ms - last_time_ms) > max_interval_ms;
}

int tee_gesture_detector_update(power_key_event_t event, uint32_t current_timestamp_ms) {
    if (event != POWER_KEY_EVENT_PRESSED) {
        return g_pkm_state.is_sequence_active ? TEE_GESTURE_IN_PROGRESS : TEE_GESTURE_SUCCESS;
    }

    if (!g_pkm_state.is_sequence_active) {
        g_pkm_state.is_sequence_active = true;
        g_pkm_state.press_count = 1;
        g_pkm_state.last_press_timestamp_ms = current_timestamp_ms;
        return TEE_GESTURE_IN_PROGRESS;
    }

    if (is_interval_exceeded(current_timestamp_ms, g_pkm_state.last_press_timestamp_ms, PKM_MAX_INTERVAL_MS)) {
        reset_gesture_state(&g_pkm_state);
        // Restart sequence for the current press that violated the timeout window
        g_pkm_state.is_sequence_active = true;
        g_pkm_state.press_count = 1;
        g_pkm_state.last_press_timestamp_ms = current_timestamp_ms;
        return TEE_GESTURE_TIMEOUT;
    }

    g_pkm_state.press_count++;
    g_pkm_state.last_press_timestamp_ms = current_timestamp_ms;

    if (g_pkm_state.press_count >= PKM_REQUIRED_PRESS_COUNT) {
        reset_gesture_state(&g_pkm_state);
        return TEE_GESTURE_SUCCESS;
    }

    return TEE_GESTURE_IN_PROGRESS;
}