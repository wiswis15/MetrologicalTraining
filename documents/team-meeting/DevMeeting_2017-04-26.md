#Metrological

##Team Meeting Embedded
##Date: 2017-04-26

Agenda:

1. Team update

Mischa:
  * Git hook work continued
  * Looking into USB class devices
  
Sander:
  * Working on Yocto build, needed to do some adaptation to cmake framework
  * Yocto build now works
  * Check out new buildroot, check out wpeframework2 branch, build for RPi
     
Coen:
  * Looked at RDK emulator, seemed to not work, already answered
  * Is used in a wider scope than it was intended for. This may lead to an update in the future
  * Looking at buildroot for HTM. Modem wrapper (RPi anywhere) will be done by Pierre.
  * Proposal is at HTM, waiting for an answer, expected around mid May
  * Looking at modem drivers and kernel
  
Ozgur:
  * Did a demonstration of Dawn box with WPE to LGI, they were impressed.
  * Now doing some improvements, especially concerning suspending and resuming the browser. Working on this with Bas.
  * Sometimes there is a latency disturbing the wake-up of the browser
  * Integrated the old UPC remote controller for PoC
  
Bram:
  * Integration of new reference drivers, which broke build. Again updated today, still breaking build.
  * Started using new WPEFramework, planning to integrate westeros into ResourceCenter
  
Rene:
  * Supporting Realtek with OMX implementation and testing, which is hard as the build fails often
  * Also learning about graphics backends (weston/westeros)
   
Pierre:
  * Absent due to urgent work at LGI
  * Called in through conference phone
  * Problem in frame rate adaptation, resulting in audio loss
  * IPC work on network access, waiting for IPV4 address
  * LGI box fails after wake up after stand by of 5 hours
  * Helped Ozgur with adaptation after demo
  * 1MW: Discussing on SVP implementation, we need to adapt WPE for this
  * Playback of Playready content working again, only Youtube leanback still not working, Igalia is working on this

2. Actions
  * Ozgur to show Dawn PoC for LGI
  * Pierre: Do presentation on wpe as presented by Albert
  * Rene: Do a presentation on Wayland
  * Sander: Create repository for shared scripting.
  * Rene: Check git document in Wiki.
  * Bram and Rene: Investigate how to integrate coding style conversion to the process by adding it as
    a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.
  * Rene: Move coding guidelines to Wiki (done), merge with notes created in last meetings. Add coding guideline: Use asserts.
  * Sander: Place code snippets and slides on Documentation wiki.
  * Pierre: Possibly present on IPC mechanisms, especially synchronization in multi-threading environment (concurrency).
  * Coen and Rene: Look into possibilities to test Plugins, possibly using stub code made by Bram.