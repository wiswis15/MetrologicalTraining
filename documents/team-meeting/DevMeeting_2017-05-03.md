#Metrological

##Team Meeting Embedded
##Date: 2017-05-03

Agenda:

1. Team update

Mischa:
  * USB device class study and notes
  * HTM project is unclear
  * Continued work on local build system
    
Sander:
  * Worked with Yocto to get buildroot based build work there.
  * Had problem not being able to boot rpi in debug build, due to partition growing large, and master boot record being 
  moved.
  * Had to remove some cmake stuff for Yocto compilation, will proceed to move this back in discussion with Rene
     
Coen:
  * Tried to build new setup, but ran into issues with key management.  
  
Ozgur:
  * Tried to integrate Dawn profile into new WPEFramework, now have to link to static library
  * For MIPS, attribute visibility hidden is not recognized, this will have to be solved in the portability layer. Ozgur 
  will pick this up.
  * Settings for memory pressure etc. are not present yet, will be added for plugins which need them.
  * For second PoC for liberty, running into issues when switching application. Netflix loses internal suspend state.
  
Bram:
  * Worked on integrating for EOS. Based on new WPEFramework. Now almost able to build and run westeros.
  * Discussed with Sander on plugin structure. Also need to align with Albert.
  * Tracing from start is currently not working well.
  * Found that in the US there is a buildbot server running. Now in Github there are icons with commits, which are 
  checked if the build succeeds. We should align with US to decide on one solution.
  
Rene:
  * Worked on trying to build weston and westeros for Linux
  * Supporting Realtek still, trying to integrate, build and test fixes.
   
Pierre:
  * Still working on audio loss in Netflix. This is due to changing decoders for frame rate adaptation.
  * This should be solved by Cisco, however this is not happening, there is a lot of politics.
  * Now performing an out of process attach / detach operation. Seems to be linked to buffering, but happens only after 
  frame rate adaptation.
  * Moved VirtualInput, Bram adapted to make it build, and make Netflix use package info.

2. Presentation Pierre on Web Platform for Embedded

3. Actions
  * Ozgur to show Dawn PoC for LGI
  * Pierre: Do presentation on wpe as presented by Albert
  * Rene: Do a presentation on Wayland
  * Sander: Create repository for shared scripting. -> Everyone will create his own branch
  * Bram and Rene: Investigate how to integrate coding style conversion to the process by adding it as
    a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.
  * Rene: Move coding guidelines to Wiki (done), merge with notes created in last meetings. Add coding guideline: Use asserts.
  * Pierre: Possibly present on IPC mechanisms, especially synchronization in multi-threading environment (concurrency).
  * Coen and Rene: Look into possibilities to test Plugins, possibly using stub code made by Bram.