#Metrological

##Team Meeting Embedded
##Date: 2017-04-12

Agenda:

1. Team update

Mischa:
  * Did some preparation on the HTM proposal.
  
Sander:
  * Still working on renaming, webbridge and cppsdk repo are still locked.
  * Technicolor project is picking up again. It looks like we are soon starting again. Naushaad has been assigned to 
  this project, we are however still waiting for code and boards.
    
Coen:
  * Helped on Android apps for Proximus
  * Did some experiments with buildroot for preparation of HTM project
  * Also worked on testing solution
  
Ozgur:
  * Continued on wpe on Dawn box. Getting ready for POC, waiting for Albert to set up a meeting at LGI
  * Encountered issue in Playready, which can be reproduced on other platforms.
  * Also continuing integration of RF4CE. Received sources from Bram concerning Qorvo. Pierre might be able to help out. 
  Issue before was mismatch in libraries for hard float vs soft float.
  
Bram, Rene:
  * Stabilized the deployment of the Realtek platform, for both puck and Giraffe board. We are now able to build, 
  deploy and flash on both platforms.
  * Pierre mentions the target price for LGI will be 6 euros for the complete puck.
  * Video playback is not stable. Seems to have to do with blocking due to not generating PTS in the sink or OMX.
  * Large move towards master buildroot branch, by removing all patches. Some of the patches removed caused compiler 
  errors, and these were solved.
  * Co-operation is still difficult, still no code received.
  * Realtek is now able to build from the repositories.
  * Found that there is no clear view of requirements on formats to be played, and no standard test set. We started 
  building a small test set.
  * Also, there are hardly any simple applications to test certain parts of the system. There are solutions around, but 
  these are not documented and hardly communicated.
  
Pierre:
  * Will be looking into PlayReady issues, especially for Horizon.
  * Had discussion with Albert on Westeros. In the US there is a move towards Westeros. For Realtek there is only 
  support for Weston. This Westeros implementation should be ready, but is not completely ready yet. The discussion 
  should be mostly on the interface, not the actual implementation.
  * Together with Sander modified WPEFramework (webbridge) to deal with split configuration. Each plugin now has its own 
  configuration, all configuration files are read, and plugins are loaded.
  * Youtube leanback certification, some issues, a.o. PlayReady issue.
  * 1MW requested for estimates for EME V1 implementation. Board is getting more memory (800Mb). Still there are issues, 
  especially performance.
  * Proximity not working completely, but largely working 
  * Due to proximity trigger, work started on building for both Linux and OSX using cmake

2. Sander on static libraries

3. Actions
  * Pierre: Do presentation on wpe as presented by Albert
  * Rene: Do a presentation on Wayland
  * Sander: Do a presentation on working with static libraries 
  * Sander: Create repository for shared scripting.
  * Rene: Check git document in Wiki.
  * Bram and Rene: Investigate how to integrate coding style conversion to the process by adding it as
    a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.
  * Rene: Move coding guidelines to Wiki (done), merge with notes created in last meetings. Add coding guideline: Use asserts.
  * Sander: Place code snippets and slides on Documentation wiki.
  * Pierre: Possibly present on IPC mechanisms, especially synchronization in multi-threading environment (concurrency).
  * Coen and Rene: Look into possibilities to test Plugins, possibly using stub code made by Bram.