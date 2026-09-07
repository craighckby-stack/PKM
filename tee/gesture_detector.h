/**
 * @file gesture_detector.h
 * @brief TrustZone (TEE) hardware gesture detector interface.
 * 
 * Monitors power button physical cycles securely beneath the untrusted OS layer
 * to detect and respond to the duress panic sequence.
 */

#ifndef TEE_GESTURE_DETECTOR_H
#define TEE_GESTURE_DETECTOR_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __c5plusplus
extern "C" {
#endif

/**
 * @brief Opaque handle representing the hardware gesture detection context.
 */
typedef struct tee_gesture_detector tee_gesture_detector_t;

/**
 * @brief Configuration parameters for the gesture detection timing thresholds.
 */
typedef struct {
    uint32_t min_press_duration_ms;
    uint32_t max_press_duration_ms;
    uint32_t max_inter_press_gap_ms;
    uint8_t  target_sequence_count;
} tee_gesture_config_t;

/**
 * @brief Initializes the TEE hardware gesture detector subsystem.
 * 
 * @param config Pointer to configuration parameters; uses secure defaults if NULL.
 * @return tee_gesture_detector_t* Pointer to the initialized detector instance, or NULL on failure.
 */
tee_gesture_detector_t* tee_gesture_detector_init(const tee_gesture_config_t* config);

/**
 * @brief Destroys and cleans up the gesture detector instance, releasing secure resources.
 * 
 * @param detector Pointer to the detector instance to decommission.
 */
void tee_gesture_detector_destroy(tee_gesture_detector_t* detector);

/**
 * @brief Processes a raw power button hardware event interrupt or state change.
 * 
 * @param detector Pointer to the active detector instance.
 * @param is_pressed Boolean indicating the current physical state of the button (true = pressed).
 * @param timestamp_ns High-resolution monotonic timestamp in nanoseconds.
 * @return true If the duress panic sequence was successfully completed.
 * @return false If the sequence is still incomplete or invalid.
 */
bool tee_gesture_detector_process_event(tee_gesture_detector_t* detector, bool is_pressed, uint64_t timestamp_ns);

#ifdef __cplusplus
}
#endif

#endif /* TEE_GESTURE_DETECTOR_H */