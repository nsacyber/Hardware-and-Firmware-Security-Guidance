# Windows operating system guidance

Based on [Microsoft's guidance](https://blogs.technet.microsoft.com/configurationmgr/2018/01/08/additional-guidance-to-mitigate-speculative-execution-side-channel-vulnerabilities/) for the order of operations, perform the following tasks on Windows operating systems:

1. [Confirm antivirus software compatibility](#confirm-antivirus-software-compatibility).
1. [Create registry value to indicate antivirus software compatibility](#create-registry-value-to-indicate-antivirus-software-compatibility), if needed.
1. [Create registry values to enable mitigations](#create-registry-values-to-enable-mitigations), if needed.
1. [Install operating system patches](#install-operating-system-patches) and reboot.
1. [Install firmware update](install-firmware-update) and reboot.
1. [Install mitigation verification tool](#install-mitigation-verification-tool).
1. [Verify mitigations are enabled](#verify-mitigations-are-enabled).

## Confirm antivirus software compatibility

[Kevin Beaumont](https://twitter.com/GossiTheDog) has a [Google Docs spreadsheet](https://docs.google.com/spreadsheets/d/184wcDt9I9TUNFFbsAVLpzAtckQxYiuirADzf3cL42FQ) that tracks the status of a number antivirus products in regards to their compatibility with the January 2018 patches and whether the product creates the registry value required by Windows Update and Windows Server Update Services (WSUS) for the patches to install. Microsoft has additional guidance in [KB4072699](https://support.microsoft.com/en-us/help/4072699/). Common antivirus products in DoD are discussed below.

### McAfee

See McAfee article [KB90167](https://kc.mcafee.com/corporate/index?page=content&id=KB90167) that documents the minimum required versions of various McAfee products needed to be compatible with the Spectre and Meltdown patches. The required minimum compatible versions of HostBased Security System (HBSS) client components are:

* Application Control 6.2.0 and later.
* Data Loss Prevention 9.3 and later.
* Host IPS 8.0 Patch 4 and later.
* McAfee Agent 4.8 Patch 2 and later.
* VirusScan Enterprise 8.7 Patch 5, or 8.8 RTW, and later.

### Symantec
See Symantec article [TECH248545](https://support.symantec.com/en_US/article.TECH248545.html) that documents a compatibility issue with the ERASER engine of Symantec Endpoint Protection (SEP) 12 and 14. ERASER engine version 117.2.1 and earlier is affected. Upgrade to version 117.3.0.358 or later to be compatible with the Meltdown and Spectre patches. For more information see [TECH248558](https://support.symantec.com/en_US/article.TECH248558.html) and [TECH248552](https://support.symantec.com/en_US/article.TECH248552.html) for known issues with the update.

### Microsoft
Windows Defender Antivirus (WDAV), formerly known as Windows Defender, is compatible with the Spectre and Meltdown patches.

## Create registry value to indicate antivirus software compatibility 

A registry value is required for delivery of the January 2018 patches via Windows Update or WSUS. **Absence of the registry value will also prevent future security updates from being installed via Windows Update or WSUS.** Microsoft [antivirus guidance](https://support.microsoft.com/en-us/help/4072699/) states: *"Customers will not receive the January 2018 security updates (**or any subsequent security updates**) and will not be protected from security vulnerabilities unless their antivirus software vendor sets the following registry key"*. If the system does not have antivirus software, then the registry value still needs to be created.

The registry value to set is:
* Key Path: HKLM\Software\Microsoft\Windows\CurrentVersion\QualityCompat
* Value Name: cadca5fe-87d3-4b96-b7fb-a231484277cc 
* Value Type: REG_DWORD 
* Value Data: 0

The registry value is not needed if manually installing the January 2018 patches or if using third-party patch management software, but its creation is still recommended in case a transition to Windows Update or WSUS occurs to ensure the system receives future patches. 

Requiring the registry value for installation of the January 2018 patches, as well as all future patches, is a safety mechanism to prevent systems from having bluescreen errors. Microsoft [antivirus guidance](https://support.microsoft.com/en-us/help/4072699) states: "*During testing, we discovered that some third-party applications have been making unsupported calls into Windows kernel memory that cause stop errors (also known as bluescreen errors) to occur.*". 

To create the registry value from an administrator command prompt: 

~~~
reg add "HKLM\Software\Microsoft\Windows\CurrentVersion\QualityCompat" /v cadca5fe-87d3-4b96-b7fb-a231484277cc  /t REG_DWORD /d 0 /f >nul
~~~

To create the registry values from an administrator PowerShell prompt:

~~~
[void](New-Item -Path 'HKLM:\Software\Microsoft\Windows\CurrentVersion\QualityCompat' -Name 'cadca5fe-87d3-4b96-b7fb-a231484277cc' -Value 0 -Force)
~~~

To create the registry values using [Group Policy Registery Preferences](https://technet.microsoft.com/en-us/library/cc753092(v=ws.11).aspx):

1. Open the Group Policy Management Console, navigate to the location containing the Group Policy object (GPO) that will contain the new preference item, and then click **Edit**.
1. In the console tree under **Computer Configuration**, expand the **Preferences** folder, and then expand the **Windows Settings** folder.
1. Right click the **Registry** node and select **New** > **Registry Item**.
1. In the New Registry Item dialog box, select **Create** or **Replace** from the **Action** dropdown menu.
1. From the **Hive** drop down menu, ensure **HKEY_LOCAL_MACHINE** is selected which should be the default selection.
1. In the **Key Path** field enter **Software\Microsoft\Windows\CurrentVersion\QualityCompat**.
1. Enter **cadca5fe-87d3-4b96-b7fb-a231484277cc** in the **Value name** field.
1. From the **Value type** dropdown menu, select **REG_DWORD**.
1. Enter **0** in the **Value data** field.
1. Click **OK**.


Some antivirus products create the value on their own while others require administrators to manually create the value. Group Policy Registry Preferences, as outlined above can be used to create the registry value if all systems are known to have compatible products installed.

### McAfee

The McAfee article [KB90167](https://kc.mcafee.com/corporate/index?page=content&id=KB90167) states: "*Starting with the January 12th DAT (**8772**), customers who use VirusScan Enterprise (VSE) 8.8 and receive their DAT updates through ePolicy Orchestrator (ePO) will have the registry key automatically updated.*". McAfee article [KB90180](https://kc.mcafee.com/corporate/index?page=content&id=KB90180) documents how to create the registry value through ePolicy Orchestrator (ePO). 

### Symantec
SEP will create the registry value via the ERASER update.

### Microsoft
Windows Defender Antivirus will create the registry value via signature update. To force creation of the registry value run **"%Program Files%\Windows Defender\MpCmdRun.exe" -SignatureUpdate** from a command prompt.

## Create registry values to enable mitigations

Certain registry values must exist on systems running Windows Server in order for the mitigations to be enabled. The registry values are not required for systems running client editions of Windows since the mitigations are enabled by default after installing the operating system and firmware patches. The Microsoft [client guidance](https://support.microsoft.com/en-us/help/4073119/windows-client-guidance-for-it-pros-to-protect-against-speculative-exe) page states: "*By default, this update is enabled. No customer action is required to enable the fixes. We are providing the following registry information for completeness in the event that customers want to disable the security fixes related to CVE-2017-5715 and CVE-2017-5754 for Windows clients*". The registry values have no effect if the operating system patches are not installed.

The registry values to set are:
* Key Path: HKLM\System\CurrentControlSet\Control\Session Manager\Memory Management
* Value Name: FeatureSettingsOverride
* Value Type: REG_DWORD 
* Value Data: 0

and

* Key Path: HKLM\System\CurrentControlSet\Control\Session Manager\Memory Management
* Value Name: FeatureSettingsOverrideMask
* Value Type: REG_DWORD 
* Value Data: 3

To create the registry values from an administrator command prompt: 

~~~
reg add "HKLM\System\CurrentControlSet\Control\Session Manager\Memory Management" /v FeatureSettingsOverride /t REG_DWORD /d 0 /f >nul

reg add "HKLM\System\CurrentControlSet\Control\Session Manager\Memory Management" /v FeatureSettingsOverrideMask /t REG_DWORD /d 3 /f >nul
~~~

To create the registry values from an administrator PowerShell prompt:

~~~
[void](New-Item -Path 'HKLM:\System\CurrentControlSet\Control\Session Manager\Memory Management' -Name 'FeatureSettingsOverride' -Value 0 -Force)

[void](New-Item -Path 'HKLM:\System\CurrentControlSet\Control\Session Manager\Memory Management' -Name 'FeatureSettingsOverrideMask' -Value 3 -Force)
~~~

To create the registry values using [Group Policy Registery Preferences](https://technet.microsoft.com/en-us/library/cc753092(v=ws.11).aspx):

1. Open the Group Policy Management Console, navigate to the location containing the Group Policy object (GPO) that will contain the new preference item, and then click **Edit**.
1. In the console tree under **Computer Configuration**, expand the **Preferences** folder, and then expand the **Windows Settings** folder.
1. Right click the **Registry** node and select **New** > **Registry Item**.
1. In the New Registry Item dialog box, select **Create** or **Replace** from the **Action*** dropdown menu.
1. From the **Hive** drop down menu, ensure **HKEY_LOCAL_MACHINE** is selected which should be the default selection.
1. In the **Key Path** field enter **System\CurrentControlSet\Control\Session Manager\Memory Management**.
1. Enter **FeatureSettingsOverride** in the **Value name** field.
1. From the **Value type** dropdown menu, select **REG_DWORD**.
1. Enter **0** in the **Value data** field.
1. Click **OK**.
1. Repeat steps 3-10 for **FeatureSettingsOverrideMask** with a value of **3**.

## Install operating system patches
**Install all January and February 2018 patches released by Microsoft.** If you have not installed the January 2018 patches, then you only need to install the Monthly Rollup or Security Only updates for February 2018. The Security Only updates are usually not cumulative, but the Feburary 2018 Security Only updates are cumulative and include the January 2018 Spectre and Meltdown patches as well as the fixes for issues on systems with AMD processors. IF you installed the January 2018 patches, then you need to install the February 2018 patches as well to ensure 32-bit Windows 10 systems are patched against CVE-2017-5754 (Meltdown).

The initial set of January 3 and January 9 patches for Spectre and Meltdown:
* [KB4056893](https://support.microsoft.com/en-us/help/4056893) - Windows 10 1507
* [KB4056888](https://support.microsoft.com/en-us/help/4056888) - Windows 10 1511
* [KB4056890](https://support.microsoft.com/en-us/help/4056890) - Windows 10 1607 / Windows Server 2016
* [KB4056891](https://support.microsoft.com/en-us/help/4056891) - Windows 10 1703
* [KB4056892](https://support.microsoft.com/en-us/help/4056892) - Windows 10 1709 / Windows Server 1709
* [KB4056894](https://support.microsoft.com/en-us/help/4056894) - Windows 7 SP1 / Windows Server 2008 R2 SP1 (Monthly Rollup)
* [KB4056897](https://support.microsoft.com/en-us/help/4056897) - Windows 7 SP1 /Windows Server 2008 R2 SP1 (Security Update Only)
* [KB4056895](https://support.microsoft.com/en-us/help/4056895) - Windows 8.1 / Windows Server 2012 R2 (Monthly Roll Up)                
* [KB4056898](https://support.microsoft.com/en-us/help/4056898) - Windows 8.1 / Windows Server 2012 R2 (Security Update Only)
* TBD - Windows Server 2012
* TBD - Windows Server 2008 SP2

**Windows Server 2008 and Windows Server 2012 do not have patches available at this time**. The Microsoft [advisory](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV180002) states: 
"*6. Why aren't Windows Server 2008 and Windows Server 2012 platforms getting an update? When can customers expect the fix?*"

"*Addressing a hardware vulnerability with a software update presents significant challenges with some operating systems requiring extensive architectural changes. Microsoft continues to work with affected chip manufacturers and investigate the best way to provide mitigations*"

The Febuary 2018 Security Only updates can also be installed as well as the Monthly Rollup which is always cumulative. Microsoft's Security Only updates usually are not cumulative, but they are cumulative for February 2018 only. The Microsoft [advisory](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV180002) states: 

"*I have not installed the January 2018 Security Only updates. If I install the February 2018 Security Only updates, am I protected from the vulnerabilities described in this advisory?*"

"*Yes. While Security Only updates are not normally cumulative, to ensure customers are protected, Microsoft is including the mitigations against these vulnerabilities in the February Security Only updates. These updates also include the updates for AMD-based devices.*"


The initial set of Windows patches for Spectre and Meltdown released on January 3 and January 9 had issues on some systems with [older AMD processors](https://www.amd.com/en/corporate/speculative-execution) (AMD Opteron, Athlon and AMD Turion X2 Ultra families) where the system would be put into an unbootable state. Microsoft [temporarily paused the patches on systems with the affected AMD processors](https://support.microsoft.com/en-us/help/4073707/).


The patches released January 10-19 to resolve the issues on AMD-based systems:
* [KB4073290](https://support.microsoft.com/en-us/help/4073290) - Windows 10 1709
* [KB4057144](https://support.microsoft.com/en-us/help/4057144) - Windows 10 1703
* [KB4057142](https://support.microsoft.com/en-us/help/4057142) - Windows 10 1607 / Windows Server 2016 
* [KB4075200](https://support.microsoft.com/en-us/help/4075200) - Windows 10 1511
* [KB4075199](https://support.microsoft.com/en-us/help/4075199) - Windows 10 1507
* [KB4073576](https://support.microsoft.com/en-us/help/4073576) - Windows 8.1 / Windows Server 2012 R2
* [KB4073578](https://support.microsoft.com/en-us/help/4073578) - Windows 7 SP1 / Windows Server 2008 R2 SP1

Apply the above patches to AMD systems OR apply the February 2018 patches which include the fixes for AMD-based systems as well as protection against Meltdown on 32-bit Windows 10 systems.


**The initial patches released by Microsoft did not fix CVE-2017-5754 (Rogue Data Cache Load), aka variant 3 and commonly referred to as Meltdown, on 32-bit operating systems.** The Microsoft [advisory](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV180002) states: "*Addressing a hardware vulnerability with a software update presents significant challenges and mitigations for older operating systems that require extensive architectural changes. The existing 32-bit update packages listed in this advisory fully address CVE-2017-5753 and CVE-2017-5715, but do not provide protections for CVE-2017-5754 at this time. Microsoft is continuing to work with affected chip manufacturers and investigate the best way to provide mitigations for x86 customers, which may be provided in a future update*".

Microsoft has released patches for some 32-bit operating systems since the initial Spectre and Meltdown patches were released in early January 2018. Microsoft released KB4073291 for 32-bit Windows 10 1709 on January 18, 2018 for CVE-2017-5754 (Meltdown). Microsoft released additional patches for 32-bit versions of Windows 10 (1507, 1511, 1607, 1703, 1709) on February 13, 2018 for CVE-2017-5754 (Meltdown).

The January 18 and February 13 patches for 32-bit Windows 10 releases for Meltdown:
* [KB4074596](https://support.microsoft.com/en-us/help/4074596) - Windows 10 1507
* [KB4074591](https://support.microsoft.com/en-us/help/4074591) - Windows 10 1511
* [KB4074590](https://support.microsoft.com/en-us/help/4074590) - Windows 10 1607
* [KB4074592](https://support.microsoft.com/en-us/help/4074592) - Windows 10 1703
* [KB4073291](https://support.microsoft.com/en-us/help/4073291) - Windows 10 1709

Other 32-bit releases of Windows (Windows 7 SP1, Windows 8/8.1, Windows Server 2008 SP2) do not have a patch for CVE-2017-5754 at this time.

## Install firmware update

Intel has [confirmed a higher amounts of reboots affecting Broadwell and Haswell processors](https://newsroom.intel.com/news/intel-security-issue-update-addressing-reboot-issues/) after applying firmware updates. As of January 22, Intel has [identified](https://security-center.intel.com/advisory.aspx?intelid=INTEL-SA-00088&languageid=en-fr) the root cause of the reboot issue impacting [Broadwell and Haswell platforms](https://newsroom.intel.com/wp-content/uploads/sites/11/2018/01/microcode-update-guidance.pdf) and is working with OEMs on testing a new update. Only apply firmware updates on production systems after new firmware updates have been published by the affected vendors and the updates have been tested on non-production systems.

**Do not install firmware patches until processor manufacturers (Intel, AMD, IBM, Arm, etc) and OEMs (Dell, Dell EMC, HP Inc, HP Enterprise, etc) have signaled the new firmware patches are ready.**

Dell and HPi have updated their advisories and recommend that customers do not install firmware updates until after new updates have been issued.

[HPi statemment](https://support.hp.com/us-en/document/c05869091):
*In response to Intel's recommendation, HP is taking the following actions:*
* *HP is removing HP BIOS softpaqs with Intel microcode patches from hp.com.*
* *HP will be reissuing HP BIOS softpaqs with previous Intel microcode starting January 25, 2018.*
* *Once Intel reissues microcode updates, HP will issue revised Softpaqs.*
*HP is working closely with our partners, and updates will be made as soon as possible. Check this Security Bulletin frequently for updates.*

[Dell EMC statement](https://www.dell.com/support/article/us/en/19/sln308588):
*Intel has communicated new guidance regarding "reboot issues and unpredictable system behavior" with the microcode included in the BIOS updates released to address Spectre (Variant 2), CVE-2017-5715. Dell is advising that all customers should not deploy the BIOS update for the Spectre (Variant 2) vulnerability at this time. We have removed the impacted BIOS updates from our support pages and are working with Intel on a new BIOS update that will include new microcode from Intel.*

*If you have already deployed the BIOS update, in order to avoid unpredictable system behavior, you can revert back to a previous BIOS version.*

[Dell statement](http://www.dell.com/support/article/us/en/19/sln308587):
*Intel has communicated new guidance regarding the "reboot issues" with the microcode included in the BIOS updates released to address Spectre (Variant 2), CVE-2017-5715. Dell is advising that all customers should not deploy the BIOS update for the Spectre (Variant 2) vulnerability at this time. We are removing the impacted BIOS updates from the web and suspending further BIOS updates for affected platforms.*

*If you have already applied the BIOS update, please wait for further information and an updated BIOS release, no other action is recommended at this point. Please continue to check back for updates.*

See the [Patches page](./../Patches.md) for links to OEM specific firmware pages.

## Install mitigation verification tool

A number of tools are available for verifying that the mitigations are correctly enabled:
* The Microsoft [SpeculationControl PowerShell module](https://gallery.technet.microsoft.com/scriptcenter/Speculation-Control-e36f0050).
* A Microsoft [compliance settings configuration baseline CAB file](https://gallery.technet.microsoft.com/Speculation-Execution-Side-1483f621) that can be imported into System Center Configuration Manager 2012 and later.
* A custom Nessus audit file included in this repository.

### SpeculationControl module
Microsoft has created a PowerShell module, available from the [PowerShell Gallery](https://www.powershellgallery.com/packages/SpeculationControl) and the [TechNet Gallery](https://gallery.technet.microsoft.com/scriptcenter/Speculation-Control-e36f0050), for verifying the firmware and operating system mitigations exist and are configured correctly.

The 1.0.4 version of module, as written, only works with PowerShell 4.0 and later by default. It can be modified to work with PowerShell 2.0 and later simply by changing the **RootModule** keyword to **ModuleToImport** in the **SpeculationControl.psd1** file.

#### Option 1
This option is for installing the module on a system that has PowerShell 5.0+ and is an internet connected system.

To install the module from an administrator PowerShell prompt:

~~~
Install-Module -Name SpeculationControl -Force -Confirm:$false
~~~

#### Option 2
This option is for installing the module on a system that has PowerShell 2.0-4.0 that is an internet connected system OR is an internet disconnected system with any version of PowerShell.

To download the SpeculationControl module:
1. Download the SpeculationControl.zip file from the [Speculation Control Validation PowerShell Script](https://gallery.technet.microsoft.com/scriptcenter/Speculation-Control-e36f0050) page.
1. Right click the zip file, select **Properties**, select the **Unblock** checkbox in the lower right corner, click **Apply**, and click **OK**.
1. Right click the zip file and select **Extract All...**.
1. In the **Files will be extracted to this folder** field, add the version number of the module to the end of the path. For example, **SpeculationControl 1.0.4** should be at the end of the value in the field.
1. Click **Extract**.
1. Windows Explorer will open the SpeculationControl 1.0.4 folder and inside it will be a **SpeculationControl** folder containing **SpeculationControl.psd1** and **SpeculationControl.psm1** files. The folder and files are the SpeculationControl PowerShell module.
1. If support for PowerShell 2.0 is required, then modify the **SpeculationControl.psd1** file by replacing the **RootModule** keyword with **ModuleToImport**.
1. Copy the **SpeculationControl** folder that contains the .psd1 and .psm1 files to a location that will be accessible to the server where the Group Policy object for deploying the module will be created.

To install the SpeculationControl module using [Group Policy File Preferences](https://technet.microsoft.com/en-us/library/cc772536(v=ws.11).aspx):
1. Open the Group Policy Management Console, navigate to the location containing the Group Policy object (GPO) that will contain the new preference item, and then click **Edit**.
1. In the console tree under **Computer Configuration**, expand the **Preferences** folder, and then expand the **Windows Settings** folder.
1. Right click the **Files** node and select **New** > **File**.
1. In the New File Item dialog box, select **Create** or **Replace** from the **Action*** dropdown menu.
1. In the **Source file(s):** field, click **...**, navigate to the SpeculationControl folder, select SpeculationControl.psd1, and click **Open**.
1. In the **Destination File** field, enter **%ProgramFilesDir%\WindowsPowerShell\Modules\SpeculationControl\SpeculationControl.psd1**.
1. Click **OK**.
1. Repeat steps 3-7 for **SpeculationControl.psm1**.

### System Center Configuration Manager
See Microsoft documentation for instructions on how to [import the CAB file](https://docs.microsoft.com/en-us/sccm/compliance/deploy-use/import-configuration-data) and [deploy the configuration baseline](https://docs.microsoft.com/en-us/sccm/compliance/deploy-use/deploy-configuration-baselines).

### Nessus
DoD components can acquire Nessus via the [ACAS](https://www.disa.mil/cybersecurity/network-defense/acas) program. See Tenable documentation for [installing](https://docs.tenable.com/nessus/6_9/Content/WindowsInstall.htm) and [configuring](https://docs.tenable.com/nessus/6_9/Content/ConfigureNessus.htm) Nessus on Windows.

## Verify mitigations are enabled

### SpeculationControl module 
Once the SpeculationControl module has been installed, it can be used for verifying the firmware and operating system mitigations exist and are correctly configured.

To verify mitigations from a non-administrator PowerShell prompt:

~~~
Import-Module -Name SpeculationControl

Get-SpeculationControlSettings
~~~

See Microsoft documentation for assistance with [understanding the output of the module](https://support.microsoft.com/en-us/help/4074629). On a system that is affected by all the vulnerabilities, has a fully patched operating system, has updated firmware, and where all mitigations are correctly configured, the output should be:

~~~
BTIHardwarePresent             : True
BTIWindowsSupportPresent       : True
BTIWindowsSupportEnabled       : True
BTIDisabledBySystemPolicy      : False
BTIDisabledByNoHardwareSupport : False
KVAShadowRequired              : True
KVAShadowWindowsSupportPresent : True
KVAShadowWindowsSupportEnabled : True
KVAShadowPcidEnabled           : False or True
~~~

KVAShadowPcidEnabled denotes a performance improvement that is enabled when available.

### System Center Configuration Manager
See Microsoft documentation for how to for instructions on how to [view the compliance results](ttps://docs.microsoft.com/en-us/sccm/compliance/deploy-use/monitor-compliance-settings). The More Information section of the [TechNet Gallery page for the CAB file](https://gallery.technet.microsoft.com/Speculation-Execution-Side-1483f621) explains how to interpret the results.

### Nessus
A custom Nessus audit file is available for scanning Windows operating systems: [Verify-Spectre-Meltdown-Mitigations-Windows-OS.audit](./../../verification/windows/Verify-Spectre-Meltdown-Mitigations-Windows-OS.audit). The custom audit file can be leveraged by using a [Policy Compliance](https://docs.tenable.com/nessus/7_0/Content/ScanAndPolicyTemplates.htm) scan. The audit file performs the following checks:
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




