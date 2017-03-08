#Metrological

##Team Meeting Embedded
##Date: 2017-03-08

1. Team update

Ozgur:
  * Worked with Bas on demo for DAWN with WPE. Managed to set up a debug environment to capture and analyze core dumps.
  * Launching WPE from inside GDB server did not work, but attaching debugger to an already running application did work.
  * Made options for debugging configurable, a.o. to disable JIT. Without JIT the system did not work.
  * Fixed Snapshot plugin, will share insights.

Sander:
  * Finished examples for glue code between JS and C++. Not in master yet, is waiting to have Bas test. Will push this soon,
  * Triggering the garbage collector from C++ was a bit harder than expected. This is now finished and complete on a branch.
  
Bram:
  * Worked on debugging on ResourceCenter. Failed to attach to resource center on Nexus.
  * Did work as a separate process, but inside out of process server things fail on RPC problems.
  * No progress on puck, waiting for an answer from Innopia.
  
Coen:
  * Reviewed unit tests. Conclusion is that unit testing is not ideal at this moment, we need to look at a different ways.
  * Also looking into Google test for threading.
  * It would be interesting to start testing plugins. Bram has written a hosting process, this is checked in in BRIDGE-155.
  
Mischa:
  * Worked on simple debugger. Struggling with pipes.
  * Might look into WebShell, as this uses pipes for redirecting std IO.
   
Pierre:
  * Horizon: working on proximity, enhanced security
  * Worked on Youtube leanback on Horizon, should be fixed. This was a problem in the web browser.
  * Had a discussion on restructuring of webbridge / cppsdk.

Rene:
  * Worked on streaming DVB with EIT. Now found library libdvbpsi which can parse SI from a DVB stream.
  * Tried to build buildroot, failed, will contact Igalia on this.
  
2. Presentation Sander on Posix file API

3. Presentation Ozgur on Snapshot plugin

4. What will happen to Webbridge / CppSdk?

  * WPE: Web Platform for Embedded
  * WPEFramework (WPEF): cppsdk + webbridge + webbridgesupport
  * WPEFrameworkPlugins (WPEFP): Open / Closed versions of plugins (closed possibly differentiated for customers / platforms)
  * WPEWebKit (WPEWK): WebKitforWayland
  * WPEUIFramework
  * WPETestFramework: MetroMetic
  * WPESecApi: LibProvision
  
  
5. Actions
  * Bram and Rene: Investigate how to integrate coding style conversion to the process by adding it as
    a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.
  * Rene: Move coding guidelines to Wiki (done), merge with notes created in last meetings. Add coding guideline: Use asserts.
  * Ozgur: Share insights on DAWN memory savings through boot parameters on Wiki.
  * Sander: Place code snippets and slides on Documentation wiki.
  * Pierre: Possibly present on IPC mechanisms, especially synchronization in multi-threading environment (concurrency).