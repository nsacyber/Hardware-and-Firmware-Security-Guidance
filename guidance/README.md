
# Specific Guidance
Specific guidance is intended to focus on products and solutions **commonly** found in government and industry spaces. This page exists to gather vendor resources in one spot so public and private entities can collaborate on our [shared need to secure computing infrastructure](https://www.nsa.gov/What-We-Do/Cybersecurity/NSCAP/).

The information and opinions contained in this document are provided "as is" and without any warranties or guarantees. **Reference herein to any specific commercial products, process, or service by trade name, trademark, manufacturer, or otherwise, does not constitute or imply its endorsement, recommendation, or favoring by the United States Government, and this guidance shall not be used for advertising or product endorsement purposes.**

## Table of Contents
- 1\. [Attack Vector Minimization](#vectors)
- 2\. [Patching Priority](#patching)
- 3\. [Operating Systems](#os)
	- 3.1 [Windows](#win)
	- 3.2 [Red Hat Enterprise Linux (RHEL)](#rhel)
	- 3.3 [Other Linux Distributions](#linux)
	- 3.4 [MacOS](#mac)
- 4\. [Hypervisors](#hyper)
	- 4.1 [Citrix](#citrix)
	- 4.2 [Microsoft Hyper-V](#hyperv)
	- 4.3 [VMware](#vmware)
	- 4.4 [Xen Project](#xen)
- 5\. [Hardware](#hard)
	- 5.1 [Dell](#dell)
	- 5.2 [HP](#hp)
	- 5.3 [Intel](#intel)
	- 5.4 [AMD](#amd)
- 6\. [General Information](#general)

## <a name="vectors"/>1. Attack Vector Minimization
Most exploits against Spectre, Meltdown, MDS, and other side-channel vulnerabilities originate from malicious code locally executed on a machine or virtual machine. Some variants require administrative privileges while others can be executed as user level, browser-downloaded JavaScript code. Machines that execute arbitrary code -- i.e. non-whitelisted user applications and scripts -- are at greatest risk. Side-channel attacks are likely to reveal credentials that enable lateral movement within an infrastructure.

To minimize attack vectors, consider the following solutions:
1. **Application whitelisting** applies a "known-good" filter to executable software on an endpoint. Chance of malicious code execution is significantly reduced.
2. **Trusted scripts** applies a "known-good" filter to executable scripts. System utilities, user utilities, and web browser scripts may all be corralled by scripting restrictions. Alternatively, consider disabling user scripts entirely.
3. **Audit new software and firmware** before introduction to an infrastructure. Perform malware scans and check provided code and documentation to the extent possible. Validate that the hash of received binaries matches the hash of what the software and firmware vendors intended to provide.
4. **Separate network infrastructures where appropriate** to limit the lateral movement of attackers. Physical and virtual solutions can prevent unfettered access to sensitive resources.

##  <a name="patching"/>2. Patching Priority
General guidance for prioritizing patching:

1. Prioritize patching software applications, such as browsers first, as they are the easiest to patch, have the least amount of issues with performance and compatibility, and the most likely widespread attack vector.
3. Prioritize installing operating system patches on desktop, laptops, and tablets -- especially systems that travel or leave the office environment. Compatibility issues with operating system patches have been largely resolved by the OS vendors and performance issues are much less on desktops since they typically do not have IO intensive workloads like servers (file storage arrays, email servers, database servers) where the majority of the performance issues are excertbated. Attacks via email, Office documents, PDFs, are the second most likely widespread attack vector.
4. Prioritize patching servers that do NOT have IO intensive workloads (no file storage arrays, no email servers, no database servers). Some organizations may want to wait on patching any servers until more performance data is available or more localized testing has been performed to determine if the risk of remaining unpatched is warranted for the performance trade offs. Attack surface reduction may be an acceptable alternative to performance-impacting patches depending on mission and use case.

## <a name="os"/>3. Operating Systems

### <a name="win" /> 3.1 Windows
* [Microsoft's MDS advisory](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/adv190013)
* [Microsoft's Spectre and Meltdown advisory](https://portal.msrc.microsoft.com/en-US/security-guidance/advisory/ADV180002)
* [Windows endpoint patching and configuration guidance](https://support.microsoft.com/en-us/help/4073119/protect-against-speculative-execution-side-channel-vulnerabilities-in)
* [Windows C++ developer and coding guidance](https://docs.microsoft.com/en-us/cpp/security/developer-guidance-speculative-execution?view=vs-2019)
* [Get-SpeculationControlSettings PowerShell script](https://support.microsoft.com/en-us/help/4074629/understanding-the-output-of-get-speculationcontrolsettings-powershell) for checking an endpoint's vulnerability to multiple side-channel exploits

Microsoft provides patches through the Windows Update service. Windows Update normally automatically selects appropriate security patches.
* [MDS patches are part of the May 2019 security updates](https://www.catalog.update.microsoft.com/Search.aspx?q=2019-05)
* MDS-mitigating microcode updates for Intel CPUs are not yet available and expected summer 2019
* [Spectre patches are part of the June and November 2018 security updates](https://portal.msrc.microsoft.com/en-US/security-guidance/advisory/ADV180012) and may also **require administrator action to enable** due to concerns regarding performance impacts
* [Meltdown patches are part of the June and November 2018 security updates](https://support.microsoft.com/en-us/help/4072698/windows-server-speculative-execution-side-channel-vulnerabilities-prot) and may also **require administrator action to enable on server systems** due to concerns regarding performance impacts

Spectre and Meltdown patches may not be activated when installed because of significant performance penalties or compatibility issues with some implementations of anti-malware solutions. The following registry keys control the behavior of mitigations:

- - -
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Memory Management\FeatureSettingsOverride

HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Memory Management\FeatureSettingsOverrideMask

HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Virtualization\MinVmVersionForCpuBasedMitigations
- - -

Use the following variable values to enable mitigations. Registry keys may not exist by default and may need to be added. The tables below select settings most commonly advised to DOD administrators. For the full variation of settings, consult Microsoft's in-depth details in the second half of [Windows Server guidance to protect against speculative execution side-channel vulnerabilities](https://support.microsoft.com/en-us/help/4072698/windows-server-speculative-execution-side-channel-vulnerabilities-prot). Intel systems only (all variables are REG_DWORD):

| Setting | Spectre, Meltdown, MDS | Spectre, Meltdown | Spectre-only |
| --- | --- | --- | --- |
| FeatureSettingsOverride | 8264 | 0 | 8 |
| FeatureSettingsOverrideMask | 3 | 3 | 3 |
| MinVmVersionForCpuBasedMitigations | "1.0" | "1.0" | "1.0" |

AMD systems only (all variables are REG_DWORD) -- Meltdown and MDS are not applicable to AMD:

| Setting | Spectre-only |
| --- | --- |
| FeatureSettingsOverride | 72 |
| FeatureSettingsOverrideMask | 3 |
| MinVmVersionForCpuBasedMitigations | "1.0"|

Windows operating systems and applications guidance in development:
* [Operating system](./windows/OS.md) patch compatibility and enabling guidance
* [Browser](./windows/Browsers.md) patching and configuration guidance
* [Hyper-V hosts](./windows/Hyper-V.md) configuration guidance

### <a name="rhel"/>3.2 Red Hat Enterprise Linux (RHEL)
* [Red Hat's MDS advisory](https://access.redhat.com/security/vulnerabilities/mds)
* [Red Hat's MDS overview](https://www.redhat.com/en/blog/understanding-mds-vulnerability-what-it-why-it-works-and-how-mitigate-it)
* [MDS mitigations in virtual environments](https://access.redhat.com/solutions/4161561)
* [Red Hat's Spectre and Meltdown overview](https://www.redhat.com/en/blog/what-are-meltdown-and-spectre-heres-what-you-need-know)
* [Red Hat's Spectre and Meltdown advisory](https://access.redhat.com/security/vulnerabilities/speculativeexecution)
* [Spectre and Meltdown patch configuration and application guidance](https://access.redhat.com/articles/3311301)
* [Spectre and Meltdown detection tool](https://access.redhat.com/labsinfo/speculativeexecution)
* [Spectre and Meltdown Virtualization-specific guidance](https://access.redhat.com/solutions/3307851)

### <a name="linux"/>3.3 Other Linux Distributions
* [Ubuntu MDS blog post](https://blog.ubuntu.com/2019/05/14/ubuntu-updates-to-mitigate-new-microarchitectural-data-sampling-mds-vulnerabilities)
* [Ubuntu Wiki for MDS](https://wiki.ubuntu.com/SecurityTeam/KnowledgeBase/MDS)
* [Ubuntu Spectre and Meltdown blog post](https://blog.ubuntu.com/2018/01/24/meltdown-spectre-and-ubuntu-what-you-need-to-know)
* [Ubuntu Spectre and Meltdown patch notice](https://blog.ubuntu.com/2018/01/04/ubuntu-updates-for-the-meltdown-spectre-vulnerabilities)
* [Ubuntu Wiki for Spectre and Meltdown](https://wiki.ubuntu.com/SecurityTeam/KnowledgeBase/SpectreAndMeltdown)
* [Side-channel vulnerability detection in Linux](./linux/README.md) guidance
* See the [Browser guidance](./windows/Browsers.md) in the Windows section (also applies to Linux)

### <a name="mac"/> 3.4 MacOS
* [Apple's MDS notice](https://support.apple.com/en-us/HT210107)
* [Apple's Spectre and Meltdown notice](https://support.apple.com/en-us/HT208394)

## <a name="hyper">4. Hypervisors
### <a name="citrix"/>4.1 Citrix
* [MDS statement](https://www.citrix.com/blogs/2019/05/14/microarchitectural-data-sampling-security-issues-and-mitigations/)
* [Spectre and Meltdown statement](https://support.citrix.com/article/CTX231399)
* [Performance impact guidance](https://www.citrix.com/blogs/2018/02/06/meltdown-and-spectre-understanding-the-performance-impact-current-state-whats-next/)
	
### <a name="hyperv"/>4.2 Microsoft Hyper-V
* [Side-channel vulnerability mitigations for multiple Microsoft platforms](https://support.microsoft.com/en-us/help/4457951/windows-guidance-to-protect-against-speculative-execution-side-channel)
* [Hyper Clear mitigation](https://techcommunity.microsoft.com/t5/Virtualization/5-14-Hyper-V-HyperClear-Update/ba-p/566499)
* [Configuration changes to mitigate side-channel vulnerabilities](https://docs.microsoft.com/en-us/virtualization/hyper-v-on-windows/CVE-2017-5715-and-hyper-v-vms)
* [Server configuration changes to mitigate side-channel vulnerabilities](https://support.microsoft.com/en-us/help/4072698/windows-server-speculative-execution-side-channel-vulnerabilities-prot)

### <a name="vmware"/>4.3 VMware
* [MDS advisory](https://www.vmware.com/security/advisories/VMSA-2019-0008.html)
* [Hypervisor mitigations for MDS](https://kb.vmware.com/s/article/67577)
* [Guest mitigations for MDS](https://kb.vmware.com/s/article/68024)
* [Spectre and Meltdown advisory](https://www.vmware.com/security/advisories/VMSA-2018-0004.html)
* [Configuration changes and microcode mitigations for Spectre, Meltdown](https://kb.vmware.com/s/article/54951)
* [VMware additional guidance on side-channel vulnerabilities](https://kb.vmware.com/s/article/52245)
* [vSphere configuration guidance](https://blogs.vmware.com/feed-items/vulnerabilities-how-to-fix-meltdown-and-spectre-on-vmware-vsphere)

### <a name="xen"/>4.4 Xen Project
* [MDS Guidance](https://www.citrix.com/blogs/2019/05/14/microarchitectural-data-sampling-security-issues-and-mitigations/)
* [Spectre and Meltdown administrator guidance](https://wiki.xenproject.org/wiki/Respond_to_Meltdown_and_Spectre)
* [Spectre and Meltdown Wiki FAQ](https://wiki.xenproject.org/wiki/Xen_Project_Meltdown_and_Spectre_Technical_FAQ)
* [Spectre and Meltdown FAQ](https://xenproject.org/2018/01/22/xen-project-spectre-meltdown-faq-jan-22-update/)
## <a name="hard"/>5. Hardware
### <a name="dell"/>5.1 Dell
* [Dell EMC Server MDS vulnerability advisory](https://www.dell.com/support/article/us/en/04/sln317156/dsa-2019-089-dell-emc-server-platform-security-advisory-for-intel-sa-00233?lang=en)
* [Spectre and Meltdown patches for Dell EMC products](https://www.dell.com/support/article/uk/en/ukbsdt1/sln308588/microprocessor-side-channel-vulnerabilities-cve-2017-5715-cve-2017-5753-cve-2017-5754-impact-on-dell-emc-servers-storage-and-networking?lang=en)
* [Spectre and Meltdown patches for Dell business and consumer products](https://www.dell.com/support/article/us/en/04/sln308587/microprocessor-side-channel-vulnerabilities-cve-2017-5715-cve-2017-5753-cve-2017-5754-impact-on-dell-products?lang=en)
* [Dell Spectre and Meltdown knowledge hub](https://www.dell.com/support/contents/us/en/04/article/product-support/self-support-knowledgebase/software-and-downloads/support-for-meltdown-and-spectre)
* [Dell Side-Channel vulnerability information hub](https://www.dell.com/support/contents/us/en/04/article/product-support/self-support-knowledgebase/software-and-downloads/speculative-store-bypass)
### <a name="hp"/>5.2 HP
* [HP Enterprise MDS vulnerability advisory](https://support.hpe.com/hpsc/doc/public/display?docId=emr_na-hpesbhf03933en_us&docLocale=en_US)
* [HP Consumer MDS vulnerability advisory](https://support.hp.com/us-en/document/c06330149)
* [HPE Spectre and Meltdown advisory](https://support.hpe.com/hpsc/doc/public/display?docLocale=en_US&docId=emr_na-a00039773en_us&withFrame)
* [HPE Spectre and Meltdown patches and configuration instructions](http://h22208.www2.hpe.com/eginfolib/securityalerts/SCFM/Side_Channel_Downloads.html)
* [HP Consumer Spectre and Meltdown advisory](https://support.hp.com/us-en/document/c05869091)
* [HP Enterprise security vulnerability dashboard](https://www.hpe.com/us/en/services/security-vulnerability.html)

### <a name="intel"/>5.3 Intel
* [MDS deep dive](https://software.intel.com/security-software-guidance/insights/deep-dive-intel-analysis-microarchitectural-data-sampling)
* [MDS advisory](https://software.intel.com/security-software-guidance/software-guidance/microarchitectural-data-sampling)
* [MDS overview](https://www.intel.com/content/www/us/en/architecture-and-technology/mds.html)
* [Side-channel vulnerability information hub](https://www.intel.com/content/www/us/en/architecture-and-technology/facts-about-side-channel-analysis-and-intel-products.html)
* [Side-channel mitigation performance impact analysis](https://www.intel.com/content/www/us/en/architecture-and-technology/l1tf.html)

As of May, 2019, NSA recommends disabling Intel Hyper-Threading on all systems handling sensitive data while able to execute arbitrary, user-acquired binaries and scripts. Endpoints with application and script whitelisting solutions may be exempt from MDS mitigations depending on mission need and data handled. Because of the wide variety of named side-channel vulnerabilities and differing impact, sometimes differing per stepping/revision in the same product generation, assume that all Core and Xeon Intel processors are vulnerable to one or more side-channel vulnerabilities.

Ensure that Spectre mitigations are installed and activated on all Intel endpoints. Like MDS, Meltdown may impart a performance impact that can be counteracted with application and user permission lockdown solutions. For information relating to a specific infrastructure, e-mail the NSA Cybersecurity GitHub team at <a href="mailto:Cybersecurity_Requests@nsa.gov?subject=NSA Cyber GitHub infrastructure question">Cybersecurity_Requests@nsa.gov</a>.

### <a name="amd"/>5.4 AMD
* [AMD security hub](https://www.amd.com/en/corporate/product-security)

As of May, 2019, NSA recommends installing Spectre mitigations on all AMD endpoints. Meltdown and MDS vulnerabilities do not appear to apply to AMD Zen products. Ensure that AMD-specific Spectre mitigations have been installed and remove erroneously applied Meltdown mitigations -- some early patches may have installed patches not intended for AMD platforms. For information relating to a specific infrastructure, e-mail the NSA Cybersecurity GitHub team at <a href="mailto:Cybersecurity_Requests@nsa.gov?subject=NSA Cyber GitHub infrastructure question">Cybersecurity_Requests@nsa.gov</a>.

### <a name="arm"/>5.5 ARM
* [Spectre and Meltdown information hub](https://developer.arm.com/support/arm-security-updates/speculative-processor-vulnerability/latest-updates/cache-speculation-issues-update)
* [Developer security updates](https://developer.arm.com/support/arm-security-updates)

Consult with the vendor of a specific ARM chip for details on side-channel vulnerability mitigations. Multiple vendors, e.g. Samsung and Qualcomm, produce ARM chips with variations that affect their vulnerability, if any.

## <a name="general"/>General information
Informational pages:
* [Patches](./Patches.md) - information on operating system, application, and firmware patches.
* [Performance](./Performance.md) - information from vendors on performance impacts of patches and mitigations.
