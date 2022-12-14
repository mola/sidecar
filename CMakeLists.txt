# -*- Mode: CMake -*-

# Top-level CMake file for the SideCar project.
#
cmake_minimum_required(VERSION 3.4)

project(SIDECAR VERSION 2.1.0)

set(SIDECAR_VERSION ${PROJECT_VERSION})
message(STATUS "Version ${SIDECAR_VERSION}")

# Enable debug build if -Debug=1; otherwise create release build
#
if(ebug)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build " FORCE)
  message(STATUS "building DEBUG targets")
else(ebug)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build " FORCE)
  message(STATUS "building RELEASE targets")
endif(ebug)

# Enable use of ccache if -Dccache=1
#
if(ccache)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
        message(STATUS "Using 'ccache' for C/C++ compiles")
    else(CCACHE_PROGRAM)
        message(STATUS "No 'ccache' program found -- not using")
    endif(CCACHE_PROGRAM)
endif(ccache)

# Load in the CMake customizations for the SideCar project
#
include("CMakeStuff/Configuration.cmake")

add_subdirectory(Scripts)
add_subdirectory(GeoStars)
add_subdirectory(XMLRPC)

add_subdirectory(Threading)
add_subdirectory(Utils)
add_subdirectory(Time)
add_subdirectory(Logger)

add_subdirectory(UnitTest)
add_subdirectory(Parameter)
add_subdirectory(Zeroconf)

add_subdirectory(IO)
add_subdirectory(Messages)
add_subdirectory(Configuration)
add_subdirectory(Runner)

add_subdirectory(Algorithms)
add_subdirectory(GUI)
# add_subdirectory(LLDRFM)

# Create a symbolic link in the build directory to the data directory
#
add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/data
                   COMMAND ${CMAKE_COMMAND} -E create_symlink ${SIDECAR}/data ${PROJECT_BINARY_DIR}/data)
add_custom_target(link-data ALL DEPENDS ${PROJECT_BINARY_DIR}/data)

# Install the data directory
#
install(DIRECTORY data DESTINATION ./ PATTERN ".svn" EXCLUDE PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

# Invoke a helper script to make a new 'latest' symbolic link to point to the latest install directory. I tried
# to do this within CMake, but was unable to do so for some reason.
#
install(CODE "execute_process(COMMAND $ENV{SHELL} Scripts/doinstall.sh ${CMAKE_INSTALL_PREFIX})")
