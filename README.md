# Spectre and Meltdown Guidance

## About this Repository
This repository provides content for aiding DoD administrators in verifying systems have applied and enabled mitigations for [Spectre](https://spectreattack.com/) and [Meltdown](https://meltdownattack.com/). The repository is a companion to Information Assurance Advisory [Updated Guidance for Spectre and Meltdown Vulnerabilities Affecting Modern Processors](https://www.iad.gov/iad/library/ia-advisories-alerts/updated-guidance-for-spectre-and-meltdown-vulnerabilities-affecting-modern-processors.cfm). This advisory is an update to the previously issued advisory [Vulnerabilties Affecting Modern Processors](https://www.iad.gov/iad/library/ia-advisories-alerts/vulnerabilities_affecting_modern_processors.cfm) The files in this repository can be downloaded as a zip file [here](https://github.com/iadgov/Spectre-and-Meltdown-Guidance/archive/master.zip).

The main files of interest in the repository include:
* [Verify-Spectre-Meltdown-Mitigations-Windows-OS.audit](verification/windows/Verify-Spectre-Meltdown-Mitigations-Windows-OS.audit) - a custom Nessus audit file useful for DoD administrators who want to verify, using Nessus, Windows operating systems on their network have correctly enabled mitigations (acquire via the [ACAS](https://www.disa.mil/cybersecurity/network-defense/acas) program).
* [Guidance](guidance) - operating system and application specific guidance

## Guidance

Three vulnerabilities affecting modern Intel, AMD, Arm, and IBM processors have been disclosed.
* CVE-2017-5753 (Bounds Check Bypass), aka variant 1, commonly referred to as Spectre.
* CVE-2017-5715 (Branch Target Inject), aka variant 2, commonly referred to as Spectre.
* CVE-2016-5754 (Rogue Data Cache Load, aka variant 3, commonly referred to as Meltdown.

Spectre has been confirmed to affect Intel, AMD, and Arm processors. Meltdown has been confirmed to affect Intel and IBM processors.

Mitigations fall under a common number of themes:
* Installing firmware patches that contain processor microcode updates.
* Installing operating system patches that change how the kernel memory is separated from application memory and in some cases configuring the operating system.
* Application specific patches such as browser patches that make attacks more difficult to execute (disabled SharedArrayBuffer support and reduce timer precision) and in some cases configuring the application.


| Name | Variant | Other | CVE | Requires OS patches | Requires firmware patches | Requires application patches | Requires configuration changes |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Spectre | 1 | Bounds Check Bypass | CVE-2017-5753 | Yes | No | Yes | Yes, for some applications |
| Spectre | 2 | Branch Target Injection | CVE-2017-5715 | Yes | Yes |  No | Yes, for some operating systems | 
| Meltdown | 3 | Rogue Data Cache Load | CVE-2017-5754 | Yes | No | No | Yes, for some operating systems |


## Patches and Updates

### Firmware

| OEM | Type | Link |
| --- | ---- | ---- |
| Acer | Client | [1](https://us.answers.acer.com/app/answers/detail/a_id/53104) |
| Asus| Client | [1](https://www.asus.com/News/YQ3Cr4OYKdZTwnQK) |
| Dell | Client | [1](https://www.dell.com/support/meltdown-spectre) |
| Dell EMC | Server | [1](https://www.dell.com/support/meltdown-spectre) |
| Fujitsu | Client / Server | [1](https://www.fujitsu.com/global/support/products/software/security/products-f/jvn-93823979e.html) | 
| HPi | Client | [1](https://support.hp.com/document/c05869091) |
| HPe | Server | [1](http://h22208.www2.hpe.com/eginfolib/securityalerts/SCAM/Side_Channel_Analysis_Method.html),[2](https://support.hpe.com/hpsc/doc/public/display?sp4ts.oid=null&docLocale=en_US&docId=emr_na-a00039267en_us),[3](https://support.hpe.com/hpsc/doc/public/display?docId=emr_na-hpesbhf03805en_us) | 
| Huawei | Server | [1](http://www.huawei.com/au/psirt/security-notices/huawei-sn-20180104-01-intel-en) |
| Lenovo | Client / Server | [1](https://support.lenovo.com/us/en/solutions/len-18282) |
| LG | Client | [1](https://www.lg.com/us/support) |
| Panasonic | Client | [1](https://pc-dl.panasonic.co.jp/itn/vuln/g18-001.html) |
| Samsung | Client | [1](http://www.samsung.com/uk/support/intel_update/) |
| Microsoft | Client | [1](https://support.microsoft.com/en-us/help/4073065) |
| Toshiba | Client | [1](http://go.toshiba.com/intel-side-channel) |
| Vaio | Client | [1](https://solutions.vaio.com/3316) |

### Operating Systems
| Product | Patch / Version | Release Date | Links |
| --- | --- | --- | --- |
| Android | 2018-01-05 | 01/02/2018 | [1](https://support.google.com/faqs/answer/7622138#android),[2](https://source.android.com/security/bulletin/2018-01-01) |
| Chrome OS | 63+ | 12/25/2017 | [1](https://support.google.com/faqs/answer/7622138#chromeos) |
| iOS | 11.2.2+ | 01/08/2018 | [1](https://support.apple.com/en-us/HT208401) |
| Linux kernel | 4.15.0+ | ~01/21/2018| |
| Linux kernel | 4.14.11+ | 01/03/2018 | [1](https://cdn.kernel.org/pub/linux/kernel/v4.x/ChangeLog-4.14.11) |
| Linux kernel | 4.9.75+ | 01/05/2018 | [1](https://cdn.kernel.org/pub/linux/kernel/v4.x/ChangeLog-4.9.75) |
| Linux kernel | 4.4.110+ | 01/05/2018 | [1](https://cdn.kernel.org/pub/linux/kernel/v4.x/ChangeLog-4.4.110)|
| macOS High Sierra | 10.13.2+ | 12/06/2017 | [1](https://support.apple.com/en-us/HT208331)|
| Windows 10 1709 / Windows Server 1709 | KB4056892 | 01/03/2018 | [1](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV180002),[2](https://support.microsoft.com/en-us/help/4073757/protect-your-windows-devices-against-spectre-meltdown),[3](https://support.microsoft.com/en-us/help/4056892) |
| Windows 10 1703 | KB4056891 | 01/03/2018 | [1](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV180002),[2](https://support.microsoft.com/en-us/help/4073757/protect-your-windows-devices-against-spectre-meltdown),[3](https://support.microsoft.com/en-us/help/4056891) |
| Windows 10 1607 / Windows Server 2016 | KB4056890 | 01/03/2018 | [1](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV180002),[2](https://support.microsoft.com/en-us/help/4073757/protect-your-windows-devices-against-spectre-meltdown),[3](https://support.microsoft.com/en-us/help/4056890) |
| Windows 10 1511 | KB4056888 | 01/03/2018 | [1](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV180002),[2](https://support.microsoft.com/en-us/help/4073757/protect-your-windows-devices-against-spectre-meltdown),[3](https://support.microsoft.com/en-us/help/4056888) |
| Windows 10 1507 | KB4056893 | 01/03/2018 | [1](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV180002),[2](https://support.microsoft.com/en-us/help/4073757/protect-your-windows-devices-against-spectre-meltdown),[3](https://support.microsoft.com/en-us/help/4056893) |
| Windows 8.1 / Windows Server 2012 R2 | KB4056898 | 01/03/2018 | [1](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV180002),[2](https://support.microsoft.com/en-us/help/4073757/protect-your-windows-devices-against-spectre-meltdown),[3](https://support.microsoft.com/en-us/help/4056898) |
| Windows 7 SP1 / Windows Server 2008 R2 SP1 (Monthly Rollup) | KB4056894 | 01/04/2018 | [1](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV180002),[2](https://support.microsoft.com/en-us/help/4073757/protect-your-windows-devices-against-spectre-meltdown),[3](https://support.microsoft.com/en-us/help/4056894) |
| Windows 7 SP1 /Windows Server 2008 R2 SP1 (Security Update Only) | KB4056897 | 01/03/2018 | [1](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV180002),[2](https://support.microsoft.com/en-us/help/4073757/protect-your-windows-devices-against-spectre-meltdown),[3](https://support.microsoft.com/en-us/help/4056897) |
| Windows Server 2012 | TBD | TBD | |
| Windows Server 2008 SP2 | TBD | TBD | |


#### Application Software

###### Browsers

| Product | Version | Release Date | Links |
| --- | --- | --- | --- |
| Chrome | 64+ | ~01/23/2018 | [1](https://support.google.com/faqs/answer/7622138#chrome) |
| Edge | Windows 10 OS patch | 01/03/2018 | [1](https://blogs.windows.com/msedgedev/2018/01/03/speculative-execution-mitigations-microsoft-edge-internet-explorer/) |
| Firefox | 57.0.4+ | 01/04/2018 | [1](https://www.mozilla.org/en-US/security/advisories/mfsa2018-01/),[2](https://blog.mozilla.org/security/2018/01/03/mitigations-landing-new-class-timing-attack/) |
| Firefox ESR | 52.6+ | ~01/23/2018 | [1](https://www.mozilla.org/en-US/security/advisories/mfsa2018-01/),[2](https://blog.mozilla.org/security/2018/01/03/mitigations-landing-new-class-timing-attack/) |
| Internet Explorer | Windows OS patch | 01/03/2018 | [1](https://blogs.windows.com/msedgedev/2018/01/03/speculative-execution-mitigations-microsoft-edge-internet-explorer/) |
| Safari on macOS High Sierra 10.13.2 | 11.0.2+ / 11.0.2 (13604.4.7.1.6) or 11.0.2 (13604.4.7.10.6)| 01/08/2018 | [1](https://support.apple.com/en-us/HT208397) |
| Safari on OS X El Capitan 10.11.6 and macOS Sierra 10.12.6 | 11.0.2+ / 11.0.2 (11604.4.7.1.6) on OS X El Capitan 10.11.6 and 11.0.2 (12604.4.7.1.6) on macOS Sierra 10.12.6 | 01/08/2018| [1](https://support.apple.com/en-us/HT208403) |

###### Virtualization

| Product | Version | Release Date | Links |
| --- | --- | --- | --- |
| Citrix XenServer | | | [1](https://support.citrix.com/article/CTX231390) |
| QEMU | 2.11.1 | | [1](https://www.qemu.org/2018/01/04/spectre/) |
| VMware ESXi 6.5 | ESXi650-201712101-SG | 12/19/2017 | [1](https://www.vnware.com/us/security/advisories/VMSA-2018-0002.html),[2](https://kb.vmware.com/s/article/2151102),[3](https://kb.vmware.com/s/article/2151099) |
| VMware ESXi 6.0 | ESXi600-201711101-SG | 11/09/2017 | [1](https://www.vnware.com/us/security/advisories/VMSA-2018-0002.html),[2](https://kb.vmware.com/s/article/2151126),[3](http://kb.vmware.com/kb/2151132) |
| VMware ESXi 5.5 | ESXi550-201709101-SG | 09/14/2017| [1](https://www.vnware.com/us/security/advisories/VMSA-2018-0002.html),[2](https://kb.vmware.com/s/article/2150882),[3](https://kb.vmware.com/s/article/2150876) |
| VMware Fusion 8 / Fusion Pro 8 | 8.5.9+ | 01/03/2018 | [1](https://www.vnware.com/us/security/advisories/VMSA-2018-0002.html) |
| VMware Workstation Pro 12 / Workstation Player 12 | 12.5.8+ | 01/03/2018 | [1](https://www.vnware.com/us/security/advisories/VMSA-2018-0002.html) |
| Xen Project | | | [1](https://xenbits.xen.org/xsa/advisory-254.html) |

###### Other

### Devices

## References
*

## License
See [LICENSE](./LICENSE.md).

## Disclaimer
See [DISCLAIMER](./DISCLAIMER.md).
