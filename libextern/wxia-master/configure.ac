dnl Process this file with autoconf to produce a configure script.
AC_INIT([wxia], [0.0.3])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX

dnl Checks for programs.
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT([disable-static])
PKG_PROG_PKG_CONFIG
sane=0
twain=0

case "$host_os" in
	darwin*)
		twain=1
		;;
	mingw*)
		twain=1
		;;
	*)
		sane=1
		PKG_CHECK_MODULES([sb], [sane-backends])
		;;
esac
AM_CONDITIONAL([SANE], [test "$sane" = 1])
AM_CONDITIONAL([TWAIN], [test "$twain" = 1])

dnl figure out wx library name
AC_ARG_WITH([wx-config],
	[AS_HELP_STRING([--with-wx-config=PATH], [Specify path to alternate wx-config])],
	[wx_config="$withval"], [wx_config="wx-config"])
AC_MSG_CHECKING([for wxWidgets])
libwx_CPPFLAGS=$($wx_config --cppflags $wxconfig_FLAGS 2>/dev/null)
ret=$?
AM_CONDITIONAL([HAVE_WXWIDGETS], [test "$ret" -eq 0])
if [[ "$ret" -eq 0 ]]; then
	AC_MSG_RESULT([yes])
	echo "wxWidgets CPPFLAGS... $libwx_CPPFLAGS"

	libwx_CXXFLAGS=$($wx_config --cppflags $wxconfig_FLAGS)
	echo "wxWidgets CXXFLAGS... $libwx_CXXFLAGS"

	libwx_LIBS=$($wx_config --libs $wxconfig_FLAGS)
	echo "wxWidgets LIBS... $libwx_LIBS"
	libwx_LIBS=$($wx_config --libs $wxconfig_FLAGS | sed -e 's/-Wl,--subsystem,windows//g' -e 's/-mwindows//g')
	echo "wxWidgets adjusted LIBS... $libwx_LIBS"
else
	AC_MSG_RESULT([no])
fi;
AC_SUBST([libwx_CPPFLAGS])
AC_SUBST([libwx_CXXFLAGS])
AC_SUBST([libwx_LIBS])

dnl output
AC_CONFIG_FILES([
	wxia.pc
	Makefile
	src/Makefile
	src/ia/Makefile
	samples/Makefile
	samples/ia/Makefile
	include/Makefile
	include/wx/Makefile
	include/wx/ia/Makefile
	])
AC_OUTPUT
