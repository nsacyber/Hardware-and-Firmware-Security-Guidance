# Verification

Verification of mitigations is currently being developed for operating systems and applications.

* [1. Windows](#win)
* [2. Linux](#linux)
* [3. Mac OS](#macos)
* [4. iOS](#ios)
* [5. Android](#and)

## <a name="win"/>1. Windows
Verification of Windows operating system and application mitigations are being developed as custom Nessus audit files. Custom audit files can be leveraged by using a [Policy Compliance](https://docs.tenable.com/nessus/7_0/Content/ScanAndPolicyTemplates.htm) scan in Nessus. DoD components can acquire Nessus via the [ACAS](https://www.disa.mil/cybersecurity/network-defense/acas) program.

See the [Windows page](./windows/README.md) for more information.

### 1.1 PowerShell
Microsoft has introduced a PowerShell module that reports on a system's vulnerability to several variants of Spectre and Meltdown. Color-coded, true or false output is returned. The presence of mitigations in BIOS/UEFI firmware and the Windows OS kernel are indicated. The tool also provides suggested actions if a missing mitigation is detected. Use the following commands from an administrator-elevated PowerShell terminal:

Install-Module SpeculationControl
Import-Module SpeculationControl
Get-SpeculationControlSettings

### 1.2 Products

### 1.3 Open Source Scripts

## <a name="linux"/>2. Linux
### 2.1 Terminal Commands
#### 2.1.1 Red Hat and Ubuntu
Kernel page tablet isolation is an indicator of Spectre and Meltdown patch application. Use the following command to query the status of isolation on Red Hat Enterprise Linux (RHEL) and Ubuntu devices:

cat /boot/config-3.10.0-957.12.2.el7.x86_64 | grep CONFIG_PAGE_TABLE_ISOLATION

Y indicates that patching is compiled into the kernel. N indicates that software vulnerability may exist.

#### 2.1.2 Red Hat Only
Red Hat Enterprise Linux (RHEL) provides a package and script for checking the status of multiple Spectre and Meltdown variants. Full instructions and download of the tool requires a Red Hat support contract. [See the Red Hat website for details.](https://access.redhat.com/labsinfo/speculativeexecution)

#### 2.1.3 Ubuntu Only
Ubuntu provides a package named "spectre-meltdown-checker" that can be downloaded, installed, and executed on an endpoint. After installing the package, simply execute the following from a terminal:

spectre-meltdown-checker

### 2.2 Products

### 2.3 Open Source Scripts
[Stéphane Lesimple (speed47)'s Spectre and Meltdown Checker](https://github.com/speed47/spectre-meltdown-checker)

## <a name="macos"/>3. Mac OS
Check the current version of Mac OS. The version number must be at least [10.13.2](https://support.apple.com/en-us/HT208394).

## <a name="ios"/>4. iOS
Check the current version of iOS. The version number must be at least [11.2](https://support.apple.com/en-us/HT208394).

## <a name="android"/>5. Android
Check the current security patch level. The version must be [2018-01-05](https://source.android.com/security/bulletin/2018-01-01) or newer.
