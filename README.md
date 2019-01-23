
# Hardware and Firmware Security Guidance
## Table of Contents
- 1\. [About this repository](#about)
- 2\. [Side-channel attacks](#scvuln)
	- 2.1 [Mitigations](#mitigations)
		- 2.1.1 [Firmware patches](#firmpatch)
		- 2.1.2 [Software patches](#softpatch)
		- 2.1.3 [Configuration changes](#config)
		- 2.1.4 [Verification](#verify)
	- 2.2 [Affected products](#products)
		- 2.2.1 [Hardware resources](#hardresources)
		- 2.2.2 [Software resources](#softresources)
		- 2.2.3 [Advisory resources](#advisories)
	- 2.3 [Publicized attacks](#attacks)
		- 2.3.1 [Spectre](#spectre)
		- 2.3.2 [Meltdown](#meltdown)
		- 2.3.3 [Foreshadow](#foreshadow)
		- 2.3.4 [BranchScope](#branchscope)
		- 2.3.5 [TLBleed](#tlbleed)
		- 2.3.6 [PortSmash](#portsmash)
		- 2.3.7 [NetSpectre](#netspectre)
- 3\. [Firmware and microcode vulnerabilities](#firmandmicro)
	- 3.1 [LoJax](#lojax)
	- 3.2 [Ryzenfall, Chimera, Fallout, and Masterkey](#amdflaws)
- 4\. [Boot configuration](#bootconfig)
	- 4.1 [UEFI Hardening](#uefihard)
	- 4.2 [UEFI Secure Boot Customization](#sbcustom)
- 5\. [License](#license)
- 6\. [Contributing](#contributing)
- 7\. [Disclaimer](#disclaimer)

## <a name="about"/>1. About this respository
This repository provides content for aiding DoD administrators in verifying systems have applied and enabled mitigations for hardware and firmware vulnerabilities such as side-channel and UEFI vulnerabilities. The repository is a companion to NSA Cybersecurity Advisories such as [Vulnerabilities Affecting Modern Processors](https://www.nsa.gov/Portals/70/documents/what-we-do/cybersecurity/professional-resources/csa-vulnerabilities-affecting-modern-processors.pdf). This repository is updated as new information, research, strategies, and guidance are developed.

## <a name="scvuln"/>2. Side-channel vulnerabilities
### <a name="mitigations"/>2.1 Mitigations
The following mitigations **generally** apply to all systems. For specific steps for a particular operating system or vendor product, consult detailed instructions and strategies at [Specific Guidance](./guidance).
#### <a name="firmpatch"/>2.1.1 Firmware updates
Apply firmware updates provided by system vendors. Updates may specifically refer to UEFI, BIOS, microcode, ucode, or individual hardware device firmware. Multiple separate firmware updates may be available for a given system, or all updates may be rolled up into a single package (no universal standard exists). Firmware updates may not be delivered through established patching services such as Windows Update and are easy to miss. Consult vendor resources such as those provided by Dell and HP.

#### <a name="softpatch"/>2.1.2 Software patches
Microsoft, Apple, Red Hat, and Google have all released patches for their respective operating systems. Some software vendors -- particularly web browsers, document readers, and development kits -- may also have side-channel attack mitigation updates. Apply all patches to software. Be sure to check software that use vendor-specific update services or patch files -- not all vendors provide patches for operating system vendors to utilize.

#### <a name="config"/>2.1.3 Configuration changes
Some updates may require configuration changes to enable the full benefit of side-channel attack mitigations. Development kits in particular may require re-compilation of binaries to enable new CPU instructions. Operating systems may need changes made to user and network policies. Consult vendor resources for guidance on applying the appropriate mitigations for your use case.

#### <a name="verify"/>2.1.4 Verification
To test that patches are successful see the section named [Verification](./verification).

### <a name="products"/>2.2 Affected products
Assume that all processor products from all processor manufacturers ([Intel](https://www.intel.com/content/www/us/en/security-center/advisory/intel-sa-00088.html), AMD, [ARM](https://developer.arm.com/support/arm-security-updates/speculative-processor-vulnerability), [IBM](https://www.ibm.com/blogs/psirt/potential-impact-processors-power-family/), Apple, Samsung, [Nvidia](https://nvidia.custhelp.com/app/answers/detail/a_id/4611/~/security-bulletin%3A-nvidia-driver-security-updates-for-cpu-speculative-side), Qualcomm, etc.) are affected by one or more side-channel vulnerabilities. Attempts have been made to quantify which [specific processors](https://www.techarp.com/guides/complete-meltdown-spectre-cpu-list/) are affected by a given attack or its variations. However, the listing of products continues to grow as more researchers put resources towards expanding the scope of analyzed products. In general, the more market share a company has, the more likely their products have discovered side-channel attacks with names and CVEs.

Processor vendor exposure to side-channel attacks varies. For example, Spectre affects nearly all processor products to some degree while Meltdown primarily affects Intel products.  As of January 23, 2018, no hardware vendor has confirmed general availability of in-silicon fixes to side-channel attacks their respective products are vulnerable to. Replacing older hardware with newer hardware does not guarantee mitigation of all vulnerabilities. However,  newer hardware features updated instructions that lessen the performance impact of patches.

#### <a name="hardresources"/>2.2.1 Hardware resources
- [AMD](https://www.amd.com/en/corporate/security-updates)
- [ARM](https://developer.arm.com/support/arm-security-updates/speculative-processor-vulnerability/latest-updates/cache-speculation-issues-update)
- [Dell](https://www.dell.com/support/contents/us/en/04/article/product-support/self-support-knowledgebase/software-and-downloads/support-for-meltdown-and-spectre)
- [Dell EMC](https://www.dell.com/support/article/us/en/04/sln308588/microprocessor-side-channel-vulnerabilities-cve-2017-5715-cve-2017-5753-cve-2017-5754-impact-on-dell-emc-servers-storage-and-networking?lang=en)
- [HP](https://support.hp.com/us-en/document/c05869091)
- [HPE](https://support.hpe.com/hpsc/doc/public/display?docId=emr_na-a00039267en_us)
- IBM [Power Processors](https://www.ibm.com/blogs/psirt/potential-impact-processors-power-family/) and [Servers](https://www.ibm.com/blogs/psirt/ibm-storage-meltdownspectre/)
- [Intel](https://www.intel.com/content/www/us/en/architecture-and-technology/facts-about-side-channel-analysis-and-intel-products.html)
- [Nvidia](https://nvidia.custhelp.com/app/answers/detail/a_id/4611/~/security-bulletin%3A-nvidia-driver-security-updates-for-cpu-speculative-side)

#### <a name="softresources"/>2.2.2 Software Resources
- [Apple](https://support.apple.com/en-us/HT208394)
- Google [Cloud](https://cloud.google.com/security/cpu-vulnerabilities/), [Developers](https://developers.google.com/web/updates/2018/02/meltdown-spectre), [Chrome OS](https://www.chromium.org/chromium-os/meltdown-spectre-vulnerability-status), and [Project Zero](https://googleprojectzero.blogspot.com/)
- [IBM Cloud](https://www.ibm.com/blogs/psirt/ibm-cloud-security-bulletin-spectre-meltdown-vulnerabilities/)
- Microsoft [Windows](https://support.microsoft.com/en-us/help/4073757/protect-your-windows-devices-against-spectre-meltdown) and [Azure](https://docs.microsoft.com/en-us/azure/virtual-machines/windows/mitigate-se)
- [Red Hat](https://access.redhat.com/security/vulnerabilities/speculativeexecution)

#### <a name="advisories"/>2.2.3 Advisory resources
- [CERT/CC at Carnegie Mellon University](https://vuls.cert.org/confluence/display/Wiki/Vulnerabilities+Associated+with+CPU+Speculative+Execution)
- [Graz University of Technology](https://meltdownattack.com/)
- [NSA January 2018 advisory](https://www.nsa.gov/Portals/70/documents/what-we-do/cybersecurity/professional-resources/csa-vulnerabilities-affecting-modern-processors.pdf?v=1)
- NSA January 2019 advisory
- [US-CERT](https://www.us-cert.gov/ncas/alerts/TA18-004A)

### <a name="attacks"/>2.3 Publicized attacks
#### <a name="spectre"/>2.3.1 Spectre | CVE-2017-5715, CVE-2017-5753, CVE-2018-3639, and CVE-2018-3665
Spectre exploits performance-enhancing behaviors known as "speculative execution." Variants include SpectreNG, SpectreRSB, Spectre Prime, and Spectre followed by version numbers. Technical names include bounds check bypass, write bounds bypass, protected memory range bypass, enclave branch target injection, return stack poisoning, and network cache shadow copy.

During speculative execution a processor may continue executing down one branch of a conditional instruction rather than waiting for the result. All execution following the conditional is referred to as "speculation" since the processor does not yet know the result of conditional instruction that has not yet returned a result. Once the result is returned, the processor's guess is either determined to be correct or incorrect. A correct guess shields the user from any performance impact, delay, or lag. Incorrect speculation temporarily pollute's the processor's cache with data that should not be present and may be available for exfiltration. Alternatively, observation of processor energy use, register use, executed instructions, speed variation, and other electronic indicators can reveal secrets without needing to read cache.

For an example, assume there exists a local application that presents the user with a login prompt. Upon successful login, a secret message is displayed. An attacker can attempt to exploit Spectre vulnerabilities by deliberately entering incorrect logins and reading processor cache. The processor may speculatively load the secret message in anticipation of a correct login. The attacker can then fish the secret message out of cache despite not having login credentials to see it.

#### <a name="meltdown"/>2.3.2 Meltdown | CVE-2017-5754 and CVE-2018-3640
Meltdown builds upon Spectre. Variants include Meltdown Prime, Total Meltdown, and Meltdown followed by version numbers. Technical names include branch target injection, rogue data cache load, and rogue register load. Meltdown exploits a weakness in permission-checking during speculative execution. Some processor implementations will execute instructions prior to checking for permissions to access restricted regions of memory. Secrets can be leaked, security boundaries can be bypassed, and even virtual separation can be defeated due to the permission check that happens after speculative execution has already loaded protected data.

For an example, assume there exists a local application that features multiple services. Some of the services require the user to have administrative credentials. An attacker could launch the application, deliberately try to use a service they don't have permission to utilize, and then recover speculative execution cache data resulting from the processor's possible assumption that the user had the correct permissions. If the application happened to be a user management system, then the attacker may have the opportunity to siphon administrative credentials out of cache.

#### <a name="foreshadow"/>2.3.3 Foreshadow | CVE-2018-3615, CVE-2018-3620, and CVE-2018-3646
Foreshadow builds upon Spectre. Variants include SGXpectre and technical names include L1 terminal fault and cache overrun. Foreshadow is all about violating trust boundaries -- different from Meltdown's focus on permissions and separations. Regions of memory can be tagged to have separations enforced at the hardware level. Secure enclaves, such as Intel's SGX, share system RAM with other applications and operating systems. Hypervisors, also known as Virtual Machine Managers (VMMs), can leverage different hardware virtualization features to separate their memory from other applications and operating systems too. Foreshadow allows a process running as a user to violate the boundaries between user memory and SGX or VMM memory via speculative execution.

For an example, assume there exists a local SGX application -- effectively a black box from some trusted vendor -- that contains a decryption key. A malicious user could leverage a Foreshadow exploit to leak the decryption key out of the SGX-protected enclave. SGX enclaves typically run as user processors meaning that no elevated credentials are necessary.

#### <a name="branchscope"/>2.3.4 BranchScope | CVE-2018-9056
BranchScope builds upon Spectre. BranchScope targets the Branch Prediction Units (BPUs) that handle speculation when branching instructions, such as a conditional, are encountered. Modern processors handle multiple process threads simultaneously. BranchScope leverages processes against each other to influence and sometimes control the flow of speculative execution. Processes may prime the Branch Predictor itself to be predisposed to a specific solution, or they may corrupt the state of BPUs.

For an example, assume there exists a local application that has a login prompt and displays a different secret message based on which user has logged in. An attacker utilizing BranchScope could convince the processor to speculatively load the secret message for a specific user even though the attacker does not have their password.

#### <a name="tlbleed"/>2.3.5 TLBleed | No CVE issued
TLBleed is a Spectre-like vulnerability that can exist in some poorly-developed software. TLBleed requires a processor that feeds multiple execution threads to a single processing core. Intel brands this technology Hyper Threading, and AMD uses the term Simultaneous Multi-Threading (SMT).

One thread acts as the victim and the other acts as the attacker. The attacker can observe the victim thread's activity by accessing memory addresses in the shared-core's Transaction Lookaside Buffer (TLB). TLB entries can be used to translate between virtual and physical memory locations. The attacker can observe any memory the victim has been using including memory used to store prime numbers for cryptographic operations. Processor vendors view the fault as a problem with obsolete third-party development libraries that are not thread-safe. As a result, no CVE has been issued.

#### <a name="portsmash"/>2.3.6 PortSmash | CVE-2018-5407
PortSmash is similar to TLBleed. However, the attack thread observes the timing, cache accesses, memory accesses, energy use, and behavior of the victim thread. The attack thread does not need to access memory via the TLB. Close observation of the processor and platform hosting the victim thread can allow the attack thread to leak data from the victim thread. PortSmash is particularly effective against cryptographic functions where entropy can be observed or stealthily mirrored.

#### <a name="netspectre"/>2.3.7 NetSpectre | No CVE issued
NetSpectre is the only side-channel vulnerability listed on this page that does not require local code execution on a target system. Attackers can target systems connected to a network and flood them with data -- particularly data that takes advantage of AVX instruction sets. Processing of that data can reveal electronic indicators that allow the leaking or duplication of system secrets. NetSpectre is a noisy and slow attack method due to the large amount of data that must be sent to the target machine. An exploitable network driver, network service, or network application, such as a web browser, is also required on the target machine.

## <a name="firmandmicro"/>3. Firmware and microcode vulnerabilities
### <a name="lojax"/>3.1 LoJax
LoJax is a malicious modification to the anti-theft solution known as Computrace or LoJack. Each of these applications exist as UEFI modules implanted into system firmware. Firmware does not get cleared when the operating system is reformatted or storage media is replaced. Such persistence enables anti-theft solutions to continue to function despite attempts to disable them.

Computrace, LoJack, and other verified and trusted anti-theft solutions are provided with UEFI Secure Boot signatures to allow execution. LoJax features an invalid signature that is not trusted by Secure Boot. When Secure Boot is enabled and functioning, LoJax is unable to execute. Note that Secure Boot does not remove LoJax -- only a firmware update can do so.

To mitigate LoJax, ensure that UEFI Secure Boot is enabled and functioning. Standard mode is sufficient. Advanced organizations can also utilize custom mode.

### <a name="amdflaws"/>3.2 Ryzenfall, Chimera, Fallout, and Masterkey
Together, these four named attacks constitute what is publicized as "AMD Flaws" and over a [dozen vulnerabilities](https://community.amd.com/community/amd-corporate/blog/2018/03/21/initial-amd-technical-assessment-of-cts-labs-research). Many vulnerabilities assume the compromise of administrator credentials or completely inept software-vetting processes. Some of the vulnerabilities are a direct result of debug features left enabled for use in advanced system tweaking common in the overclocking and gaming communities.

To mitigate AMD Flaws, purchase business-class machines that lack "gamer" features such as overclocking, fan control, custom thermal management, RGB lighting, and firmware modding support. Also ensure that all firmware, microcode, and software updates are applied. Carefully analyze software before using it in conjunction with the AMD Secure Processor (SP) or Platform Security Processor (PSP) protected enclaves.

## <a name="bootconfig"/>4. Boot configuration

### <a name="uefihard"/>4.1 UEFI Hardening
See [UEFI Defensive Practices Guidance](https://www.nsa.gov/Portals/70/documents/what-we-do/cybersecurity/professional-resources/ctr-uefi-defensive-practices-guidance.pdf?ver=2018-11-06-074836-090) technical report.

### <a name="sbcustom"/>4.2 UEFI Secure Boot Customization
See UEFI Secure Boot Customization technical report.

## <a name="license"/>5. License
See [LICENSE](./LICENSE.md).

## <a name="contributing"/>6. Contributing
See [CONTRIBUTING](./CONTRIBUTING.md)

## <a name="disclaimer"/>7. Disclaimer
See [DISCLAIMER](./DISCLAIMER.md).
