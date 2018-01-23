# Spectre and Meltdown Guidance

## About this Repository
This repository provides content for aiding DoD administrators in verifying systems have applied and enabled mitigations for [Spectre](https://spectreattack.com/) and [Meltdown](https://meltdownattack.com/). The repository is a companion to a forthcoming Information Assurance Advisory [Updated Guidance for Spectre and Meltdown Vulnerabilities Affecting Modern Processors](https://www.iad.gov/iad/library/ia-advisories-alerts/updated-guidance-for-spectre-and-meltdown-vulnerabilities-affecting-modern-processors.cfm). This advisory will be an update to the previously issued advisory [Vulnerabilties Affecting Modern Processors](https://www.iad.gov/iad/library/ia-advisories-alerts/vulnerabilities_affecting_modern_processors.cfm).

Three vulnerabilities affecting modern [Intel](https://security-center.intel.com/advisory.aspx?intelid=INTEL-SA-00088&languageid=en-fr), [AMD](https://www.amd.com/en/corporate/speculative-execution), [Arm](https://developer.arm.com/support/security-update), and [IBM](https://www.ibm.com/blogs/psirt/potential-impact-processors-power-family/) processors have been [disclosed](https://googleprojectzero.blogspot.com/2018/01/reading-privileged-memory-with-side.html).

| Marketing Name | Variant | Technical Name | CVE | Requires OS patches | Requires firmware patches | Requires application patches | Requires configuration changes |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Spectre | 1 | Bounds Check Bypass | CVE-2017-5753 | Yes | No | Yes | Yes, for some applications |
| Spectre | 2 | Branch Target Injection | CVE-2017-5715 | Yes | Yes |  No | Yes, for some operating systems | 
| Meltdown | 3 | Rogue Data Cache Load | CVE-2017-5754 | Yes | No | No | Yes, for some operating systems |

Mitigations fall under a common number of themes:
* Installing firmware patches that contain processor microcode updates.
* Installing operating system patches and in some cases configuring the operating system.
* Installing application specific patches and in some cases configuring the application.

The main areas of interest in the repository are:
* [Guidance](./guidance) - Operating system and application specific guidance. Currently only for Windows.
* [Verification](./verification) - Operating and application specific verification of mitigations. Currently only for Windows.

The files in this repository can be downloaded as a zip file [here](https://github.com/iadgov/Spectre-and-Meltdown-Guidance/archive/master.zip).

## Affected processors
Below is a list of known affected processors as documented by [Intel](https://security-center.intel.com/advisory.aspx?intelid=INTEL-SA-00088&languageid=en-fr), [AMD](https://www.amd.com/en/corporate/speculative-execution), [Arm](https://developer.arm.com/support/security-update), and [IBM](https://www.ibm.com/blogs/psirt/potential-impact-processors-power-family/).

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

## License
See [LICENSE](./LICENSE.md).

## Contributing
See [CONTRIBUTING](./CONTRIBUTING.md)

## Disclaimer
See [DISCLAIMER](./DISCLAIMER.md).
