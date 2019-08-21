#!/usr/bin/env python3

import json, os, sys
from collections import OrderedDict

playreadyName = "PlayReady"
if len(sys.argv) > 1:
   playreadyName = sys.argv[1]

# config.json
configPath = "staging/etc/WPEFramework/config.json"

configFile = open(configPath, "r")
configuration = json.load(configFile, object_pairs_hook=OrderedDict)
configFile.close()

configuration["port"] = 8081

# TODO: find plugin by callsign
controllerConfiguration = configuration["plugins"][0]["configuration"]
if not controllerConfiguration["subsystems"]:
   controllerConfiguration["subsystems"] = ["Decryption"]

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
   addedMapping["name"] = "Null2"
   addedMapping["designators"] = ["com.metrological.null", "com.metrological.null2"]
   addedMappings.append(addedMapping)

   addedSettings = {}
   addedSettings["read-dir"] = os.getcwd() + "/build/netflix5/build/src/platform/gibbon/data/dpi/playready"
   addedSettings["store-location"] = os.getcwd() + "/build/netflix5/build/src/platform/gibbon/data/var/dpi/playready/storage/drmstore"

   addedMapping = {}
   addedMapping["name"] = playreadyName
   addedMapping["designators"] = ["com.microsoft.playready", "com.netflix.playready"]
   addedMapping["configuration"] = addedSettings
   addedMappings.append(addedMapping)

   configuration["configuration"]["systems"] = addedMappings

   configFile = open(configPath, "w")
   json.dump(configuration, configFile, indent=1, separators=(',', ':'))
   configFile.close()

# Provisioning.json
configPath = "staging/etc/WPEFramework/plugins/Provisioning.json"
if os.path.isfile(configPath):
   configFile = open(configPath, "r")
   configuration = json.load(configFile, object_pairs_hook=OrderedDict)
   configFile.close()

   configuration["configuration"]["operator"] = "liberty"
   configuration["configuration"]["server"] = "provisioning-sdk.metrological.com:80"

   configFile = open(configPath, "w")
   json.dump(configuration, configFile, indent=1, separators=(',', ':'))
   configFile.close()

