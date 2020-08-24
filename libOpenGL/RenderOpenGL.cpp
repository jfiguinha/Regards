#include <header.h>
#ifdef RENDEROPENGL
// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderOpenGL.h"
#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <utility.h>

using namespace Regards::OpenGL;

CRenderOpenGL::CRenderOpenGL(wxGLCanvas * canvas)
	: wxGLContext(canvas)
{
	textureDisplay = nullptr;
	cl_textureDisplay = nullptr;
	width = 0;
	height = 0;

    

}


bool CRenderOpenGL::IsInit()
{
    return isInit;
}

void CRenderOpenGL::Init(wxGLCanvas * canvas)
{
    if(!isInit)
    {
        SetCurrent(*canvas);
        myGLVersion = 0;
        version = glGetString(GL_VERSION);
        sscanf(CConvertUtility::ConvertToUTF8(version), "%f", &myGLVersion);

        GLuint err;
        err = glewInit();

        if (GLEW_OK != err)
        {
          /* Problem: glewInit failed, something is seriously wrong. */
          printf("Error: %s\n", glewGetErrorString(err));
        }
        printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION)); 

        isInit = true;
    }  
}

GLSLShader * CRenderOpenGL::CreateShader(const wxString &shaderName)
{
    GLSLShader * m_pShader = new GLSLShader();
    m_pShader->CreateProgram(shaderName, GL_FRAGMENT_PROGRAM_ARB);
    return m_pShader;
}

GLSLShader * CRenderOpenGL::FindShader(const wxString &shaderName)
{
    for(COpenGLShader * shader : listShader)
    {
        if(shader->shaderName == shaderName)
            return shader->m_pShader;
    }
    
    COpenGLShader * openGLShader = new COpenGLShader();
    openGLShader->m_pShader = CreateShader(shaderName);
    openGLShader->shaderName = shaderName;
    
    listShader.push_back(openGLShader);
    
    return openGLShader->m_pShader;
}

CRenderOpenGL::~CRenderOpenGL()
{
	if (textureDisplay != NULL)
		delete(textureDisplay);

    for(COpenGLShader * shader : listShader)
        delete shader;
        
    listShader.clear();

	if(cl_textureDisplay != nullptr)
	{
		cl_int err;
		err = clReleaseMemObject(cl_textureDisplay);
		Error::CheckError(err);
	}
}

wxGLContext * CRenderOpenGL::GetGLContext()
{
	return this;
}

void CRenderOpenGL::DeleteTexture()
{
	if (textureDisplay != NULL)
		delete(textureDisplay);
    textureDisplay = nullptr;

	if(cl_textureDisplay != nullptr)
	{
		cl_int err;
		err = clReleaseMemObject(cl_textureDisplay);
		Error::CheckError(err);
	}
    cl_textureDisplay = nullptr;
}

void CRenderOpenGL::Print(int x, int y, const char * text)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x,height - 10);
	//get the length of the string to display
	int len = (int) strlen(text);

	//loop to display character by character
	for (auto i = 0; i < len; i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
	}

};


GLvoid CRenderOpenGL::ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}




GLTexture * CRenderOpenGL::GetDisplayTexture(const int &width, const int &height, cl_context context)
{
	if (textureDisplay == nullptr || (textureDisplay->GetWidth() != width || textureDisplay->GetHeight() != height))
	{
		cl_int err;

		if (textureDisplay != NULL)
			delete(textureDisplay);
		textureDisplay = nullptr;

		if(cl_textureDisplay != nullptr)
		{
			err = clReleaseMemObject(cl_textureDisplay);
			Error::CheckError(err);
		}
		//this->widthBitmap = width;
		//this->heightBitmap = height;
		textureDisplay = GLTexture::CreateTextureOutput(width, height);

		glBindTexture( GL_TEXTURE_2D, textureDisplay->GetTextureID() );
		if (context != nullptr)
		{
			cl_textureDisplay = clCreateFromGLTexture2D(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureDisplay->GetTextureID(), &err);
			Error::CheckError(err);
		}
	}

	return textureDisplay;
}

int CRenderOpenGL::GetWidth()
{
	return width;
}

int CRenderOpenGL::GetHeight()
{
	return height;
}

cl_mem CRenderOpenGL::GetOpenCLTexturePt()
{
    return cl_textureDisplay;
}

void CRenderOpenGL::CreateScreenRender(const int &width, const int &height, const CRgbaquad &color)
{
    
	if (this->width != width || this->height != height)
	{
		this->width = width;
		this->height = height;	
		ReSizeGLScene(width, height);
	}
    
    this->width = width;
    this->height = height;	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
   
	glClearColor(color.GetFRed() / 255.0f, color.GetFGreen() / 255.0f, color.GetFBlue() / 255.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(color.GetFRed() / 255.0f, color.GetFGreen() / 255.0f, color.GetFBlue() / 255.0f, 0.0f);
	//glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

}

void CRenderOpenGL::RenderQuad(GLTexture * texture, int left, int top, bool inverted)
{
    glPushMatrix();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    GLfloat vertices[] = {
        static_cast<GLfloat>(left), static_cast<GLfloat>(top),
        static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
        static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left), static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top),
        static_cast<GLfloat>(left), static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top)};
    
    GLfloat texVertices[8];
    
    if(inverted)
    {
       GLfloat vertices[] = {
            0, 1,
            1, 1,
            1, 0,
            0, 0 };
        memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
    }
    else
    {
        GLfloat vertices[] = {
        0, 0,
        1, 0,
        1, 1,
        0, 1 };
        memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);

    }
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
    
    glDrawArrays(GL_QUADS, 0, 4);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glPopMatrix();
    
    glFlush();

	glEnd();
    
}


void CRenderOpenGL::RenderQuad(GLTexture * texture, const bool & flipH,const bool & flipV, int left, int top, bool inverted)
{
    glPushMatrix();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    GLfloat vertices[] = {
        static_cast<GLfloat>(left), static_cast<GLfloat>(top),
        static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
        static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left), static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top),
        static_cast<GLfloat>(left), static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top)};
    
    GLfloat texVertices[8];
    
    if(inverted)
    {
        GLfloat vertices[] = {
                0, 1,
                1, 1,
                1, 0,
                0, 0 };
        
        if(flipV)
        {
            vertices[1] = 0;
            vertices[3] = 0;
            vertices[5] = 1;
            vertices[7] = 1;
            
        }        
        
        if(flipH)
        {
            vertices[0] = 1;
            vertices[2] = 0;
            vertices[4] = 0;
            vertices[6] = 1;
        }
        
        memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);

    }
    else
    {
        GLfloat vertices[] = {
            0, 0,
            1, 0,
            1, 1,
            0, 1 };
            
        if(flipV)
        {
            vertices[1] = 1;
            vertices[3] = 1;
            vertices[5] = 0;
            vertices[7] = 0;
            
        }        
        
        if(flipH)
        {
            vertices[0] = 1;
            vertices[2] = 0;
            vertices[4] = 0;
            vertices[6] = 1;
        }            
            
        memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);

    }

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
    
    glDrawArrays(GL_QUADS, 0, 4);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glPopMatrix();
    
	glFlush();

	glEnd();
    
}

void CRenderOpenGL::RenderToTexture()
{
	GLTexture * displayTexture = GetDisplayTexture();
	textureDisplay->Enable();
	//textureSource->Enable();
	glBindTexture(GL_TEXTURE_2D, displayTexture->GetTextureID());					// Bind To The Blur Texture
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);
	//textureSource->Disable();
	textureDisplay->Disable();
}


void CRenderOpenGL::RenderToScreen(IMouseUpdate * mousUpdate, CEffectParameter * effectParameter, const int &left, const int &top, const bool &inverted)
{
	bool renderPreview = false;
	textureDisplay->Enable();
	if (mousUpdate != nullptr)
	{
		if (mousUpdate->IsOpenGLCompatible())
		{
			mousUpdate->ApplyOpenGLShader(this, effectParameter, textureDisplay->GetTextureID());
			RenderQuad(textureDisplay, left, top, inverted);
			mousUpdate->DisableOpenGLShader();
			renderPreview = true;
		}

	}

	if(!renderPreview)
	{
		/*
		GLSLShader * m_pShader = FindShader(L"IDR_GLSL_ALPHA_SHADER");
		if (m_pShader != nullptr)
		{
			m_pShader->EnableShader();
			if (!m_pShader->SetTexture("textureScreen", textureDisplay->GetTextureID()))
			{
				printf("SetTexture textureScreen failed \n ");
			}
			if (!m_pShader->SetParam("intensity", 100))
			{
				printf("SetParam intensity failed \n ");
			}
		}
        */

		RenderQuad(textureDisplay, left, top, inverted);
		//if (m_pShader != nullptr)
		//	m_pShader->DisableShader();
	}

	textureDisplay->Disable();
}

GLTexture * CRenderOpenGL::GetDisplayTexture()
{
	if (textureDisplay == nullptr || (textureDisplay->GetWidth() != width || textureDisplay->GetHeight() != height))
	{
		if (textureDisplay != NULL)
			delete(textureDisplay);

		textureDisplay = GLTexture::CreateTextureOutput(width, height);
	}
    return textureDisplay;
}


GLTexture * CRenderOpenGL::GetDisplayTexture(const int &width, const int &height)
{
	if (textureDisplay == nullptr || (textureDisplay->GetWidth() != width || textureDisplay->GetHeight() != height))
	{
		if (textureDisplay != NULL)
			delete(textureDisplay);

		textureDisplay = GLTexture::CreateTextureOutput(width, height);
	}
    return textureDisplay;    
}

#endif