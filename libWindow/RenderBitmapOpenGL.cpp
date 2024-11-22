#include "header.h"

// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderBitmapOpenGL.h"
//#include <ConfigParam.h>
#include <GLSLShader.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <LibResource.h>
#include <libPicture.h>
#include <wx/sstream.h>
#include <ImageLoadingFormat.h>
using namespace Regards::OpenGL;
using namespace Regards::Picture;

CRenderBitmapOpenGL::CRenderBitmapOpenGL(CRenderOpenGL* renderOpenGL)
{
	textureArrowRight = nullptr;
	textureArrowLeft = nullptr;
	this->renderOpenGL = renderOpenGL;
}

GLSLShader* CRenderBitmapOpenGL::FindShader(const wxString& shaderName, GLenum glSlShaderType_i)
{
	return renderOpenGL->FindShader(shaderName, glSlShaderType_i);
}

void CRenderBitmapOpenGL::LoadingResource(const double& scale_factor, wxColor & colorActifReplacement)
{
	wxColor colorToReplace = wxColor(0, 0, 0);

	wxImage arrowPrevious = CLibResource::CreatePictureFromSVG("IDB_ARROWLPNG", 32 * scale_factor, 32 * scale_factor);

	wxImage arrowNext = CLibResource::CreatePictureFromSVG("IDB_ARROWRPNG", 32 * scale_factor, 32 * scale_factor);


    arrowPrevious.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                      colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue()); 
    arrowNext.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                      colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());


	CImageLoadingFormat arrowLeft;
	arrowLeft.SetPicture(arrowPrevious);

	CImageLoadingFormat arrowRight;
	arrowRight.SetPicture(arrowNext);

	if (textureArrowRight == nullptr)
		textureArrowRight = new GLTexture();

	Regards::Picture::CPictureArray mat = arrowRight.GetMatrix();
	textureArrowRight->SetData(mat);

	if (textureArrowLeft == nullptr)
		textureArrowLeft = new GLTexture();
	
    mat = arrowRight.GetMatrix();
	textureArrowLeft->SetData(mat);
}

void CRenderBitmapOpenGL::RenderWithAlphaChannel(GLTexture* glTexture, const int& alpha, const int& left,
                                                 const int& top, const bool& flipH, const bool& flipV,
                                                 const bool& inverted)
{
	glTexture->Enable();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	printf("GLSLShader IDR_GLSL_ALPHA_SHADER \n ");
	GLSLShader* m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_ALPHA_SHADER");
	if (m_pShader != nullptr)
	{
		m_pShader->EnableShader();
		if (!m_pShader->SetTexture("textureScreen", glTexture->GetTextureID()))
		{
			printf("SetTexture textureScreen failed \n ");
		}
		if (!m_pShader->SetParam("intensity", alpha))
		{
			printf("SetParam intensity failed \n ");
		}
	}
	renderOpenGL->RenderQuad(glTexture, flipH, flipV, left, top, inverted);
	if (m_pShader != nullptr)
		m_pShader->DisableShader();

	glDisable(GL_BLEND);
	glTexture->Disable();
}

void CRenderBitmapOpenGL::ShowSecondBitmap(GLTexture* textureTransition, const int& width, const int& height,
                                           const int& left, const int& top, const bool& blend)
{
	textureTransition->Enable();
	if (blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glPushMatrix();

#if defined(__APPLE__)
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#else
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
#endif
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat vertices[] = {
		static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(width) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(width) + static_cast<GLfloat>(left),
		static_cast<GLfloat>(height) + static_cast<GLfloat>(top),
		static_cast<GLfloat>(left), static_cast<GLfloat>(height) + static_cast<GLfloat>(top)
	};

	GLfloat texVertices[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};


	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

#if defined(__APPLE__)
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else
	glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
#endif

	glPopMatrix();

	if (blend)
		glDisable(GL_BLEND);

	textureTransition->Disable();
}


void CRenderBitmapOpenGL::ShowSecondBitmapWithAlpha(GLTexture* textureTransition, const int& alpha, const int& width,
                                                    const int& height, const int& left, const int& top)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	textureTransition->Enable();
	printf("GLSLShader IDR_GLSL_ALPHA_SHADER \n ");
	GLSLShader* m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_ALPHA_SHADER");
	if (m_pShader != nullptr)
	{
		m_pShader->EnableShader();
		if (!m_pShader->SetTexture("textureScreen", textureTransition->GetTextureID()))
		{
			printf("SetTexture textureScreen failed \n ");
		}
		if (!m_pShader->SetParam("intensity", alpha))
		{
			printf("SetParam intensity failed \n ");
		}
	}
	ShowSecondBitmap(textureTransition, width, height, left, top);

	textureTransition->Disable();
	if (m_pShader != nullptr)
		m_pShader->DisableShader();
}


CRenderBitmapOpenGL::~CRenderBitmapOpenGL()
{
	if (textureArrowRight != nullptr)
		delete(textureArrowRight);

	if (textureArrowLeft != nullptr)
		delete(textureArrowLeft);
}

void CRenderBitmapOpenGL::ShowArrowNext()
{
	int left = renderOpenGL->GetWidth() - textureArrowLeft->GetWidth();
	int top = (renderOpenGL->GetHeight() - textureArrowRight->GetHeight()) / 2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textureArrowRight->Enable();
	renderOpenGL->RenderQuad(textureArrowRight, left, top);
	textureArrowRight->Disable();
	glDisable(GL_BLEND);
}

void CRenderBitmapOpenGL::ShowArrowPrevious()
{
	int left = 0;
	int top = (renderOpenGL->GetHeight() - textureArrowLeft->GetHeight()) / 2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textureArrowLeft->Enable();
	renderOpenGL->RenderQuad(textureArrowLeft, left, top);
	textureArrowLeft->Disable();

	glDisable(GL_BLEND);
}
