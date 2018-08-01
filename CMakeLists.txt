cmake_minimum_required (VERSION 2.8)
project (vp_tutorial)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-files)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -std=c++11 -DSC_INCLUDE_DYNAMIC_PROCESSES")

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
if(DEFINED ENV{GCC_HOME})
	if(EXISTS "$ENV{GCC_HOME}/lib64")
		set(CMAKE_INSTALL_RPATH "$ENV{GCC_HOME}/lib64")
	else()
		set(CMAKE_INSTALL_RPATH "$ENV{GCC_HOME}/lib")
	endif()
endif()

if (DEBUG_LEVEL)
    add_definitions(-DDEBUG_LEVEL=${DEBUG_LEVEL})
endif()


find_package(SystemC REQUIRED)
find_package(SystemCAMS REQUIRED)

include_directories(${SYSTEMC_INCLUDE_DIRS}
                    ${SYSTEMCAMS_INCLUDE_DIRS}
                    )

add_subdirectory(SystemC_TLM/tlm_demo1)
add_subdirectory(SystemC_TLM/tlm_demo2)
add_subdirectory(SystemC_TLM/tlm_demo3_sync)
add_subdirectory(SystemC_TLM/tlm_demo3_decop)
