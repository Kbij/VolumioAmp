INCLUDE_DIRECTORIES( ./src )
INCLUDE_DIRECTORIES( ${SYSROOT}/include )
file(GLOB app_SRC
    "./src/*.h"
    "./src/*.cpp"
)
add_executable (AmpControlTest ${app_SRC})

target_link_libraries (AmpControlTest AmpControlLib)
target_link_libraries (AmpControlTest rt)
target_link_libraries (AmpControlTest gtest)
target_link_libraries (AmpControlTest gmock)
target_link_libraries (AmpControlTest pthread)

#if("${AMPCONTROL_PLATFORM}" STREQUAL "arm")
#    add_custom_command(TARGET RgbTest POST_BUILD COMMAND scp $<TARGET_FILE:RgbTest> pi@motioncam.lan:/home/pi )
#endif()