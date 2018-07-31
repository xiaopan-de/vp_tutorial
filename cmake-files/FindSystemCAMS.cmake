# - Find SystemC-AMS
# Find the SystemC-AMS libraries (systemc-ams)
#
#  This module defines the following variables:
#     SYSTEMCAMS_FOUND       - True if SYSTEMCAMS_INCLUDE_DIR & SYSTEMCAMS_LIBRARY are found
#     SYSTEMCAMS_LIBRARIES   - Set when SYSTEMCAMS_LIBRARY is found
#     SYSTEMCAMS_INCLUDE_DIRS - Set when SYSTEMCAMS_INCLUDE_DIR is found
#
#     SYSTEMCAMS_INCLUDE_DIR - where to find systemc-ams.h, etc.
#     SYSTEMCAMS_LIBRARY     - the systemc-ams library
#     SYSTEMCAMS_VERSION_STRING - the version of SystemC-AMS found (since CMake 2.8.8)
#

#=============================================================================
# Copyright 2012 Javier Moreno
# Copyright 2012 Javier Moreno <moreno@cs.uni-kl.de>
#
# Distributed under the OSI-approved BSD License (the "License");
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

find_path(SYSTEMCAMS_INCLUDE_DIR NAMES systemc-ams systemc-ams.h HINTS $ENV{SYSTEMCAMS_BASE} $ENV{SYSTEMC_AMS}
          PATH_SUFFIXES include
          DOC "The SystemC-AMS include directory"
)

find_library(SYSTEMCAMS_LIBRARY NAMES systemc-ams HINTS $ENV{SYSTEMCAMS_BASE} $ENV{SYSTEMC_AMS} 
          PATH_SUFFIXES lib lib-linux lib-linux64 lib-macosx lib-macosx64 lib-darwin
          DOC "The SystemC-AMS library"
)

if(SYSTEMCAMS_INCLUDE_DIR AND EXISTS "${SYSTEMCAMS_INCLUDE_DIR}/sysc/kernel/sc_ver.h")
  file(STRINGS "${SYSTEMCAMS_INCLUDE_DIR}/config.h" systemc-ams_version_str REGEX "^#define[\t ]+VERSION[\t ].[0-9].[0-9].*")
  string(REGEX REPLACE "^.*VERSION.([0-9]).([0-9]).*$" "\\1.\\2" SYSTEMCAMS_VERSION_STRING "${systemc-ams_version_str}")
  unset(systemc-ams_version_str)
endif()

# handle the QUIETLY and REQUIRED arguments and set SYSTEMCAMS_FOUND to TRUE if 
# all listed variables are TRUE
include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SYSTEMCAMS
                                  REQUIRED_VARS SYSTEMCAMS_LIBRARY SYSTEMCAMS_INCLUDE_DIR
                                  VERSION_VAR SYSTEMCAMS_VERSION_STRING)

if(SYSTEMCAMS_FOUND)
  set( SYSTEMCAMS_LIBRARIES ${SYSTEMCAMS_LIBRARY} )
  set( SYSTEMCAMS_INCLUDE_DIRS ${SYSTEMCAMS_INCLUDE_DIR} )
endif()