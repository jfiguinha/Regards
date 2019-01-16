# Install script for directory: C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/MediaInfoLib")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/MediaInfoLib/lib/libmediainfo.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/MediaInfoLib/lib" TYPE STATIC_LIBRARY FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/libmediainfo.a")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}C:/Program Files (x86)/MediaInfoLib/lib/cmake/mediainfolib/MediaInfoLibTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}C:/Program Files (x86)/MediaInfoLib/lib/cmake/mediainfolib/MediaInfoLibTargets.cmake"
         "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/CMakeFiles/Export/C_/Program_Files_(x86)/MediaInfoLib/lib/cmake/mediainfolib/MediaInfoLibTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}C:/Program Files (x86)/MediaInfoLib/lib/cmake/mediainfolib/MediaInfoLibTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}C:/Program Files (x86)/MediaInfoLib/lib/cmake/mediainfolib/MediaInfoLibTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/MediaInfoLib/lib/cmake/mediainfolib/MediaInfoLibTargets.cmake")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/MediaInfoLib/lib/cmake/mediainfolib" TYPE FILE FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/CMakeFiles/Export/C_/Program_Files_(x86)/MediaInfoLib/lib/cmake/mediainfolib/MediaInfoLibTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/MediaInfoLib/lib/cmake/mediainfolib/MediaInfoLibTargets-noconfig.cmake")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/MediaInfoLib/lib/cmake/mediainfolib" TYPE FILE FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/CMakeFiles/Export/C_/Program_Files_(x86)/MediaInfoLib/lib/cmake/mediainfolib/MediaInfoLibTargets-noconfig.cmake")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/MediaInfo" TYPE FILE FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/../../Source/MediaInfo/MediaInfo.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/MediaInfo" TYPE FILE FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/../../Source/MediaInfo/MediaInfoList.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/MediaInfo" TYPE FILE FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/../../Source/MediaInfo/MediaInfo_Const.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/MediaInfo" TYPE FILE FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/../../Source/MediaInfo/MediaInfo_Events.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/MediaInfoDLL" TYPE FILE FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/../../Source/MediaInfoDLL/MediaInfoDLL.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/MediaInfoDLL" TYPE FILE FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/../../Source/MediaInfoDLL/MediaInfoDLL_Static.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/MediaInfoLib/lib/pkgconfig/libmediainfo.pc")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/MediaInfoLib/lib/pkgconfig" TYPE FILE FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/libmediainfo.pc")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/MediaInfoLib/lib/cmake/mediainfolib/MediaInfoLibConfig.cmake;C:/Program Files (x86)/MediaInfoLib/lib/cmake/mediainfolib/MediaInfoLibConfigVersion.cmake")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/MediaInfoLib/lib/cmake/mediainfolib" TYPE FILE FILES
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/MediaInfoLibConfig.cmake"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/MediaInfoLibConfigVersion.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/ZenLib/cmake_install.cmake")
  include("C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/zlib/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
