⚠️ Legal & Usage Disclaimer — defensive security project; user responsibility applies



PKM-Soft is an L-pill for data: it protects what the data protects — sources, contacts, operations — not the person carrying it. That trade is as old as tradecraft itself

# PKM-Soft — 

"Permanently Kill Myself"


### Duress Self-Destruct Protocol — Process Specification (Flash-Layer Implementation)

---

## 0. Design Change from Full PKM

| Full PKM | PKM-Soft |
|---|---|
| Boot ROM modified in silicon | Boot ROM left intact — **turned against the device** |
| Run Token removed from Secure Element | Bootloader destroyed at the storage layer |
| eFuse flag | **Permanent Write Protection** — the storage controller's own fuse |

**Principle:** The Boot ROM cannot be changed — but it can be *fed garbage it is obligated to reject, forever.*

---

## 1. Trigger Gesture

| Step | Action |
|---|---|
| 1 | Press and hold power button for 1 second, release |
| 2 | Repeat — 3 complete hold-release cycles within a 10-second window |
| 3 | Self-destruct executes immediately on third cycle |

**Constraints:**
- Gesture detected in TrustZone (secure world), below the OS
- Functions when Android is frozen, crashed, or maliciously replaced
- Rhythmic hold-release pattern unachievable via pocket pressure

---

## 2. Core Components

| Component | Location | Role |
|---|---|---|
| Boot ROM | Silicon (intact, unmodified) | Verifies bootloader signatures — becomes the executioner |
| Bootloader partitions / boot LUNs | Flash storage | Corruption target; garbage after trigger |
| Duress Handler | TrustZone (secure world) | Detects gesture; orchestrates sequence; OS-independent |
| Data Keys | Derived per-boot, hardware keystore | Zeroized — files become permanent noise |
| UFS Secure Purge / eMMC Sanitize | Storage controller | Media-level physical erase of trimmed data |
| **Permanent Write Protection** | Storage controller (JEDEC feature) | Irreversible lock on boot partitions — rescue becomes impossible |

**Boot chain dependency:**
```
Boot ROM (silicon) → reads bootloader from flash → GARBAGE → signature check fails → silent halt
```
The chain is structurally incapable of completing. Forever.

---

## 3. Self-Destruct Sequence

| # | Action | Result |
|---|---|---|
| 1 | Duress Handler validates gesture | TEE suspends all OS operations |
| 2 | Zeroize all data encryption keys in memory | Filesystem = unrecoverable ciphertext |
| 3 | Zeroize hardware keystore / key derivation material | Keys can never be re-derived |
| 4 | Trigger UFS Secure Purge / eMMC Sanitize | Physical media erase of trimmed blocks — defeats cold-chip remanence |
| 5 | Overwrite bootloader partitions + boot LUNs with garbage | Boot chain verification now fails at Stage 1 |
| 6 | Enable **Permanent Write Protection** on boot partitions | Storage controller rejects all future writes to those regions |
| 7 | Zeroize remaining key material in volatile memory | RAM yields nothing |
| 8 | Halt / power off | Black screen. No logo. No bootloop. No error. |

---

## 4. Post-Trigger State

| Attack Path | Outcome |
|---|---|
| Boot the phone | Boot ROM loads garbage bootloader → signature fail → silent halt, black screen |
| Rescue mode (EDL / Download Mode / DFU) | Signed programmer accepted → **reflash write rejected by Permanent WP** → rescue fails permanently |
| Move storage chip to donor device | Donor's Boot ROM reads the corrupted chip → **donor halts too** (see §5) |
| Reflash the transplanted chip | Permanent WP travels **with the chip** — writes rejected on any host |
| Chip-off forensics (remove NAND, read raw) | Purged media + zero keys = noise, indistinguishable from random |
| Decap / die-level access | Controller-level WP is fused state; key material physically erased |
| Any code execution on the device | **None.** Boot never completes → zero attack surface |

---

## 5. The "Infection" Property

**How the brick behaves when the chip migrates:**

```
Hostile chip installed in donor device
   └─ donor Boot ROM trusts the chip as its boot medium
        └─ reads garbage bootloader → signature fail → donor halts, black screen
             └─ attacker reflashes → Permanent WP rejects writes → still dead
                  └─ ONLY escape: physically remove the chip
```

| Property | Status |
|---|---|
| Brick travels with the chip to any host | **Yes** |
| Donor device appears fully bricked while chip is installed | **Yes** |
| Reflashing the hostile chip on the donor | **Rejected — WP is chip-resident** |
| Donor's own original storage corrupted | **No** — the corruption is a guest, not a virus |
| Donor revives after chip removal | Yes — own storage is intact |

**The brick doesn't spread — it *hosts*.** Any machine that accepts this chip as its boot medium dies until the chip is evicted. From the attacker's chair, the difference is invisible.

---

## 6. Design Invariants

1. **No data is recoverable** — key destruction + Secure Purge defeats every forensic tier
2. **No code ever executes again** — Boot ROM refuses garbage forever; rescue is write-blocked
3. **The brick is chip-resident** — it follows the storage onto any host and kills the host's boot process while installed
4. **Silent failure mode** — black screen reads as hardware death, not kill-switch activation
5. **Trigger is OS-independent** — compromise of Android cannot block, delay, or fake the gesture
6. **Irreversibility is layered** — keys zeroized (crypto), media purged (physical), bootloader corrupted (firmware), boot partitions fused shut (storage controller)
7. **No silicon modification required** — the device's own verification logic and storage controller are weaponized against itself

---

## 7. Hardware Dependency

| Requirement | Risk |
|---|---|
| UFS/eMMC supports **Permanent Write Protection** | Optional in JEDEC spec — must be verified per chip |
| Secure Purge / Sanitize supported | Common on modern UFS, less universal on eMMC |
| TrustZone access for raw storage writes from secure world | Requires custom ROM / signed TEE applet |

**Failure of any single dependency degrades gracefully:** no WP → brick is reflashable (data still destroyed); no purge → chip-off still yields encrypted noise. Only key zeroization is mandatory — and it is always available.


PKM/
├── README.md                 ← the pitch: duress brick, not data wipe
├── docs/
│   ├── SPEC.md               ← the PKM-Soft process spec (§1–7 from before)
│   ├── THREAT-MODEL.md       ← forced handover; pre-compromise out of scope
│   └── HARDWARE.md           ← per-device: WP support? Secure Purge? TEE access?
├── tee/
│   └── duress-ta/            ← Trusted Applet: gesture detection, orchestrator
├── android/
│   └── ...                   ← userland shim (registering gesture, nothing more)
├── storage/
│   ├── wipe-keys/            ← keystore zeroization (portable — start here)
│   ├── purge/                ← UFS Secure Purge / eMMC Sanitize trigger
│   ├── corrupt-boot/         ← bootloader partition overwrite
│   └── write-protect/        ← Permanent WP enable (chip-dependent)
└── tests/
    └── recovery-attempts/    ← documented attempts to resurrect post-trigger
