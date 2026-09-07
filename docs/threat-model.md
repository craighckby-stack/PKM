# Threat Model: PKM (Panic Key Mechanism)

## Overview

This document outlines the threat model for the Panic Key Mechanism (PKM). It defines adversary profiles, potential attack vectors, and core security assumptions underlying the duress self-destruct protocol.

## Adversary Profiles

| Adversary Profile | Capabilities | Motivation |
| :--- | :--- | :--- |
| **Opportunistic Observer** | Physical proximity during device operation; direct visual line of sight. | Coercion or unauthorized access to active data. |
| **Coercive Actor** | Direct physical confrontation; ability to issue verbal or physical threats demanding decryption keys or device unlock. | Extraction of sensitive information under duress. |
| **Advanced Forensic Examiner** | Access to specialized hardware, JTAG/chip-off extraction capabilities, and software tools for persistent non-volatile memory analysis. | Uncovering remnants of wiped data or system state history. |

## Attack Vectors

### Device Seizure
* **Description:** An adversary physically acquires the active device while unlocked or in a suspended state.
* **Mitigation:** Immediate execution of the PKM protocol triggers secure memory sanitization before deep forensic analysis can begin.

### Forensic Extraction
* **Description:** Post-seizure analysis utilizing non-invasive or invasive hardware techniques to read flash memory directly.
* **Mitigation:** Implementation of cryptographic erasure, overwriting critical master keys to render persistent data unrecoverable.

## Security Assumptions

1. **Hardware Integrity:** The underlying storage controller and secure enclave operate as intended and properly execute overwrite commands.
2. **Timing Window:** The user has sufficient time and safety to trigger the panic key sequence before physical incapacitation or device confiscation prevents action.
3. **Entropy Quality:** Cryptographic keys are generated using a cryptographically secure pseudorandom number generator (CSPRNG), ensuring that key destruction permanently severs access to encrypted volumes.