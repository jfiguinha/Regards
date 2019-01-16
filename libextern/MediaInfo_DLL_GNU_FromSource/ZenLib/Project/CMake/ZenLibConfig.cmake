
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was ZenLibConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

if(NOT TARGET zen)
  include(${CMAKE_CURRENT_LIST_DIR}/ZenLibTargets.cmake)
endif()

set_and_check(ZenLib_INCLUDE_DIR "${PACKAGE_PREFIX_DIR}/include")
set(ZenLib_LIBRARY zen)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ZenLib DEFAULT_MSG ZenLib_INCLUDE_DIR ZenLib_LIBRARY)
mark_as_advanced(ZenLib_INCLUDE_DIR ZenLib_LIBRARY)

set(ZenLib_INCLUDE_DIRS ${ZenLib_INCLUDE_DIR})
set(ZenLib_LIBRARIES ${ZenLib_LIBRARY})
