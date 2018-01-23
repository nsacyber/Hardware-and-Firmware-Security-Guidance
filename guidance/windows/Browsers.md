# Windows browser guidance

Prioritize installation of browser patches since Javascript can be used as a mechanism for exploiting Spectre.

1. [Install browser patches](#install-browser-patches).
1. [Perform configuration changes](#perform-configuration-changes), if needed.
1. [Verify mitigations](#verify-mitigations).

## Install browser patches

### Edge and Internet Explorer
A [blog entry from the Edge team](https://blogs.windows.com/msedgedev/2018/01/03/speculative-execution-mitigations-microsoft-edge-internet-explorer/) states: "*Initially, we are removing support for SharedArrayBuffer from Microsoft Edge (originally introduced in the [Windows 10 Fall Creators Update](https://developer.microsoft.com/en-us/microsoft-edge/platform/changelog/desktop/16299/?compareWith=15063)), and reducing the resolution of performance.now() in Microsoft Edge and Internet Explorer from 5 microseconds to 20 microseconds, with variable jitter of up to an additional 20 microseconds. These two changes substantially increase the difficulty of successfully inferring the content of the CPU cache from a browser process.*"

The [Microsoft advisory](https://portal.msrc.microsoft.com/en-US/security-guidance/advisory/ADV180002) only lists patches being available for Internet Explorer 11 which is the only supported version of Internet Explorer for Windows desktop operating systems. Windows Server 2008 SP2 supports Internet Explorer 9 and Windows Server 2012 supports Internet Explorer 10. Patches do not appear to be available at this time for IE9 or IE10 on Windows server operating systems. See the **What is the Lifecycle Policy for Internet Explorer?** section of the [Internet Explorer Lifecycle FAQ](https://support.microsoft.com/en-us/help/17454/lifecycle-faq-internet-explorer) for more information.

Install all January 2018 patches released by Microsoft.

### Chrome

The [Chromium page](https://www.chromium.org/Home/chromium-security/ssca) states: "*Chrome has disabled SharedArrayBuffer on Chrome 63 starting on Jan 5th, and will modify the behavior of other APIs such as performance.now, to help reduce the efficacy of speculative side-channel attacks. This is intended as a temporary measure until other mitigations are in place.*"

Download ([32-bit](https://dl.google.com/edgedl/chrome/install/GoogleChromeStandaloneEnterprise.msi)/**[64-bit](https://dl.google.com/edgedl/chrome/install/GoogleChromeStandaloneEnterprise64.msi)**) and install Chrome [63.0.3239.132](https://chromereleases.googleblog.com/2018/01/stable-channel-update-for-desktop.html), or later, released on January 5, 2018. Install Chrome 64, or later, since it will have [additional mitigations](https://github.com/v8/v8/wiki/Untrusted-code-mitigations) when released on or around January 23, 2018. 

### Opera
Opera [blog entry]((https://blogs.opera.com/desktop/2018/01/opera-50-0-2762-67-stable-update/)) states: "*In today’s Opera 50 update we have a fix that blunts the main tool for the attack: The very high precision timer you get with performance.now(). After the change, performance.now() has a precision of 100μs and additionally it has received a small amount of randomness in it. That will make the attacks both much harder and less efficient. It is likely that the timer change is enough to make attacks inefficient enough to not at all be practical but we are going to implement further mitigations in Opera 51*".

Install [Opera 50.0.2762.67] released on January 22, 2018. Install Opera 51 when it is made available.

### Firefox

A [Mozilla blog entry](https://blog.mozilla.org/security/2018/01/03/mitigations-landing-new-class-timing-attack/) describes the Firefox mitigations:
* "*The resolution of performance.now() will be reduced to 20µs.*"
* "*The SharedArrayBuffer feature is being disabled by default.*"

[Download](https://www.mozilla.org/en-US/firefox/all/?q=English%20(US)) and install [Firefox 57.0.4](https://www.mozilla.org/en-US/firefox/57.0.4/releasenotes/), or later, released on January 4, 2018.

### Firefox ESR

Firefox 52 ESR has partial mitigations already in place. A [Mozilla blog entry](https://blog.mozilla.org/security/2018/01/03/mitigations-landing-new-class-timing-attack/) states: "*Firefox 52 ESR does not support SharedArrayBuffer and is less at risk; the performance.now() mitigations will be included in the regularly scheduled Firefox 52.6 ESR release on January 23, 2018.*"

[Download](https://www.mozilla.org/en-US/firefox/organizations/all/?q=English%20(US)) and install [Firefox 52.6 ESR](https://www.mozilla.org/en-US/firefox/52.6.0/releasenotes/), or later, when released on or around Janauary 23, 2018. 

## Perform configuration changes

Chromium-based browsers have additional configuration settings that may protect against exploitation of Spectre.

### Chrome

The [Site Isolation](http://www.chromium.org/Home/chromium-security/site-isolation) feature, introduced in Chrome 63, can also be used as a mitigation for Spectre. The Site Isolation page states: "**_Site Isolation is an experimental security feature in Chrome..._**". **Due to Site Isolation being an experimental feature with known issues, caution should be used in its adoption until local testing has been performed to determine the real world impact of the setting outside of increased memory use.** Site specific compatibility issues have been reported online. Upgrading to Chrome 64 is likely a more effective near term mitigation for Chrome. Site Isolation is an effective long term mitigation to consider once compatibility issues have been resolved. 

The Site Isolation feature is described as "*It ensures that pages from different websites are always put into different processes, each running in a sandbox that limits what the process is allowed to do.  It also blocks the process from receiving certain types of sensitive documents from other sites.*" Each web site will open in a new process resulting more processes existing and more memory being used. Users on systems that have a larger amount of RAM (>=8GB) may not notice a difference while users on systems with less RAM (4GB), and use RAM intensive applications, will likely notice a difference.

The Site Isolation page describes a number of known issues:
* "*Higher memory use (about 10-20% when isolating all sites with many tabs open). This overhead can be greatly reduced by only isolating certain sites, as noted below.*"
* "*When printing a page, cross-site iframes appear blank. To print the complete web page content, save the page locally, then open and print the saved file.*"
* "*In some cases, clicking or scrolling on cross-site iframes may not work properly. For example, this can happen when there is a partly transparent overlay above an iframe.*"
* "*When using the "Isolating all sites" approach below, Gmail may not load properly if started from the Gmail icon on chrome://apps. To work around this, visit https://mail.google.com rather than using the Gmail icon on chrome://apps.  (This is fixed in Chrome 64.)*"


To enable Site Isolation on a single system:
1. Visit **chrome://flags#enable-site-per-process** in the browser.
1. Click **Enable** next to **Strict site isolation**.
1. Restart the browser. 

To enable Site Isolation enterprise wide:
1. Download the latest Chrome [Group Policy templates](https://dl.google.com/dl/edgedl/chrome/policy/policy_templates.zip).
1. Extract the zip file.
1. Copy the **chrome.admx** and **google.admx** files to **C:\Windows\PolicyDefinitions** and copy the **chrome.adml** and **google.adml** files to **C:\Windows\PolicyDefinitions\en-us** on your Active Directory Domain Controller or to your [Group Policy Central Store](https://support.microsoft.com/en-us/help/3087759) location.
1. Open the Group Policy Management Console, navigate to the location containing the Group Policy object (GPO) that will contain the Site Isolation policy, and then click **Edit**.
1. In the console tree under **Computer Configuration**, expand the **Administrative Templates** folder, and then expand the **Google Chrome** folder.
1. Double click **Enable Site Isolation for every site**.
1. Select **Enabled** and click **OK**.

### Opera
Opera browser users can follow [similar guidance](https://blogs.opera.com/security/2018/01/opera-mitigates-critical-cpu-vulnerabilities/) for enabling Site Isolation since Opera is based on the same code base as Chrome: 

1. Visit **opera://flags/?search=enable-site-per-process** in the browser. 
1. Click **Enable**.
1. Restart the browser.

## Verify mitigations

Verify the latest version of each browser is installed. The only browsers that may require additional verification are Chromium-based browsers, such as Chrome or Opera, if the Site Isolation feature was enabled through Group Policy. The Site Isolation feature can be manually verified when configured through Group Policy:
1. Visit chrome://policy in the browser.
1. Look for a value of **[SitePerProcess](https://www.chromium.org/administrators/policy-list-3#SitePerProcess)** under the **Policy name** column and ensure the value of **true** is in the corresponding **Policy value** column.

Site Isolation can also be manually verified when configured through the Chrome flags page:
1. Visit chrome://flags#enable-site-per-process in the browser.
1. Find **Strict site isolation** which will have a blue circle, with a checkmark inside the circle, next to the words **Strict site isolation**. The blue button to the right will say **Disable** indicating the feature is enabled. If the button is clicked, then the feature will be disabled.

### Nessus
A custom Nessus audit file is available for scanning browsers on Windows operating systems: [Verify-Spectre-Meltdown-Mitigations-Windows-Browsers.audit](./../../verification/windows/Verify-Spectre-Meltdown-Mitigations-Windows-Browsers.audit). The custom audit file can be leveraged by using a [Policy Compliance](https://docs.tenable.com/nessus/7_0/Content/ScanAndPolicyTemplates.htm) scan. The audit file performs the following checks:
* PowerShell is installed.
* PowerShell version is at least 2.0.
* If Chrome is installed, then at least 63.0.3239.132 or newer is installed and that the Site Isolation registry value is correctly configured (optional).
* Internet Explorer 11 patch is installed.
* If Edge exists, then that the Edge patched is installed.
* If Firefox is installed, then that Firefox 57.0.4 or newer is installed.
* If Firefox ESR is installed, then that Firefox 52.6 or newer is installed.

