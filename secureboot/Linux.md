# UEFI Secure Boot Customization for Linux
## Create New Keys and Certificates
The following commands create a PK, KEK, and one DB Signing Certificate. The .crt files are certificates that contain a public key. The .key files are the private keys. Remember to protect the private key. Never store the private key on a production endpoint.

Do not bother with Certificate Signing Requests (CSRs) because UEFI Secure Boot does not use them or understand certificate chains.

    openssl req -new -x509 -newkey rsa:2048 -subj "/CN=Custom PK/" -keyout PK.key -out PK.crt -days 3650 -nodes -sha256
    openssl req -new -x509 -newkey rsa:2048 -subj "/CN=Custom KEK/" -keyout KEK.key -out KEK.crt -days 3650 -nodes -sha256
    openssl req -new -x509 -newkey rsa:2048 -subj "/CN=Custom DBK/" -keyout DBK.key -out DBK.crt -days 3650 -nodes -sha256
    
## Convert  from PEM to DER
The following commands convert the PEM encoded CRT files to DER encoded format. PEM is a textual format utilizing base64 encoding. DER is a binary format. DER is part of the UEFI reference implementation. UEFI does not understand PEM, and most implementations do not support base64 decoding.

    openssl x509 -outform der -in PK.crt -out PK.cer
    openssl x509 -outform der -in KEK.crt -out KEK.cer
    openssl x509 -outform der -in DBK.crt -out DBK.cer

## Sign an EFI Binary or Bootloader
First, check to see if a signature is already present. Some utilities and implementations allow for binaries to have multiple signatures, but testing revealed that UEFI only understands the first signature. Assume that we are checking the signature of the Linux bootloader Shim.

    pesign -S -i=shimx64.efi

Second, remove a signature if one is present. Shim normally features a Microsoft UEFI Third Party Marketplace CA 2011 signature. Another bootloader, GRUB, may feature a signature from Microsoft, Red Hat, Canonical, or another source. For this example we will continue using Shim.

    pesign -r -i=shimx64.efi -o=shimx64.efi

Third, sign the binary using your DBK. In this example, DBK.key is the key created in section FIXME and DBK.crt was created in section FIXME. The code below assumes that sbsigntool outputs the signed Shim as shimx64.efi.signed. UEFI will not recognize a .signed file extension so the mv commands chang extensions.

    sbsign --key DBK.key --cert DBK.crt shimx64.efi
    mv shimx64.efi shimx64.efi.unsigned
    mv shimx64.efi.signed shimx64.efi

## Sign a Driver or Kernel Module
Signing a driver or kernel module is different than signing an EFI binary. In this example, a kernel module intended for use with Host Based Security System (HBSS) is to be signed.

    kmodsign sha512 DBK.key DBK.der hbss.ko

Some drivers and installation packages have their own signing mechanism. A mention of DKMS may be present. If so, use the DKB.key file where mentions of .priv or private key are found. Use the DBK.der file where certificate mentions are found.

## Add Certificate to Support Your Signatures (Partial Customization)
UEFI Secure Boot customization does not require a complete purge and replacement of all factory values. Partial customization can be utilized. Why leverage partial customization?

1. Make a driver work while Secure Boot is enabled. Some anti-malware products provide drivers that are not signed for use with Secure Boot and therefore are blocked from execution during run time. Don't disable Secure Boot! Sign the driver with your DBK private key and load your DBK certificate into the DB!

2. Add support for booting to a customized operating system. Normally used when organizations or power users compile their own Linux variants.

3. Add support for hypervisors. Some hypervisor products include a vendor certificate intended to be loaded into the Secure Boot DB. Treat the vendor certificate like a DBK.

### Use UEFI/BIOS Configuration/Setup
First, enter UEFI/BIOS Configuration/Setup. Some system vendors require the user to press a hotkey (such as F2 or F10) at boot, use a pinhole button, or have the operating system perform a restart to setup mode.

Second, find the security options. Look for settings relating to UEFI Secure Boot. Most systems ship with Secure Boot enabled and placed in the Standard Mode. Change the mode to User/Custom Mode. Some implementations may require a restart to perform this change.

Third, check the built-in customization options. Some implementations allow for users to set their own PK, KEK, DB, and DBX values. Some implementations do not expose the same functionality. Follow steps for your given system vendor:
- [Dell](#)
- [HP](#)
- [Intel](#)
- [Asus](#)
- [Lenovo](#)

### Use Keytool.efi
First, download a copy of efitools for your Linux distribution and compile the software. Alternatively, download only [Keytool.efi from this repository](#).

Second, place Keytool.efi and your DBK.cer file on a thumb drive, external drive, disc media, or within the EFI System Partition (/boot) directory.



### Use RACADM on Dell Servers

### Use Terminal Utilities
#### Ubuntu

    update-secureboot-policy

## Revoke a Signature
Reasons to revoke a signature:
- Your key signed something it shouldn't have.
- A previously signed binary should no longer be used.
- A vulnerability has been discovered in a signed binary.
- Revoking a signature makes more sense for your situation than revoking a certificate/key.

UEFI Secure Boot does not understand Certificate Revocation Lists (CRLs). Likewise, Secure Boot has no mechanism to download and parse revocation information at boot time.

### Use UEFI/Bios Configuration/Setup

### Use Keytool.efi

### Use Dell RACADM

### Use Terminal Utilities
#### Ubuntu

    update-secureboot-policy

## Revoke or Remove a Certificate/Key
Revoking a certificate/key will revoke **everything** validated by that certificate/key. For example, if Microsoft revoked the Windows Production CA certificate, then multiple generations of Windows signed by that key would fail to validate Secure Boot. Use certificate/key revocation when a large number of objects need to be distrusted or a certificate/key is to be retired. Utilize signature revocation when a small number of objects need to be distrusted and a certificate/key will continue to be used.

Revoking a certificate/key is different than revoking a signature. Simply delete the certificate from the Secure Boot DB or KEK. There is no need to move the certificate to the DBX. Any DBX entries relating to signatures revoked for a distrusted certificate may also be removed.

### Use UEFI/Bios Configuration/Setup

### Use Keytool.efi

### Use Dell RACADM

### Use Terminal Utilities
#### Ubuntu

    update-secureboot-policy
