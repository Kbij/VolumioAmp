INCLUDE_DIRECTORIES( ./src )
INCLUDE_DIRECTORIES( ${SYSROOT}/include )
file(GLOB lib_SRC
    "./src/*.cpp"
    "./src/lib/*.cpp"
)
add_library (AmpControlLib ${lib_SRC})
target_include_directories(AmpControlLib PUBLIC ./src )
target_link_libraries (AmpControlLib glog)
target_link_libraries (AmpControlLib gflags)
#target_link_libraries (AmpControlLib tinyxml2)


