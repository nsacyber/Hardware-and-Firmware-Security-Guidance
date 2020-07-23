# UEFI Secure Boot Customization
Before disabling Secure Boot to make something compatible, consider customizing Secure Boot! Customization can be as simple as
adding a new certificate to the DB Allow List, or as complex as purging all factory certificates and hashes to start fresh.
Customization allows organizations to leverage boot time security mechanisms that protect endpoints before traditional anti-
malware solutions are initialized.

## Windows
The Windows Secure Boot ecosystem is managed by Microsoft. Most machines feature a Microsoft KEK, Microsoft Windows Production
CA DB certificate, and UEFI Third Party Marketplace CA 2011 DB certificate. Most hardware and software intended for use with
Windows already come with compatibile Secure Boot signatures. Use customization commands in this section to:

- Add support for an operating system that lacks Secure Boot signatures (e.g. older versions of Windows; some Linux distrubutions)
- Add support for an unsigned driver
- Test DBX entries that may have been marked as optional Windows Updates
- Add a new DB certificate to support a vendor that lacks Microsoft signatures
- Migrate to or from a DB certificate

[Windows Secure Boot Customization Hub](Windows.md)

## Linux
Mainstream Linux distributions contain a pre-bootloader named Shim that features a Microsoft signature. Shim contains a
distribution certificate known as the Machine Owner Key (MOK). MOK acts like a software implementation of Secure Boot's
firmware DB Allow List.

[Linux Secure Boot Customization Hub](Linux.md)
