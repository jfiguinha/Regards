﻿// ReSharper disable All
#include <header.h>
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

CRenderOpenGL::CRenderOpenGL(wxGLCanvas* canvas)
	: wxGLContext(canvas), base(0), myGLVersion(0), mouseUpdate(nullptr)
{
	textureDisplay = nullptr;
	width = 0;
	height = 0;
}


bool CRenderOpenGL::IsInit()
{
	return isInit;
}

GLTexture* CRenderOpenGL::GetTextureDisplay()
{
	return textureDisplay;
}

void CRenderOpenGL::Init(wxGLCanvas* canvas)
{
	if (!isInit)
	{
		SetCurrent(*canvas);
		myGLVersion = 0;
		version = glGetString(GL_VERSION);
		sscanf(CConvertUtility::ConvertToUTF8(version), "%f", &myGLVersion);
        isInit = true;
	}
}

GLSLShader* CRenderOpenGL::CreateShader(const wxString& shaderName, GLenum glSlShaderType_i)
{
	auto m_pShader = new GLSLShader();
	m_pShader->CreateProgram(shaderName, glSlShaderType_i);
	return m_pShader;
}

GLSLShader* CRenderOpenGL::FindShader(const wxString& shaderName, GLenum glSlShaderType_i)
{
	for (COpenGLShader* shader : listShader)
	{
		if (shader->shaderName == shaderName)
			return shader->m_pShader;
	}

	auto openGLShader = new COpenGLShader();
	openGLShader->m_pShader = CreateShader(shaderName, glSlShaderType_i);
	openGLShader->shaderName = shaderName;

	listShader.push_back(openGLShader);

	return openGLShader->m_pShader;
}

CRenderOpenGL::~CRenderOpenGL()
{
	if (textureDisplay != nullptr)
		delete(textureDisplay);

	for (COpenGLShader* shader : listShader)
		delete shader;

	listShader.clear();
}

wxGLContext* CRenderOpenGL::GetGLContext()
{
	return this;
}

void CRenderOpenGL::DeleteTexture()
{
	if (textureDisplay != nullptr)
		delete(textureDisplay);
	textureDisplay = nullptr;
}

void CRenderOpenGL::Print(int x, int y, const char* text)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, height - 10);
	//get the length of the string to display
	int len = static_cast<int>(strlen(text));

	//loop to display character by character
	for (auto i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
	}
};


GLvoid CRenderOpenGL::ReSizeGLScene(GLsizei width, GLsizei height) // Resize And Initialize The GL Window
{
	if (height == 0) // Prevent A Divide By Zero By
	{
		height = 1; // Making Height Equal One
	}

	glViewport(0, 0, width, height); // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glLoadIdentity(); // Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, static_cast<GLfloat>(width) / static_cast<GLfloat>(height), 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
}


GLTexture* CRenderOpenGL::GetDisplayTexture(const int& width, const int& height, const bool& openclOpenGLInterop)
{
	if (textureDisplay == nullptr || (textureDisplay->GetWidth() != width || textureDisplay->GetHeight() != height))
	{
		cl_int err;

		if (textureDisplay != nullptr)
			delete(textureDisplay);
		textureDisplay = nullptr;

		textureDisplay = new GLTexture(width, height, openclOpenGLInterop);
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

void CRenderOpenGL::CreateScreenRender(const int& width, const int& height, const CRgbaquad& color)
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

	glClearColor(color.GetFRed() / 255.0f, color.GetFGreen() / 255.0f, color.GetFBlue() / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(color.GetFRed() / 255.0f, color.GetFGreen() / 255.0f, color.GetFBlue() / 255.0f, 1.0f);
	//glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
}

void CRenderOpenGL::RenderQuad(GLTexture* texture, int left, int top, bool inverted)
{
	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat vertices[] = {
		static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left),
		static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top),
		static_cast<GLfloat>(left), static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top)
	};

	GLfloat texVertices[8];

	if (inverted)
	{
		GLfloat vertices[] = {
			0, 1,
			1, 1,
			1, 0,
			0, 0
		};
		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}
	else
	{
		GLfloat vertices[] = {
			0, 0,
			1, 0,
			1, 1,
			0, 1
		};
		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texVertices);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();

	glFlush();

}


void CRenderOpenGL::RenderQuad(GLTexture* texture, const int& width, const int& height, const bool& flipH,
                               const bool& flipV, int left, int top, bool inverted)
{
	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat vertices[] = {
		static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(width) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(width) + static_cast<GLfloat>(left),
		static_cast<GLfloat>(height) + static_cast<GLfloat>(top),
		static_cast<GLfloat>(left), static_cast<GLfloat>(height) + static_cast<GLfloat>(top)
	};

	GLfloat texVertices[8];

	if (inverted)
	{
		GLfloat vertices[] = {
			0, 1,
			1, 1,
			1, 0,
			0, 0
		};

		if (flipV)
		{
			vertices[1] = 0;
			vertices[3] = 0;
			vertices[5] = 1;
			vertices[7] = 1;
		}

		if (flipH)
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
			0, 1
		};

		if (flipV)
		{
			vertices[1] = 1;
			vertices[3] = 1;
			vertices[5] = 0;
			vertices[7] = 0;
		}

		if (flipH)
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

}


void CRenderOpenGL::RenderQuad(GLTexture* texture, const bool& flipH, const bool& flipV, int left, int top,
                               bool inverted)
{
	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat vertices[] = {
		static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left),
		static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top),
		static_cast<GLfloat>(left), static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top)
	};

	GLfloat texVertices[8];

	if (inverted)
	{
		GLfloat vertices[] = {
			0, 1,
			1, 1,
			1, 0,
			0, 0
		};

		if (flipV)
		{
			vertices[1] = 0;
			vertices[3] = 0;
			vertices[5] = 1;
			vertices[7] = 1;
		}

		if (flipH)
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
			0, 1
		};

		if (flipV)
		{
			vertices[1] = 1;
			vertices[3] = 1;
			vertices[5] = 0;
			vertices[7] = 0;
		}

		if (flipH)
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

}

/*
void CRenderOpenGL::RenderToTexture()
{
	GLTexture* displayTexture = GetGLTexture();
	textureDisplay->Enable();
	//textureSource->Enable();
	glBindTexture(GL_TEXTURE_2D, displayTexture->GetTextureID()); // Bind To The Blur Texture
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);
	//textureSource->Disable();
	textureDisplay->Disable();
}
*/
void CRenderOpenGL::RenderToScreen(IMouseUpdate* mousUpdate, CEffectParameter* effectParameter, const int& left,
                                   const int& top, const bool& inverted)
{
	bool renderPreview = false;
	textureDisplay->Enable();

	if (!renderPreview)
	{
		RenderQuad(textureDisplay, left, top, inverted);
	}

	textureDisplay->Disable();
}

GLTexture* CRenderOpenGL::GetGLTexture()
{
	return textureDisplay;
}

