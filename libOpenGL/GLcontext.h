#if defined (__APPLE__)
#include "OpenGL/gl3.h"
#include "OpenGL/glu.h"
#else

#include <GL/glew.h>
#if defined(_WIN32)
#include <GL/wglew.h>
#elif defined(__APPLE__) && !defined(GLEW_APPLE_GLX)
#include <AGL/agl.h>
#else
#include <GL/glxew.h>
#endif

#ifdef GLEW_MX
extern GLEWContext _glewctx;
#  define glewGetContext() (&_glewctx)
#  ifdef _WIN32
extern WGLEWContext _wglewctx;
#    define wglewGetContext() (&_wglewctx)
#  elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
extern GLXEWContext _glxewctx;
#    define glxewGetContext() (&_glxewctx)
#  endif
#endif /* GLEW_MX */

#endif




