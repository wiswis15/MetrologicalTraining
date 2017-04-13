#Metrological

##Team Meeting Embedded
##Date: 2017-04-05

Meeting minutes for 2017-03-29 are in Dutch, as he meeting was too, but will be converted to English soon, bare with me.

Agenda:

1. Team update

Mischa:
  * On holiday
  
Sander:
  * Still working on rename action, recently a new merge was done, now lost track of previous state, and still needs to
  convert naming for this part.
  * Also preparing presentation on how to deal with static libraries.
  * Was working on Resource Center, would like to hear a presentation on Wayland.
    
Coen:
  * Working on mocking for testing plugins, now diving into sockets. Also new testing mocks themselves.
  * Needs to discuss this a bit.
  * Status HTM project still under discussion.
  
Ozgur:
  * Was in Turkey for a discussion with Turkcell. They were impressed. Still have to decide between Huawei and Sagem.
  * They are not happy with the Huawei box. Performance is not as good as expected.
  * Started working on Nexus 16.2 Resource Center.
  * Resource Center is also working on Dawn box.
  * Committed WebGL node.js as requested by Albert.
  * Will be working on the demo of Dawn. This will also need support for IR remote.

Bram, Rene:
  * Still working on Realtek RTD1295 integration. Integrated kernel build, and flashing the puck.
  * Kernel is 64 bit, software is 32 bit.
  * Since yesterday we have video output combined with wpe.
  * Politics are in the way, as well as language
  * There are still some issue, possibly caused by hardware differences between the puck and the development board.
  * Things take a long time. Getting good answers is also challenges.
  * Phil Sellick is from Virgin Media.
  
Pierre:
  * Youtube certification started on Horizon 3.4.2. 
  * Found some vulnerabilities in code (wpe).
    * Problem with automatically accepting even self-signed certificates.
    * Also language headers required for Youtube leanback, these have now been added.
  * Had a look at resource center, which is now merged into master.
  * Will be closing access to cppsdk, webbridge, libprovision, playready, netflix for renaming action.
  * Presentation on wpe done by Albert, could be presented in team.
  * Next plugin will be SI information extraction for DVB, in US this is being done for ATSC.

2. Actions
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