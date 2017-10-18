# - Try to find the VirtualInput
# Once done this will define
#  LIBVIRTUAL_KEYBOARD_FOUND - System has a virtual input
#  LIBVIRTUAL_KEYBOARD_INCLUDE_DIRS - The include directories
#  LIBVIRTUAL_KEYBOARD_LIBRARIES - The libraries needed to use 
#
# Copyright (C) 2016 Metrological.
#
find_package(PkgConfig)
pkg_check_modules(LIBVIRTUAL_INPUT virtualkeyboard)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBVIRTUAL_INPUT DEFAULT_MSG
        LIBVIRTUAL_INPUT_FOUND
        LIBVIRTUAL_INPUT_INCLUDE_DIRS
        LIBVIRTUAL_INPUT_LIBRARIES
        )

mark_as_advanced(LIBVIRTUAL_INPUT_INCLUDE_DIRS LIBVIRTUAL_INPUT_LIBRARIES)
