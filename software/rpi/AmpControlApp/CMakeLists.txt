INCLUDE_DIRECTORIES( ./src )
INCLUDE_DIRECTORIES( ${SYSROOT}/include )
file(GLOB app_SRC
    "./src/*.h"
    "./src/*.cpp"
)
add_executable (AmpControl ${app_SRC})

target_link_libraries (AmpControl AmpControlLib)
target_link_libraries (AmpControl pthread)
target_link_libraries (AmpControl rt)

#if("${RGBCLOCK_PLATFORM}" STREQUAL "arm")
#    add_custom_command(TARGET RgbClock POST_BUILD COMMAND scp $<TARGET_FILE:RgbClock> pi@motioncam.lan:/home/pi )
#
#endif()    





