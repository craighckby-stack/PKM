# Neural Engine Post-Mortems

## Auto-Generated Lessons & Negative Constraints

### ❌ [2026-09-09] tee/duress-ta/include/pkm_gesture_fsm.h `source: mutation-cycle`
**Symptom:** Active Linter / Compiler Gate Rejection on LLM Output (Option B)
**EVIDENCE (Machine-Copied Fact):**
```
[LINT REJECT: NO_UNUSED_MACROS] Macro 'PKM_HOLD_MIN_MS' was defined but never applied in any function or type signature.
```
**CONSTRAINT (Model Generalization):** Never repeat code patterns that produce this compiler/linter error on tee/duress-ta/include/pkm_gesture_fsm.h.
