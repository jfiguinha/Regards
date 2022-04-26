#pragma once
#include <epoxy/gl.h>
#include <epoxy/glx.h>


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
