#Metrological

##Team Meeting Embedded
##Date: 2017-01-25

1. Previous meeting: 
* No remarks

2. Usage of const

Rene shows some code and explains that the usage of const is different than he thought. Will place 
this code in the Documentation wiki as example code, in a new folder. All code examples and team 
specific information will be placed in the [Documentation wiki] (https://github.com/Metrological/Documentation).

3. New C++11 features

After a discussion with Pierre, Rene checked whether a number of new C++11 features are supported on 
all platforms. As the compiler for MIPS / DAWN is normally the oldest, these were tested and verified 
using this compuiler. Rene shows how **override** and **final** can be used. It is agreed that
standard features suppored by the C++11 compiler can be used. 
Some of the **#pragma** directives, such as **#pragma** once however are not standard, and therefore 
are preferably not used. For some in depth discussion see [this post](http://stackoverflow.com/questions/787533/is-pragma-once-a-safe-include-guard).

4. Process for repositories

As the team is growing, the risk of getting in each other's way in repositories is also increasing. 
Rene does a small presentation on [Git flow](http://nvie.com/posts/a-successful-git-branching-model/). See also his [Github location](https://github.com/nvie/gitflow).
The team will let the details sink in, and we will later decide if and how to adopt Git flow. For
now, it seems like a good idea to at least start using feature branches.

5. Coding style

As the team has decided to adhere to the Webkit coding style, it seems like a good idea to make sure
all code in the repository matches that style. Bram and Rene had a discussion on tooling for this,
Bram is using **clang-format** to convert coding style inside CLion. After some research Rene posted 
information on this [here](https://github.com/metrological/wiki/wiki/Coding-Style-Conversion).
Apparently, Albert is using the same tool, he also wrote on a [wiki](https://github.com/Metrological/wiki/wiki/tools-C---style-formatting).
Bram and Rene will investigate how to integrate coding style conversion to the process by adding it as
a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.

6. Team update

Bram:
* Worked on id for provisioning, has it working now. Now planning to integrate into master to link to provisioning.
Pierre would like to also integrate NTP client into this.
* Brought his personal CI server and installing. This might replace cloud server over time.

Sander:
* Worked on DAWN, need to show a demo end of the week. Also looked at how rendering is tuned in node.js.

Mischa:
* Studying linear algebra to understanding work from Bas. Also set up Mac again.

Coen:
* Still working on network setup. Setting up local network with DNS. Also with IPV6. Need to talk to Gelke.
* Pierre suggests having a look at **Happy eyeball**1 algorithm for IPV6 discovery.

Ozgur:
* Worked on DAWN project, to take over discussions with parties. DAWN code is now running on his 
machine, trying to use NFS for loading binaries.
* The way of working with DAWN has changed, LGI now has to set priorities. Browser related issues 
gave a chance to related to Igalia.
* While Sander is on holiday, Pierre is willing to jump in. Naushaad will be the first contact.

Pierre:
* Had discussions on one middleware again. LGI is still struggling how to deliver project in time.
* Did a quick port to Qubiware (TiVo), this needs its own uclibc, which had to be integrated into the 
toolchain. There is now a toolchain called Homebox.
* Did a framerate adaptation for Horizon.
* Also partially delivered proximity checking.
* Problems with combination of SCART with HDMI.
* Videoland playback is ok, Youtube leanback is not working correctly.
* Picking up Technicolor while Sander is on holiday.

7. Code review NTPClient plugin

As Pierre and Sander had to finalize a demo, we did not complete the code review, however some 
comments include:
* Using non-standard extensions such as #pragma pack is risky. Although on most compilers it does as 
you would expect, there may be alignment issues for the full packet.
* Also sizes of types are never guaranteed.
* It is better to create a class which serializes and deserializes a packet.
* Conversion from host to network byte order and back is confusing, use accessors to do conversion when 
setting / getting.
* Some discussion on the use of iterators
 
Open issues
  * Have a discussion on process (Scrum, Agile, ….)
  * How to deal with network issues at customer site
  * How to deal with repository(ies) and branches
  * How to revive Jenkins and Continuous Integration (CI)
  * How to enforce coding guidelines, as a tool but possibly even as part of CI

8. Actions
  * Bram and Rene: Investigate how to integrate coding style conversion to the process by adding it as
    a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.
  * Rene: Share link to Git flow document (done in minutes above)
  * Rene: Share code examples on wiki (done: see [here](https://github.com/Metrological/Documentation/tree/master/sources/code-snippets))
  * Rene: Plan for continued code review NTPClient plugin 
