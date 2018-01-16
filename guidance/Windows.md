# Windows operating system guidance

Based on [Microsoft's guidance](https://blogs.technet.microsoft.com/configurationmgr/2018/01/08/additional-guidance-to-mitigate-speculative-execution-side-channel-vulnerabilities/) for the order of operations, perform the following tasks on Windows operating systems:

1. Confirm antivirus software compatibility
1. Create antivirus software compatibility registry value, if needed
1. Create mitigation registry values to enable mitigations, if needed
1. Install operating system patches and reboot
1. Install firmware update and reboot
1. Install mitigation vertification tool
1. Verify mitigations

## Install firmware update

Intel has [confirmed a higher amount of reboots affecting systems with Broadwell and Haswell processors](https://newsroom.intel.com/news/intel-security-issue-update-addressing-reboot-issues/) after applying firmware updates. Apply firmware updates on production systems after new firmware updates have been published by the affected vendors.

## Check antivirus compatibility

[Kevin Beaumont](https://twitter.com/GossiTheDog) has a [Google Docs spreadsheet](https://docs.google.com/spreadsheets/d/184wcDt9I9TUNFFbsAVLpzAtckQxYiuirADzf3cL42FQ) that tracks the status of a number antivirus products in regards to their compatibility with the January 2018 patches and whether or not the product creates the registry value required by Windows Update and WSUS.  Microsoft has additional guidance in [KB4072699](https://support.microsoft.com/en-us/help/4072699/january-3-2018-windows-security-updates-and-antivirus-software). Common antivirus products in DoD are discussed below.

### McAfee

See McAfee article [KB90167](https://kc.mcafee.com/corporate/index?page=content&id=KB90167) that documents the minimum required version of various McAfee products required to be compatible with the Spectre and Meltdown patches. For HBSS client components:

* Application Control 6.2.0 and later
* Data Loss Prevention 9.3 and later
* Host IPS 8.0 Patch 4 and later
* McAfee Agent 4.8 Patch 2 and later
* VirusScan Enterprise 8.7 Patch 5, or 8.8 RTW, and later

The article states *Starting with the January 12th DAT (**8772**), customers who use VirusScan Enterprise (VSE) 8.8 and receive their DAT updates through ePolicy Orchestrator (ePO) will have the registry key automatically updated.*. McAfee article [KB90180](https://kc.mcafee.com/corporate/index?page=content&id=KB90180) documents on how to create the registry value through ePO. Additionally, Group Policy Registry Preferences can also be used to create the registry value.

### Symantec
See Symantec article [TECH248545](https://support.symantec.com/en_US/article.TECH248545.html) that documents a compatibility issue with the ERASER engine of Symantec Endpoint Protection (SEP) 12 and 14. is affected. ERASER engine 117.2.1 and earlier is affected. Upgrade to 117.3.0.358 or later. For more information see [TECH248558](https://support.symantec.com/en_US/article.TECH248558.html) and [TECH248552](https://support.symantec.com/en_US/article.TECH248552.html) for known issues with the update.

### Microsoft
Windows Defender Antivirus (WDAV), formerly known as Windows Defender, will create the registry value automatically. To force creation of the registry value run **"%Program Files%\Windows Defender\MpCmdRun.exe" -SignatureUpdate**.

## Create antivirus compatibility registry value

A registry value is required for delivery of the January 2018 patches via Windows Update or Windows Server Update Services (WSUS). The registry value is needed because Microsoft states *some third-party applications have been making unsupported calls into Windows kernel memory that cause stop errors (also known as bluescreen errors) to occur*. Requiring the registry value is a safety mechanism to prevent systems from potentially having bluescreen errors.

The registry value to set is:
* Key Path: HKLM\Software\Microsoft\Windows\CurrentVersion\QualityCompat
* Value Name: cadca5fe-87d3-4b96-b7fb-a231484277cc 
* Value Type: REG_DWORD 
* Value Data: 0

The registry value is not needed if manually installing the January 2018 patches or if using third-party patch management software. Some antivirus products create the value on their own while others require administrators to manually create the value.

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
1. In the New Registry Item dialog box, select **Create** or **Replace** from the **Action*** dropdown menu.
1. From the **Hive** drop down menu, ensure **HKEY_LOCAL_MACHINE** is selected which should be the default selection.
1. In the **Key Path** field enter **Software\Microsoft\Windows\CurrentVersion\QualityCompat**.
1. Enter **cadca5fe-87d3-4b96-b7fb-a231484277cc** in the **Value name** field.
1. From the **Value type** dropdown menu, select **REG_DWORD**.
1. Enter **0** in the **Value data** field.
1. Click **OK**.

## Install patches
Install operating system patches using a patch deployment mechanism.

Note that the patches do not fix CVE-2017-5754 on 32-bit operating systems. Microsoft's [advisory](https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/ADV180002) states *Addressing a hardware vulnerability with a software update presents significant challenges and mitigations for older operating systems that require extensive architectural changes. The existing 32-bit update packages listed in this advisory fully address CVE-2017-5753 and CVE-2017-5715, but do not provide protections for CVE-2017-5754 at this time. Microsoft is continuing to work with affected chip manufacturers and investigate the best way to provide mitigations for x86 customers, which may be provided in a future update*.

## Create mitigation registry values

Registry values are required for systems running Windows Server. The registry values are not required for systems running client editions of Windows. The [client guidance](https://support.microsoft.com/en-us/help/4073119/windows-client-guidance-for-it-pros-to-protect-against-speculative-exe) page says:
*By default, this update is enabled. No customer action is required to enable the fixes. We are providing the following registry information for completeness in the event that customers want to disable the security fixes related to CVE-2017-5715 and CVE-2017-5754 for Windows clients.*

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

## Install mitigation verification tool

A number of verification tools are available:
* The Microsoft [SpeculationControl PowerShell module](https://gallery.technet.microsoft.com/scriptcenter/Speculation-Control-e36f0050).
* A Microsoft [compliance settings configuration baseline CAB file](https://gallery.technet.microsoft.com/Speculation-Execution-Side-1483f621) that can be imported into System Center Configuration Manager 2012 and later.
* A custom Nessus audit file included in this repository.

### SpeculationControl module
Microsoft has created a PowerShell module, available from the [PowerShell Gallery](https://www.powershellgallery.com/packages/SpeculationControl) and the [TechNet Gallery](https://gallery.technet.microsoft.com/scriptcenter/Speculation-Control-e36f0050), for verifying the firmware and operating system mitigations exist and are configured correctly.

The 1.0.4 version of module, as written, only works with PowerShell 4.0 and later by default. It can be modified to work with PowerShell 2.0 and later simply by changing the **RootModule** keyword to **ModuleToImport** in the **SpeculationControl.psd1** file.

To install the module on a system that has PowerShell 5.0+ and is an internect connected system:

~~~
Install-Module -Name SpeculationControl -Force -Confirm:$false
~~~

To install the module on a system that has PowerShell 2.0-4.0 that is an internet connected OR is an internet disconnected system with any version of PowerShell:

To download the SpeculationControl module:
1. Download the SpeculationControl.zip file from the [Speculation Control Validation PowerShell Script](https://gallery.technet.microsoft.com/scriptcenter/Speculation-Control-e36f0050) page.
1. Right click the zip file, select **Properties**, select the **Unblock** checkbox in the lower right corner, click **Apply**, and click **OK**.
1. Right click the zip file and select **Extract All...**.
1. In the **Files will be extracted to this folder** field, add the version number of the module to the end of the path. For example **SpeculationControl 1.0.4** should be at the end of the value in the field.
1. Click **Extract**.
1. Windows Explorer will open the SpeculationControl 1.0.4 folder and inside it will be a **SpeculationControl** folder containing **SpeculationControl.psd1** and **SpeculationControl.psm1** files. The folder and files are the SpeculationControl module.
1. If support for PowerShell 2.0 is required, then modify the **SpeculationControl.psd1** file by replacing the **RootModule** keyword with **ModuleToImport**.
1. Copy the SpeculationControl folder that contains the .psd1 and .psm1 files to a location that will be accessible to the server where the Group Policy will be created.

To install the Speculation module using [Group Policy File Preferences](https://technet.microsoft.com/en-us/library/cc772536(v=ws.11).aspx):
1. Open the Group Policy Management Console, navigate to the location containing the Group Policy object (GPO) that will contain the new preference item, and then click **Edit**.
1. In the console tree under **Computer Configuration**, expand the **Preferences** folder, and then expand the **Windows Settings** folder.
1. Right click the **Files** node and select **New** > **File**.
1. In the New File Item dialog box, select **Create** or **Replace** from the **Action*** dropdown menu.
1. In the **Source file(s):** field, click **...***, navigate to the SpeculationControl folder, select SpeculationControl.psd1, and click **Open**.
1. In the **Destination File** field, enter **%ProgramFilesDir%\WindowsPowerShell\Modules\SpeculationControl\SpeculationControl.psd1**.
1. Click **OK**
1. Repeat steps 3-7 for **SpeculationControl.psm1**.

### System Center Configuration Manager
See https://docs.microsoft.com/en-us/sccm/compliance/deploy-use/import-configuration-data for instructions on how to import the CAB file. See https://docs.microsoft.com/en-us/sccm/compliance/deploy-use/deploy-configuration-baselines for instructions on how to deploy the configuration baseline.

### Nessus
See Nessus documentation for [installing](https://docs.tenable.com/nessus/6_9/Content/WindowsInstall.htm) and [configuring](https://docs.tenable.com/nessus/6_9/Content/ConfigureNessus.htm) Nessus on Windows.

## Verify mitigations


### SpeculationControl module 
Once the SpeculationControl module has been installed, it can be used for verifying the firmware and operating system mitigations exist and are configured correctly.

From a non-administrator PowerShell prompt:
~~~
Import-Module -Name SpeculationControl

Get-SpeculationControlSettings
~~~

See [Microsoft's article](https://support.microsoft.com/en-us/help/4074629) for assistance with understanding the output of the module. On system that is affected by all the vulnerabilities, has a fully patched operating system, has updated firmware, and where all correct settings are configured, the output should be:

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

KVAShadowPcidEnabled denotes a performance improvement is enabled when available and is not security relevant.

A custom Nessus audit file is available to scan Windows systems. The audit file performs the following checks:
* PowerShell is installed
* PowerShell version is at least 2.0 
* operating system patches are installed
* the SpeculationControl module is installed
* a firmware updated has been installed
* the antivirus compatibility registry value exists and is set to the correct value
* the registry values for configuring the operating system mitigations are set to the correct value when they are required to exist
* BTIHardwarePresent value is True
* BTIWindowsSupportPresent is True
* BTIWindowsSupportEnabled is True
* KVAShadowWindowsSupportPresent is True when KVAShadowRequired is True
* KVAShadowWindowsSupportEnabled is True when KVAShadowRequired is True

### System Center Configuration Manager
See https://docs.microsoft.com/en-us/sccm/compliance/deploy-use/monitor-compliance-settings for instructions on how to view the compliance results. The More Information section of the [TechNet Gallery page for the CAB file](https://gallery.technet.microsoft.com/Speculation-Execution-Side-1483f621) explains how to interpret the results.

### Nessus
DoD components can acquire Nessus via the [ACAS](https://www.disa.mil/cybersecurity/network-defense/acas) program. The custom audit file can be deployed using a [Policy Compliance](https://docs.tenable.com/nessus/7_0/Content/ScanAndPolicyTemplates.htm) scan. 

Tenable has also published an initial list of plugin IDs that can used for detecting if spectre and meltdown patches are install in [this blog post](https://www.tenable.com/blog/the-first-major-security-logos-of-2018-spectre-and-meltdown-vulnerabilities). Additional plugins can be found by searching for **spectre** or **meltdown** using the [plugin search page](https://www.tenable.com/plugins/index.php?view=search). A [Spectre and Metldown dashboard](https://www.tenable.com/sc-dashboards/spectre-meltdown) is also available in SecurityCenter.




