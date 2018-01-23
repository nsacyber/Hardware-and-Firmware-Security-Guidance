# Verification

Verification of mitigations is currently being developed for operating systems and applications.

## Windows
Verification of Windows operating systems and applications mitigations are being developed as custom Nessus audit files. Custom audit files can be leveraged by using a [Policy Compliance](https://docs.tenable.com/nessus/7_0/Content/ScanAndPolicyTemplates.htm) scan in Nessus. DoD components can acquire Nessus via the [ACAS](https://www.disa.mil/cybersecurity/network-defense/acas) program.

### Operating system

A custom Nessus audit file is available for scanning Windows operating systems: [Verify-Spectre-Meltdown-Mitigations-Windows-OS.audit](./windows/Verify-Spectre-Meltdown-Mitigations-Windows-OS.audit)
 
The audit file performs the following checks:
* PowerShell is installed.
* PowerShell version is at least 2.0.
* The operating system patches are installed.
* The SpeculationControl module is installed.
* A firmware updated has been installed.
* The antivirus compatibility registry value exists and is set to the correct value.
* The registry values for configuring the operating system mitigations are set to the correct value when they are required to exist.
* CVE-2017-5715 (Branch Target Injection), aka variant 2 and commonly known as Spectre, mitigation support is present in firmware/hardware (BTIHardwarePresent value is True).
* CVE-2017-5715 (Branch Target Injection), aka variant 2 and commonly known as Spectre, mitigation support is present in Windows (BTIWindowsSupportPresent is True).
* CVE-2017-5715 (Branch Target Injection), aka variant 2 and commonly known as Spectre, mitigation support is enabled in Windows (BTIWindowsSupportEnabled is True).
* CVE-2017-5754 (Rogue Data Cache Load), aka variant 3 and commonly referred to as Meltdown, mitigation support is present in Windows when required (KVAShadowWindowsSupportPresent is True when KVAShadowRequired is True).
* CVE-2017-5754 (Rogue Data Cache Load), aka variant 3 and commonly referred to as Meltdown, mitigation support is enabled in Windows when required   (KVAShadowWindowsSupportEnabled is True when KVAShadowRequired is True).

Tenable has also published an initial list of plugin IDs that can used for detecting if Spectre and Meltdown patches are install in [this blog post](https://www.tenable.com/blog/the-first-major-security-logos-of-2018-spectre-and-meltdown-vulnerabilities). Additional plugins can be found by searching for **spectre** or **meltdown** using the [plugin search page](https://www.tenable.com/plugins/index.php?view=search). A [Spectre and Metldown dashboard](https://www.tenable.com/sc-dashboards/spectre-meltdown) is also available in SecurityCenter.

### Browsers
A custom Nessus audit file is available for scanning browsers on Windows operating systems: [Verify-Spectre-Meltdown-Mitigations-Windows-Browsers.audit](./windows/Verify-Spectre-Meltdown-Mitigations-Windows-Browsers.audit)

The audit file performs the following checks:
* PowerShell is installed.
* PowerShell version is at least 2.0.
* If Chrome is installed, then at least 63.0.3239.132 or newer is installed and that the Site Isolation registry value is correctly configured (optional).
* Internet Explorer 11 patch is installed. - partially implemented
* If Edge exists, then that the Edge patched is installed. - partially implemented
* If Firefox is installed, then that Firefox 57.0.4 or newer is installed. - partially implemented
* If Firefox ESR is installed, then that Firefox 52.6 or newer is installed. - not implemented yet
* If Opera is installed, then that Opera 50.0.2762.67 or newer is installed. - not implemented yet

**The Nessus audit file for browers on Windows is still a work in progress**.

### Hyper-V
A custom Nessus audit file is being developed for Hyper-V hosts.