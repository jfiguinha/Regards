AM_CPPFLAGS = -I${top_srcdir}/include
AM_CXXFLAGS = ${libwx_CXXFLAGS}

noinst_PROGRAMS = iatest

iatest_SOURCES = iatest.cpp
iatest_LDADD = ${libwx_LIBS} -L${top_builddir}/src/ia -lwxia
