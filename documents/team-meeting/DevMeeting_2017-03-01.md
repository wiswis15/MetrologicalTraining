#Metrological

##Team Meeting Embedded
##Date: 2017-03-01

1. Team update

Pierre:
  * LGI: some issues with audio, and macro-blocks with Youtube leanback.
  * Did work on next generation of resource center for Nexus. Later this may also be moved to Intel, or Realtek.
  * During discussion proxy/stubs came up, so Pierre will present on this.

Sander:
  * Still working on JS / C++ interface. It is possible to trigger the Garbage collector from C++.
  * Will be writing documentation on how to may the interface work. 
  * There was some discussion on what to expose through the injected bundle, as this may be an issue for Nagra. Next to 
  this, code will be running in the browser, instead of in its own process.
  * There is a special feature built into the injected bundle for testing purposes.
  * Sensitive things (that might impede security) for testing will be handled through a specific plugin.
  * Is struggling with specifics on Linux file handles in combination with FILE * structures.
  
Coen:
  * Helped Ozgut on the Snapshot plugin
  * There is a chance to get a project for HTM, based on RET project.
  * Now looking at unit tests for CPPSDK.
  
Mischa:
  * Did work for HTM and RET

Ozgur:
  * Worked on Snapshot plugin. Had some issues with images getting corrupted. Another issue was having multiple 
  snapshots in progress at the same time.
  * Also worked on buildroot for Dawn, to run with debug symbols. This resulted in a 3.2 Gb image, especially due to 
  wpe. Is looking for a smaller solution, will involve Igalia in the discussion.
  
Bram:
  * Delivered NxResourceCenter
  * Tried 64 bit image on Kylin box (puck). Which crashed the box. Now trying to revive.
  * Trying to get Nexus version of resourcecenter improved.
  
Rene:
  * Worked on cmake refactoring to support build on multiple platform
  * Worked on DVB demo, but got stuck. Needs some discussion
  * Spent quite some time on helping in git, and preparing a presentation for it.
  * Spent a little time on Wifi plugin.

2. Presentation Pierre on Proxy/Stubs: [here](https://github.com/Metrological/Documentation/wiki/presentations/ProxyStub.key)

3. Presentation Pierre on Using Git: [here](https://github.com/Metrological/Documentation/wiki/presentations/UsingGit.key)

4. Actions
  * Bram and Rene: Investigate how to integrate coding style conversion to the process by adding it as
    a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.
  * Rene: Move coding guidelines to Wiki (done), merge with notes created in last meetings. Add coding guideline: Use asserts.
  * Ozgur: Share insights on DAWN memory savings through boot parameters on Wiki.
  * Next week:
    * Ozgur: Short presentation on Snapshot plugins
    * Sander: Linux file handling
  