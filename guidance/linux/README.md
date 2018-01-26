## Linux
Generally the best mitigation against Spectre and Meltdown and other 
known vulnerabilities is to choose an Operating System with good 
support and leverage that support by applying provided updates as quickly as possible.

Meltdown (and other KASLR bypass attacks) was addressed by the Linux 
kernel developers in January 2018.
The mitigation is known as 
[Kernel Page Table Isolation](https://en.wikipedia.org/wiki/Kernel_page-table_isolation) 
(KPTI, or simply, PTI) and it discontinues mapping the entire
kernel image into userspace, so that sensitive kernel memory is protected from 
user processes and not simply hidden.
KPTI comes at a cost of more Translation Lookaside Buffer flushes (in 
order to access and cache the now-separate kernel memory) and thus 
slower performance.
Many mainline distributions have already incorporated these changes into their 
supported versions and are now largely safe from Meltdown.
Although Meltdown only affects Intel-based processors, KPTI helps 
mitigate other KASLR-bypass attacks and should be employed to all systems regardless of chipset if 
performance is not an issue.

Spectre, on the other hand, has not been completely addressed yet by the Linux community,
but short-term mitigation efforts are coalescing around a technique called 
["Retpoline"](https://support.google.com/faqs/answer/7625886) that devised by Google.
Retpoline works by patching critical executables (kernel, hypervisors, 
system programs, browsers, and other applications) such that indirect 
branches cannot be manipulated to cause speculative mechanisms to 
cache privileged memory reliably.
Since this mitigation impacts much more than the OS, it has been much slower to productize and distribute.
While disabling branch prediction would completely rectify Spectre attacks, the  
catastrophic degredation on performance is generally deemed unacceptable.
A permanent fix to Spectre requires a redesign of branch predictor and 
speculative code execution mechanisms in the firmware.

For Specter and Meltdown, several detection tools have been developed and published on the Internet:
"spectre-meltdown-checker" by _speed47_ team available at https://github.com/speed47/spectre-meltdown-checker
seems to be the most popular.
