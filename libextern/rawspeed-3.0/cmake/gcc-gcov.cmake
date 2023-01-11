if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3)
    message(FATAL_ERROR "Clang version must be 3.0.0 or greater! Aborting...")
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
elseif(NOT CMAKE_COMPILER_IS_GNUCXX)
  message(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
endif()

if(NOT RAWSPEED_COVERAGE_BUILD)
  message(WARNING "Wrong build type, need COVERAGE.")
endif()

find_package(GCov REQUIRED)
find_package(Find REQUIRED)

set(GCOV_OPTS "-pb")

if(NOT APPLE)
  # DON'T elide source prefix.
  set(GCOV_OPTS ${GCOV_OPTS} -aflu)
endif()

add_custom_target(
  gcov
  COMMAND "${FIND_PATH}" "${CMAKE_BINARY_DIR}" -type f -name '*.gcno' -exec "${GCOV_PATH}" ${GCOV_OPTS} {} \\\\; > /dev/null
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  COMMENT "Running gcov tool on all the *.gcno files to produce *.gcov files"
  USES_TERMINAL
)

# DON'T remove *.gcno/*.gcov files here!
add_custom_target(
  gcov-clean
  COMMAND "${FIND_PATH}" "${CMAKE_BINARY_DIR}" -type f -name '*.gcda' -delete > /dev/null
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  COMMENT "Removing all the *.gcda files"
)
