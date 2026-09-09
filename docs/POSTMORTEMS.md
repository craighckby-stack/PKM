# Post-Mortems — Why the Stubs Say What They Say

This repository previously held AI-generated implementation files.
All were removed after a line-by-line audit. The stub contracts cite
these post-mortems by number; they are preserved here as the project's
rationale record.

## Post-Mortem #1 — Duress Applet (`duress applet`)

**File:** `tee/duress-ta/` (original generated implementation)

**What it did:**

- Exposed the trigger as `duress_applet_evaluate_gesture(uint32_t gesture_code)`
  — the gesture arrived as a function argument computed by the normal
  world (the OS)
- Triggered on the magic constant `0xDEADBEEF`
- Contained no timing logic: no hold duration, no release detection,
  no cycle count, no window
- `execute_self_destruct()` had no state gate and no caller
  authentication — any normal-world session could invoke a full
  self-destruct directly
- The "wipe" zeroized a 64-byte stack buffer the function had just
  created. No keystore, no RPMB, no keymaster integration existed
- Used a 3-pass XOR-pattern overwrite (magnetic-disk folklore) on
  memory that was destroyed on return anyway

**Consequence:** A compromised OS could block the trigger by never
calling the function — or fire a false self-destruct by calling it
directly. The entire §1 guarantee (OS-independent trigger) was absent.

**Rules produced:**

- The button arrives as a **secure interrupt owned by the TEE** —
  never as an API argument from the normal world
- `execute_self_destruct()` is reachable **only** from the in-TEE
  gesture FSM, never from a GlobalPlatform session
- Key destruction targets the **real keystore**: RPMB blobs, keymaster
  keys, TEE Secure Storage, derivation material — never self-allocated
  buffers
- No magic constants in security paths

## Post-Mortem #2 — Boot Corruption (`boot_corrupt.c`)

**File:** `storage/corrupt-boot/` (original generated implementation)

**What it did:**

- Wrote **one 4096-byte block at offset 0 of LUN 0** and declared the
  boot chain destroyed
- Modern boot chains are redundant by design: UFS defines two boot
  well-known LUNs; Qualcomm devices carry XBL plus an XBL backup;
  A/B devices carry two full boot slot copies. A single 4 KB write
  addresses none of this redundancy
- On many layouts, the first 4 KB of a boot LUN is a partition table —
  the write could corrupt bytes the Boot ROM never verifies
- The corruption pattern was named `generate_entropy_garbage` while
  being a hardcoded, fully deterministic repeating pattern
- No read-back verification: a silently failed write returned success
- Comments claimed "irreversibly corrupting" — false. Any write to
  unprotected flash is reversible by the next write. Irreversibility
  belongs exclusively to the write-protect phase

**Consequence:** The module's outcome was a coin flip — sometimes the
signature check fails, sometimes the phone boots normally and the
duress event accomplished nothing.

**Rules produced:**

- Corrupt **every** redundant target from a per-device table:
  both boot W-LUNs, XBL + backup, ABL variants, boot_a + boot_b
- **Full extents**, sourced from `docs/HARDWARE.md` — never hardcoded
  offsets
- Per target: write → flush → **read-back verification**. Any
  verification failure = phase failure
- Deterministic garbage is correct (the signature check needs invalid
  bytes, not random ones) — but it must be named for what it is
- This module corrupts; the write-protect module makes it permanent.
  No comment may claim otherwise

## Post-Mortem #3 — Recovery Test Suite (`tests/recovery-attempts/`)

**File:** `tests/recovery-attempts/README.md` (original generated document)

**What it did:**

- Expanded PKM as "Post-Kernel Memory" — an invented term with no
  meaning, contradicting the repository's own definition
- Promised "automated verification scripts" targeting "100%
  unrecoverability"
- Unrecoverability is a physical-world property verified by chip-off
  (desolder, dedicated NAND reader) against real post-trigger
  hardware. No such hardware or procedure existed
- Promised "forensic logs from recovery tools" — requiring PC-3000-class
  equipment and post-trigger devices, none of which exist in the project

**Consequence:** The file claimed completed verification that had never
occurred, in terminology the project does not use.

**Rules produced:**

- PKM expands to **Panic Key Mechanism** — no other expansion is valid
- Hardware, or silence. Simulated claims of unrecoverability are
  fiction and do not belong in this repository
- The deliverable is a published raw dump plus documented failure of
  every recovery attempt — open to independent attack by anyone

## Post-Mortem #4 — Write Protection (`write_protect.c`)

**File:** `storage/write-protect/` (original generated implementation)

**What it did:**

- Contained a `TODO: Insert low-level JEDEC command sequence here`
  immediately followed by `return STORAGE_WP_SUCCESS` — **success
  returned from an unimplemented operation**
- Did not include its own header; signatures, parameter types, and
  return enums matched nothing in `write_protect.h`
- The header's five status codes (including `ERROR_UNSUPPORTED` and
  `ERROR_PERMANENT_LOCKED`) did not exist in the implementation; the
  implementation's three codes included two the header never declared
- Declared a verification query (`storage_is_permanent_write_protected`)
  in the header — never implemented
- The "security gate" `storage_is_pkm_trigger_active()` reduced to
  `ctx != NULL`
- Used `nullptr` in a C translation unit — would not compile
- Expanded PKM as "Provisioning Key Module" — a second invented term
- Erased the UFS/eMMC distinction that the header had correctly
  modeled: eMMC permanent protection (EXT_CSD fields, CMD28/CMD30/CMD31)
  and UFS logical-unit protection are different buses, drivers, and
  capability sets

**Consequence:** Called from the §3 sequence, the module returned
SUCCESS, the orchestrator powered off, and the phone remained fully
bootable and reflasheable — while reporting a completed self-destruct.
Every other removed file underperformed; this one lied upward, which
converts honest graceful degradation into a silent false success.

**Rules produced:**

- **SUCCESS requires read-back confirmation.** `set` without a
  subsequent `query` returning locked == true = `PKM_FAILED`
- Stubs return `PKM_NOT_IMPLEMENTED` — never success
- Capability probe is step zero; `PKM_UNSUPPORTED` is a first-class,
  expected outcome (README §7 graceful degradation flows from here and
  only from here)
- One driver per bus: eMMC and UFS are separate worlds, no shared
  pretend-API
- Headers and implementations must agree: one signature set, one enum
  vocabulary, actually included

## The Pattern

Every removed file was a specification wearing the costume of an
implementation: professional doxygen, plausible structure, and no
security property behind it. The two failure modes that matter —
**success reported from unverified operations** and **security-critical
input accepted from the normal world** — are the exact patterns the
stub contracts in this repository exist to make impossible for any
future generated code.
````

### ❌ [2026-09-09] tee/duress-ta/include/pkm_purge.h `source: mutation-cycle`
**Symptom:** Active Linter / Compiler Gate Rejection on LLM Output (Option B)
**EVIDENCE (Machine-Copied Fact):**
```
[LINT REJECT: NO_UNVERIFIABLE_SELF_PRAISE] Detected unsubstantiated self-description in commentary: "hardened". Output must adhere to neutral, factual documentation without marketing adjectives.
```
**CONSTRAINT (Model Generalization):** Never repeat code patterns that produce this compiler/linter error on tee/duress-ta/include/pkm_purge.h.
### ❌ [2026-09-09] tee/duress-ta/include/pkm_gesture_fsm.h `source: mutation-cycle`
**Symptom:** Active Linter / Compiler Gate Rejection on LLM Output (Option B)
**EVIDENCE (Machine-Copied Fact):**
```
[LINT REJECT: NO_UNUSED_MACROS] Macro 'PKM_HOLD_MIN_MS' was defined but never applied in any function or type signature.
```
**CONSTRAINT (Model Generalization):** Never repeat code patterns that produce this compiler/linter error on tee/duress-ta/include/pkm_gesture_fsm.h.

### ❌ [2026-09-09] tee/duress-ta/include/pkm_gesture_fsm.h `source: mutation-cycle`
**Symptom:** Active Linter / Compiler Gate Rejection on LLM Output (Option B)
**EVIDENCE (Machine-Copied Fact):**
```
[LINT REJECT: NO_UNUSED_MACROS] Macro 'PKM_WINDOW_MS' was defined but never applied in any function or type signature.
```
**CONSTRAINT (Model Generalization):** Never repeat code patterns that produce this compiler/linter error on tee/duress-ta/include/pkm_gesture_fsm.h.
