# Copyright 2008 Pino Toscano, <pino@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

macro(POPPLER_ADD_TEST exe build_flag)
  set(build_test ${${build_flag}})
  if(NOT build_test)
    set(_add_executable_param ${_add_executable_param} EXCLUDE_FROM_ALL)
  endif(NOT build_test)

  add_executable(${exe} ${_add_executable_param} ${ARGN})

  # if the tests are EXCLUDE_FROM_ALL, add a target "buildtests" to build all tests
  if(NOT build_test AND NOT MSVC_IDE)
    get_property(_buildtestsAdded GLOBAL PROPERTY BUILDTESTS_ADDED)
    if(NOT _buildtestsAdded)
      add_custom_target(buildtests)
      set_property(GLOBAL PROPERTY BUILDTESTS_ADDED TRUE)
    endif(NOT _buildtestsAdded)
    add_dependencies(buildtests ${exe})
  endif(NOT build_test AND NOT MSVC_IDE)
endmacro(POPPLER_ADD_TEST)

macro(POPPLER_ADD_UNITTEST exe build_flag)
  set(build_test ${${build_flag}})
  if(NOT build_test)
    set(_add_executable_param ${_add_executable_param} EXCLUDE_FROM_ALL)
  endif(NOT build_test)

  add_executable(${exe} ${_add_executable_param} ${ARGN})
  add_test(${exe} ${EXECUTABLE_OUTPUT_PATH}/${exe})

  # if the tests are EXCLUDE_FROM_ALL, add a target "buildtests" to build all tests
  if(NOT build_test)
    get_property(_buildtestsAdded GLOBAL PROPERTY BUILDTESTS_ADDED)
    if(NOT _buildtestsAdded)
      add_custom_target(buildtests)
      set_property(GLOBAL PROPERTY BUILDTESTS_ADDED TRUE)
    endif(NOT _buildtestsAdded)
    add_dependencies(buildtests ${exe})
  endif(NOT build_test)
endmacro(POPPLER_ADD_UNITTEST)

macro(POPPLER_CREATE_INSTALL_PKGCONFIG generated_file install_location)
  configure_file(${generated_file}.cmake ${CMAKE_CURRENT_BINARY_DIR}/${generated_file} @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${generated_file} DESTINATION ${install_location})
endmacro(POPPLER_CREATE_INSTALL_PKGCONFIG)

macro(SHOW_END_MESSAGE what value)
  string(LENGTH ${what} length_what)
  math(EXPR left_char "20 - ${length_what}")
  set(blanks)
  foreach(_i RANGE 1 ${left_char})
    set(blanks "${blanks} ")
  endforeach(_i)

  message("  ${what}:${blanks} ${value}")
endmacro(SHOW_END_MESSAGE)

macro(SHOW_END_MESSAGE_YESNO what enabled)
  if(${enabled})
    set(enabled_string "yes")
  else(${enabled})
    set(enabled_string "no")
  endif(${enabled})

  show_end_message("${what}" "${enabled_string}")
endmacro(SHOW_END_MESSAGE_YESNO)

macro(POPPLER_CHECK_LINK_FLAG flag var)
   set(_save_CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES}")
   include(CheckCXXSourceCompiles)
   set(CMAKE_REQUIRED_LIBRARIES "${flag}")
   check_cxx_source_compiles("int main() { return 0; }" ${var})
   set(CMAKE_REQUIRED_LIBRARIES "${_save_CMAKE_REQUIRED_LIBRARIES}")
endmacro(POPPLER_CHECK_LINK_FLAG)


set(CMAKE_SYSTEM_INCLUDE_PATH ${CMAKE_SYSTEM_INCLUDE_PATH}
                              "${CMAKE_INSTALL_PREFIX}/include" )

set(CMAKE_SYSTEM_PROGRAM_PATH ${CMAKE_SYSTEM_PROGRAM_PATH}
                              "${CMAKE_INSTALL_PREFIX}/bin" )

set(CMAKE_SYSTEM_LIBRARY_PATH ${CMAKE_SYSTEM_LIBRARY_PATH}
                              "${CMAKE_INSTALL_PREFIX}/lib" )

# under Windows dlls may be also installed in bin/
if(WIN32)
  set(CMAKE_SYSTEM_LIBRARY_PATH ${CMAKE_SYSTEM_LIBRARY_PATH}
                                "${CMAKE_INSTALL_PREFIX}/bin" )
endif(WIN32)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

if(CMAKE_COMPILER_IS_GNUCXX)
  # set the default compile warnings
  set(_warn "-Wall -Wextra -Wpedantic")
  set(_warn "${_warn} -Wno-unused-parameter")
  set(_warn "${_warn} -Wcast-align")
  set(_warn "${_warn} -Wformat-security")
  set(_warn "${_warn} -Wframe-larger-than=65536")
  set(_warn "${_warn} -Wlogical-op")
  set(_warn "${_warn} -Wmissing-format-attribute")
  set(_warn "${_warn} -Wnon-virtual-dtor")
  set(_warn "${_warn} -Woverloaded-virtual")
  set(_warn "${_warn} -Wmissing-declarations")
  set(_warn "${_warn} -Wundef")
  set(_warn "${_warn} -Wzero-as-null-pointer-constant")
  set(_warn "${_warn} -Wshadow")
  if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.0.0")
    set(_warn "${_warn} -Wsuggest-override")
  endif()

  # set extra warnings
  set(_warnx "${_warnx} -Wconversion")
  set(_warnx "${_warnx} -Wuseless-cast")

  set(DEFAULT_COMPILE_WARNINGS "${_warn}")
  set(DEFAULT_COMPILE_WARNINGS_EXTRA "${_warn} ${_warnx}")

  set(_save_cxxflags "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS                "-fno-exceptions -fno-check-new -fno-common -D_DEFAULT_SOURCE")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g ${_save_cxxflags}")
  set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG ${_save_cxxflags}")
  set(CMAKE_CXX_FLAGS_DEBUG          "-g -O2 -fno-reorder-blocks -fno-schedule-insns -fno-inline ${_save_cxxflags}")
  set(CMAKE_CXX_FLAGS_DEBUGFULL      "-g3 -fno-inline ${_save_cxxflags}")
  set(CMAKE_CXX_FLAGS_PROFILE        "-g3 -fno-inline -ftest-coverage -fprofile-arcs ${_save_cxxflags}")
  set(_save_cflags "${CMAKE_C_FLAGS}")
  set(CMAKE_C_FLAGS                  "-std=c99 -D_DEFAULT_SOURCE")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g ${_save_cflags}")
  set(CMAKE_C_FLAGS_RELEASE          "-O2 -DNDEBUG ${_save_cflags}")
  set(CMAKE_C_FLAGS_DEBUG            "-g -O2 -fno-reorder-blocks -fno-schedule-insns -fno-inline ${_save_cflags}")
  set(CMAKE_C_FLAGS_DEBUGFULL        "-g3 -fno-inline ${_save_cflags}")
  set(CMAKE_C_FLAGS_PROFILE          "-g3 -fno-inline -ftest-coverage -fprofile-arcs ${_save_cflags}")

  poppler_check_link_flag("-Wl,--as-needed" GCC_HAS_AS_NEEDED)
  if(GCC_HAS_AS_NEEDED)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--as-needed")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--as-needed")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed")
  endif(GCC_HAS_AS_NEEDED)
endif (CMAKE_COMPILER_IS_GNUCXX)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
# set the default compile warnings
  set(_warn "-Wall -Wextra -Wpedantic")
  set(_warn "${_warn} -Wno-unused-parameter")
  set(_warn "${_warn} -Wcast-align")
  set(_warn "${_warn} -Wformat-security")
  set(_warn "${_warn} -Wframe-larger-than=65536")
  set(_warn "${_warn} -Wmissing-format-attribute")
  set(_warn "${_warn} -Wnon-virtual-dtor")
  set(_warn "${_warn} -Woverloaded-virtual")
  set(_warn "${_warn} -Wmissing-declarations")
  set(_warn "${_warn} -Wundef")
  set(_warn "${_warn} -Wzero-as-null-pointer-constant")
  set(_warn "${_warn} -Wshadow")

  # set extra warnings
  set(_warnx "${_warnx} -Wconversion")

  set(DEFAULT_COMPILE_WARNINGS "${_warn}")
  set(DEFAULT_COMPILE_WARNINGS_EXTRA "${_warn} ${_warnx}")
endif()

if(CMAKE_C_COMPILER MATCHES "icc")
  set(_save_cxxflags "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g ${_save_cxxflags}")
  set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG ${_save_cxxflags}")
  set(CMAKE_CXX_FLAGS_DEBUG          "-O2 -g -0b0 -noalign ${_save_cxxflags}")
  set(CMAKE_CXX_FLAGS_DEBUGFULL      "-g -Ob0 -noalign ${_save_cxxflags}")
  set(_save_cflags "${CMAKE_C_FLAGS}")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g ${_save_cflags}")
  set(CMAKE_C_FLAGS_RELEASE          "-O2 -DNDEBUG ${_save_cflags}")
  set(CMAKE_C_FLAGS_DEBUG            "-O2 -g -Ob0 -noalign ${_save_cflags}")
  set(CMAKE_C_FLAGS_DEBUGFULL        "-g -Ob0 -noalign ${_save_cflags}")
endif(CMAKE_C_COMPILER MATCHES "icc")

