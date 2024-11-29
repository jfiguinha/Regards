// ReSharper disable All
#include <header.h>
// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderOpenGL.h"
#include <OpenCLContext.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif`

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#elif defined(__WXGTK__)
#if wxUSE_GLCANVAS_EGL == 1
#include <EGL/egl.h>
#endif
#endif

#include <utility.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>

extern string platformName;
extern cv::ocl::OpenCLExecutionContext clExecCtx;
extern bool isOpenCLInitialized;
using namespace Regards::OpenGL;
using namespace Regards::OpenCL;

CRenderOpenGL::CRenderOpenGL(wxGLCanvas* canvas)
	: wxGLContext(canvas), base(0), myGLVersion(0), mouseUpdate(nullptr)
{
	width = 0;
	height = 0;
	this->openclOpenGLInterop = false;
	textureDisplay = nullptr;
}


bool CRenderOpenGL::IsInit()
{
	return isInit;
}

GLTexture* CRenderOpenGL::GetTextureDisplay()
{
	return textureDisplay;
}

bool CRenderOpenGL::GetOpenGLInterop()
{
	return openclOpenGLInterop;
}



void CRenderOpenGL::Init(wxGLCanvas* canvas)
{
	if (!isInit)
	{
		SetCurrent(*canvas);

		int epoxyversion = epoxy_gl_version();
		bool pboSupported = epoxy_has_gl_extension("GL_ARB_pixel_buffer_object");

        printf("CRenderOpenGL::Init \n");

		CRegardsConfigParam* regardsParam = CParamInit::getInstance();
		if (regardsParam != nullptr)
		{
             printf("CRenderOpenGL::Init 1 OpenCL Support : %d GetIsOpenCLOpenGLInteropSupport : %d \n",regardsParam->GetIsOpenCLSupport(), regardsParam->GetIsOpenCLOpenGLInteropSupport());
            
#ifdef USE_CUDA

			if (regardsParam->GetIsCudaSupport())
			{
				openclOpenGLInterop = false;
				platformName = "NVIDIA";
				COpenCLContext::AssociateToVulkan();
			}
			else if (regardsParam->GetIsOpenCLSupport())
#else
			 if (regardsParam->GetIsOpenCLSupport())
#endif
			{
				if (cv::ocl::haveOpenCL() && !isOpenCLInitialized && regardsParam->GetIsOpenCLOpenGLInteropSupport())
				{
                     printf("CRenderOpenGL::Init 2 \n");
					try
					{
						COpenCLContext::initializeContextFromGL();
						isOpenCLInitialized = true;
						openclOpenGLInterop = true;
						//cv::ocl::Device(clExecCtx.getContext().device(0));
					}
					catch (cv::Exception& e)
					{
                        openclOpenGLInterop = false;
						const char* err_msg = e.what();
						std::cout << "exception caught: " << err_msg << std::endl;
						std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
						COpenCLContext::CreateDefaultOpenCLContext();
					}

				}



				if (!isOpenCLInitialized)
				{
					regardsParam->SetIsOpenCLSupport(false);
				}
				regardsParam->SetIsOpenCLOpenGLInteropSupport(openclOpenGLInterop);
			}
		}
		
		myGLVersion = 0;
		version = glGetString(GL_VERSION);
		sscanf(CConvertUtility::ConvertToUTF8(version), "%f", &myGLVersion);
		isInit = true;


		textureDisplay = new GLTexture();
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

void CRenderOpenGL::Print(int x, int y, double scale_factor, const char* text)
{
    float font_height = 15;
    
    if(scale_factor > 1.0f)
        font_height = font_height * 2;


	glRasterPos2f(x, height - font_height);
	//get the length of the string to display
	int len = static_cast<int>(strlen(text));
    
    //glScalef(scale_factor,scale_factor,scale_factor); 

	//loop to display character by character
	for (auto i = 0; i < len; i++)
	{
        if(scale_factor > 1.0f)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
        else
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
	}
    
    /*

    glPushMatrix(); 
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, height - 10);
	//get the length of the string to display
	int len = static_cast<int>(strlen(text));

    glScalef(scale_factor,scale_factor,scale_factor); 

	//loop to display character by character
	for (auto i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
    
    glPopMatrix(); 
     * */
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


bool CRenderOpenGL::SetData(Regards::Picture::CPictureArray& bitmap)
{
	return textureDisplay->SetData(bitmap);
}


GLTexture* CRenderOpenGL::GetDisplayTexture(const int& width, const int& height)
{
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
        

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, width, 0, height);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
	}

    glClearColor(color.GetFRed() / 255.0f, color.GetFGreen() / 255.0f, color.GetFBlue() / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(color.GetFRed() / 255.0f, color.GetFGreen() / 255.0f, color.GetFBlue() / 255.0f, 1.0f);

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
