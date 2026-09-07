# 🧟 PKM — Panic Key Mechanism

**Duress Self-Destruct Protocol — Flash-Layer Implementation**

[![Status](https://img.shields.io/badge/status-specification-orange)](https://github.com)
[![Type](https://img.shields.io/badge/type-defensive%20security-blue)](https://github.com)
[![License](https://img.shields.io/badge/license-Apache--2.0-green)](LICENSE)
[![Disclaimer](https://img.shields.io/badge/%E2%9A%A0%EF%B8%8F-legal%20disclaimer-red)](DISCLAIMER.md)

> ⚠️ **[Legal & Usage Disclaimer](DISCLAIMER.md)** — Defensive security project; user responsibility applies.

> *"PKM-Soft is an L-pill for data: it protects what the data protects — sources, contacts, operations — not the person carrying it. That trade is as old as tradecraft itself."*

---

## 💀 The Concept

**Three deliberate presses. The phone becomes permanently inert hardware containing unrecoverable ciphertext—before it is ever surrendered.**

This is not a software wipe or a standard device lock. It is **a structural death of the device itself**—achieved entirely at the flash-storage layer without any silicon modification.

```
Before panic:  Phone works normally. Files remain ordinary files.
3 presses:     Keys destroyed. Media purged. Bootloader corrupted.
               Boot partitions fused shut. Screen goes black.
Afterward:     Nothing boots. Nothing recovers. Nothing runs. Ever.
```

---

## ⚔️ Design Variants

| Full PKM *(Silicon Variant)* | PKM-Soft *(This Specification — Flash Variant)* |
| :--- | :--- |
| Boot ROM modified in silicon | Boot ROM left intact — **turned against the device** |
| Run Token removed from Secure Element | Bootloader destroyed at the storage layer |
| eFuse flag | Permanent Write Protection — the storage controller's own fuse |

> **Core Principle:** *The Boot ROM cannot be changed — but it can be fed garbage it is **obligated** to reject. Forever.*

---

## 👆 §1 — Trigger Gesture

| Step | Action |
| :---: | :--- |
| **1** | Press and hold the power button for **1 second**, then release. |
| **2** | Repeat — **3 complete hold-release cycles** within a **10-second window**. |
| **3** | Self-destruct executes **immediately** upon the third cycle. |

### Constraints

- 🛡️ Gesture detected in **TrustZone** (secure world) — operating *below* the OS.
- ⚡ Functions even when Android is frozen, crashed, or maliciously replaced.
- 🚫 The rhythmic hold-release pattern is unachievable via accidental pocket pressure.

---

## 🧩 §2 — Core Components

| Component | Location | Role |
| :--- | :--- | :--- |
| **Boot ROM** | Silicon (intact, unmodified) | Verifies bootloader signatures — *becomes the executioner*. |
| **Bootloader Partitions / Boot LUNs** | Flash storage | Corruption target; becomes unreadable garbage after trigger. |
| **Duress Handler** | TrustZone (secure world) | Detects gesture; orchestrates sequence; operates independently of OS. |
| **Data Keys** | Derived per-boot, hardware keystore | Zeroized — files become permanent noise. |
| **UFS Secure Purge / eMMC Sanitize** | Storage controller | Media-level physical erase of trimmed data. |
| **Permanent Write Protection** | Storage controller (JEDEC feature) | Irreversible lock on boot partitions — rescue becomes impossible. |

### Boot Chain Dependency

```
Boot ROM (silicon) → reads bootloader from flash → GARBAGE
                   → signature check fails → SILENT HALT
```

> 🔒 *The chain is structurally incapable of completing. Forever.*

---

## 💥 §3 — Self-Destruct Sequence

| # | Action | Result |
| :---: | :--- | :--- |
| **1** | Duress Handler validates gesture. | TEE suspends all OS operations. |
| **2** | Zeroize all data encryption keys in memory. | Filesystem becomes unrecoverable ciphertext. |
| **3** | Zeroize hardware keystore and key derivation material. | Keys can never be re-derived. |
| **4** | Trigger UFS Secure Purge / eMMC Sanitize. | Physical media erase of trimmed blocks defeats cold-chip remanence. |
| **5** | Overwrite bootloader partitions and boot LUNs with garbage. | Boot chain verification now fails at Stage 1. |
| **6** | Enable Permanent Write Protection on boot partitions. | Storage controller rejects all future writes to those regions. |
| **7** | Zeroize remaining key material in volatile memory. | RAM yields no artifacts. |
| **8** | Halt and power off device. | 🖤 Black screen. No logo. No boot loop. No error message. |

---

## 🚫 §4 — Post-Trigger State

| Attack Path | Outcome |
| :--- | :--- |
| 🔌 **Boot the phone** | Boot ROM loads garbage bootloader → signature check fails → silent halt, black screen. |
| 🔧 **Rescue mode (EDL / Download Mode / DFU)** | Signed programmer accepted → reflash write **rejected by Permanent WP** → rescue fails permanently. |
| 📱 **Storage chip → identical-model donor** | Donor's Boot ROM reads the corrupted chip → donor halts too *(see §5)*. |
| 📱 **Storage chip → different model** | Host rejects foreign boot layouts/signatures regardless — independent of PKM. |
| ♻️ **Reflash the transplanted chip** | Permanent WP travels **with the chip** — writes are rejected on any host that mounts it. |
| 🔬 **Chip-off forensics (remove NAND, read raw)** | Purged media + zeroed keys = **noise, indistinguishable from random data**. |
| 🧪 **Decap / die-level access** | Controller-level WP is a fused state; key material is physically erased. |
| 💻 **Any code execution on the device** | **NONE.** Boot never completes → **zero attack surface**. |

---

## 🧟 §5 — Transplant Behavior: The Zombie Property

> ***The zombie doesn't chase anyone — anyone who accepts it, walks into it.***

The corrupted boot images and Permanent Write Protection are properties of the **storage medium** — they travel directly with the chip.

```
Same-model donor + transplanted chip
   └─ donor's Boot ROM reads the corrupted boot partitions (as Boot ROMs do)
        └─ signature fail → donor halts, black screen
             └─ reflash attempt → rejected by chip-resident WP → still dead
                  └─ ONLY escape: physically remove the chip
```

| Property | Status |
| :--- | :---: |
| Brick travels with the chip to an **identical-model** donor | ✅ |
| Cross-model donor | ❌ *(Rejects foreign chips regardless; PKM irrelevant)* |
| Reflashing the transplanted chip | ❌ *(Rejected — WP is chip-resident)* |
| Donor's own storage corrupted | ✅ *(No — the corruption is a guest, not a virus)* |
| Forensic chip-readers | *(Bypass boot entirely — read stored data directly)* |

**The chip requires no active logic.** The destruction state is passive: any compatible host that accepts this chip as its boot medium inherits the failure.

> 🔑 **Data security is independent of this behavior.** File-Based Encryption (FBE) keys are wrapped by the original device's hardware root and are non-functional on any host. Key destruction and Secure Purge serve as the load-bearing defense; boot-chain destruction denies *reuse* of the device.

---

## 📜 §6 — Design Invariants

| # | Invariant | Detail |
| :---: | :--- | :--- |
| **1** | 🗝️ **No data is recoverable** | Key destruction and Secure Purge defeat every forensic tier. |
| **2** | ⛔ **No code ever executes again** | Boot ROM refuses garbage forever; rescue is write-blocked. |
| **3** | 💾 **The brick is chip-resident** | Follows storage onto any **compatible** host and halts that host's boot while installed. |
| **4** | 🖤 **Silent failure mode** | Black screen reads as hardware failure — not kill-switch activation. |
| **5** | 🛡️ **Trigger is OS-independent** | Compromise of Android cannot block, delay, or fake the gesture. |
| **6** | 🧅 **Irreversibility is layered** | Keys zeroized *(crypto)* → media purged *(physical)* → bootloader corrupted *(firmware)* → boot partitions fused shut *(storage controller)*. |
| **7** | 🔩 **No silicon modification required** | The device's own verification logic and storage controller are **weaponized against itself**. |

---

## ⚙️ §7 — Hardware Dependency

| Requirement | Risk |
| :--- | :--- |
| UFS/eMMC supports **Permanent Write Protection** | ⚠️ Optional in JEDEC spec — must be verified per chip. |
| **Secure Purge / Sanitize** supported | Common on modern UFS, less universal on eMMC. |
| **TrustZone** access for raw storage writes from secure world | Requires custom ROM or signed TEE applet. |

> **Graceful degradation:** No WP means the brick is reflashable *(data is still destroyed)*; no purge means chip-off still yields encrypted noise. **Only key zeroization is mandatory — and it is always available.**

---

## 📂 Repository Structure

```
PKM/
├── README.md                 ← This document — the process specification
├── DISCLAIMER.md             ← Legal & usage disclaimer
├── LICENSE
├── docs/                     ← Planned: threat model, per-device hardware matrix
├── tee/                      ← Planned: duress Trusted Applet (gesture, orchestrator)
├── storage/                  ← Planned: wipe-keys / purge / corrupt-boot / write-protect
└── tests/
    └── recovery-attempts/    ← Planned: documented attempts to resurrect post-trigger
```

---

## 🎯 Project Status

**📦 Specification Phase** — *not yet an implementation.*

**The proof-of-life milestone:**

```
Trigger  →  Chip-off  →  Raw dump published  →  The dump is noise.
```

---

## 📄 License & Disclaimer

Code and specification: **Apache-2.0**. Read the **[full disclaimer](DISCLAIMER.md)** before use. This project is published for defensive security research; users are solely responsible for lawful use in their jurisdiction.

---

### 🧟 *Dies on purpose. Stays dead.*