# Install script for directory: C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ZenLib")
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
   "C:/Program Files (x86)/ZenLib/lib/libzen.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/ZenLib/lib" TYPE STATIC_LIBRARY FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/libzen.a")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/ZenLib/include/ZenLib/BitStream.h;C:/Program Files (x86)/ZenLib/include/ZenLib/BitStream_Fast.h;C:/Program Files (x86)/ZenLib/include/ZenLib/BitStream_LE.h;C:/Program Files (x86)/ZenLib/include/ZenLib/Conf.h;C:/Program Files (x86)/ZenLib/include/ZenLib/Conf_Internal.h;C:/Program Files (x86)/ZenLib/include/ZenLib/CriticalSection.h;C:/Program Files (x86)/ZenLib/include/ZenLib/Dir.h;C:/Program Files (x86)/ZenLib/include/ZenLib/File.h;C:/Program Files (x86)/ZenLib/include/ZenLib/FileName.h;C:/Program Files (x86)/ZenLib/include/ZenLib/InfoMap.h;C:/Program Files (x86)/ZenLib/include/ZenLib/MemoryDebug.h;C:/Program Files (x86)/ZenLib/include/ZenLib/OS_Utils.h;C:/Program Files (x86)/ZenLib/include/ZenLib/PreComp.h;C:/Program Files (x86)/ZenLib/include/ZenLib/Thread.h;C:/Program Files (x86)/ZenLib/include/ZenLib/Trace.h;C:/Program Files (x86)/ZenLib/include/ZenLib/Translation.h;C:/Program Files (x86)/ZenLib/include/ZenLib/Utils.h;C:/Program Files (x86)/ZenLib/include/ZenLib/Ztring.h;C:/Program Files (x86)/ZenLib/include/ZenLib/ZtringList.h;C:/Program Files (x86)/ZenLib/include/ZenLib/ZtringListList.h;C:/Program Files (x86)/ZenLib/include/ZenLib/ZtringListListF.h;C:/Program Files (x86)/ZenLib/include/ZenLib/int128s.h;C:/Program Files (x86)/ZenLib/include/ZenLib/int128u.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/ZenLib/include/ZenLib" TYPE FILE FILES
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/BitStream.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/BitStream_Fast.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/BitStream_LE.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Conf.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Conf_Internal.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/CriticalSection.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Dir.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/File.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/FileName.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/InfoMap.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/MemoryDebug.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/OS_Utils.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/PreComp.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Thread.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Trace.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Translation.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Utils.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Ztring.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/ZtringList.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/ZtringListList.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/ZtringListListF.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/int128s.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/int128u.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/ZenLib/include/ZenLib/Format/Html/Html_Handler.h;C:/Program Files (x86)/ZenLib/include/ZenLib/Format/Html/Html_Request.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/ZenLib/include/ZenLib/Format/Html" TYPE FILE FILES
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Format/Html/Html_Handler.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Format/Html/Html_Request.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/ZenLib/include/ZenLib/Format/Http/Http_Cookies.h;C:/Program Files (x86)/ZenLib/include/ZenLib/Format/Http/Http_Handler.h;C:/Program Files (x86)/ZenLib/include/ZenLib/Format/Http/Http_Request.h;C:/Program Files (x86)/ZenLib/include/ZenLib/Format/Http/Http_Utils.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/ZenLib/include/ZenLib/Format/Http" TYPE FILE FILES
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Format/Http/Http_Cookies.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Format/Http/Http_Handler.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Format/Http/Http_Request.h"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/../../Source/ZenLib/Format/Http/Http_Utils.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}C:/Program Files (x86)/ZenLib/lib/cmake/zenlib/ZenLibTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}C:/Program Files (x86)/ZenLib/lib/cmake/zenlib/ZenLibTargets.cmake"
         "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/CMakeFiles/Export/C_/Program_Files_(x86)/ZenLib/lib/cmake/zenlib/ZenLibTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}C:/Program Files (x86)/ZenLib/lib/cmake/zenlib/ZenLibTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}C:/Program Files (x86)/ZenLib/lib/cmake/zenlib/ZenLibTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/ZenLib/lib/cmake/zenlib/ZenLibTargets.cmake")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/ZenLib/lib/cmake/zenlib" TYPE FILE FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/CMakeFiles/Export/C_/Program_Files_(x86)/ZenLib/lib/cmake/zenlib/ZenLibTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/ZenLib/lib/cmake/zenlib/ZenLibTargets-noconfig.cmake")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/ZenLib/lib/cmake/zenlib" TYPE FILE FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/CMakeFiles/Export/C_/Program_Files_(x86)/ZenLib/lib/cmake/zenlib/ZenLibTargets-noconfig.cmake")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/ZenLib/lib/pkgconfig/libzen.pc")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/ZenLib/lib/pkgconfig" TYPE FILE FILES "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/libzen.pc")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/ZenLib/lib/cmake/zenlib/ZenLibConfig.cmake;C:/Program Files (x86)/ZenLib/lib/cmake/zenlib/ZenLibConfigVersion.cmake")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/ZenLib/lib/cmake/zenlib" TYPE FILE FILES
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/ZenLibConfig.cmake"
    "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/ZenLibConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Regards/libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
