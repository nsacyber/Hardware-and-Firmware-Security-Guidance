# Guidance

## General guidance
General guidance for prioritizing patching:
1. Prioritize patching applications, such as browsers first, as they are the easiest to patch, have the least amount of issues with performance and compatibility, and the most likely widespread attack vector.
1. Prioritize installing operating system patches on desktop, laptops, and tablets. Compatibility issues with operating system patches have been largely resolved by the OS vendors and performance issues are much less on desktops since they typically do not have IO intensive workloads like servers (file storage arrays, email servers, database servers) where the majority of the performance issues are excertbated. Attacks via email, Office documents, PDFs, are the second most likely widespread attack vector.
1. Prioritize patching servers that do NOT have IO intensive workloads (no file storage arrays, no email servers, no database servers). Some organizations may want to wait on patching any servers until more performance data is available or more localized testing has been performed to determine if the risk of remaining unpatched is warranted for the performance trade offs.
1. Do not install firmware patches until processor manufacturers (Intel, AMD, IBM, Arm, etc) and OEMs (Dell, Dell EMC, HP Inc, HP Enterprise, etc) have signaled the new firmware patches are ready.

## Application and operating system specific guidance
Guidance is currently being developed for operating systems and applications.

### Windows
Windows operating systems and applications guidance in development:
* [Operating system](./windows/OS.md) guidance
* [Browser](./windows/Browsers.md) guidance
* [Hyper-V hosts](./windows/Hyper-V.md) guidance

### Linux
* [Operating System](./linux/README.md) guidance
* See the [Browser guidance](./windows/Browsers.md) in the Windows section

## General information
Informational pages:
* [Patches](./Patches.md) - information on operating system, application, and firmware patches.
* [Performance](./Performance.md) - information from vendors on performance impacts of patches and mitigations.


