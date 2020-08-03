# UEFI Secure Boot Customization
Before disabling Secure Boot to make something compatible, consider customizing Secure Boot! Customization can be as simple as
adding a new certificate to the DB Allow List, or as complex as purging all factory certificates and hashes to start fresh.
Customization allows organizations to leverage boot time security mechanisms that protect endpoints before traditional anti-
malware solutions are initialized.

### Important Definitions
**Partial Customization** involves adding content to the DB, DBX, and/or KEK. Some or all of the Secure Boot values set at the factory will be retained. Influence from system and software vendors is retained. Useful for Windows, Linux, and hypervisors.

***Full Customization*** involves replacing the PK, KEK, and DB records to those created by the infrastructure administrators. All influence from the system vendor and software vendors is removed. Useful for Linux and hypervisors.

## Windows
The Windows Secure Boot ecosystem is managed by Microsoft. Most machines feature a Microsoft KEK, Microsoft Windows Production
CA DB certificate, and UEFI Third Party Marketplace CA 2011 DB certificate. Most hardware and software intended for use with
Windows already come with compatibile Secure Boot signatures. Use customization commands in this section to:

- Add support for an operating system or hypervisor that lacks Secure Boot signatures (e.g. older versions of Windows; some Linux distrubutions)
- Add support for an unsigned driver
- Add DBX entries that may have been marked as optional Windows Updates or that the organization does not trust
- Add a new DB certificate to support a vendor that lacks Microsoft signatures
- Add DB records to trust binaries
- Migrate to or from a DB certificate
- Implement partial Secure Boot Customization

[Windows Secure Boot Customization Hub](Windows.md)

## Linux
Mainstream Linux distributions contain a pre-bootloader named Shim that features a Microsoft signature. Shim contains a
distribution certificate known as the Machine Owner Key (MOK). MOK acts like a software extension of Secure Boot's
firmware DB Allow List. A corresponding extension of the DBX Deny List is known as MOKX. Use the customization commands in this section to:

- Add support for an operating system, hypervisor, driver, bootloader, or other executable content denied by Secure Boot policy
- Add support for a software package that contains a kernel module
- Add DBX entries to distrust binaries
- Add DB entries to trust binaries
- Customize the PK, KEK, DB, DBX, MOK, and/or MOKX
- Reduce the influence of system and software vendors on a system's security posture
- Implement complete and total Secure Boot Customization
- Implement partial Secure Boot customization

[Linux Secure Boot Customization Hub](Linux.md)

## UEFI Configuration
Secure Boot can be customized via the UEFI Configuration interfaces provided on many systems today. System vendors that implement some level of customization support in firmware include:
- Asus
- [Dell EMC Servers](dellemc)
- [Dell Workstations](dell)
- [HPE Servers](hpe)
- [HP Workstations](hp)
- [Intel](intel)
- Lenovo
- Toshiba

System vendors that do not support customization (or Secure Boot) include:
- Apple
- Microsoft
