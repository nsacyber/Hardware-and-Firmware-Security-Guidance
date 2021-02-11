
# UEFI Secure Boot Customization for Windows
- 1\. Recipes
  - 1.1. Trust a Boot Component
  - 1.2. Distrust a Boot Component
  - 1.3. Edit PK, KEK, DB, or DBX
- 2\. Scripts and Commands
	- 2.1. Create Certificates and Keys
	- 2.2. Convert from PVK to PFX
	- 2.3. Sign an EFI Binary or Bootloader
	- 2.4. Create Hashes
	- 2.5. Create EFI Signature List (ESL)
	- 2.6. Extract Certificates and Hashes from an ESL
	- 2.7. Backup Secure Boot Values
	- 2.8. Check a Signature
	- 2.9. Remove a Signature
  
## 1\. Recipes
### 1.1. Trust a Boot Component
The Windows boot manager includes a signature from Microsoft. The signature is compatible with the standard Secure Boot implementation offered by most vendors. Some users may want to add dual-boot support to allow the use of Linux on the same system as Windows. Other users may want to take advantage of type-1 hypervisor products that must execute at boot time.

Boot components that change frequently should be signed. The certificate used to validate signatures should be placed in the DB (except when using Linux Shim; then MOK is an option). Boot components that rarely change may be hashed -- SHA-256 -- and the hash placed in the DB.

### 1.2. Distrust a Boot Component
To distrust a boot component, perform one of the following actions:
 - Remove the certificate that can validate the component's signature from the DB or MOK.
 - Remove the SHA-256 hash of the component from the DB or MOK.
 - Create a SHA-256 hash of the component and add the hash to the DBX or MOKX.

### 1.3. Edit PK, KEK, DB, or DBX
If a PK is not present, then Secure Boot must be in Setup Mode to allow initial values to be set. Administrators should place trusted hashes and certificates into the DB, untrusted hashes and certificates into the DBX, and place certificates for trustworthy update signers in the KEK. The PK certificate should be set last -- setting the PK locks out unsigned changes to the KEK, DB, and DBX.

Systems that already have a PK may perform runtime updates. Any changes to the KEK must be signed by the PK. Any changes to the DB or DBX must be signed by a KEK.

## 2\. Scripts and Commands
### 2.1. Create Certificates and Keys
The built-in makecert command can be used instead of OpenSSL. Use the following commands to create a KEK certificate and a DB certificate. CER denotes DER-encoded certificates. Keys are stored as PVK private keys.
```
makecert -n "CN=Custom KEK" -a sha256 -r -sv KEK.pvk KEK.cer
makecert -n "CN=Custom DBK" -a sha256 -r -sv DBK.pvk DBK.cer
```

### 2.2. Convert from PVK to PFX
Some tools require a PKCS#12 key format. The Windows PVK format may not be accepted. Convert the PVK files to PFX using the following commands.
```
pvk2pfx -pvk KEK.pvk -spc KEK.cer -pfx KEK.pfx -f
pvk2pfx -pvk DBK.pvk -spc DBK.cer -pfx DBK.pfx -f
```

### 2.3. Sign an EFI Binary or Bootloader
The following command signs the pre-bootloader, Shim. Instructing a Windows system to boot to Shim involves utilizing BCDEdit (see section 3).
```
signtool sign /f DBK.pfx /fd sha256 shimx64.efi
```

### 2.4. Create Hashes
The alternative to signing bootable content is to create a hash. Hashes must be in the SHA-256 format. EFI binaries are considered PE (Portable Executable) files that contain a DOS/WINNT header, obey a structural schema, and omit some information from the contents that should be hashed. Use PowerShell Get-AppLockerFileInformation to compute the correct hashes.

Earlier versions of this repository and the UEFI Secure Boot Customization technical report erroneously advise the use of sha256sum, get-filehash, and other whole-file hashing mechanisms. Only use solutions that are aware of the PE COFF file format and Microsoft Authenticode signing paradigms.
```
$hashString = Get-AppLockerFileInformation shimx64.efi | select -ExpandProperty hash | select -ExpandProperty HashDataString
$hashString.Trim("0x") > shimx64.txt
$hashBytes = [byte[]]::new($hashString.length / 2)
For($i=0; $i -lt $hashString.length; $i+=2) {
	$hashBytes[$i/2] = [convert]::ToByte($hashString,Substring($i, 2), 16)
	}
$hashBytes | set-content shimx64.hsh -encoding byte
```

### 2.5. Create EFI Signature List (ESL)
ESL files are particularly well-suited to storing multiple hashes. To create an ESL file containing multiple hashes, use this repository's [hex-hashes-to-esl.c](hex-hashes-to-esl.c) program. Not built-in utility within Windows is able to combine multiple SHA-256 hashes into an ESL. DER encoded certificates are normally recognized by EFI utilities and rarely need an ESL structure.

### 2.6. Extract Certificates and Hashes from an ESL
No built-in utility for processing ESL files exists on Windows. We have created [esl-parser.c](esl-parser.c) to assist. The ESL parser is designed for minimal dependencies and to parse ESLs created on Windows, Linux, KeyTool, or other platforms. Hashes output from UEFI configuration (files with format HSH) may also be supported.

### 2.7. Backup Secure Boot Values
Use PowerShell to extract the current Secure Boot values stored in the PK, KEK, DB, and DBX. The resulting files will be binary files in the EFI signature list (ESL) format.

```
Get-SecureBootUEFI -Name PK -OutputFilePath PK.esl
Get-SecureBootUEFI -Name KEK -OutputFilePath KEK.esl
Get-SecureBootUEFI -Name DB -OutputFilePath DB.esl
Get-SecureBootUEFI -Name DBX -OutputFilePath DBX.esl
```

### 2.8. Check a Signature
To see if an EFI binary has been signed, run the following command.

```
Get-AuthenticodeSignature -FilePath shimx64.efi
```

### 2.9. Remove a Signature
To remove an unwanted or unnecessary signature, run the following command. Removing an existing signature may be necessary prior to applying a new signature.

```
signtool remove /s shimx64.efi
```
