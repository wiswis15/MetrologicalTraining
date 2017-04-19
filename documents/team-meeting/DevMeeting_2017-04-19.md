#Metrological

##Team Meeting Embedded
##Date: 2017-04-19

Agenda:

1. Team update

Mischa:
  * Working on HTM: Status still unknown, expecting a go in around two weeks. Platform changing from industrial PC to 
  Raspberry Pi (Pi Anywhere). 
  * Working on GIT hooks for building repository
  
Sander:
  * Renaming is largely ready, in buildroot. Would be good to test the buildroot clone on the wpeframework (cppsdk), 
  webbridge, on buildroot wpeframework2
  * Repos are still closed. Still some cleanup to be done for buildroot, now working on Yocto
  * Configuration generation in cmake is now separated for each plugin. Initially cmakepp conde was abandoned, but now 
  cmake code is installed as a staging step.
  * Found out Netflix silently hangs, when webbridge is not provisioned.
     
Coen:
  * Diving into buildroot again for preparation of HTM project.
  
Ozgur:
  * Absent due to demo at LGI
  
Bram:
  * Making steps towards integrating libdrm, enabling steps from weston towards westeros
  * Starting to work on EOS Resource center, to integrate westeros
  
Rene:
  * Rene and Bram are working on 
  * Progress with Realtek is slow, now still issues with stable playback 
   
Pierre:
  * Absent due to urgent work at LGI

2. Sander on static libraries

3. Actions
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