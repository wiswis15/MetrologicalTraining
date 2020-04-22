#!/usr/bin/env python3

# Small app to recursively collect all info about git repos in Yocto build.
# This helps you restoring a working situation if a "repo sync" messes up the build.
# Run it once when you have a successfull build. If later the build fails because of
# build errors pushed to the recipe git repo, rerunning this tool and a simple
# `diff` on the two output dirs can tell you which repos have updated git hashes

# Returning the faulty repos can be done as follows:
#  1. Stash or checkout the current modifications
#  2. "git checkout" the hash stored in "hash.txt" in the correct output dir
#  3. "patch -p1 <" the diff stored in "diff.patch" file in the correct output dir
#  4. In the unlikely event the origin URL has changed, the file "config.txt" 
#        contains all the info you need to git clone the correct repo.

import os, subprocess as sp, sys

def RemoveTrailingHash(path):
   while path.endswith("/"):
      path = path[:len(path)-1]
   return path

def GetCommandOutput(command):
   output = sp.check_output(command).decode("utf-8").split("\n")
   while "" in output:
      output.remove("")
   return output

if len(sys.argv) != 4:
   print("Usage: " + sys.argv[0] + " [base-dir] [build-dir-name] [output-dir]")
   print("For example:")
   print("   " + sys.argv[0] + " /home/sander/Projects/vip5202w/yocto build-vip5202w /home/sander/vip5202w/hash-backup")
   sys.exit(1)

baseDir = RemoveTrailingHash(sys.argv[1])
skippedBuildDir = RemoveTrailingHash(sys.argv[2])
outputDir = RemoveTrailingHash(sys.argv[3])

if os.path.isdir(outputDir):
   print("output dir \"" + outputDir + "\" already exists, don't want to deletethis one.\nPlease pass non-existent output dir on command line.")
   sys.exit(1)

rmCall = ["rm", "-rf", outputDir]
sp.call(rmCall)

findCall = ["find", baseDir, "-type", "d", "-name", ".git"]

gitPaths = GetCommandOutput(findCall)

ignoredPrefix = os.path.join(baseDir, skippedBuildDir)

for gitPath in gitPaths:
   if gitPath.startswith(ignoredPrefix):
      continue
   repoDir = gitPath[:len(gitPath) - len(".git")]
   relOutputDir = repoDir[len(baseDir):]
   if relOutputDir.startswith("/"):
      relOutputDir = relOutputDir[1:]
   print(relOutputDir)
   targetDir = os.path.join(outputDir, relOutputDir)
   mkdirCall = ["mkdir", "-p", targetDir]
   sp.call(mkdirCall)
   os.chdir(repoDir)
   hashPath = os.path.join(targetDir, "hash.txt")
   gitCall = ["git", "rev-parse", "HEAD"]
   headHash = GetCommandOutput(gitCall)[0]
   with open(hashPath, "w") as outFile:
      outFile.write(headHash + "\n")
   gitCall = ["git", "diff"]
   gitDiff = GetCommandOutput(gitCall)
   diffPath = os.path.join(targetDir, "diff.patch")
   with open(diffPath, "w") as outFile:
      for diffLine in gitDiff:
         outFile.write(diffLine + "\n")
   originPath = os.path.join(targetDir, "config.txt")
   cpCall = ["cp", repoDir + "/.git/config", originPath]
   sp.call(cpCall)

print("done!")

