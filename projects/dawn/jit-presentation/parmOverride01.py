#!/usr/bin/python
import sys, os, fcntl, subprocess, re

logFileNameOrg = "/data_local/workspace/build_steps_org.txt"
logFileNameNew = "/data_local/workspace/build_steps_new.txt"
orgAppAppend = "_org"

def appendToLogFile(logFileName, line):
   with open(logFileName, "a") as g:
      fcntl.flock(g, fcntl.LOCK_EX)
      g.write(line)
      fcntl.flock(g, fcntl.LOCK_UN)
      
def invokeOrgApp(newArgs):
   argv = [sys.argv[0]]
   for newArg in newArgs:
      argv.append(newArg)
   appName = argv[0]
   newName = appName + orgAppAppend
   argv[0] = newName
   #argv[0] = "gcc"
   #print("args to Popen: " + str(argv))
   p = subprocess.Popen(argv)
   p.wait()
   return p.returncode
   
def buildOrgCmdLine():
   output = ""
   for arg in sys.argv:
      output = output + " " + arg

   output = output.strip()
   return output

def buildNewCmdLine(newArgs):
   output = ""
   for arg in newArgs:
      output = output + " " + arg

   output = output.strip()
   return output

# def getBaseToolName():
#    p = re.compile("\\-([^\\-]+)$")
#    m = p.search(sys.argv[0])
#    #m = p.search("mipsel-linux-uclibc-gcc")
#    baseTool = ""
#    if m:
#       baseTool = m.group(1)
#    return baseTool

def getBaseToolName():
   expectedPrefix = "mipsel-linux-uclibc-"
   p = re.compile(".*\\/([^\\/]+)$")
   #m = p.search(sys.argv[0])
   m = p.search("/opt/blah/mipsel-linux-uclibc-c++")
   baseTool = ""
   if m:
      baseExecName = m.group(1)
      if baseExecName.startswith(expectedPrefix):
         baseTool = baseExecName[len(expectedPrefix):]
   return baseTool

def modifyGccArgs():
   newArgs = sys.argv[1:]
   baseToolName = getBaseToolName()
   if (baseToolName == "c++") or (baseToolName == "cpp") or (baseToolName == "gcc") or (baseToolName == "g++"):
      newArgs = ["-g", "-O0", "-frecord-gcc-switches"]
      for arg in sys.argv[1:]:
         if not (arg.startswith("-g") or arg.startswith("-O")):
            newArgs.append(arg)
      
   return newArgs
   
if __name__ == "__main__":   
   #print("App name: " + sys.argv[0])
   #print("basetool name: " + getBaseToolName())

   argCount = len(sys.argv) - 1
   #print("Arg count: " + str(argCount))
   
   #for index in range(1, argCount + 1):
   #   print("Arg #" + str(index) + ": " + sys.argv[index])
      
   currentDir = os.getcwd()
   #print("Current dir: " + currentDir)

   writtenLine = ""
   writtenLine = writtenLine + "cd " + currentDir + "\n"
   writtenLine = writtenLine + buildOrgCmdLine() + "\n"
   appendToLogFile(logFileNameOrg, writtenLine)
   
   newArgs = modifyGccArgs()
   #print("New args: " + str(newArgs))

   writtenLine = ""
   writtenLine = writtenLine + "cd " + currentDir + "\n"
   writtenLine = writtenLine + buildNewCmdLine(newArgs) + "\n"
   appendToLogFile(logFileNameNew, writtenLine)

   returnCode = invokeOrgApp(newArgs)
   #print("Return code: " + str(returnCode))
   sys.exit(returnCode)
