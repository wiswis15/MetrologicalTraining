cmake_minimum_required(VERSION 3.6)
project(Documentation)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        documents/team-meeting/Code_agreements.md
        documents/team-meeting/DevMeeting_2017-01-11.md
        documents/team-meeting/DevMeeting_2017-01-18.md
        documents/team-meeting/DevMeeting_2017-01-25.md
        documents/team-meeting/DevMeeting_2017-02-01.md
        documents/team-meeting/General_agreements.md
        projects/cmake/mastering-cmake.pdf
        projects/dawn/jit-presentation/jit-presentation.odp
        projects/dawn/jit-presentation/jit-presentation.pdf
        projects/dawn/jit-presentation/parmOverride01.py
        projects/dawn/PaceBuildEnvironment/createTgz.sh
        projects/dawn/PaceBuildEnvironment/QtBrowser_Command-line.txt
        projects/dawn/Browser-Release_Notes.txt
        "projects/dawn/Dawn Workbook.docx"
        projects/dawn/MeasurementsApril.pdf
        "projects/dawn/TN001 - Dawn-Pace build instructions.doc"
        "projects/dawn/TN002 - QtBrowser Tooling.odt"
        "projects/dawn/TN002 - QtBrowser Tooling.pdf"
        "projects/dawn/TN002 - wkmemlogparser.scm"
        "projects/dawn/TN010 - QtWebDriver.odt"
        "projects/dawn/TN010 - QtWebDriver.pdf"
        projects/dawn/whitelist.json
        "projects/eos/DCX960EPR1 Bootloader.zip"
        "projects/eos/Instructions to Update Bootloader DCX960.txt"
        "projects/eos/Steps to boot DCX960 EPR1 box.pdf"
        projects/gdb/gdb-guide.pdf
        projects/gdb/GDB_Cheat_Sheet.pdf
        projects/gdb/gdbref.pdf
        "projects/gdb/TN002 - QtCreator Debugging Instructions.odt"
        "projects/gdb/TN002 - QtCreator Debugging Instructions.pdf"
        "projects/gstreamer/GStreamer - cheat sheet.doc"
        projects/gstreamer/Gstreamer-webkit.key
        projects/gstreamer/Gstreamer-webkit.pdf
        "projects/horizon/Horizon CPE Error codes - HZN 33.pdf"
        projects/jekyll/install.md
        projects/mkdocs/install.md
        projects/playready/PlayReady.chm
        "projects/rdk/TN - Build RDK emulator.doc"
        projects/rpi/linux_keys_mapping.numbers
        "projects/templates/Metrological Template.doc"
        projects/testtools/loop-buildroot
        projects/testtools/restartBrowser
        projects/vpn/ca.crt
        projects/vpn/config.conf
        "projects/vpn/VPN Client Configuration on Ubuntu.pdf"
        "projects/Coding Standard - C++.doc"
        "projects/Coding Standard - C++.pdf"
        projects/Commitnumbers.xlsx
        sources/code-snippets/C++11_features/test_c++11.cpp
        sources/code-snippets/C++11_features/test_c++11.h
        sources/code-snippets/Using_const/using_const.cpp
        sources/cppunitdemo/src/Calculator.cpp
        sources/cppunitdemo/src/Calculator.h
        sources/cppunitdemo/src/main.cpp
        sources/cppunitdemo/tst/CalculatorTest.cpp
        sources/cppunitdemo/tst/CalculatorTest.h
        sources/cppunitdemo/tst/CppUnitWrapper.cpp
        sources/cppunitdemo/tst/CppUnitWrapper.h
        sources/cppunitdemo/tst/mainTest.cpp
        sources/cppunitdemo/tst/UnitTests.h
        README.md)

add_executable(Documentation ${SOURCE_FILES})