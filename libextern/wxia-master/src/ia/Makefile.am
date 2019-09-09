AM_CPPFLAGS = -I${top_srcdir}/include ${sb_CFLAGS}
AM_CXXFLAGS = ${libwx_CXXFLAGS}

lib_LTLIBRARIES = libwxia.la

libwxia_la_LDFLAGS = -release @VERSION@ -no-undefined
libwxia_la_LIBADD  = ${libwx_LIBS}

libwxia_la_SOURCES = ia.cpp
if TWAIN
libwxia_la_SOURCES += iatwainmsw.cpp twainbase.cpp twainmsw.cpp
libwxia_la_LIBADD  += -lgdi32
endif
if SANE
libwxia_la_SOURCES += sane.cpp iasane.cpp iasanedlg.cpp
libwxia_la_LIBADD  += ${sb_LIBS}
endif
