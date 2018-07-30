
# Spectre and Meltdown Guidance

## Table of Contents
1. [About this Repository](#about-this-repository)
1. [General Guidance](#general-guidance)
1. [Affected Processors](#affected-processors)
1. [Additional Processor Flaw Guidance](#additional-processor-flaw-guidance)
   1. [SpectrePrime and MeltdownPrime](#spectreprime-and-meltdownprime)
   1. [SgxPectre](#sgxpectre)
   1. [Total Meltdown](#total-meltdown)
   1. [BranchScope](#branchscope)
   1. [Ryzenfall, Chimera, Fallout, and Masterkey](#ryzenfall-chimera-fallout-masterkey)
1. [License](#license)
1. [Contributing](#contributing)
1. [Disclaimer](#disclaimer)

## About This Repository
This repository provides content for aiding DoD administrators in verifying systems have applied and enabled mitigations for [Spectre](https://spectreattack.com/) and [Meltdown](https://meltdownattack.com/). The repository is a companion to a forthcoming Information Assurance Advisory [Updated Guidance for Spectre and Meltdown Vulnerabilities Affecting Modern Processors](https://www.iad.gov/iad/library/ia-advisories-alerts/updated-guidance-for-spectre-and-meltdown-vulnerabilities-affecting-modern-processors.cfm). This advisory will be an update to the previously issued advisory [Vulnerabilties Affecting Modern Processors](https://www.iad.gov/iad/library/ia-advisories-alerts/vulnerabilities_affecting_modern_processors.cfm).

## General Guidance
Vulnerabilities affecting modern [Intel](https://security-center.intel.com/advisory.aspx?intelid=INTEL-SA-00088&languageid=en-fr), [AMD](https://www.amd.com/en/corporate/speculative-execution), [Arm](https://developer.arm.com/support/security-update), and [IBM](https://www.ibm.com/blogs/psirt/potential-impact-processors-power-family/) processors have been [disclosed](https://googleprojectzero.blogspot.com/2018/01/reading-privileged-memory-with-side.html).

| Marketing Name | Variant | Technical Name | CVE | Requires OS patches | Requires firmware patches | Requires application patches | Requires configuration changes |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Spectre | 1 | Bounds Check Bypass | CVE-2017-5753 | Yes | No | Yes | Yes, for some applications |
| Spectre | 1.1 | Write Bounds Bypass | CVE-2018-3693 | TBD | TBD | No | TBD |
| Spectre | 1.2 | Protected Memory Range Read Bypass | CVE-2018-3693 | TBD | TBD | No | TBD |
| Spectre | 2 | Branch Target Injection | CVE-2017-5715 | Yes | Yes | No | Yes, for some operating systems and work roles |
| Meltdown | 3 | Rogue Data Cache Load | CVE-2017-5754 | Yes | No | No | Yes, for some operating systems |
| Meltdown | 3.1 | Rogue Register Load | CVE-2018-3640 | Yes | No | No | Yes, for some operating systems |
| SpectreNG | 4 | Bounds Check Bypass| CVE-2018-3639 | Yes | Yes | No | No |
| SgxPectre | | Enclave Branch Target Injection | | Yes | No | Yes | No. Devs update SGX SDK. |
| SpecreRSB | | Return Stack Poisoning | | TBD | TBD | TBD | TBD |
| NetSpectre | | Network-attached Cache Siphon | | TBD | TDB | No | Yes, firewall configuration updates |

Mitigations fall under a common number of themes:
* Installing application specific patches and in some cases configuring the application.
* Installing operating system patches and in some cases configuring the operating system.
* Installing firmware patches that contain processor microcode updates.

General guidance for prioritizing patching:
1. Prioritize patching applications, such as browsers first, as they are the easiest to patch, have the least amount of issues with performance and compatibility, and the most likely widespread attack vector.
1. Prioritize installing operating system patches on desktop, laptops, and tablets. Compatibility issues with operating system patches have been largely resolved by the OS vendors and performance issues are much less on desktops since they typically do not have IO intensive workloads like servers (file storage arrays, email servers, database servers) where the majority of the performance issues are excertbated. Attacks via email, Office documents, PDFs, are the second most likely widespread attack vector.
1. Prioritize patching servers that do NOT have IO intensive workloads (no file storage arrays, no email servers, no database servers). Some organizations may want to wait on patching any servers until more performance data is available or more localized testing has been performed to determine if the risk of remaining unpatched is warranted for the performance trade offs.
1. Do not install firmware patches until processor manufacturers (Intel, AMD, IBM, Arm, etc) and OEMs (Dell, Dell EMC, HP Inc, HP Enterprise, etc) have signaled the new firmware patches are ready.

The main areas of interest in the repository are:
* [Guidance](./guidance) - Operating system and application specific guidance. Currently only for Windows and Linux.
* [Verification](./verification) - Operating and application specific verification of mitigations. Currently only for Windows.

The files in this repository can be downloaded as a zip file [here](https://github.com/nsacyber/Spectre-and-Meltdown-Guidance/archive/master.zip).

## Affected Processors
Below is a list of known affected processors as documented by [Intel](https://security-center.intel.com/advisory.aspx?intelid=INTEL-SA-00088&languageid=en-fr), [AMD](https://www.amd.com/en/corporate/speculative-execution), [Arm](https://developer.arm.com/support/security-update), and [IBM](https://www.ibm.com/blogs/psirt/potential-impact-processors-power-family/). It is likely that more processors than documented below are affected.

| Manufacturer | Processor Family |
| --- | --- |
| AMD | Ryzen |
| AMD | EPYC |
| AMD | Opteron |
| AMD | Athlon |
| AMD | Turion X2 Ultra |
| Arm | Cortex-R7 |
| Arm | Cortex-R8 |
| Arm | Cortex-A8 |
| Arm | Cortex-A9 |
| Arm | Cortex-A15 |
| Arm | Cortex-A17 |
| Arm | Cortex-A57 |
| Arm | Cortex-A72 |
| Arm | Cortex-A73 |
| Arm | Cortex-A75 |
| IBM | POWER7 |
| IBM | POWER7+ |
| IBM | POWER8 |
| IBM | POWER9 |
| Intel | Core i3 processor (45nm and 32nm) |
| Intel | Core i5 processor (45nm and 32nm) |
| Intel | Core i7 processor (45nm and 32nm) |
| Intel | Core M processor family (45nm and 32nm) |
| Intel | 2nd generation Core processors |
| Intel | 3rd generation Core processors |
| Intel | 4th generation Core processors |
| Intel | 5th generation Core processors |
| Intel | 6th generation Core processors |
| Intel | 7th generation Core processors |
| Intel | 8th generation Core processors |
| Intel | Core X-series Processor Family for Intel X99 platforms |
| Intel | Core X-series Processor Family for Intel X299 platforms |
| Intel | Xeon processor 3400 series |
| Intel | Xeon processor 3600 series |
| Intel | Xeon processor 5500 series |
| Intel | Xeon processor 5600 series |
| Intel | Xeon processor 6500 series |
| Intel | Xeon processor 7500 series |
| Intel | Xeon Processor E3 Family |
| Intel | Xeon Processor E3 v2 Family |
| Intel | Xeon Processor E3 v3 Family |
| Intel | Xeon Processor E3 v4 Family |
| Intel | Xeon Processor E3 v5 Family |
| Intel | Xeon Processor E3 v6 Family |
| Intel | Xeon Processor E5 Family |
| Intel | Xeon Processor E5 v2 Family |
| Intel | Xeon Processor E5 v3 Family |
| Intel | Xeon Processor E5 v4 Family |
| Intel | Xeon Processor E7 Family |
| Intel | Xeon Processor E7 v2 Family |
| Intel | Xeon Processor E7 v3 Family |
| Intel | Xeon Processor E7 v4 Family |
| Intel | Xeon Processor Scalable Family |
| Intel | Xeon Phi Processor 3200, 5200, 7200 Series |
| Intel | Atom Processor C Series |
| Intel | Atom Processor E Series |
| Intel | Atom Processor A Series |
| Intel | Atom Processor x3 Series |
| Intel | Atom Processor Z Series |
| Intel | Celeron Processor J Series |
| Intel | Celeron Processor N Series |
| Intel | Pentium Processor J Series |
| Intel | Pentium Processor N Series |

## Additional Processor Flaw Guidance
### SpectrePrime and MeltdownPrime
[General Guidance](#general-guidance) is sufficient for mitigating the prime variants of Spectre and Meltdown. Prime variants feature an implementation difference and speculative exploitation across processor core caches.

### SgxPectre
SgxPecture (sometimes referred to as SgxSpectre) leverages a race condition built into the Intel SGX SDK. A Spectre variant 2-like branch target injection vulnerability results. The attack affects Intel processors with SGX instructions.

Developers should install SDK updates and recompile SGX applications. All SGX SDKs that are derived from the Intel SDK must be updated. All SGX applications built from Intel-derived SGX SDKs must be recompiled or patched.

Administrators should update SGX-enabled applications and apply OS SGX-related patches.

### Total Meltdown
A patching flaw has been identified in Windows 7 and Server 2008 64-bit operating systems. Security fixes intended to mitigate Meltdown may not have been effective. Install Microsoft's March 2018 rollup patches as a solution. Refrence patch KB4088878.

[Microsoft Update Catalog](https://www.catalog.update.microsoft.com/Search.aspx?q=KB4088878)

[Microsoft Support Advisory](https://support.microsoft.com/en-us/help/4088878/windows-7-update-kb4088878)

### BranchScope
Firmware patches are expected Q2 2018. No further information is available at this time.

### Ryzenfall, Chimera, Fallout, Masterkey
Software and firmware patches are expected in Q2 2018. Ryzenfall, Fallout, and Masterkey flaws affect AMD products. Chimera flaws affect ASMedia products with debugging features regardless of processor brand or architecture. The following mitigations are advised:

1. Revisit security practices involving local machine administrative privileges. Attacks in this category require administrative credentials. Properly safeguarding and limiting the use of such credentials is critical.
1. Set a UEFI configuration administrator password if not already present.
1. Investigate UEFI configuration to identify and disable unused devices and ports. Unused SATA controller ports or unused USB controller ports are example candidates for disabling. Tailor the UEFI configuration based on a device's use case.

## License
See [LICENSE](./LICENSE.md).

## Contributing
See [CONTRIBUTING](./CONTRIBUTING.md)

## Disclaimer
See [DISCLAIMER](./DISCLAIMER.md).
