cmake_minimum_required (VERSION 3.23)
#set( CMAKE_VERBOSE_MAKEFILE on )
project (AmpControl)
message(STATUS "CMAKE platform: " ${AMPCONTROL_PLATFORM})

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin-${AMPCONTROL_PLATFORM})
set (SYSROOT ${CMAKE_CURRENT_SOURCE_DIR}/libraries/sysroot-${AMPCONTROL_PLATFORM})
message(STATUS "Sysroot: " ${SYSROOT})
link_directories(${SYSROOT}/lib)

set(BUILD_STATIC_LIBS ON)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


add_subdirectory (./AmpControlLib)
add_subdirectory (./AmpControlApp)
add_subdirectory (./AmpControlTest)
