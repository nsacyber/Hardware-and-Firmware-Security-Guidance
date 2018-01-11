# Performance Impacts from Meltdown and Spectre Mitigations

Initial reports, based on microbenchmarks, lead to a performance impact of up to 30% being commonly cited. This page provides vendor statements about performance.

## Vendor statements

### Google

Link: https://security.googleblog.com/2018/01/more-details-about-mitigations-for-cpu_4.html

#### Statement
There has been speculation that the deployment of KPTI causes significant performance slowdowns. Performance can vary, as the impact of the KPTI mitigations depends on the rate of system calls made by an application. On most of our workloads, including our cloud infrastructure, we see negligible impact on performance.

In our own testing, we have found that microbenchmarks can show an exaggerated impact. Of course, Google recommends thorough testing in your environment before deployment; we cannot guarantee any particular performance or operational impact.


### Apple

https://support.apple.com/en-us/HT208394

**Meltdown**

Our testing with public benchmarks has shown that the changes in the December 2017 updates resulted in no measurable reduction in the performance of macOS and iOS as measured by the GeekBench 4 benchmark, or in common Web browsing benchmarks such as Speedometer, JetStream, and ARES-6.

**Spectre**

Our current testing indicates that the Safari mitigations have no measurable impact on the Speedometer and ARES-6 tests and an impact of less than 2.5% on the JetStream benchmark.

### Microsoft

#### Link
* https://cloudblogs.microsoft.com/microsoftsecure/2018/01/09/understanding-the-performance-impact-of-spectre-and-meltdown-mitigations-on-windows-systems/

#### Statement
One of the questions for all these fixes is the impact they could have on the performance of both PCs and servers. It is important to note that many of the benchmarks published so far do not include both OS and silicon updates. We’re performing our own sets of benchmarks and will publish them when complete, but I also want to note that we are simultaneously working on further refining our work to tune performance. In general, our experience is that Variant 1 and Variant 3 mitigations have minimal performance impact, while Variant 2 remediation, including OS and microcode, has a performance impact.

Here is the summary of what we have found so far:

* With Windows 10 on newer silicon (2016-era PCs with Skylake, Kabylake or newer CPU), benchmarks show single-digit slowdowns, but we don’t expect most users to notice a change because these percentages are reflected in milliseconds.
* With Windows 10 on older silicon (2015-era PCs with Haswell or older CPU), some benchmarks show more significant slowdowns, and we expect that some users will notice a decrease in system performance.
* With Windows 8 and Windows 7 on older silicon (2015-era PCs with Haswell or older CPU), we expect most users to notice a decrease in system performance.
* Windows Server on any silicon, especially in any IO-intensive application, shows a more significant performance impact when you enable the mitigations to isolate untrusted code within a Windows Server instance. This is why you want to be careful to evaluate the risk of untrusted code for each Windows Server instance, and balance the security versus performance tradeoff for your environment.

For context, on newer CPUs such as on Skylake and beyond, Intel has refined the instructions used to disable branch speculation to be more specific to indirect branches, reducing the overall performance penalty of the Spectre mitigation. Older versions of Windows have a larger performance impact because Windows 7 and Windows 8 have more user-kernel transitions because of legacy design decisions, such as all font rendering taking place in the kernel. We will publish data on benchmark performance in the weeks ahead.

### Red Hat

#### Link
* https://access.redhat.com/articles/3307751

#### Statement
Red Hat has tested complete solutions, including updated kernels and updated microcode, on variants of the following modern high volume Intel systems: Haswell, Broadwell, and Skylake. In each instance, there is performance impact caused by the additional overhead required for security hardening in user-to-kernel and kernel-to-user transitions. The impact varies with workload and hardware implementation and configuration. As is typical with performance, the impact can be best characterized by sharing a range between 1-20% for the ISB set of application workloads tested.

In order to provide more detail, Red Hat’s performance team has categorized the performance results for Red Hat Enterprise Linux 7, (with similar behavior on Red Hat Enterprise Linux 6 and Red Hat Enterprise Linux 5), on a wide variety of benchmarks based on performance impact:

* Measureable: 8-19% - Highly cached random memory, with buffered I/O, OLTP database workloads, and benchmarks with high kernel-to-user space transitions are impacted between 8-19%. Examples include OLTP Workloads (tpc), sysbench, pgbench, netperf (< 256 byte), and fio (random I/O to NvME).

* Modest: 3-7% - Database analytics, Decision Support System (DSS), and Java VMs are impacted less than the “Measurable” category. These applications may have significant sequential disk or network traffic, but kernel/device drivers are able to aggregate requests to moderate level of kernel-to-user transitions. Examples include SPECjbb2005, Queries/Hour and overall analytic timing (sec).

* Small: 2-5% - HPC (High Performance Computing) CPU-intensive workloads are affected the least with only 2-5% performance impact because jobs run mostly in user space and are scheduled using cpu-pinning or numa-control. Examples include Linpack NxN on x86 and SPECcpu2006.

* Minimal: Linux accelerator technologies that generally bypass the kernel in favor of user direct access are the least affected, with less than 2% overhead measured. Examples tested include DPDK (VsPERF at 64 byte) and OpenOnload (STAC-N). Userspace accesses to VDSO like get-time-of-day are not impacted. We expect similar minimal impact for other offloads.

* NOTE: Because microbenchmarks like netperf/uperf, iozone, and fio are designed to stress a specific hardware component or operation, their results are not generally representative of customer workload. Some microbenchmarks have shown a larger performance impact, related to the specific area they stress


## The Role of PCID
Intel processors that support PCID reduce the performance impact of the mitigations. PCID support in Intel processors goes back to 2010 but PCID is only supported by 64-bit processors. Operating system and hypervisor support for PCID varies. Using modern hardware, operating system, and software running on 64-bit platforms results in the lowest performance impact.