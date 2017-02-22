#Metrological

##Team Meeting Embedded
##Date: 2017-02-22

1. Team update

Bram:
  * Refactoring of NXResourceCenter
  * Kylin / LGI puck code migration, almost done, will be the first full 64 bit version. Netflix is still incompatible, 
  due to JavaScript core, will be by version 5, but we can't migrate to this yet.
  * Previously ran into issues with 64 bit kernel versus 32 bit applications. Now we have a 64 bit driver.

Sander:
  * Worked on NodeJS based rendering with Bas van Meurs, to enable direct access to C++ code from JS using injected
  bundle.
  * Still to do: enable / disable modules automatically. Some extensions still needed.
  * Also planning to do small tutorial for interoperability between JS and C++.
  * This interop could be an entry point to testing middleware, and on the JS side could help speed up code.
  * Coen: What happened to webdriver? Webdriver was specific for Dawn / PACE, but the value of Selenium tests is limited.
  * On tracing, logging can now also be switched on for all modules by leaving the module string empty.
  
Coen:
  * Trying to build webbridge on desktop. Needs some help.
  * RET: visiting next week, reboot the systems. Hopefully last batch of systems.
  * Might be working on a sales demo on RPi.
  * Helping Ozgur debug on Snapshot plugin
  * Had some discussions on testing, will pick up again later.
  
Mischa:
  * Some support for RET
  * Writing examples for instrumentation and code ingestion
  
Ozgur:
  * Worked on WPE support for DAWN box. Ran some smoke tests on DAWN, leading to some Jira tickets.
  * Issues: Stream quality changes, Audio / video sync issues, invalid state while streaming, graphics layer crash.
  * Also looked at Snapshot problems. Seems to be dependent on the browser used.
  
Rene:
  * Refactored Time class due to some issues.
  * Refactored CMake structure to support building on different platforms. Still to be tested.
  * Tried to help and keep up with Bram on puck / Kylin platform.

2. Ideas and issues
  * Maybe do more presentations? Ideas:
    * Using remote debugging setups.
    * New study items: Windows / Linux interop.
    * Using private repos for scripts.
    * Learnings from the Puck migration.
    * Learnings on flashing / booting builds.
    * Git flow branching bugfix / feature.
    
  * Next week: Proxy / stub implementation, IDL

3. Actions
  * Bram and Rene: Investigate how to integrate coding style conversion to the process by adding it as
    a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.
  * Rene: Move coding guidelines to Wiki (done), merge with notes created in last meetings. Add coding guideline: Use asserts.
  * Ozgur: Share insights on DAWN memory savings through boot parameters on Wiki.
  