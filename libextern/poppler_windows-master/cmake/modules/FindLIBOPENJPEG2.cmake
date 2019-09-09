# - Try to find the libopenjpeg2 library
# Once done this will define
#
#  LIBOPENJPEG2_FOUND - system has libopenjpeg
#  LIBOPENJPEG2_INCLUDE_DIRS - the libopenjpeg include directories
#  LIBOPENJPEG2_LIBRARIES - Link these to use libopenjpeg

# Copyright (c) 2008, Albert Astals Cid, <aacid@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (LIBOPENJPEG2_LIBRARIES AND LIBOPENJPEG2_INCLUDE_DIR)
	# in cache already
	set(LIBOPENJPEG2_FOUND TRUE)
else ()
	set(LIBOPENJPEG2_FOUND FALSE)
	set(LIBOPENJPEG2_INCLUDE_DIRS)
	set(LIBOPENJPEG2_LIBRARIES)

	find_path (LIBOPENJPEG2_INCLUDE_DIRS openjpeg.h PATH_SUFFIXES openjpeg-2.0)
	find_library(LIBOPENJPEG2_LIBRARIES openjp2)
	if(LIBOPENJPEG2_INCLUDE_DIRS AND LIBOPENJPEG2_LIBRARIES)
		set(LIBOPENJPEG2_FOUND TRUE)
		set(LIBOPENJPEG2_INCLUDE_DIR ${LIBOPENJPEG2_INCLUDE_DIRS} )
	endif()
	
	if (LIBOPENJPEG2_FOUND)
		add_definitions(-DUSE_OPENJPEG2)
	endif ()
endif ()

message(STATUS "LIBOPENJPEG2_FOUND = ${LIBOPENJPEG2_FOUND}")
message(STATUS "LIBOPENJPEG2_INCLUDE_DIRS = ${LIBOPENJPEG2_INCLUDE_DIRS}")
message(STATUS "LIBOPENJPEG2_LIBRARIES = ${LIBOPENJPEG2_LIBRARIES}")
