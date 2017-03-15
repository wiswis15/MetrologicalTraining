#Metrological

##Team Meeting Embedded
##Date: 2017-03-15

Agenda:

1. Team update

Ozgur:
  * Worked on wpe on Dawn box. Found that Netflix is extremely slow. Gets stuck after a few seconds of play-out.
  * Seemed to have to do with wpe, but also occurs in stand-alone version.
  * On RPi, loading gets stuck at 25%. This may have to do with rights.
  * Trying to integrate perf tool on Dawn to measure performance issues.
  * Building for Dawn with uclibc results in very slow UI.

Sander:
  * Working on renaming action for repositories.
  * Trying to finalize before end of the week.
  
Coen:
  * Investigating how to use gtest with plugins tests
  * Worked on acquisition of HTM project. A POC will be developed. POC would start in two weeks and last for 2 months
   (End of May).
  
Bram:
  * Revived puck. Now is able to displaying wpe.
  * Built applications (32 bit), now able to run webbridge on the puck.
  * Also working on resource center. For some reason the build is currently not working. Calls for Nexus while it is not 
  started yet.
  
Mischa:
  * Continued work on pipes and std stream reassignment
  * Writing down technical aspects of HTM POC proposal
  
Pierre:
  * Finalizing Horizon 342 with Netflix. Releasing single binary.
  * Now working on Nexus resource center for this.
  * Added to Netflix and Webkit plugins to wait for hardware.
  * After wake-up do focus again.
  * 1MW dicussion on WhiteVine (Google's PlayReady version) and OpenCDMI (abstract interface for WhiteVine and PlayReady).

Rene:
  
2. RPC discussion

  * Virtual inheritance of all interfaces from IUnknown (AddRef, Release, IUnknown)
  * When implementing a interface ITest, this will derive from IUnknown. The interface implementation Test will derive
   fromITest.
  * If Test also implements ITest2, this will also inherit from ITest2, and therefore IUnknown. There is multiple 
  inheritance from IUnknown. 
  * By using virtual inheritance, you then only implement IUnknown once.
  * Doing a reinterpret_cast is dangerous and does not work. Casting to a different interface may mean that the stub
   for the interface is not even created yet. QueryInterface does exactly that: It creates a stub if needed, and returns
   the new interface.
  * So the lesson is: when working with interfaces, use QueryInterface to go from one interface to another, do not use 
  casts.

3. Discuss cmake

  * Using a GLOB in the cmake file directive may miss some source file without refreshing a project.
  * After some discussion it was decided that this does not deliver a real problem, as for the build root build the
    project is regenerated anyhow, and for development it is easy to refresh the project after adding files. 

4. Uninitialized variables and {}
  * C++11 brought in a way to initialize a variable to its default, e.g.
    * int x{}; // default to 0
    * bool y{}; // default to false
    * struct info z{}; // default to all default values for struct members
    
  * An argument is that we should not initialize variables twice, however not initializing variables is even worse. 
    Next to this, the compiler will most probably optimize away duplicate initialization.  

5. Software build quaility
  * quite frequently, builds fail, or if they succeed, the applications do not run. Especially on buildroot, the master 
    build should always be dependable. One of the issues is that builds are not (well) tested. Another is that it is 
    not always clear which version is being built, and what its expected status is.
  * Conclusions:
    * We need a (embedded software) tester
    * We need some form of release management
    * Let's start thinking about possible solutions

6. Actions
  * Bram and Rene: Investigate how to integrate coding style conversion to the process by adding it as
    a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.
  * Rene: Move coding guidelines to Wiki (done), merge with notes created in last meetings. Add coding guideline: Use asserts.
  * Sander: Place code snippets and slides on Documentation wiki.
  * Pierre: Possibly present on IPC mechanisms, especially synchronization in multi-threading environment (concurrency).
  * Coen and Rene: Look into possibilities to test Plugins, possibly using stub code made by Bram.