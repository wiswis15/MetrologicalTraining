#Metrological

##Team Meeting Embedded
##Date: 2017-03-22

Agenda:

1. Team update

Ozgur:

Sander:
  
Coen:
  
Bram:
  
Mischa:
  
Pierre:

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