



# Linux Secure Boot Customization
- 1\. Recipes
	- 1.1. Trust a Kernel Module
	- 1.2. Distrust a Kernel Module
	- 1.3. Trust a Boot Component (Bootloader, Kernel)
	- 1.4. Distrust a Boot Component (Bootloader, Kernel)
	- 1.5. Trust a Certificate via DB or MOK
	- 1.6. Trust a Hash via DB or MOK
	- 1.7. Distrust a Certificate via DBX or MOKX
	- 1.8. Distrust a Hash via DBX or MOKX
- 2\. Scripts and Commands
	- 2.1. Create Certificates and Keys
	- 2.2. Convert from PEM to DER
	- 2.3. Sign an EFI Binary or Bootloader
	- 2.4. Sign a Driver or Kernel Module
	- 2.5. Create Hashes
	- 2.6. Create EFI Signature List (ESL)
	- 2.7. Extract Certificates and Hashes from an ESL
	- 2.8. Backup Secure Boot Values
	- 2.9. Check a Signature
	- 2.10. Remove a Signature
- 3\. Examples
	- 3.1. Trust McAfee Kernel Modules via MOK
	- 3.2. Distrust CHIPSEC Kernel Module via DBX
	- 3.3. Compile a Custom Linux Distribution with Secure Boot Support

## 1\. Recipes
### 1\.1. Trust a Kernel Module
Many Linux distributions utilize driver signing when Secure Boot is enabled. Unsigned drivers will be blocked from execution if no signature or an untrusted signature is present. Many anti-malware solutions ship without a Linux-compatible signature or feature a signature created by the solution vendor.

Always prefer to sign kernel modules and drivers that are intended for use. Signatures are easy to rapidly update without needing to change the DB or MOK. SHA-256 hashes may also work for some implementations.

To add Secure Boot support for a kernel module that does not have a signature, follow these steps:

1. Create an RSA-2048 certificate and private key.
1. Convert the certificate into DER/CER format for use with DB or MOK.
1. Sign the kernel module using kmodsign and the private key.
1. Append the certificate to DB or MOK.

To add Secure Boot support for a kernel module that is signed by the vendor:

1. Locate the vendor's certificate. The vendor should be able to provide an RSA-2048 public key certificate.
1. If the certificate is in PEM/CRT format, convert it to DER/CER format.
1. Append the certificate to DB or MOK.

To add Secure Boot support for a kernel module via hashing:

1. Capture a SHA-256 hash of the module.
2. Create an ESL file.
3. Append the ESL hash record to the DB or MOK.

### 1\.2. Distrust a Kernel Module
Revocation of a previously trusted kernel module is normally handled via hashing. A SHA-256 hash of the revoked module is necessary and must be placed in the DBX or MOKX. Hashes are specific to a version or build of a given module to allow for targeted revocation.

Alternatively, a certificate can be revoked. Revoking the certificate will distrust **all** content holding a signature that can be validated with the certificate. Revoking a certificate normally involves deleting it from the DB or MOK instead of moving it to the DBX or MOKX.

To distrust a kernel module via SHA-256 hash:

1. Identify the specific build of kernel module to revoke.
2. Perform a pesign hash calculation.
3. Append the hash to the DBX or MOKX.

To distrust a certificate:

1. Delete the certificate from the DB or MOK.

### 1\.3. Trust a Boot Component (Bootloader, Kernel)
Some operating systems and hypervisors ship without Secure Boot signatures -- especially older products. Custom compilations of Linux typically also do not support Secure Boot due to the lack of a distribution vendor signature. There are multiple ways to add trust.

Is the object to be trusted going to change periodically? Does the object need to be distributed with minimal administrative engagement? A signature is normally the best bet. To use a signature to trust a boot component:

1. Create an RSA-2048 certificate and private key.
2. Convert the certificate into DER/CER format for use with DB or MOK.
3. Sign the bootloader, kernel, or other boot executable using sbsign the private key.
4. Append the certificate to DB or MOK.

Is the object to be trusted relatively static? Kernels and complex, configuration-interpreting bootloaders like GRUB are likely to change. However, some boot components -- such as Shim and purpose-built EFI applications -- may stay static for months or years at a time. If the boot object is mostly static, consier hashing an option by following these steps:


### 1\.4. Distrust a Boot Component (Bootloader, Kernel)
There are many reasons to distrust a boot component. Revocation can be used as a mechanism to prevent older versions of software from executing. Revocation can also be used to protect a system against compromised or defective boot components (e.g. [BootHole](https://eclypsium.com/2020/07/29/theres-a-hole-in-the-boot/)). The loss, misuse, retirement, or compromise of a private key may also necessitate distrusting a formerly trusted boot component.

To revoke a specific boot component or small selection of boot components, consider using DBX or MOKX deny list hashes -- an approach most effective when the list of compromise signed-objects is known. Other objects signed by the same private key will not be affected. Using hashes to revoke individual signatures is a targeted way to revoke untrusted boot components. Carry out the following steps:

1. Identify which components are to be distrusted.
2. Create a SHA-256 hash of each component.
3. Load each hash into the DBX if execution happens prior to Shim. Execution after Shim provides the option of leveraging MOKX.

Sometimes the number of items to be revoked exceeds the amount of memory available to Secure Boot. Also, sometimes a private key is compromised meaning signatures from it can no longer be trusted. Both situations necessitate the revocation of a certificate associated with validating problematic signatures or associated with a compromised private key. To revoke a certificate:

1. Create a replacement RSA-2048 certificate and private key if needed.
2. Update signatures on trustworthy boot components if needed.
3. Examine the full scope of certificate's revocation will have. Was the associated private key responsible for signing drivers, firmware binaries, or other components beyond the scope of a traditional bootloader or kernel? How old is the certificate and how many weeks, months, years, or decades of content could be disrupted?
4. Remove the certificate from the DB/MOK allow list. Moving the certificate to the DBX/MOKX deny list is not necessary.

### 1\.5. Trust a Certificate via DB or MOK
Systems with UEFI Secure Boot have access to the DB allow list at all times. Most firmware and software that relies upon Secure Boot can read the KEK, DB, and DBX. Some software -- such as the Shim bootloader -- extends Secure Boot to add MOK and MOKX. All existing Secure Boot values and value stores are retained. MOK and MOKX and the values contained within are added as an extension of the standard Secure Boot implementation.

In general, use MOK when the following conditions are met:

1. The bootloader Shim is in use.
2. The certificate or hash is being used to validate a boot component that executes after Shim.
3. The certificate or hash is associated with a boot component, module, or driver that is part of the same OS image or virtual machine as Shim.

Secure Boot's DB allow list is stored within endpoint firmware. The value travels with the hardware, not the software image. In general, prefer use of the DB when the following is true:

1. The boot component, module, or driver executes before Shim or before MOK and MOKX are initialized.
2. The boot component relates to a piece of hardware or firmware that is specific to the endpoint.

After determining the correct location to store the certificate, perform the following actions:

1. Acquire or create an RSA-2048 certificate and private key.
2. Add the certificate to the DB/MOK. Use the append option to add on to the existing values. Omitting append usually clears all values in the DB/MOK and installs only the new certificate.
3. Sign content with the private key and test that the certificate can validate signed binaries while Secure Boot is enforcing.

### 1\.6. Trust a Hash via DB or MOK
Trusting a hash is similar to trusting a certificate. See section 1.5 above for an explanation of when to choose DB or MOKX to store the hash. In general, prefer the use of certificates for content that changes or updates frequently; prefer the use of hashes for comparatively static information. Follow these steps to trust a certificate:

1. Calculate a SHA-256 hash of a boot component, firmware binary, module, driver, or other executable scrutinized by Secure Boot.
2. Add the hash to the DB/MOK. Use the append option to extend the existing list of certificates and hashes. Omit the append option to wipe out the existing values.

### 1\.7. Distrust a Certificate via DBX or MOKX
To distrust a certificate, simply remove the certificate from the DB or MOK. Moving the certificate to the DBX or MOKX isn't normally necessary. However, there are use cases in which moving a certificate to DBX or MOKX makes sense.

Consider the scenario of a live media disc that needs to distrust a certificate loaded into firmware on a machine. The untrusted certificate would be placed in Shim's MOKX. During boot, MOKX is checked first followed by DBX, MOK, and then DB. MOKX can invalidate a certificate stored in the DB.

1. Identify the certificate that is to be distrusted. Make sure it is in DER-encoded format (CER file).
2. Append the certificate to MOKX using mok-util. Do not place the certificate in DBX if it is already present in the DB.

Also consider the scenario of a machine that should not be used for a certain operating system or live media disc. The system would need to distrust the operating system distribution vendor certificate at the DBX level. DBX is checked prior to MOK and the DB.

1. Identify the certificate that is to be distrusted. make sure it is in DER-encoded format (CER file).
2. Append the certificate to DBX.

### 1\.8. Distrust a Hash via DBX or MOKX
Using a hash to distrust a boot executable avoids the need to revoke a certificate and the entire history of things signed by that certificate. Use distrusted hashes to selectively block exploitable binaries, disable risky drivers, stop older versions of software from executing, or to block unwanted boot services. To block a hash, follow these steps:

1. Compute a SHA256 hash of the binary to be blocked.
2. Append the hash to a Secure Boot deny list...
	1. If the binary executes before Shim, utilize the DBX. Prefer the DBX for blocking bootloaders and UEFI firmware, OROMs, services, hardware, etc.
	1. If the binary executes after Shim, utilize DBX or MOKX. Prefer MOKX when blocking kernels, kernel modules, and other highly privileged software.

## 2\. Scripts and Commands
### 2\.1. Create Certificates and Keys
The following commands create a KEK, and DB Signing Certificate (referred to as the DBK). The .crt files are certificates that contain a public key. The .key files are the private keys. Remember to protect the private key. Never store the private key on a production endpoint. The DBK will be used for signature operations. The KEK will be used in the event that the DBK needs to be revoked and replaced, or in the event records need to be added to the DBX.
```
openssl req -new -x509 -newkey rsa:2048 -subj "/CN=Custom KEK/" -keyout KEK.key -out KEK.crt -days 3650 -nodes -sha256
openssl req -new -x509 -newkey rsa:2048 -subj "/CN=Custom DBK/" -keyout DBK.key -out DBK.crt -days 3650 -nodes -sha256
```
Do not bother with Certificate Signing Requests (CSRs) because UEFI Secure Boot does not use them or understand certificate chains. Likewise, UEFI also does not understand Certificate Revocation Lists (CRLs) delivered over the internet.

### 2\.2. Convert from PEM to DER
The following commands convert the PEM encoded CRT files to DER encoded format. PEM is a textual format utilizing base64 encoding. DER is a binary format. DER is part of the UEFI reference implementation. UEFI does not understand PEM, and most implementations do not support base64 decoding.
```
openssl x509 -outform der -in KEK.crt -out KEK.cer
openssl x509 -outform der -in DBK.crt -out DBK.cer
```

### 2\.3. Sign an EFI Binary or Bootloader

First, check to see if a signature is already present. Some utilities and implementations allow for binaries to have multiple signatures, but testing revealed that UEFI only understands the first signature. Assume that we are checking the signature of the Shim bootloader.
```
pesign -S -i=shimx64.efi
```
Second, remove a signature if one is present. Shim normally features a Microsoft UEFI Third Party Marketplace CA 2011 signature. Another bootloader, GRUB, may feature a signature from Microsoft, Red Hat, Canonical, or another source. For this example we will continue using Shim.
```
pesign -r -i=shimx64.efi -o=shimx64.efi
```
Third, sign the binary using your DBK. In this example, DBK.key is the key created in section 2.1. The code below assumes that sbsigntool outputs the signed Shim as shimx64.efi.signed. UEFI will not recognize a .signed file extension so the mv commands chang extensions.
```
sbsign --key DBK.key --cert DBK.crt shimx64.efi
mv shimx64.efi shimx64.efi.unsigned
mv shimx64.efi.signed shimx64.efi
```

### 2\.4. Sign a Driver or Kernel Module

Signing a driver or kernel module is different than signing an EFI binary. In this example, a kernel module intended for use with an anti-malware product is to be signed. The same commands are commonly used for hypervisor drivers.
```
kmodsign sha512 DBK.key DBK.der antimalware.ko
```
Some drivers and installation packages have their own signing mechanism. A mention of DKMS may be present. If so, use the DKB.key file where mentions of .priv or private key are found. Use the DBK.der file where certificate mentions are found.

### 2\.5. Create Hashes
There are multiple ways to create SHA-256 hashes in Linux. Hexadecimal hashes are useful for use when creating ESL files. The following commands demonstrate two different ways to create a hex hash for the GRUB bootloader:
```
openssl dgst -sha256 -hex -out grubosl.txt grubx64.efi

pesign -h -i grubx64.efi | awk 'NF{print $NF}' > grubs2s.txt
```
Binary hashes may also be created. Some UEFI Configuration implementations expect binary hashes. Use the following commands to create a binary hash of the GRUB bootloader:
```
openssl dgst -sha256 -binary -out grubosl.hsh grubx64.efi
```
Finally, hashes of EFI binaries can be immediately converted into single-hash ESL files. See the following section for details.

### 2\.6. Create EFI Signature List (ESL)
To create an ESL made up of a single hash:
```
hash-to-efi-sig-list grubx64.efi grub.esl
```
To create an ESL made up of a single certificate:
```
cert-to-efi-sig-list DBK.crt dbk.esl
```
To sign an ESL with a KEK such that the ESL can authorize runtime DB, DBX, MOK, and MOKX updates:
```
sign-efi-sig-list -k KEK.key -c KEK.crt db dbk.esl dbupdate.auth
```
Multiple hashes or certificates may exist in a single ESL. However, tools to compile a list of hashes or certificates do not readily exist. Consider the anatomy of an ESL:
* 16 byte EFI_GUID identifying if SHA-256 hashes or x509 certificates are used
* 4 byte UINT32 overall file size (a file containing 1 hash results in 76 bytes of data, 2 hashes 124 bytes of data, 3 hashes 172 bytes of data, and so on)
* An array of individual records matching the following structure:
	* 4 byte UINT32 signature header size (normally zeroed out)
	* 4 byte UINT32 signature + originator UUID size (set to 48 in the case of a SHA-256 hash)
	* 16 byte originator UUID
	* Payload

### 2\.7. Extract Certificates and Hashes from an ESL
To extract certificates from an ESL:
```
sig-list-to-certs db.backup.esl db
```
No tool or script exists in OS packages to readily extract hashes from an ESL. Consult the anatomy of an ESL outlined in section 2.6 above. Alternatively, try the following script:
```
#!/bin/bash
# FIXME
```

### 2\.8. Backup Secure Boot Values
Always create a copy of Secure Boot values before engaging in customization. Some firmware implementations can restore the factory default values, and some do not. Likewise, not every firmware image provided by a system vendor has the complete listing of factory-provided Secure Boot values.

Secure Boot values vary based on make, model, and firmware version. Some values may also differ based on OS patches -- particularly UEFI capsule updates provided as part of the Windows Update infrastructure. To output the current Secure Boot values to ESL files, use the following commands:
```
efi-readvar -v PK -o PK.old.esl
efi-readvar -v KEK -o KEK.old.esl
efi-readvar -v db -o db.old.esl
efi-readvar -v dbx -o dbx.old.esl
```

### 2\.9. Check a Signature
Pesign is used to check signatures. The signer's common name provides insight into which specific certificate and key pair was used to attach a signature. Some executables may feature multiple signatures. To check a signature on shimx64.efi, use the example code:
```
pesign -S -i=shimx64.efi
```

### 2\.10. Remove a Signature
Removing a signature may be necessary when changes are made to the certificates trusted by Secure Boot. In the below example. shimx64.efi's Microsoft signature will be removed (assuming the shimx64.efi provided by a mainstream Linux distribution is used). Removing a different vendor's signature may also be necessary when implementing Secure Boot customization. Only the first signature -- in the event multiple signatures are present -- is utilized by Secure Boot.

To remove a signature on the example shimx64.efi, use the following command:
```
pesign -r -i=shimx64.efi -o=shimx64.efi
```

## 3\. Examples
### 3.1. Trust McAfee Kernel Modules via MOK
Assume that the organization is using McAfee anti-malware products that lack Secure Boot signatures. Two kernel modules are provided which require the addition of signatures. The following script generates a signing key, certificates for use with other scripts and UEFI configuration, and an ESL file for use with Keytool.efi. The signing function in the below script signs 2 McAfee kernel modules that may lack Secure Boot signatures.

```
#!/bin/bash
if [ "$1" == "setup" ]; then
	# Create a signing key (do this once)
	echo "Create private key and public key PEM certificate..."
	openssl req -new -x509 -newkey rsa:2048 -subj "/CN=Custom DBK/" -keyout dbk.key -out dbk.crt -days 3650 -nodes -sha256

	echo "Create DER certificate for use with UEFI Configuration..."
	openssl x509 -outform der -in dbk.crt -out dbk.cer

	echo "Create EFI Signature List for use with Keytool..."
	UUID=$(uuidgen)
	cert-to-efi-sig-list -g $UUID dbk.crt dbk.esl

elif [ "$1" == "mok" ]; then
	# Adding the DBK to the DB should be done via UEFI configuration or Keytool. The following terminal command will probably fail
	# efi-updatevar -e -f dbk.esl db -a

	# Add the new DBK to the Shim MOK (do this once iff not using DB)
	echo "Add certificate to MOK..."
	sudo mokutil --import dbk.cer

elif [ "$1" == "sign" ]; then
	# Sign the kernel modules (do this on every update)
	echo "Sign fileaccess_mod.ko..."
	kmodsign sha256 dbk.key dbk.cer fileaccess_mod.ko

	echo "Sign mfeaack.ko..."
	kmodsign sha256 dbk.key dbk.cer mfeaack.ko

else
	echo "Use arguments setup (once), mok (once iff not using DB), or sign."
	echo "   setup results in dbk.key .crt .cer .esl"
	echo "   mok adds the dbk.cer to Shim's Machine Owner Key allow list (confirm next boot)"
	echo "   sign results in signed copies of fileaccess_mod.ko and mfeaack.ko"
fi
```

### 3.2. Distrust CHIPSEC Kernel Module via DBX
```
#!/bin/bash
# Create a SHA256 hash of the CHIPSEC kernel module
HASH=`sha256sum < chipsec.ko | awk '{ print $1 }'`

# Create TXT file
echo $HASH >> chipsec.txt

# Write an HSH file for use with UEFI or as a binary input
echo $HASH | xxd -r -p - chipsec.hsh

# Write an ESL file for use with Keytool
EFI_GUID='2616C4C14C509240ACA941F936934328'
ListSize='4C000000'
HeaderSize='00000000'
SignatureSize='30000000'
UUID=$(uuidgen)
ESL=$EFI_GUID$ListSize$HeaderSize$SignatureSize$UUID$HASH
echo $ESL >> chipsec.esl

# Add the hash to DBX via UEFI configuration or Keytool because the following terminal command will probably fail
# efi-updatevar -e -f chipsec.esl dbx -a
# Alternatively, add the hash to MOKX
mokutil --import-hash --mokx -f chipsec.hsh
```

### 3.3. Compile a Custom Linux Distribution with Secure Boot Support
A custom Linux distribution or "spin" may involve 1) a custom bootloader, 2) a custom kernel, and 3) modified kernel modules/drivers. In some cases, the pre-bootloader Shim may also be modified or absent. To add Secure Boot support, consult the following steps:

1. Identify the first bootloader. Some implementations use Shim, GRUB, Syslinux, isolinux, rEFInd, or a mixture of bootloaders -- usually Shim + a bootloader. Check the signature for the first bootloader. If the signature is validated by a certificate in the DB allow list, then no additional action is necessary. If not, then the bootloader must be signed by a key whose certificate is placed in the DB, or the bootloader must have its hash placed in the DB.
2. Identify the second bootloader, if present. Shim + GRUB is the most common implementation. Shim sets up MOK and MOKX. A distribution vendor certificate is normally included with Shim as part of the MOK. The vendor cert may be able to validate signatures placed on the second bootloader (GRUB in this case), the kernel, and kernel modules. A distribution vendor certificate may also be placed in the DB, but this is uncommon.
	- **How to choose between DB and MOK:** DB/DBX exists in an individual endpoint's firmware implementation. Endpoints must be individually configured to have the same DB/DBX configuration. MOK/MOKX exists as part of Shim software. This means that MOK/MOKX has an element of portability not present with DB/DBX. For example, MOK/MOKX values on a live media disc are the same regardless of endpoint configuration.
3. Identify the kernel. Does the kernel have a signature? Kernels straight from distribution repositories usually have signatures that can be validated by the distribution vendor's certificate. Custom-compiled kernels usually lack signatures unless a key is provided at compile time. The same key certificate pair can be used to sign and validate both a bootloader(s) and kernel. The certificate can be placed in MOK (if using Shim) or DB.
4. Identify kernel modules. Are the modules signed by a trusted key certificate pair? Kernel modules will need to be signed by a trusted key certificate pair. The key may be provided at compile time to automate the process.
