#Metrological

##Team Meeting Embedded
##Date: 2017-03-29

Agenda:

Meeting of 2017-03-22 was skipped due to absence of Pierre and pressure with Rene and Bram
This meeting will be a short update round.

1. Team update

Ozgur:
  * Absent due to travel to Istanbul

Sander:
  * Rename actie loopt nog steeds. Alle changes zijn mee overgenomen. Venijn zit nu in rename WebBridge, omdat daar 
  naming clashes naar voren kwamen.
  * Ondertussen zijn wijzigingen geweest, ook deze zijn meegenomen.
  * Er zijn behoorlijk wat leermomenten geweest. Een verandering in provisioning voor nog niet in productie genomen 
  systemen, dit wordt WPE<mac address>. Dit is altijd ook de system identifier. 
  
Coen:
  * Aanzet gemaakt voor HTM aanbesteding.
  * Nu bezig een opzet te maken voor testen van sockets op basis van sockets.
  * Wat apps gemaakt voor Proximus.
  
Bram:
  * Realtek integratie, vordert maar traag. Veel functionaliteit overgedragen in binaire vorm.
  * Nog steeds geen code gedeeld, er speelt nogal wat politiek.
  * 
  
Mischa:
  * Sander geholpen met C++ kennis.
  * Bezig geweest met sockets, libevent.
  
Pierre:
  * Bij klaar voor release 3.4.2. Huidige release 3.4.1 heeft ook Netflix voor ISTB, laatste is RC2, en is in productie,
   wacht of certificering. Release 3.4.2 gaat werken voor IBC, ook Youtube leanback bieden, en heeft features proximity, 
   en framerate fix voor Netflix. Er lopen nu testen bij LGI.
  * Les, QA moet echt beter, en requirements moeten duidelijker. Er was een probleem met DIAL, en testen wezen dat niet 
  uit. Er miste zelfs implementatie.
  * Er zijn ook problemen met invalid SSL certificates die niet tegengehouden worden.
  * 1MW: Het wordt steeds stiller, alles zou op track liggen, maar daar is twijfel over. Nu draaien twee node.js 
  instanties, en dan is er geen ruimte meer voor wpe. De twee node.js instantiaties zijn nodig vanwege threading.
  * Vraag om Whitevine (Google alternatief voor Playready)support op EMEv1, mogelijk v3.
  
Rene:
  * 
  
2. Actions
  * Sander: Create repository for shared scripting.
  * Rene: Check git document in Wiki.
  * Bram and Rene: Investigate how to integrate coding style conversion to the process by adding it as
    a pre-commit plugin in Github. This can now be tested on the new server Bram brought in.
  * Rene: Move coding guidelines to Wiki (done), merge with notes created in last meetings. Add coding guideline: Use asserts.
  * Sander: Place code snippets and slides on Documentation wiki.
  * Pierre: Possibly present on IPC mechanisms, especially synchronization in multi-threading environment (concurrency).
  * Coen and Rene: Look into possibilities to test Plugins, possibly using stub code made by Bram.