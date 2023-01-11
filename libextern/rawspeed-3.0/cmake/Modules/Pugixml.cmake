# Download and unpack pugixml at configure time
set(PUGIXML_PREFIX "${RAWSPEED_BINARY_DIR}/src/external/pugixml")
configure_file(${RAWSPEED_SOURCE_DIR}/cmake/Modules/Pugixml.cmake.in ${PUGIXML_PREFIX}/CMakeLists.txt @ONLY)

execute_process(
  COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
  -DALLOW_DOWNLOADING_PUGIXML=${ALLOW_DOWNLOADING_PUGIXML} -DPUGIXML_PATH:PATH=${PUGIXML_PATH} .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${PUGIXML_PREFIX}
)

if(result)
  message(FATAL_ERROR "CMake step for pugixml failed: ${result}")
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${PUGIXML_PREFIX}
)

if(result)
  message(FATAL_ERROR "Build step for pugixml failed: ${result}")
endif()

set(Pugixml_FOUND 1)

# want static pugixml?
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(CMAKE_C_FLAGS_SAVE "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS_SAVE "${CMAKE_CXX_FLAGS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

set(CMAKE_CXX_CLANG_TIDY_SAVE "${CMAKE_CXX_CLANG_TIDY}")
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE_SAVE "${CMAKE_CXX_INCLUDE_WHAT_YOU_USE}")

unset(CMAKE_CXX_CLANG_TIDY)
unset(CMAKE_CXX_INCLUDE_WHAT_YOU_USE)

include(${PUGIXML_PREFIX}/pugixml-paths.cmake)

# Add pugixml directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${PUGIXML_SOURCE_DIR}
                 ${PUGIXML_BINARY_DIR})

set_target_properties(pugixml PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:pugixml,INTERFACE_INCLUDE_DIRECTORIES>)

set(Pugixml_LIBRARIES pugixml)
set(Pugixml_INCLUDE_DIRS "$<TARGET_PROPERTY:pugixml,SOURCE_DIR>/src/")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_SAVE}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_SAVE}")

set(CMAKE_CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY_SAVE}")
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${CMAKE_CXX_INCLUDE_WHAT_YOU_USE_SAVE}")
