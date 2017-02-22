#Metrological

##Team Meeting Embedded
##Date: 2017-02-15

1. Team update

Pierre:
  * Had discussion with LGI on 1MVW
  * Worked on issues in webbridge to stabilize.
  * Worked on Qubieware box (CWC / Homecast)
  * Looked at some issues in Dawn (delay)
  * Looked at memory usage, it appears 8Mb of memory were allocated for the stack of each thread. It is now configurable 
  per thread how much stack to use.
  * Coen: Does the order in the JSON configuration matter? Answer: No, and if it does matter, raise it as a bug.

Coen:
  * Worked on RET with Mischa
  * Had some introduction from Pierre
  * Worked on network stack, now have a local network of RPi devices linked through Wifi
  * Will have a discussion on testing with Rene
  
Ozgur:
  * Worked on crash on DAWN in QtBrowser. It had to do with uclibc linkage with runtime. Now trying to find the cause 
  in the build system. They are using extra flags related to the toolchain. This may also interfere.
  * Support for demo of DAWN box. Manage to make some extra memory available through the boot parameters. Should share 
  this information on the Wiki. Intention is to show that we can run a full stack on DAWN / Nexus without third party 
  software.
  * There is an issue with sync between video and audio. Will check with Igalia to get more information. Issue is not 
  known on ARM platforms and EOS.
  * Started to work on Snapshot plugin crash (when frequently pressing the button)
  
Mischa:
  * Worked on RET, able to decrypt images
  * Instrumentation ongoing
  
Bram:
  * Working on webbridge with NXResourceCenter (HAl for Nexus). Issue is waiting for NX layer to become available 
  (hardware operational), so that the browser can start. This is partly related to ID retrieval but is more linked to 
  using EGL on the screen. 
  * Worked on Realtek based LGI (Kylin) platform. Tried to integrate into buildroot with 64 bit build, so had to switch 
  back to 32 bit. Inbetween introduced Rene into the topic.
  * Researched memory issues with webbridge.

Rene:
  * Started working on Wifi plugin. Quite complicated due to dependency on libnl.
  * Started working with Bram on LGI Kylin (puck) STB. Now kernel and kernel modules are building, however we had to 
  fall back from 64 to 32 bit due to video driver issue (32 bit only)
  * Fixed bug in JSON derialization
  * Set up build for different buildroot configurations, as well as cppsdk and webbridge.
  * Started adding (gtest based) tests, and collecting statistics on tests and code.
  * We might need to move the build server.

2. Status webbridge
  * There has been quite a bit of discussion with LGI on application of webbridge. LGI is afraid of using webbridge as 
  it may mean we can do full stack (vendor lock-in)
  * As they are dacing issues on 1MV, things are very sensitive.
  * So we will do some window dressing: we will introduce a component wpe_platform or wpe_core, including cppsdk and 
  webbridge. This component will open sourced.
  * Public plugins will also be open sourced, but not the private plugins (backoffice, netflix, provisioning)
  * wpe now stands for webkit platform embedded
  * This means we have to stabilize webbridge and cppsdk
  * Others will be able to develop with cppsdk / webbridge, however we have control over uodated, especially it should 
  fit with the architecture. We are still in a niche market.
  * Discussion on repositories for cppsdk and webbridge. Still not finalized. We might end up having one repository 
  divided in two parts, which can be built and deployed separately.
  * Very important is keeping plugins independent.

3. Architecture changes
  * Changes in CPPSDK to deal with System Identifier (unique identification of a box)
  * For Nexus we need to check whether the system has started successfully. This brings in a dependency on 
  NXResourceCenter inside CPPSDK.
  * The same way, for Fusion there would be a dependency (through QueueCommunicator).
  * System ID is used for Playready.
  * For this the concept of device identifier is introduced. This is linked to the hardware, and is known from the time 
  the system starts.
  * So some things will change state over time, and this also includes network connectivity (IPV4/V6), system time, 
  location info, hardware state.


    Controller implements ISystemInfo
    Provisioning implements INotification, and IProvisioning, and registers to notification of ISystemInfo
    Netflix implements IProvisioning::INotification

  
    NXResourceCenter will eventually call SetID on ISystemInfo.
    SystemInfo will then call the Notification
    Provisioning will then be called, and requests provisioning from the backoffice. When successful, it will call the 
    provisioning notification.

  * The order of initialization should not matter, use the PluginInfo to listen to plugins waking up or stopping.
 
4. Actions
  * Bram and Rene: Investigate how to integrate coding style conversion to the process by adding it as
    a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.
  * Rene: Move coding guidelines to Wiki (done), merge with notes created in last meetings. Add coding guideline: Use asserts.
  * Ozgur: Share insights on DAWN memory savings through boot parameters on Wiki.
  