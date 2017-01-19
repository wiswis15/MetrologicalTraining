#Metrological

##Team Meeting Embedded
##Date: 2017-01-11 

1. Previous meeting: 

2. Team meeting goals:
* Interaction with team members
* Knowledge transfer (e.g. using code reviews)
* Help each other (prevent re-inventing the wheel)
* Find candidate subjects for documentation

3. Update:
* Özgür:
  * Seeing how to run web bridge on RPi. Removing plugins to see what happens. Gone to browser only.
  * Tried a first simple plugin, but did not finalise.
  * Now assigned to DAWN project with Sander. Trying to build an image of expectations in DAWN. Working on blocking issues.
  * Still working on registration with IND
* René:
  * Working on NTP client plugin implementation for RPi platform provisioning
  * Also planning to pick up DIAL implementation enhancement (needs some fixes)
* Mischa:
  * Writing down notes on distributed building on Wiki
* Coen:
  * Working on communication on local network. Mostly to gain knowledge.
  * Reorganising notes, converting private notes into Wiki
  * Might spend some time on IPV6 to tackle any issues we may have.
  * Idea: how to deal with network issues at customer site. Some people take a private router with them to make sure they have a reproducible situation.
* Bram:
  * Working on One Middleware, which uses Yocto instead of Buildroot
  * Working on System / Device Identifier. Device identifier is normally distributed by organisations like Nagra. System identifier is derived from the hardware, this also holds for RPi. System ID is always unique, can be derived from MAC address for example. For e.g. Horizon, this is derived from software, which takes some time to start. The Device Identifier will be equal to the number on the box in such cases.
* Pierre:
  * Working on One Middleware. LGI seems to be getting worried. Trying to start take some load off the team by using web bridge. Another meeting planning on Jan. 12.
  * Still working on Horizon. Proximity checking to see whether the gateway is near to the client to prevent streaming over VPS connections, this is a requirement from Netflix.
  * Also working on frame rate conversions. Strangele enough especially expensive TV sets seems to be very picky on drop frame versus non drop frame rates.
  * Bumped stable to a newer version. Stable branch will be renamed to Horizon and only only for Horizon. This is for now the only project running web bridge, wpe and cppsdk. DAWN is using qtwebkit browser. One middleware will move to head of master.
  * Some details on platforms:
    * Horizon: Intel, 1Gb RAM
    * DAWN: Mips, 1Gb RAM
    * EOS: ARM, 2Gb RAM
  * DAWN platform development is done in Mechelen (B) by LGI,app development is done in Schiphol-Rijk.

4. Discussion on locking
Due to a discussion between Bram and Pierre, the subject of locking was put on the table. Rough lessons:
* Make duration of lock as short as possible
* Only using re-entrant locks
* Make sure you unlock before returning
* Make it clear what you are locking for, and do it in all locations with the same purpose
5. Next Meeting
  * Presentation by Sander on GStreamer with wpe
  * Other subjects to be inventorized
    * Have a discussion on const values, pointers, and references
    * Have a discussion on process (Scrum, Agile, ….)
    * How to deal with network issues at customer site