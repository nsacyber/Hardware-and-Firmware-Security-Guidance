# Spectre and Meltdown Guidance

## About this Repository
This repository provides content for aiding DoD administrators in verifying systems have applied and enabled mitigations for [Spectre](https://spectreattack.com/) and [Meltdown](https://meltdownattack.com/). The repository is a companion to Information Assurance Advisory [Updated Guidance for Spectre and Meltdown Vulnerabilities Affecting Modern Processors](https://www.iad.gov/iad/library/ia-advisories-alerts/updated-guidance-for-spectre-and-meltdown-vulnerabilities-affecting-modern-processors.cfm). The files in this repository can be downloaded as a zip file [here](https://github.com/iadgov/Spectre-and-Meltdown-Guidance/archive/master.zip).

The main files of interest in the repository include:
* [Verify-Spectre-Meltdown-Mitigations-Windows-OS.audit](verification/windows/Verify-Spectre-Meltdown-Mitigations-Windows-OS.audit) - a custom Nessus audit file useful for DoD administrators who want to verify, using Nessus, Windows operating systems on their network have correctly enabled mitigations (acquire via the [ACAS](https://www.disa.mil/cybersecurity/network-defense/acas) program).
* [Guidance](guidance) - operating system and application specific guidance

## Guidance

Three vulnerabilities affecting modern Intel, AMD, Arm, and IBM processors have been disclosed.

| Name | Variant | Other | CVE | Requires OS patches | Requires firmware patches | Requires application patches | Requires configuration changes |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Spectre | 1 | Bounds Check Bypass | CVE-2017-5753 | Yes | No | Yes | Yes, for some applications |
| Spectre | 2 | Branch Target Injection | CVE-2017-5715 | Yes | Yes |  No | Yes, for some operating systems | 
| Meltdown | 3 | Rogue Data Cache Load | CVE-2017-5754 | Yes | No | No | Yes, for some operating systems |

Spectre has been confirmed to affect Intel, AMD, and Arm processors. Meltdown has been confirmed to affect Intel and IBM processors.

* CVE-2017-5753 (Bounds Check Bypass), aka variant 1, commonly referred to as Spectre
* CVE-2017-5715 (Branch Target Inject), aka variant 2, commonly referred to as Spectre
* CVE-2016-5754 (Rogue Data Cache Load, aka variant 3, commonly referred to as Meltdown

## References
*

## License
See [LICENSE](./LICENSE.md).

## Disclaimer
See [DISCLAIMER](./DISCLAIMER.md).
