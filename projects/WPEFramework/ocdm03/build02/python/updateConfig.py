#!/usr/bin/env python3

import json, os
from collections import OrderedDict

# config.json
configPath = "staging/etc/WPEFramework/config.json"

configFile = open(configPath, "r")
configuration = json.load(configFile, object_pairs_hook=OrderedDict)
configFile.close()

configuration["port"] = 8081

pathPrefix = os.getcwd() + "/staging"

if not configuration["datapath"].startswith(pathPrefix):
   configuration["datapath"] = pathPrefix + configuration["datapath"]

if not configuration["systempath"].startswith(pathPrefix):
   configuration["systempath"] = pathPrefix + configuration["systempath"]

if not configuration["proxystubpath"].startswith(pathPrefix):
   configuration["proxystubpath"] = pathPrefix + configuration["proxystubpath"]

configFile = open(configPath, "w")
json.dump(configuration, configFile, indent=1, separators=(',', ':'))
configFile.write("\n\n")
configFile.close()

# LocationSync.json
configPath = "staging/etc/WPEFramework/plugins/LocationSync.json"
if os.path.isfile(configPath):
   configFile = open(configPath, "r")
   configuration = json.load(configFile, object_pairs_hook=OrderedDict)
   configFile.close()

   configuration["configuration"]["source"] = "http://ip-api.com/json"

   configFile = open(configPath, "w")
   json.dump(configuration, configFile, indent=1, separators=(',', ':'))
   configFile.close()

# OCDM.json
configPath = "staging/etc/WPEFramework/plugins/OCDM.json"
if os.path.isfile(configPath):
   configFile = open(configPath, "r")
   configuration = json.load(configFile, object_pairs_hook=OrderedDict)
   configFile.close()

   addedMappings = []

   addedMapping = {}
   addedMapping["key"] = "com.metrological.null"
   addedMapping["system"] = "Null2"
   addedMappings.append(addedMapping)
   addedMapping = {}
   addedMapping["key"] = "com.metrological.null2"
   addedMapping["system"] = "Null2"
   addedMappings.append(addedMapping)
   addedMapping = {}
   addedMapping["key"] = "com.microsoft.playready"
   addedMapping["system"] = "PlayReady"
   addedMappings.append(addedMapping)

   configuration["configuration"]["mapping"] = addedMappings

   configFile = open(configPath, "w")
   json.dump(configuration, configFile, indent=1, separators=(',', ':'))
   configFile.close()

