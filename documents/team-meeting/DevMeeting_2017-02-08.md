#Metrological

##Team Meeting Embedded
##Date: 2017-02-08

1. Previous meeting: 

2. Demo Technicolor box

Most of the functionality is already working, both Youtube leanback and Netflix. On of the things to add in the near 
future is Amazon Prime.
Now waiting for customers to be interested.

Some functionality is now brought into the native domain, by a JS call into the browser, and loading a specific library 
to handle such a call. This leads to considerable speed-up.

3. Team update

Pierre:
  * Horizon, problems have been found concerning DIAL, certification for Youtube is still ongoing.
  * GCC 5 support for provisioning taken up. Previously this did not work, which had to do with a different code ABI. 
  Now, a plain C interface is used.
  * JSON parser bug is solved, now the null value is supported. Incorporated comments by Rene.

Coen:
  * Paired with Ozgur on Snapshot plugin
  * Did some work for RET.
  * Finally got DNS working.

Ozgur:

Mischa:
  * Working for RET.
  * Working on code instrumentation
  * Running against some issues with PGP.

Bram:
  * DeviceID delivered. Is now fully functional.
  * Helped get provisioning to build with GCC 5.
  * Nu working on Nexus initialisation of Webbridge. This concerns determining and then setting the device id as soon as 
  it becomes available.

Rene:
  * Reviewed JSON parsing of String and Boolean. Made unit tests based on gtest for these.
  * Consequently added existing other tests and transferred to gtest.
  * Set up build server for cppsdk and webbridge, with support for feature branches.
  * Started on Wifi Plugin, to retrieve SSID of networks, pass the key to log in, and possibly determine and display 
  signal strength information. 
  * Coding guidelines en rules document overgezet naar Wiki. Nog te doen is incorporeren van nieuwe inzichten en regels.
  
4. Code review

  4.1. DeviceInfo
  
  * Implementation of IDeviceInfo seems a bit obscure. Interface should be symmetrical, also in implementation. 
  GetIdentifier is not implemented, while SetIdentifier is. It would seem logical to extract GetIdentifier method.
  * Header files are quite complicated due to nested classes. There is a number of reasons for this:
    * To prevent name clashes. Don't ever use macros to define things.
    * To hide implementation details.
    * To enable direct inlining of non-virtual methods.
  * Discussion on testing with nested classes (making them accessible to tests, and to plugin in different classes). A 
  way is to change private to a define we change for testing). This preferred over adding more code just for the sake of 
  testing.
  * Do not use magic numbers, for editing purposes, but also for clarity purposes. Prefer constexpr, as it does not use 
  memory, unless it is referred to by pointer or reference.

  4.2. LocationSync
  
  * We might at some point also add LL (Latitude/Longitude) information. 
  
Open issues
  * Have a discussion on process (Scrum, Agile, ….)
  * How to deal with network issues at customer site
  * How to deal with repository(ies) and branches
  * How to revive Jenkins and Continuous Integration (CI)
  * How to enforce coding guidelines, as a tool but possibly even as part of CI

8. Actions
  * Bram and Rene: Investigate how to integrate coding style conversion to the process by adding it as
    a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.
  * All: prepare review of new and updated plugins: DeviceInfo, Provisioning, TimeSync, LocationSync, SkeletonWebPlugin, 
    SkeletonSocketPlugin.
  * Rene: Move coding guidelines to Wiki (done), merge with notes created in last meetings. Add coding guideline: Use asserts.