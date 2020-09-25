# Use Case: Plug and play compatibility

Some devices and drivers may lack signatures that can be validated by Secure Boot. Examples of devices likely to be affected include:

- Video cards
- Storage controllers
- Storage interface adapter cards
- Network controllers
- Live media
- Anti-malware solution drivers
- Anti-theft boot binaries
- Hypervisors
- Older operating systems
- USB token-based system integrity tools

Many incompatible devices advise users to disable Secure Boot. Instead of disabling Secure Boot and losing access to mechanisms that
validate dozens, potentially hundreds, of binaries, consider adding support for the incompatible device(s).

If the incompatible device features a boot-time EFI binary or run-time driver executable, consider creating an RSA 2048 key pair,
signing the affected object using the private key, and storing the associated public key certificate in the Secure Boot DB. The
signature operation usually does not require modification to the vendor's source code, recompilation, or naming and path changes.

If the incompatible device features firmware that cannot be modified, then contact the vendor for a SHA-256 hash to store in the
Secure Boot DB. Alternatively, many vendors sign products thought to be incompatible and don't expect end-users or administrators
to understand how to install their certificate to the DB.

Examples of specific components that can be made compatible with Secure Boot:

- Custom-compiled Linux kernels and live media
- Windows 7 in UEFI boot mode
- Versions of Linux that don't advertise Secure Boot support
- NVME/PCIe storage controller adapters for older motherboards
- Hypervisors
