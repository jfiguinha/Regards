//
//  glPane.cpp
//  libMinimal
//
//  Created by figuinha jacques on 28/05/2015.
//  Copyright (c) 2015 figuinha jacques. All rights reserved.
//

#include "VideoWindowContext.h"

//#include "OpenGL/glu.h"
//#include "OpenGL/gl.h"



CVideoWindowContext::CVideoWindowContext(wxGLCanvas *canvas) :
wxGLContext(canvas)
{

    SetCurrent(*canvas);
		//Init glew
		int err;

#ifdef GLEW_MX
		err = glewContextInit(glewGetContext());
#  ifdef _WIN32
		err = err || wglewContextInit(wglewGetContext());
#  elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
		err = err || glxewContextInit(glxewGetContext());
#  endif
#else
		err = glewInit();
#endif

	if(GLEW_OK != err)
	{
		printf("Error : %s \n", glewGetString(GLEW_VERSION));
	}

    glShadeModel( GL_SMOOTH );                // Enable smooth shading
    glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );   // Black background
    glClearDepth( 1.0f );                     // Depth buffer setup
    glEnable( GL_DEPTH_TEST );                // Enable depth testing
    glDepthFunc( GL_LEQUAL );                 // Type of depth test to do
    // Really nice perspective calculations
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

}

