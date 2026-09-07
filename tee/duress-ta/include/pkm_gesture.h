/**
 * @file pkm_gesture.h
 * @brief Secure-world power-button acquisition — PKM trigger input path
 *
 * CONTRACT (non-negotiable):
 *   The power button GPIO is registered as a SECURE interrupt, owned by
 *   the TEE. Button events NEVER travel through the normal world.
 *
 *   No function in this module accepts a gesture code, pattern, or
 *   payload from the Rich OS. An API of the form evaluate_gesture(code)
 *   is architecturally wrong: it hands a compromised OS the power to
 *   block, fake, or fire the trigger. Forbidden. (Post-mortem #1.)
 *
 * GROUNDING:
 *   OP-TEE optee_os — secure interrupt registration (core/main.c,
 *   itr_ APIs), platform GPIO drivers under core/drivers/.
 *
 * STATUS: STUB — all functions return PKM_NOT_IMPLEMENTED.
 */

#ifndef PKM_GESTURE_H
#define PKM_GESTURE_H

#include "pkm_types.h"

/*
 * Subscribe the power button as a secure interrupt owned by this
 * TA / TEE core. Platform-specific: requires the SoC GPIO/IRQ mapping
 * documented in docs/HARDWARE.md. Without this, there is no PKM.
 */
pkm_result_t pkm_button_subscribe_secure_irq(void);

/* Unsubscribe. Test/init-failure path ONLY — never during duress. */
pkm_result_t pkm_button_unsubscribe(void);

/*
 * Feed one raw press/release event, with a secure-timer timestamp,
 * into the gesture FSM. Callable from the secure interrupt handler
 * ONLY. There is deliberately no normal-world entry point.
 */
pkm_result_t pkm_button_event(bool pressed, uint64_t timestamp_ms);

#endif /* PKM_GESTURE_H */
