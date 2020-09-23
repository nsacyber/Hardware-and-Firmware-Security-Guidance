  
# Hardware and Firmware Security Guidance
## Table of Contents
- 1\. [About this repository](#about)
- 2\. [Side-channel attacks](#scvuln)
	- 2.1 [Mitigations](#mitigations)
		- 2.1.1 [Firmware patches](#firmpatch)
		- 2.1.2 [Software patches](#softpatch)
		- 2.1.3 [Configuration changes](#config)
		- 2.1.4 [Disable Intel Hyper-Threading](#hyper) (Updated!)
		- 2.1.5 [Verification](#verify)
	- 2.2 [Resources and Affected products](#products)
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
		- 2.3.8 [Microarchitectural Data Sampling (MDS)](#MDS)
		- 2.3.9 [CacheOut](#CacheOut) (New!)
		- 2.3.10 [Take A Way](#TAW) (New!)
		- 2.3.11 [Load Value Injection](#LVI) (New!)
- 3\. [Firmware and microcode vulnerabilities](#firmandmicro)
	- 3.1 [LoJax](#lojax)
	- 3.2 [Ryzenfall, Chimera, Fallout, and Masterkey](#amdflaws)
	- 3.3 [Microsoft Secure Boot Bypass](#MSFTSBB) (New!)
- 4\. [Boot configuration](#bootconfig)
	- 4.1 [UEFI Hardening](#uefihard)
	- 4.2 [UEFI Secure Boot Customization](#sbcustom) (Updated!)
- 5\. [Hardware Upgrade Guidance](#upgrade) (New!)
- 6\. [License](#license)
- 7\. [Contributing](#contributing)
- 8\. [Disclaimer](#disclaimer)

## <a name="about"/>1. About this respository
This repository provides content for aiding DoD administrators in verifying systems have applied and enabled mitigations for hardware and firmware vulnerabilities such as side-channel and UEFI vulnerabilities. The repository is a companion to NSA Cybersecurity Advisories such as [Vulnerabilities Affecting Modern Processors](https://www.nsa.gov/Portals/70/documents/what-we-do/cybersecurity/professional-resources/csa-vulnerabilities-affecting-modern-processors.pdf). This repository is updated as new information, research, strategies, and guidance are developed.

## <a name="scvuln"/>2. Side-channel vulnerabilities
### <a name="mitigations"/>2.1 Mitigations
The following mitigations **generally** apply to all systems. For specific steps for a particular operating system or vendor product, consult detailed instructions and strategies at [Specific Guidance](./guidance).
#### <a name="firmpatch"/>2.1.1 Firmware updates
Apply firmware updates provided by system vendors. Updates may specifically refer to UEFI, BIOS, microcode, ucode, or individual hardware device firmware. Multiple separate firmware updates may be available for a given system, or all updates may be rolled up into a single package (no universal standard exists). Firmware updates may not be delivered through established patching services such as Windows Update and are easy to miss. Consult vendor resources such as those provided by Dell and HP.

Ensure that firmware updates are allowed to execute uninterrupted. Firmware updates may have multiple phases. For example, firmware updates for Intel platforms typically have a distinct phase for updating the Management Engine (ME) that follows platform firmware update. Reboots may exist between phases.

#### <a name="softpatch"/>2.1.2 Software patches
Microsoft, Apple, Red Hat, and Google have all released patches for their respective operating systems. Some software vendors -- particularly web browsers, document readers, and development kits -- may also have side-channel attack mitigation updates. Apply all patches to software. Be sure to check software that use vendor-specific update services or patch files -- not all vendors provide patches for operating system vendors to utilize.

VMware, Citrix, Xen Project, Amazon, Microsoft, and other virtualization and cloud providers have also provided side-channel attack mitigation patches. Some infrastructures require system administrators to apply patches, and others -- typically cloud infrastructures -- automatically apply patches without any input.

**Some OS vendors have released patches with disabled mitigations due to performance impact concerns.** Perform a risk analysis to determine the infrastructure's exposure to arbitrary executable and script execution. Apply mitigations to endpoints most exposed and at risk. Check to ensure Spectre, Meltdown, MDS, and other mitigations are actually enforced. Application and script whitelisting combined with endpoint permissions lockdown may be sufficient alternatives to performance-impacting mitigations.

#### <a name="config"/>2.1.3 Configuration changes
Some updates may require configuration changes to enable the full benefit of side-channel attack mitigations. Development kits in particular may require re-compilation of binaries to enable new CPU instructions. Operating systems may need changes made to user and network policies. Consult vendor resources for guidance on applying the appropriate mitigations for your use case.

#### <a name="hyper">2.1.4 Disable Intel Hyper-Threading on Sensitive Systems
Disable Hyper-Threading on systems that match ALL of the following requirements:

1. Intel systems handling sensitive information (e.g. classified, financial, medical)
2. Intel systems that allow the execution of arbitrary programs and scripts (e.g. systems lacking application whitelisting)
3. Intel systems that are part of the 9th generation or older

All form factors are affected (e.g. desktops, servers, notebooks, tablets). Disabling Hyper-Threading may impart a significant performance penalty on some use cases. Virtual processor and Simultaneous MultiThreading (SMT) solutions from vendors other than Intel are not implicated as susceptible to MDS as of March, 2020.

#### <a name="verify"/>2.1.5 Verification
To test that patches are successful see the section named [Verification](./verification).

### <a name="products"/>2.2 Resources and Affected products
Assume that all processor products from all processor manufacturers ([Intel](https://www.intel.com/content/www/us/en/security-center/advisory/intel-sa-00088.html), AMD, [ARM](https://developer.arm.com/support/arm-security-updates/speculative-processor-vulnerability), [IBM](https://www.ibm.com/blogs/psirt/potential-impact-processors-power-family/), Apple, Samsung, [Nvidia](https://nvidia.custhelp.com/app/answers/detail/a_id/4611/~/security-bulletin%3A-nvidia-driver-security-updates-for-cpu-speculative-side), Qualcomm, etc.) are affected by one or more side-channel vulnerabilities. Attempts have been made to quantify which [specific processors](https://www.techarp.com/guides/complete-meltdown-spectre-cpu-list/) are affected by a given attack or its variations. However, the listing of products continues to grow as more researchers put resources towards expanding the scope of analyzed products. In general, the more market share a company has, the more likely their products have discovered side-channel attacks with names and CVEs.

Processor vendor exposure to side-channel attacks varies. For example, Spectre affects nearly all processor products to some degree; Meltdown and Microarchitectural Data Sampling (MDS) primarily affect Intel products. Take A Way only affects AMD products. Replacing older hardware with newer hardware does not guarantee mitigation of all side-channel vulnerabilities. However, newer hardware features updated instructions that lessen the performance impact of patches.

As of March, 2020, processor vendor exposure to side-channel attacks still varies. In general, older CPUs have more vulnerabilities and are affected more adversely by mitigations than newer CPUs. However, new CPUs have proven to still have vulnerabilities as security researchers continue to discover clever ways to break security boundaries and even defeat recent mitigation efforts. Look for products that specifically boast side-channel mitigation and performance benefits over previous generations.

NSA does not have the mission to test every processor released. Researchers, product vendors, and [tech websites](https://www.techarp.com/guides/complete-meltdown-spectre-cpu-list/) have compiled lists of affected products.

#### <a name="hardresources"/>2.2.1 Hardware resources
- [AMD](https://www.amd.com/en/corporate/security-updates)
- [ARM](https://developer.arm.com/support/arm-security-updates/speculative-processor-vulnerability/latest-updates/cache-speculation-issues-update)
- [Dell](https://www.dell.com/support/contents/us/en/04/article/product-support/self-support-knowledgebase/software-and-downloads/support-for-meltdown-and-spectre)
- Dell EMC [Spectre, Meltdown](https://www.dell.com/support/article/us/en/04/sln308588/microprocessor-side-channel-vulnerabilities-cve-2017-5715-cve-2017-5753-cve-2017-5754-impact-on-dell-emc-servers-storage-and-networking?lang=en) and [MDS](https://www.dell.com/support/article/us/en/04/sln317156/dell-emc-server-security-notice-for-intel-sa-00233?lang=en)
- [HP](https://support.hp.com/us-en/document/c05869091)
- HPE [Spectre, Meltdown](https://support.hpe.com/hpsc/doc/public/display?docId=emr_na-a00039267en_us) and [MDS](https://support.hpe.com/hpsc/doc/public/display?docLocale=en_US&docId=emr_na-hpesbhf03933en_us)
- IBM [Power Processors](https://www.ibm.com/blogs/psirt/potential-impact-processors-power-family/) and [Servers](https://www.ibm.com/blogs/psirt/ibm-storage-meltdownspectre/)
- Intel [Spectre, Meltdown, and more](https://www.intel.com/content/www/us/en/architecture-and-technology/facts-about-side-channel-analysis-and-intel-products.html) plus [MDS-specific update](https://www.intel.com/content/www/us/en/architecture-and-technology/mds.html)
- [Nvidia](https://nvidia.custhelp.com/app/answers/detail/a_id/4611/~/security-bulletin%3A-nvidia-driver-security-updates-for-cpu-speculative-side)

#### <a name="softresources"/>2.2.2 Software Resources
- Apple [Spectre, Meltdown](https://support.apple.com/en-us/HT208394) and [MDS](https://support.apple.com/en-us/HT210107)
- Google [Cloud](https://cloud.google.com/security/cpu-vulnerabilities/), [Developers](https://developers.google.com/web/updates/2018/02/meltdown-spectre), [Chrome OS](https://www.chromium.org/chromium-os/meltdown-spectre-vulnerability-status), [Project Zero](https://googleprojectzero.blogspot.com/), and [MDS update](https://support.google.com/faqs/answer/9330250)
- [IBM Cloud](https://www.ibm.com/blogs/psirt/ibm-cloud-security-bulletin-spectre-meltdown-vulnerabilities/)
- Microsoft [Windows](https://support.microsoft.com/en-us/help/4073757/protect-your-windows-devices-against-spectre-meltdown) and [Azure](https://docs.microsoft.com/en-us/azure/virtual-machines/windows/mitigate-se) and [MDS update](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV190013)
- Red Hat [Spectre, Meltdown](https://access.redhat.com/security/vulnerabilities/speculativeexecution) and [MDS update](https://access.redhat.com/security/vulnerabilities/mds)
- VMware [MDS update](https://kb.vmware.com/s/article/67577)
- Xen Project [MDS Update](https://xenbits.xen.org/xsa/advisory-297.html)

#### <a name="advisories"/>2.2.3 Advisory resources
- [CacheOut vulnerability information](https://cacheoutattack.com/)
- [CERT/CC at Carnegie Mellon University](https://vuls.cert.org/confluence/display/Wiki/Vulnerabilities+Associated+with+CPU+Speculative+Execution)
- Graz University of Technology [Spectre, Meltdown](https://meltdownattack.com/) [MDS (AKA ZombieLoad)](https://zombieloadattack.com/), [Take A Way](https://mlq.me/download/takeaway.pdf), and [LVI](https://lviattack.eu/)
- [NSA January 2018 advisory](https://www.nsa.gov/Portals/70/documents/what-we-do/cybersecurity/professional-resources/csa-vulnerabilities-affecting-modern-processors.pdf?v=1)
- [NSA January 2019 advisory](https://www.nsa.gov/Portals/70/documents/what-we-do/cybersecurity/professional-resources/CSA_Updated_Guidance_For_Vulnerabilities_Affecting_Modern_Processors_20190130.pdf)
- [NSA May 2019 advisory](https://www.nsa.gov/Portals/70/documents/what-we-do/cybersecurity/professional-resources/CSA-Guidance_For_Vulnerabilities_Affecting_Modern_Processors_UPDATE2.pdf)
- [US-CERT](https://www.us-cert.gov/ncas/alerts/TA18-004A)
- Vrije Universiteit Amsterdam [MDS, Fallout, RIDL](https://mdsattacks.com/)

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

#### <a name="MDS"/>2.3.8 Microarchitectural Data Sampling (MDS) | CVE-2018-12130, CVE-2018-12127, CVE-2018-12126, and CVE-2019-11091
Microarchitectural Data Sampling (MDS) vulnerabilities are also referred to as ZombieLoad, Fallout, and Rogue In-flight Data Load (RIDL). MDS vulnerabilities expand upon weaknesses identified in Meltdown and TLBleed. Mitigations have focused on controlling access and frequently clearing processor caches. However, processor cache data is staged in buffers when transiting into and out of cache memory space. MDS exploits target the buffers. Information can be leaked from or injected into buffers to manipulate processor execution and speculative behaviors.

#### 2.3.9 <a name="CacheOut" />CacheOut | _CVE_-2020-0548 | _CVE_-2020-0549
CacheOut expands upon MDS and RIDL vulnerabilities and targets Intel platforms. Researchers introduce new mechanisms for accessing data across all security boundaries and in the presence of recent mitigations. New microcode and kernel patches are expected in the first half of 2020 to address CacheOut vulnerabilities.

#### 2.3.10 <a name="TAW" />Take A Way (TAW)
Take A Way (TAW) targets AMD platforms with a new class of side-channel vulnerabilities. TAW involves manipulating power management features (specifically Cache Way Predictors), defeating Address Space Layout Layout Randomization (ASLR), and leveraging Spectre-like attacks to covertly access unauthorized memory. Successful attacks can be carried out via JavaScript found on a malicious website. However, the attack is slow to leak data, only captures incomplete snippets of data structures, and can readily be mitigated via software patches.

#### 2.3.11 <a name="LVI" />Load Value Injection (LVI) | CVE-2020-0551
Load Value Injection (LVI) expands upon concepts introduced by Spectre, Meltdown, and MDS. LVI targets Intel platforms. Instead of leaking data through side-effects, LVI takes a more direct approach by having the CPU volunteer data as a result of carefully placed instructions that are triggered in sequence by fault and load commands. The result is a type of undetectable, covert channel that bypasses existing side-channel mitigations and violates the boundaries of security enclaves -- specifically Intel SGX.

Mitigation measures involve the recompilation and redeployment of SGX-enabled applications. Speculation and control flow are also more tightly controlled post-mitigation. A significant performance penalty is expected.

## <a name="firmandmicro"/>3. Firmware and microcode vulnerabilities
### <a name="lojax"/>3.1 LoJax
LoJax is a malicious modification to the anti-theft solution known as Computrace or LoJack. Each of these applications exist as UEFI modules implanted into system firmware. Firmware does not get cleared when the operating system is reformatted or storage media is replaced. Such persistence enables anti-theft solutions to continue to function despite attempts to disable them.

Computrace, LoJack, and other verified and trusted anti-theft solutions are provided with UEFI Secure Boot signatures to allow execution. LoJax features an invalid signature. Note that Secure Boot does not remove LoJax -- only a firmware write can do so. To check for the presence of or vulnerability to LoJax on a specific endpoint, isolate the machine and use [CHIPSEC](https://github.com/chipsec/chipsec) (highly privileged driver not appropriate for production environments).

LoJax mitigation depends on platform and configuration. Secure Boot is sufficient to prevent LoJax execution at boot time on most server systems. Both standard mode and custom mode will block LoJax. However, security researchers have discovered that common business and consumer workstations skip firmware signature checks especially when placed into fast boot or minimal boot check mode. Use the thorough boot option and consult with the system vendor to determine the correct set of UEFI options and system firmware version to minimize the risk of LoJax.

### <a name="amdflaws"/>3.2 Ryzenfall, Chimera, Fallout, and Masterkey
Together, these four named attacks constitute what is publicized as "AMD Flaws" and over a [dozen vulnerabilities](https://community.amd.com/community/amd-corporate/blog/2018/03/21/initial-amd-technical-assessment-of-cts-labs-research). Many vulnerabilities assume the compromise of administrator credentials or completely inept software-vetting processes. Some of the vulnerabilities are a direct result of debug features left enabled for use in advanced system tweaking common in the overclocking and gaming communities.

To mitigate AMD Flaws, purchase business-class machines that lack or limit enthusiast features such as overclocking, fan control, custom thermal management, RGB lighting, and firmware modding support. Also ensure that all firmware, microcode, and software updates are applied. Carefully analyze software before using it in conjunction with the AMD Secure Processor (SP) or Platform Security Processor (PSP) protected enclaves.

### <a name="MSFTSBB" />Microsoft Secure Boot Bypass | CVE-2020-0689
Microsoft plays a prominent role in the assigning of UEFI Secure Boot signatures. Most modern machines ship with a Microsoft Windows Key Exchange Key (KEK) and a Microsoft Third-Party UEFI Marketplace KEK. Sometimes signatures are issued to bootable binaries by mistake. Rather than revoke the KEK and invalidate thousands of products, Microsoft can issue a Blacklist Database (DBX) hash for a specific signed binary.

Microsoft's patch KB4524244 issues a DBX record for a bootloader with the ability to bypass UEFI Secure Boot Protections. Initial boot firmware begins the Secure Boot process. After the Boot Device Select (BDS) phase of UEFI boot, execution control and responsibility for Secure Boot enforcement transfers to the software environment -- specifically the bootloader. The bootloader identified by Microsoft's DBX update is known to ignore Secure Boot and break a chain of trust that should extend to Microsoft's kernel. However, some endpoints have had difficulty applying the DBX update record which has caused Microsoft to pull back KB4524244.

To mitigate the threat of the dangerous bootloader, add the following SHA-256 hash to each system's DBX records if it is not already present:

> 81d8fb4c9e2e7a8225656b4b8273b7cba4b03ef2e9eb20e0a0291624eca1ba86

Future Microsoft software patches and system vendor firmware patches may also add the hash to the DBX. NSA is working to identify the ideal contents of DBX and share information to help partners maintain and customize UEFI Secure Boot. See [UEFI Secure Boot Customization](#sbcustom).

## <a name="bootconfig"/>4. Boot configuration

### <a name="uefihard"/>4.1 UEFI Hardening
See [UEFI Defensive Practices Guidance](https://www.nsa.gov/Portals/70/documents/what-we-do/cybersecurity/professional-resources/ctr-uefi-defensive-practices-guidance.pdf?ver=2018-11-06-074836-090) technical report.

### <a name="sbcustom"/>4.2 UEFI Secure Boot Customization
Check out our recent report on [UEFI Secure Boot Customization](https://www.nsa.gov/news-features/press-room/Article/2347822/nsa-releases-cybersecurity-technical-report-on-uefi-secure-boot-customization/). The report is the result of numerous prototypes and lessons learned. While not intended as a response to BootHole, the Customization report does provide commands and strategies to mitigate hypothetical boot-time threats of which BootHole is a real example.

See the [UEFI Secure Boot Customization](./secureboot.md) section for similar info as the report, but presented in a manner more focused on researchers, administrators, and system integrators.

## <a name="upgrade" />5. Hardware Upgrade Guidance

NSA does not endorse or promote specific products. See the [National Information Assurance Partnership (NIAP)](https://www.niap-ccevs.org/) for specific products that have been vetted for compliance to protection profiles and applicable standards. NSA does have the following generic recommendations:
1. [Refresh workstations every 3 to 4 years and servers every 5 to 7 years](https://media.defense.gov/2019/Sep/09/2002180345/-1/-1/0/LEVERAGE%20MODERN%20HARDWARE%20SECURITY%20FEATURES_20190821.PDF)
2. Look for processors that carry Intel's vPro branding or AMD's PRO branding or that the ARM licensee advertises side-channel mitigations
3. Upgrading to the latest generation reduces the impact of performance penalties incurred by some side-channel vulnerability mitigations. However, total protection from side-channel vulnerabilities is unlikely given the evolving threat landscape. Choose the vendor solution that provides the best protection at the time of purchase.
4. Practice defense-in-depth by layering hardware, firmware, software, and run-time monitoring endpoint security solutions.

## <a name="license"/>6. License
See [LICENSE](./LICENSE.md).

## <a name="contributing"/>7. Contributing
See [CONTRIBUTING](./CONTRIBUTING.md)

## <a name="disclaimer"/>8. Disclaimer
See [DISCLAIMER](./DISCLAIMER.md).
