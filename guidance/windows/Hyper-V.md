# Hyper-V host guidance

Systems running Hyper-V need to follow the [general Windows operating system guidance](./OS.md) and may also need additional mitigation actions performed by administrators to ensure the hosted virtual machines are fully protected against Spectre and Meltdown. The additional actions are required to protect against VM to VM and VM to host attacks. The additional actions ensure the VM firmware is updated.

The system running Hyper-V must have the operating system and firmware updated on the host system first. The host system firmware update from OEM contains new processor capabilities (IBRS, STIBP, IBPB) that can be used to protect against CVE-2017-5715. Once the host's firmware has been updated, the hypervisor can make these additional capabilities available to guest virtual machines after taking the following steps.

See [Protecting guest virtual machines from CVE-2017-5715 (Branch Target Injection)](https://docs.microsoft.com/en-us/virtualization/hyper-v-on-windows/CVE-2017-5715-and-hyper-v-vms) for more information.  

## Determine vulnerable virtual machines
Administrators need to determine if any of the virtual machines have a VM Version that is less than 8.0. VM Version 8.0 was introduced with Windows Server 2016 and the Windows 10 1607. Administrators can use the following PowerShell code to determine VMs that have a version less than 8.0.

~~~
Get-VM * | Select-Object -Property 'Name','Version','State' | Where-Object { ([decimal]$_.Version) -lt 8.0 }
~~~

## Shutdown vulnerable virtual machines
The mitigation requires a VM to be completely shutdown rather than just restarted. Administrators can use the following PowerShell code to determine VMs that need to be shutdown.

~~~
Get-VM * | Select-Object -Property 'Name','Version','State' | Where-Object { ([decimal]$_.Version) -lt 8.0 -and $_.State -ne 'Off' }
~~~

Administrators can use the following PowerShell code to shutdown all VMs that need to be shutdown:

~~~
Get-VM * | Select-Object -Property 'Name','Version','State' | Where-Object { ([decimal]$_.Version) -lt 8.0 -and $_.State -ne 'Off' } | Stop-VM
~~~

## Create registry values to enable mitigations

The Hyper-V host system needs the MinVmVersionForCpuBasedMitigations registry value to be created in order for the VMs to receive the firmware update. 

To create the registry value from an administrator command prompt: 

~~~
reg add "HKLM\Software\Microsoft\Windows NT\CurrentVersion\Virtualization" /v MinVmVersionForCpuBasedMitigations /t REG_SZ /d "1.0" /f
~~~

To create the registry values from an administrator PowerShell prompt:

~~~
[void](New-Item -Path 'HKLM:\Software\Microsoft\Windows NT\CurrentVersion\Virtualization' -Name 'MinVmVersionForCpuBasedMitigations' -Value '1.0' -Force)
~~~

To create the registry values using [Group Policy Registery Preferences](https://technet.microsoft.com/en-us/library/cc753092(v=ws.11).aspx):

1. Open the Group Policy Management Console, navigate to the location containing the Group Policy object (GPO) that will contain the new preference item, and then click **Edit**.
1. In the console tree under **Computer Configuration**, expand the **Preferences** folder, and then expand the **Windows Settings** folder.
1. Right click the **Registry** node and select **New** > **Registry Item**.
1. In the New Registry Item dialog box, select **Create** or **Replace** from the **Action** dropdown menu.
1. From the **Hive** drop down menu, ensure **HKEY_LOCAL_MACHINE** is selected which should be the default selection.
1. In the **Key Path** field enter **Software\Microsoft\Windows NT\CurrentVersion\Virtualization'**.
1. Enter **MinVmVersionForCpuBasedMitigations** in the **Value name** field.
1. From the **Value type** dropdown menu, select **REG_SZ**.
1. Enter **1.0** in the **Value data** field.
1. Click **OK**.

## Update guest operating systems
Follow the [general Windows operating system guidance](./OS.md) to ensure the VM operating systems are updated and configured to have mitigations enabled.
