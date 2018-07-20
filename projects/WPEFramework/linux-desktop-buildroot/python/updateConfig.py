#!/usr/bin/env python3

import json, os
from collections import OrderedDict

# config.json
configPath = "staging/etc/WPEFramework/config.json"

configFile = open(configPath, "r")
configuration = json.load(configFile, object_pairs_hook=OrderedDict)
configFile.close()

for plugin in configuration["plugins"]:
   callsign = plugin["callsign"]
   if callsign == "Controller":
      plugin["configuration"]["resumes"] = ["OutOfProcessTest"]

configuration["port"] = 8081

pathPrefix = os.getcwd() + "/staging"

configuration["datapath"] = pathPrefix + configuration["datapath"]
configuration["systempath"] = pathPrefix + configuration["systempath"]
configuration["proxystubpath"] = pathPrefix + configuration["proxystubpath"]

configFile = open(configPath, "w")
json.dump(configuration, configFile, indent=1, separators=(',', ':'))
configFile.write("\n\n")
configFile.close()

# LocationSync.json
configPath = "staging/etc/WPEFramework/plugins/LocationSync.json"

configFile = open(configPath, "r")
configuration = json.load(configFile, object_pairs_hook=OrderedDict)
configFile.close()

configuration["configuration"]["source"] = "jsonip.metrological.com:80"

configFile = open(configPath, "w")
json.dump(configuration, configFile, indent=1, separators=(',', ':'))
configFile.close()

