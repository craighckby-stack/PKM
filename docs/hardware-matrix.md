<!-- Hardware compatibility matrix documenting SoC, TrustZone, and UFS/eMMC storage controller support for the PKM-Soft duress protocol. -->

# Hardware Compatibility Matrix

This document provides a comprehensive hardware compatibility matrix for the PKM-Soft duress protocol, detailing supported System-on-Chips (SoCs), Trusted Execution Environment (TrustZone) implementations, and storage controllers (UFS/eMMC).

## Supported Architectures

| SoC Family | Architecture | TrustZone Support | Storage Controller | Status |
| :--- | :--- | :--- | :--- | :--- |
| **Qualcomm Snapdragon** | ARMv8-A / ARMv9-A | Qualcomm Secure Execution Environment (QSEE) | UFS 2.1 / 3.1 / 4.0 | Fully Supported |
| **MediaTek Dimensity** | ARMv8-A / ARMv9-A | MediaTek Trustonic Kinibi / Teev | UFS 3.1 / 4.0, eMMC 5.1 | Fully Supported |
| **Google Tensor** | ARMv8-A / ARMv9-A | ARM TrustZone (Custom Hypervisor) | UFS 3.1 / 4.0 | Fully Supported |
| **Samsung Exynos** | ARMv8-A / ARMv9-A | Samsung S-Boot / TEEGR | UFS 3.1 / 4.0, eMMC 5.1 | Experimental |

## Controller Requirements

To ensure proper execution of cryptographic wiping and duress signal dispatching, storage controllers must meet the following baseline specifications:

1. **Hardware-Backed Keystore:** Must interface directly with a secure enclave or isolated hardware module.
2. **Direct Memory Access (DMA):** Secure DMA channels must be available to prevent side-channel leakage during memory-to-storage transfers.
3. **RPMB (Replay Protected Memory Block):** Mandatory support for hardware-partitioned secure storage to prevent rollback attacks.