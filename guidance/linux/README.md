## Mitigation

The best defensive against Spectre and Meltdown and other n-days 
is to choose an Operating System with good
support and to apply provided updates as quickly as possible.

### CVE-2017-5754 (Meltdown): rogue data cache load(
CVE-2017-5754 and other similar types of KASLR bypass attacks were addressed by the Linux kernel developers in January 2018.
The mitigation is known as Kernel Page Table Isolation (KPTI, or simply, PTI) and it discontinues mapping the entire kernel image into userspace, so that sensitive kernel memory is protected from
user processes and not simply hidden.
KPTI comes at a cost of more Translation Lookaside Buffer accesses and thus system slower performance.
Many mainline distributions have already incorporated these changes into their supported versions protecting them from Meltdown.

Although Meltdown primarily affects Intel and ARM processors, KPTI mitigates other KASLR-bypass attacks and should be employed to all systems regardless of chipset if performance is not an issue.

### Spectre

While disabling branch prediction would completely rectify Spectre attacks, the catastrophic degredation on performance is generally deemed unacceptable.
Current defense techniques center around risk minimalization rather than elimination.
A permanent fix to Spectre requires a redesign of branch predictor and speculative code execution mechanisms in the firmware.

#### CVE-2017-5715 (Spectre variant 2): branch target injection
For CVE-2017-5715, short-term mitigation efforts are coalescing around a technique called
["Retpoline"](https://support.google.com/faqs/answer/7625886) devised by Google.
Retpoline is a compile time feature that mitigates CVE-2017-5715 by coverting indirect branches to call and return thunks to avoid speculative execution via indirect calls and jmps.
GCC 8 has implemented this feature by adding specific flags outlined in the following [patch notes.](https://gcc.gnu.org/ml/gcc-patches/2018-01/msg00422.html).
Recompilation is required in order to take advantage of these new compiler flags.
Since this mitigation involves much more than a simple patch to the OS, it has been much slower to productize and distribute.

#### CVE-2017-5753 (Spectre variant 1): bounds check bypass
For CVE-2017-5753, the Linux community does not yet appear to be coalescing around a single mitigation technique.
This vulnerability affects specific sequences within compiled applications, and are being addressed on a per-executable basis.

## Detection
For Specter and Meltdown, several detection tools have been developed and published on the Internet:
"spectre-meltdown-checker" by the _speed47_ team available at https://github.com/speed47/spectre-meltdown-checker
seems to be the most popular.


