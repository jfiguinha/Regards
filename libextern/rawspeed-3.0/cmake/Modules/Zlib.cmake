# Download and unpack zlib at configure time
set(ZLIB_PREFIX "${RAWSPEED_BINARY_DIR}/src/external/zlib")
configure_file(${RAWSPEED_SOURCE_DIR}/cmake/Modules/Zlib.cmake.in ${ZLIB_PREFIX}/CMakeLists.txt @ONLY)

execute_process(
  COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
  -DALLOW_DOWNLOADING_ZLIB=${ALLOW_DOWNLOADING_ZLIB} -DZLIB_PATH:PATH=${ZLIB_PATH} .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${ZLIB_PREFIX}
)

if(result)
  message(FATAL_ERROR "CMake step for zlib failed: ${result}")
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${ZLIB_PREFIX}
)

if(result)
  message(FATAL_ERROR "Build step for zlib failed: ${result}")
endif()

set(ZLIB_FOUND 1)

set(CMAKE_C_FLAGS_SAVE "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS_SAVE "${CMAKE_CXX_FLAGS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

set(CMAKE_CXX_CLANG_TIDY_SAVE "${CMAKE_CXX_CLANG_TIDY}")
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE_SAVE "${CMAKE_CXX_INCLUDE_WHAT_YOU_USE}")

unset(CMAKE_CXX_CLANG_TIDY)
unset(CMAKE_CXX_INCLUDE_WHAT_YOU_USE)

include(${ZLIB_PREFIX}/zlib-paths.cmake)

# XXX make sure that zlib is using it's own headers
# see https://github.com/madler/zlib/issues/218
include_directories(BEFORE SYSTEM ${ZLIB_PREFIX}/zlib-src)
include_directories(BEFORE SYSTEM ${ZLIB_PREFIX}/zlib-build)

# Add zlib directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${ZLIB_SOURCE_DIR}
                 ${ZLIB_BINARY_DIR})

set(_zlib_lib zlib)       # shared
set(_zlib_lib zlibstatic) # static

set_target_properties(${_zlib_lib} PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${_zlib_lib},INTERFACE_INCLUDE_DIRECTORIES>)

set(ZLIB_LIBRARIES ${_zlib_lib})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_SAVE}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_SAVE}")

set(CMAKE_CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY_SAVE}")
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${CMAKE_CXX_INCLUDE_WHAT_YOU_USE_SAVE}")
