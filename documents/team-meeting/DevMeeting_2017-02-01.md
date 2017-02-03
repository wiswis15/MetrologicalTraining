#Metrological

##Team Meeting Embedded
##Date: 2017-02-01

1. Previous meeting: 
* It is a good idea to track decisions from a.o. code reviews, however then a separate stream of information starts to
exist next to the coding guidelines. So we will transfer the coding guidelines onto the wiki, and then merge the notes 
from last week.
* As we introduced quite some plugins, we will schedule a code review for these for next week.
* Also Pierre will do a demo on the Technicolor box next week.

2. Team update

Pierre:
  * Working on Technicolor (based on Nexus), preparing a demo for Proximus. They are now in a JV with Metrological to 
deliver a box for customers. This could be ported to DAWN and EOS.
  * One of the drivers is the possibility to run Netflix.
  * This is the first time that we deliver a full stack for the STB. I.e. no other partners such as PACE, Seachange or others is involved.
  * 1MW team is looking at possibilities to not run the app store in the browser. There is quite some discussion on 
  interfaces.
  * Horizon: frame rate discussion still ongoing, proof that framerate is the problem is still not hard.
  * Working on new plugins for location, time, network, identification.

Coen:
  * Still experimenting with network (IPV6). 
  * Doing some work on RET.
  * Will sit with Pierre to discuss IPV6.

Ozgur:
  * Played around with DAWN. Discussed with Naushaad. Will try to run WPE on DAWN as well. Pierre or Wouter US have 
  some configuration on this.
  * Trying to make NFS work on RPi3. Now working directly with bootcode.bin, directly with firmware of RPi. U-boot is 
  booting, but not possible to enter command lines.
  * Pierre: There are notes of using NFS in the Wiki.
  * Will discuss priorities with Albert

Mischa:
  * Built some tools for Mac, to not have to fall back to VMs. 
  * Researched instrumentation, and worked on documentation.

Bram:
  * Installed the local server he brought in, as a clean server. Mindset is to use it for building, but also to test 
  for pre- and post-commit hooks.
  * Still working on device id. Now Netflix startup is broken. So still have some work to do. Quite intensive as he 
  needs to work on 4 different branches in different repositories.

3. What should we improve on?

Pierre:
  * Not struggling, we should not force any tooling.
  * What is lacking, is documentation. Source should be leading, but we need some more information on architecture.
  * We should use Jira more, but should not pollute it with non-issues.

Coen:
  * There is a lot of knowledge in Pierre's head. We should write down more information, as a kind of knowledge base. 
  Pierre is sharing a lot of knowledge one on one, but the knowledge does not spread.
  * Pick up on the documentation again, make some time of Pierre available.

Bram:
  * In one on one reviews Pierre shares a lot of information, but this is not written down. We could write down more, 
  but is it used?
  * Testing needs to be done on different platforms, takes a lot of time, it would help if we could build for multiple 
  platforms in parallel on an external server.

Rene:
  * We should create more structure in defects we find, put them in Jira, and discuss what the priority is. This does 
  not mean a full CCB process, but somewhat more structure.

Ozgur:
  * There is a lack of information on how information flows in software, the execution architecture.

Mischa:
  * Provide guidance on tooling, but do not enforce. We might decide on some basics. We could give guidelines for new 
  employees.

Open issues
  * Have a discussion on process (Scrum, Agile, ….)
  * How to deal with network issues at customer site
  * How to deal with repository(ies) and branches
  * How to revive Jenkins and Continuous Integration (CI)
  * How to enforce coding guidelines, as a tool but possibly even as part of CI

8. Actions
  * Bram and Rene: Investigate how to integrate coding style conversion to the process by adding it as
    a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.
  * Pierre: Do a demo of the Technicolor box
  * All: prepare review of new and updated plugins: DeviceInfo, Provisioning, TimeSync, LocationSync, SkeletonWebPlugin, 
    SkeletonSocketPlugin.
  * Rene: Move coding guidelines to Wiki, merge with notes created in last meetings. Add coding guideline: Use asserts.