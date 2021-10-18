#pragma once
#include <GL/glew.h>
#if defined(GLEW_EGL)
#include <GL/eglew.h>
#elif defined(GLEW_OSMESA)
#define GLAPI extern
#include <GL/osmesa.h>
#elif defined(_WIN32)
#include <GL/wglew.h>
#elif !defined(__APPLE__) && !defined(__HAIKU__) || defined(GLEW_APPLE_GLX)
#include <GL/glxew.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <wx/glcanvas.h>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#include <CL/cl_gl.h>
#endif
