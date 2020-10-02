# UEFI Secure Boot Customization for Windows
- 1\. Recipes
  - 1.1. Trust a Driver
  - 1.2. Distrust a Driver
  - 1.3. Trust a Boot Component
  - 1.4. Distrust a Boot Component
  - 1.5. Edit PK, KEK, DB, or DBX
- 2\. Scripts and Commands
	- 2.1. Create Certificates and Keys
	- 2.2. Convert from PEM to DER
	- 2.3. Sign an EFI Binary or Bootloader
	- 2.4. Sign a Driver
	- 2.5. Create Hashes
	- 2.6. Create EFI Signature List (ESL)
	- 2.7. Extract Certificates and Hashes from an ESL
	- 2.8. Backup Secure Boot Values
	- 2.9. Check a Signature
	- 2.10. Remove a Signature
- 3\. Examples
  - 3.1. Trust or Distrust a Driver
  - 3.2. Modify Hyper-V VM Secure Boot Values
  - 3.3. Dual Boot a Custom Linux Distribution
  - 3.4. Mitigate BootHole
  
## 1\. Recipes
### 1.1. Trust a Driver

### 1.2. Distrust a Driver
Windows drivers are usually signed by one of several trusted sources. Some infrastructure administrators may detect faults, instability, vulnerabilities, user errors, or other issues that necessitate blocking a driver. To block a trusted driver at boot time, create a SHA-256 hash of the driver. Load the hash into Secure Boot's DBX.

### 1.3. Trust a Boot Component
The Windows boot manager includes a signature from Microsoft. The signature is compatible with the standard Secure Boot implementation offered by most vendors. Some users may want to add dual-boot support to allow the use of Linux on the same system as Windows. Other users may want to take advantage of type-1 hypervisor products that must execute at boot time.

Boot components that change frequently should be signed. The certificate used to validate signatures should be placed in the DB (except when using Linux Shim; then MOK is an option). Boot components that rarely change may be hashed -- SHA-256 -- and the hash placed in the DB.

### 1.4. Distrust a Boot Component

### 1.5. Edit PK, KEK, DB, or DBX

## 2\. Scripts and Commands
### 2.1. Create Certificates and Keys
```
makecert -n "CN=Custom KEK" -a sha256 -r -sv KEK.pvk KEK.cer
makecert -n "CN=Custom DBK" -a sha256 -r -sv DBK.pvk DBK.cer
```

### 2.2. Convert from PVK to PFX
```
pvk2pfx -pvk KEK.pvk -spc KEK.cer -pfx KEK.pfx -f
pvk2pfx -pvk DBK.pvk -spc DBK.cer -pfx DBK.pfx -f
```

### 2.3. Sign an EFI Binary or Bootloader


### 2.4. Sign a Driver

### 2.5. Create Hashes
```
get-filehash -algorithm SHA256 shimx64.efi | select -ExpandProperty hash > shimx64.txt
$hashString = get-filehash -algorithm SHA256 shimx64.efi | select -ExpandProperty hash
$hashBytes = [byte[]]::new($hashString.length / 2)
For($i=0; $i -lt $hashString.length; $i+=2) {
	$hashBytes[$i/2] = [convert]::ToByte($hashString,Substring($i, 2), 16)
	}
$hashBytes | set-content shimx64.hsh -encoding byte
```

### 2.6. Create EFI Signature List (ESL)

### 2.7. Extract Certificates and Hashes from an ESL

### 2.8. Backup Secure Boot Values
```
Get-SecureBootUEFI -Name PK -OutputFilePath PK.esl
Get-SecureBootUEFI -Name KEK -OutputFilePath KEK.esl
Get-SecureBootUEFI -Name DB -OutputFilePath DB.esl
Get-SecureBootUEFI -Name DBX -OutputFilePath DBX.esl
```

### 2.9. Check a Signature

### 2.10. Remove a Signature

## 3\. Examples
### 3.1. Trust or Distrust a Driver

### 3.2. Modify Hyper-V VM Secure Boot Values

### 3.3. Dual Boot a Custom Linux Distribution

### 3.4. Mitigate BootHole
