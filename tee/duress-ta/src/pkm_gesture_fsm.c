/**
 * @file pkm_gesture_fsm.c
 * @brief §1 trigger recognition — REAL CODE (pure logic, no hardware)
 *
 * Implements the README §1 contract exactly:
 *   3 complete hold-release cycles, each hold >= 1 second,
 *   all within a 10-second window opened by the first press.
 *
 * Design decisions (deliberate, documented):
 *  - Short holds are IGNORED cycles, not errors: a fumbled cycle does
 *    not restart the rhythm. This cannot weaken the trigger — three
 *    full-length holds are still mandatory before arming.
 *  - The whole gesture (third release) must land inside the window.
 *    Expiry resets the cycle count; the next press opens a fresh attempt.
 *  - Timestamps are assumed monotonic (secure timer). Any backwards
 *    step is treated as a fault and resets the FSM defensively.
 *  - Press-while-held / release-without-press are bounce or noise and
 *    are ignored. Electrical debounce belongs in the IRQ layer
 *    (pkm_gesture.c — hardware stub, stays stubbed).
 *
 * WIRING (when pkm_gesture.c becomes real):
 *   secure IRQ -> pkm_button_event() -> this FSM -> on PKM_ARMED ->
 *   pkm_sequence_run() (orchestrator), exactly once. This is the ONLY
 *   path to the orchestrator. That is the §1 guarantee: the normal
 *   world can neither fire nor block the trigger.
 *
 * STATUS: implemented against the contract; host unit-tested in
 *         host-tests/test_fsm.c. Needs no hardware — by design.
 */

#include "pkm_gesture_fsm.h"

typedef enum {
    FSM_WAIT_PRESS = 0,    /* no cycle in progress */
    FSM_WAIT_RELEASE       /* button held, timing the hold */
} fsm_state_t;

static struct {
    fsm_state_t state;
    uint8_t     valid_cycles;     /* qualifying holds completed */
    uint64_t    window_start_ms;  /* first press of current attempt */
    uint64_t    last_press_ms;    /* press timestamp of cycle in progress */
    bool        armed;            /* latches at third valid release */
} g_fsm;

static void fsm_reset_internal(void)
{
    g_fsm.state           = FSM_WAIT_PRESS;
    g_fsm.valid_cycles    = 0;
    g_fsm.window_start_ms = 0;
    g_fsm.last_press_ms   = 0;
    g_fsm.armed           = false;
}

void pkm_fsm_reset(void)
{
    fsm_reset_internal();
}

bool pkm_fsm_armed(void)
{
    return g_fsm.armed;
}

pkm_result_t pkm_fsm_press(uint64_t timestamp_ms)
{
    if (g_fsm.armed) {
        return PKM_INVALID_STATE;         /* latched; ignore until reset */
    }
    if (g_fsm.state != FSM_WAIT_PRESS) {
        return PKM_OK;                    /* bounce: already held */
    }

    if (g_fsm.valid_cycles == 0) {
        /* First press of a fresh attempt: open the window. */
        g_fsm.window_start_ms = timestamp_ms;
    } else if (timestamp_ms < g_fsm.window_start_ms) {
        /* Secure timer went backwards: fault, reset defensively. */
        fsm_reset_internal();
        return PKM_HW_FAULT;
    } else if ((timestamp_ms - g_fsm.window_start_ms) > (uint64_t)PKM_WINDOW_MS) {
        /* Window expired: this press begins a new attempt. */
        g_fsm.window_start_ms = timestamp_ms;
        g_fsm.valid_cycles    = 0;
    }

    g_fsm.last_press_ms = timestamp_ms;
    g_fsm.state         = FSM_WAIT_RELEASE;
    return PKM_OK;
}

pkm_result_t pkm_fsm_release(uint64_t timestamp_ms)
{
    if (g_fsm.armed) {
        return PKM_INVALID_STATE;
    }
    if (g_fsm.state != FSM_WAIT_RELEASE) {
        return PKM_OK;                    /* release without press: noise */
    }
    g_fsm.state = FSM_WAIT_PRESS;

    /* Monotonicity faults reset defensively. */
    if (timestamp_ms < g_fsm.last_press_ms ||
        g_fsm.last_press_ms < g_fsm.window_start_ms) {
        fsm_reset_internal();
        return PKM_HW_FAULT;
    }

    /* The whole gesture must complete inside the window. */
    if ((timestamp_ms - g_fsm.window_start_ms) > (uint64_t)PKM_WINDOW_MS) {
        g_fsm.valid_cycles = 0;           /* expired; fresh attempt next press */
        return PKM_OK;
    }

    if ((timestamp_ms - g_fsm.last_press_ms) >= (uint64_t)PKM_HOLD_MIN_MS) {
        g_fsm.valid_cycles++;
        if (g_fsm.valid_cycles >= PKM_REQUIRED_CYCLES) {
            g_fsm.armed = true;
            return PKM_ARMED;             /* the one and only fire signal */
        }
    }

    return PKM_OK;                        /* short hold: ignored cycle */
}
