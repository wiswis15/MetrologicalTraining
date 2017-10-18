This application tries to test the Cyclic Buffer in such a way that 100% of its code is touched ("100% coverage").

To run the test:
1. run "build.sh", it will rebuild WPEFramework in a local folder. The build will instruct gcc to add code to track code execution.
2. run "run.sh", it will invoke the test app as server and client. They will run a test script hardcoded in the application.
3. run "genCoverage.h", this script will create the "coverages" dir and write the coverage files in it.

There is also a "clean.sh" script, this one removes all generated directories. Use this one to reset the coverage numbers.

Currently (sept 6, 2016), CyclicBuffer.cpp only has a coverage of 38.78%.

