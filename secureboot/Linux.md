# Linux Secure Boot Customization
1. Recipes
	1. Trust a Kernel Module
	2. Revoke a Kernel Module
	3. Sign a Boot Component (Bootloader, Kernel)
	4. Hash a Boot Component (Bootloader, Kernel)
	5. Trust a Certificate via DB or MOK
	6. Trust a Hash via DB or MOK
	7. Distrust a Certificate via DBX or MOKX
	8. Distrust a Hash via DBX or MOKX
2. Scripts and Commands
	1. Create Certificates and Keys
	2. Convert from PEM to DER
	3. Sign an EFI Binary or Bootloader
	4. Sign a Driver or Kernel Module
	5. Create Hashes
	6. Create EFI Signature List (ESL)
	7. Backup Secure Boot Values
	8. Check a Signature
	9. Remove a Signature

## Recipes
### Trust a Kernel Module
Many Linux distributions utilize driver signing when Secure Boot is enabled. Unsigned drivers will be blocked from execution if no signature or an untrusted signature is present. Many anti-malware solutions ship without a Linux-compatible signature or feature a signature created by the solution vendor.

Always prefer to sign kernel modules and drivers that are intended for use. Signatures are easy to rapidly update without needing to change the DB or MOK. SHA-256 hashes may also work for some implementations.

To add Secure Boot support for a kernel module that does not have a signature, follow these steps:

1. Create an RSA-2048 certificate and private key.
1. Convert the certificate into DER/CER format for use with DB or MOK.
1. Sign the kernel module using the private key.
1. Append the certificate to DB or MOK.

To add Secure Boot support for a kernel module that is signed by the vendor:

1. Locate the vendor's certificate. The vendor should be able to provide an RSA-2048 public key certificate.
1. If the certificate is in PEM/CRT format, convert it to DER/CER format.
1. Append the certificate to DB or MOK.

To add Secure Boot support for a kernel module via hashing:

1. Capture a SHA-256 hash of the module.
2. Create an ESL file.
3. Append the ESL hash record to the DB or MOK.

### Revoke a Kernel Module
Revocation of a previously trusted kernel module is normally handled via hashing. A SHA-256 hash of the revoked module is necessary and must be placed in the DBX or MOKX. Hashes are specific to a version or build of a given module to allow for targeted revocation.

Alternatively, a certificate can be revoked. Revoking the certificate will distrust **all** content holding a signature that can be validated with the certificate. Revoking a certificate normally involves deleting it from the DB or MOK instead of moving it to the DBX or MOKX.

To distrust a kernel module via SHA-256 hash:

1. Identify the specific build of kernel module to revoke.
2. Perform a sha256sum or other hashing mechanism against the module.
3. Append the hash to the DBX or MOKX.

To distrust a certificate:

1. Delete the certificate from the DB or MOK.

### Sign a Boot Component (Bootloader, Kernel)

### Hash a Boot Component (Bootloader, Kernel)

### Trust a Certificate via DB or MOK

### Trust a Hash via DB or MOK

### Distrust a Certificate via DBX or MOKX

### Distrust a Hash via DBX or MOKX

## Scripts and Commands
### Create Certificates and Keys
The following commands create a KEK, and DB Signing Certificate (referred to as the DBK). The .crt files are certificates that contain a public key. The .key files are the private keys. Remember to protect the private key. Never store the private key on a production endpoint. The DBK will be used for signature operations. The KEK will be used in the event that the DBK needs to be revoked and replaced, or in the event records need to be added to the DBX.
```
openssl req -new -x509 -newkey rsa:2048 -subj "/CN=Custom KEK/" -keyout KEK.key -out KEK.crt -days 3650 -nodes -sha256
openssl req -new -x509 -newkey rsa:2048 -subj "/CN=Custom DBK/" -keyout DBK.key -out DBK.crt -days 3650 -nodes -sha256
```
Do not bother with Certificate Signing Requests (CSRs) because UEFI Secure Boot does not use them or understand certificate chains. Likewise, UEFI also does not understand Certificate Revocation Lists (CRLs) delivered over the internet.

### Convert from PEM to DER
The following commands convert the PEM encoded CRT files to DER encoded format. PEM is a textual format utilizing base64 encoding. DER is a binary format. DER is part of the UEFI reference implementation. UEFI does not understand PEM, and most implementations do not support base64 decoding.
```
openssl x509 -outform der -in KEK.crt -out KEK.cer
openssl x509 -outform der -in DBK.crt -out DBK.cer
```

### Sign an EFI Binary or Bootloader

First, check to see if a signature is already present. Some utilities and implementations allow for binaries to have multiple signatures, but testing revealed that UEFI only understands the first signature. Assume that we are checking the signature of the Shim bootloader.
```
pesign -S -i=shimx64.efi

```
Second, remove a signature if one is present. Shim normally features a Microsoft UEFI Third Party Marketplace CA 2011 signature. Another bootloader, GRUB, may feature a signature from Microsoft, Red Hat, Canonical, or another source. For this example we will continue using Shim.
```
pesign -r -i=shimx64.efi -o=shimx64.efi

```
Third, sign the binary using your DBK. In this example, DBK.key is the key created in section FIXME and DBK.crt was created in section FIXME. The code below assumes that sbsigntool outputs the signed Shim as shimx64.efi.signed. UEFI will not recognize a .signed file extension so the mv commands chang extensions.
```
sbsign --key DBK.key --cert DBK.crt shimx64.efi
mv shimx64.efi shimx64.efi.unsigned
mv shimx64.efi.signed shimx64.efi
```

### Sign a Driver or Kernel Module

Signing a driver or kernel module is different than signing an EFI binary. In this example, a kernel module intended for use with an anti-malware product is to be signed. The same commands are commonly used for hypervisor drivers.
```
kmodsign sha512 DBK.key DBK.der antimalware.ko
```
Some drivers and installation packages have their own signing mechanism. A mention of DKMS may be present. If so, use the DKB.key file where mentions of .priv or private key are found. Use the DBK.der file where certificate mentions are found.

### Create Hashes
There are multiple ways to create SHA-256 hashes in Linux. Hexadecimal hashes are useful for use when creating ESL files. The following commands demonstrate two different ways to create a hex hash for the GRUB bootloader:
```
openssl dgst -sha256 -hex -out grubosl.txt grubx64.efi
sha256sum grubx64.efi | cut -d " " -f 1 > grubs2s.txt
```
Binary hashes may also be created. Some UEFI Configuration implementations expect binary hashes. Use the following commands to create a binary hash of the GRUB bootloader:
```
openssl dgst -sha256 -binary -out grubosl.hsh grubx64.efi
```
Finally, hashes of EFI binaries can be immediately converted into single-hash ESL files. See the following section for details.

### Create EFI Signature List (ESL)

### Backup Secure Boot Values

### Check a Signature

### Remove a Signature
