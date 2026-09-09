/**
 * @file test_fsm.c
 * @brief Host-side unit tests for pkm_gesture_fsm.c
 *
 * The FSM is pure logic — no TEE, no hardware, no mocks.
 * Build & run:
 *   cc -I../include -o test_fsm test_fsm.c ../src/pkm_gesture_fsm.c
 *   ./test_fsm
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pkm_gesture_fsm.h"

static int failures = 0;

#define CHECK(cond) do { \
    if (!(cond)) { \
        (void)fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        failures++; \
    } \
} while (0)

/**
 * @brief Helper to execute a full press and release cycle.
 * @param t Timestamp of the press event.
 * @param hold_ms Duration of the hold in milliseconds.
 * @return pkm_result_t Result code from the final FSM state transition.
 */
static pkm_result_t cycle(uint64_t t, uint64_t hold_ms)
{
    pkm_result_t r = pkm_fsm_press(t);
    if (r != PKM_OK) { 
        return r; 
    }
    return pkm_fsm_release(t + hold_ms);
}

int main(void)
{
    /* 1. Happy path: 3x >=1s holds, well inside the window. */
    pkm_fsm_reset();
    CHECK(cycle(1000, 1000) == PKM_OK);
    CHECK(cycle(3000, 1200) == PKM_OK);
    CHECK(!pkm_fsm_armed());
    CHECK(cycle(5000, 1000) == PKM_ARMED);
    CHECK(pkm_fsm_armed());
    CHECK(pkm_fsm_press(7000) == PKM_INVALID_STATE);  /* latched */

    /* 2. Short hold neither counts nor restarts the rhythm. */
    pkm_fsm_reset();
    CHECK(cycle(0,    1000) == PKM_OK);
    CHECK(cycle(2500,  300) == PKM_OK);   /* fumble: ignored */
    CHECK(!pkm_fsm_armed());
    CHECK(cycle(4000, 1000) == PKM_OK);
    CHECK(cycle(6000, 1000) == PKM_ARMED);/* third VALID hold fires */

    /* 3. Window expiry kills the attempt; next press starts fresh. */
    pkm_fsm_reset();
    CHECK(cycle(0,     1000) == PKM_OK);
    CHECK(cycle(11000, 1000) == PKM_OK);  /* press at 11s: expired, new attempt */
    CHECK(!pkm_fsm_armed());
    CHECK(cycle(13000, 1000) == PKM_OK);  /* cycle 2 of new attempt */
    CHECK(cycle(15000, 1000) == PKM_ARMED);

    /* 4. The third RELEASE must itself be in-window. */
    pkm_fsm_reset();
    CHECK(cycle(0,    1000) == PKM_OK);
    CHECK(cycle(2000, 1000) == PKM_OK);
    CHECK(cycle(4000, 6000) == PKM_OK);   /* release at exactly 10000: in */
    CHECK(pkm_fsm_armed());

    pkm_fsm_reset();
    CHECK(cycle(0,    1000) == PKM_OK);
    CHECK(cycle(2000, 1000) == PKM_OK);
    CHECK(cycle(4000, 6001) == PKM_OK);   /* release at 10001: out */
    CHECK(!pkm_fsm_armed());

    /* 5. Bounce and noise are ignored. */
    pkm_fsm_reset();
    CHECK(pkm_fsm_press(0)      == PKM_OK);
    CHECK(pkm_fsm_press(10)     == PKM_OK);   /* press while held */
    CHECK(pkm_fsm_release(1010) == PKM_OK);   /* valid 1010ms hold */
    CHECK(pkm_fsm_release(2000) == PKM_OK);   /* release without press */

    /* 6. Backwards time = fault = defensive reset. */
    pkm_fsm_reset();
    CHECK(cycle(5000, 1000)   == PKM_OK);
    CHECK(pkm_fsm_press(4000) == PKM_HW_FAULT);
    CHECK(!pkm_fsm_armed());
    CHECK(cycle(10000, 1000)  == PKM_OK);     /* fresh attempt, cycle 1 */
    CHECK(!pkm_fsm_armed());

    if (failures != 0) {
        (void)fprintf(stderr, "TESTS FAILED: %d\n", failures);
    } else {
        (void)printf("All tests passed.\n");
    }
    
    return (failures != 0) ? 1 : 0;
}