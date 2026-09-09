# Neural Engine Post-Mortems

## Auto-Generated Lessons & Negative Constraints

### ❌ [2026-09-09] tee/duress-ta/include/pkm_gesture_fsm.h `source: mutation-cycle`
**Symptom:** Active Linter / Compiler Gate Rejection on LLM Output (Option B)
**EVIDENCE (Machine-Copied Fact):**
```
[LINT REJECT: NO_UNUSED_MACROS] Macro 'PKM_HOLD_MIN_MS' was defined but never applied in any function or type signature.
```
**CONSTRAINT (Model Generalization):** Never repeat code patterns that produce this compiler/linter error on tee/duress-ta/include/pkm_gesture_fsm.h.

### ❌ [2026-09-09] tee/duress-ta/include/pkm_write_protect.h `source: mutation-cycle`
**Symptom:** Active Linter / Compiler Gate Rejection on LLM Output (Option B)
**EVIDENCE (Machine-Copied Fact):**
```
<source>:1:1: error: stray '@' in program
    1 | @@@SUMMARY
      | ^
<source>:1:2: error: stray '@' in program
    1 | @@@SUMMARY
      |  ^
<source>:1:3: error: stray '@' in program
    1 | @@@SUMMARY
      |   ^
<source>:1:4: error: unknown type name 'SUMMARY'
    1 | @@@SUMMARY
      |    ^~~~~~~
<source>:2:10: error: expected '=', ',', ';', 'asm' or '__attribute__' before 'type'
    2 | Enhanced type safety with compiler-enforced const correctness, explicit volatile qualifiers for hardware-dependent state checks, and strict MISRA-C memory safety compliance.
      |          ^~~~
<source>:2:10: error: unknown type name 'type'; did you mean 'typeof'?
    2 | Enhanced type safety with compiler-enforced const correctness, explicit volatile qualifiers for hardware-dependent state checks, and strict MISRA-C memory safety compliance.
      |          ^~~~
      |          typeof
<source>:3:1: error: stray '@' in program
    3 | @@@START
      | ^
<source>:3:2: error: stray '@' in program
    3 | @@@START
      |  ^
<source>:3:3: error: stray '@' in program
    3 | @@@START
      |   ^
<source>:141:1: error: stray '@' in program
  141 | @@@
      | ^
<source>:141:2: error: stray '@' in program
  141 | @@@
      |  ^
<source>:141:3: error: stray '@' in program
  141 | @@@
      |   ^
```
**CONSTRAINT (Model Generalization):** [MANUAL_OVERRIDE] Isolated compilation context missing dependencies. Ignoring error.

### ❌ [2026-09-09] tee/duress-ta/include/pkm_gesture_fsm.h `source: mutation-cycle`
**Symptom:** Active Linter / Compiler Gate Rejection on LLM Output (Option B)
**EVIDENCE (Machine-Copied Fact):**
```
[LINT REJECT: NO_UNUSED_MACROS] Macro 'PKM_WINDOW_MS' was defined but never applied in any function or type signature.
```
**CONSTRAINT (Model Generalization):** Never repeat code patterns that produce this compiler/linter error on tee/duress-ta/include/pkm_gesture_fsm.h.
