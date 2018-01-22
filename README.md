# Spectre and Meltdown Guidance

## About this Repository
This repository provides content for aiding DoD administrators in verifying systems have applied and enabled mitigations for [Spectre](https://spectreattack.com/) and [Meltdown](https://meltdownattack.com/). The repository is a companion to a forthcoming Information Assurance Advisory [Updated Guidance for Spectre and Meltdown Vulnerabilities Affecting Modern Processors](https://www.iad.gov/iad/library/ia-advisories-alerts/updated-guidance-for-spectre-and-meltdown-vulnerabilities-affecting-modern-processors.cfm). This advisory will be an update to the previously issued advisory [Vulnerabilties Affecting Modern Processors](https://www.iad.gov/iad/library/ia-advisories-alerts/vulnerabilities_affecting_modern_processors.cfm).

Three vulnerabilities affecting modern [Intel](https://security-center.intel.com/advisory.aspx?intelid=INTEL-SA-00088&languageid=en-fr), [AMD](https://www.amd.com/en/corporate/speculative-execution), [Arm](https://www.amd.com/en/corporate/speculative-execution), and [IBM](https://www.ibm.com/blogs/psirt/potential-impact-processors-power-family/) processors have been [disclosed](https://googleprojectzero.blogspot.com/2018/01/reading-privileged-memory-with-side.html).

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

## License
See [LICENSE](./LICENSE.md).

## Contributing
See [CONTRIBUTING](./CONTRIBUTING.md)

## Disclaimer
See [DISCLAIMER](./DISCLAIMER.md).
