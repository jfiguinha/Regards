/* cairo features define for msvc */
#ifndef CAIRO_FEATURES_H
#define CAIRO_FEATURES_H

#ifdef  __cplusplus
# define CAIRO_BEGIN_DECLS  extern "C" {
# define CAIRO_END_DECLS    }
#else
# define CAIRO_BEGIN_DECLS
# define CAIRO_END_DECLS
#endif

#ifndef cairo_public
	#ifdef LIBCAIRO_EXPORTS
		#define cairo_public __declspec(dllexport)
	#else
		#define cairo_public __declspec(dllimport)
	#endif
#endif

#define CAIRO_VERSION_MAJOR 1
#define CAIRO_VERSION_MINOR 4
#define CAIRO_VERSION_MICRO 10

#define CAIRO_VERSION_STRING "1.4.10"

#define HAVE_WINDOWS_H 1

#define CAIRO_HAS_SVG_SURFACE 1
#define CAIRO_HAS_PDF_SURFACE 1
#define CAIRO_HAS_PS_SURFACE 1
#define CAIRO_HAS_WIN32_SURFACE 1
#define CAIRO_HAS_WIN32_FONT 1
#define CAIRO_HAS_PNG_FUNCTIONS 1

#define PACKAGE_NAME "cairo"
#define PACKAGE_TARNAME "cairo"
#define PACKAGE_VERSION "1.4.10"
#define PACKAGE_STRING "cairo 1.4.10"
#define PACKAGE_BUGREPORT "http://bugs.freedesktop.org/enter_bug.cgi?product=cairo"

#endif
