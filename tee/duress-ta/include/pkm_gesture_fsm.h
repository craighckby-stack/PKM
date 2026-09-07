/**
 * @file pkm_gesture_fsm.h
 * @brief Duress gesture recognition state machine
 *
 * TRIGGER SPEC (README §1):
 *   3 complete hold-release cycles. Each hold >= 1 second. All three
 *   cycles within a 10-second window from the first press. Destruction
 *   arms at the third release. The hold-release rhythm is the
 *   pocket-pressure defense — do not weaken it.
 *
 * CONTRACT:
 *   All state lives in static secure SRAM and is never returned to
 *   callers. Timestamps come ONLY from pkm_button_event() (secure
 *   timer). Recognition completes in-TEE: the normal world never
 *   learns a gesture is being tracked.
 *
 * STATUS: STUB — pure logic, zero hardware beyond the secure IRQ +
 * timer. Together with pkm_orchestrator.c this is one of the two
 * modules allowed to become real code BEFORE hardware exists,
 * with unit tests. Every other module waits for its hardware proof.
 */

#ifndef PKM_GESTURE_FSM_H
#define PKM_GESTURE_FSM_H

#include "pkm_types.h"

/* §1 timing constants — the single approved source of truth */
#define PKM_HOLD_MIN_MS       1000u
#define PKM_WINDOW_MS         10000u
#define PKM_REQUIRED_CYCLES   3u

/*
 * Press/release entry points (from pkm_button_event, secure world only).
 * Returns PKM_OK exactly once — at the third qualifying release.
 * That single return is the ONLY signal that may arm the orchestrator.
 */
pkm_result_t pkm_fsm_press(uint64_t timestamp_ms);
pkm_result_t pkm_fsm_release(uint64_t timestamp_ms);

/* Orchestrator accessors */
bool pkm_fsm_armed(void);
void pkm_fsm_reset(void);

#endif /* PKM_GESTURE_FSM_H */
