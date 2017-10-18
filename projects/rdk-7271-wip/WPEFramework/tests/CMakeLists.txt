option(WPEFRAMEWORK_TEST_MQCTEST
    "include the MQC tests." OFF)
option(WPEFRAMEWORK_TEST_RASPIO
    "include the RaspberryPi IO tests." OFF)
option(WPEFRAMEWORK_TEST_NRF24L01
    "include the NRF24L01 tests." OFF)
option(WPEFRAMEWORK_TEST_IPCTEST
    "include the IPC tests." OFF)
option(WPEFRAMEWORK_TEST_COMMCLIENT
    "include the CommClient tests." OFF)
option(WPEFRAMEWORK_TEST_CYCLICBUFFER
    "include the CyclicBuffer tests." OFF)
option(WPEFRAMEWORK_TEST_CYCLICBUFFERSTRESS
    "include the CyclicBuffer stress test." OFF)
option(WPEFRAMEWORK_TEST_CYCLICINSPECTOR
    "include the CyclicBuffer inspector." OFF)
option(WPEFRAMEWORK_TEST_JSONTESTS
    "include the JSON tests." OFF)
option(WPEFRAMEWORK_TEST_LOADER
    "include the test plugin loader." OFF)
option(WPEFRAMEWORK_TEST_MEMORYTEST
    "include the memory tests." OFF)
option(WPEFRAMEWORK_TEST_NETWORKTEST
    "include the network tests." OFF)
option(WPEFRAMEWORK_TEST_QTWEBINSPECTOR
    "include the QT web inspector tests." OFF)
option(WPEFRAMEWORK_TEST_TESTCONSOLE
    "include the test console tests." OFF)
option(WPEFRAMEWORK_TEST_TESTSERVER
    "include the test server tests." OFF)
option(WPEFRAMEWORK_TEST_TRACINGTEST
    "include the tracing tests." OFF)
option(WPEFRAMEWORK_TEST_WEBINSPECTOR
    "include the web inspector tests." OFF)
option(WPEFRAMEWORK_TEST_DEVICEID
    "include the web inspector tests." OFF)
option(WPEFRAMEWORK_TEST_PIDINFO
    "include the PID info tests." OFF)
option(WPEFRAMEWORK_TEST_DEADLOCK
    "include the deadlock tests." OFF)
option(WPEFRAMEWORK_TEST_RPCDEPTH
    "include the RPC max-depth ASSERT tests." OFF)
option(WPEFRAMEWORK_TEST_TIMESET
    "include the Time setting tests." OFF)
option(WPEFRAMEWORK_TEST_SIMPLEWEBCLIENT
    "include the Simple web client tests." OFF)
option(WPEFRAMEWORK_TEST_IPV6TEST
    "include the IPv6 NodeId tests." OFF)
option(WPEFRAMEWORK_TEST_OUTOFPROCESS
        "include the out of process test." OFF)


if(WPEFRAMEWORK_TEST_DEVICEID)
  add_subdirectory(DeviceInfoTest) 
  add_subdirectory(getid_tool) 
endif()
        
if(WPEFRAMEWORK_TEST_MQCTEST AND WPEFRAMEWORK_MQC)
  add_subdirectory(MQCTest) 
endif()
 
if(WPEFRAMEWORK_TEST_RASPIO)
  add_subdirectory(raspio) 
endif()

if(WPEFRAMEWORK_TEST_NRF24L01)
  add_subdirectory(nrf24l01) 
endif()
 
if(WPEFRAMEWORK_TEST_IPCTEST)
  add_subdirectory(IPCTest) 
endif()
 
if(WPEFRAMEWORK_TEST_CYCLICBUFFER)
  add_subdirectory(CyclicBuffer) 
endif()

if(WPEFRAMEWORK_TEST_COMMCLIENT)
  add_subdirectory(CommClientTest) 
endif()
 
if(WPEFRAMEWORK_TEST_CYCLICBUFFERSTRESS)
  add_subdirectory(CyclicBufferStress) 
endif()
 
if(WPEFRAMEWORK_TEST_CYCLICINSPECTOR)
  add_subdirectory(CyclicInspector)
endif()

if(WPEFRAMEWORK_TEST_TRACINGTEST)
  add_subdirectory(TracingTest)
endif()

if(WPEFRAMEWORK_TEST_JSONTESTS)
  add_subdirectory(JSONTests) 
endif()

if(WPEFRAMEWORK_TEST_LOADER)
  add_subdirectory(loader) 
endif()

if(WPEFRAMEWORK_TEST_MEMORYTEST)
  add_subdirectory(memorytest) 
endif()

if(WPEFRAMEWORK_TEST_NETWORKTEST)
  add_subdirectory(NetworkInfo) 
endif()
 
if(WPEFRAMEWORK_TEST_QTWEBINSPECTOR)
  add_subdirectory(QtWebInspector) 
endif()
 
if(WPEFRAMEWORK_TEST_TESTCONSOLE)
  add_subdirectory(testconsole) 
endif()
 
if(WPEFRAMEWORK_TEST_TESTSERVER)
  add_subdirectory(testserver) 
endif()  

if(WPEFRAMEWORK_TEST_WEBINSPECTOR)
  add_subdirectory(webinspector) 
endif()
  
if(WPEFRAMEWORK_TEST_PIDINFO)
  add_subdirectory(PidInfoTest) 
endif()

if(WPEFRAMEWORK_TEST_DEADLOCK)
  add_subdirectory(DeadlockTest) 
endif()

if(WPEFRAMEWORK_TEST_RPCDEPTH)
  add_subdirectory(RPCDepthTest) 
endif()

if(WPEFRAMEWORK_TEST_TIMESET)
  add_subdirectory(TimeSetTest)
endif()

if(WPEFRAMEWORK_TEST_SIMPLEWEBCLIENT)
  add_subdirectory(SimpleWebClient)
endif()

if(WPEFRAMEWORK_TEST_IPV6TEST)
  add_subdirectory(IPv6)
endif()

if(WPEFRAMEWORK_TEST_OUTOFPROCESS)
  add_subdirectory(OutOfProcess)
endif()