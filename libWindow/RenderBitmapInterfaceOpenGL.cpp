#include "header.h"
// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderBitmapInterfaceOpenGL.h"
#include <ConfigParam.h>
#include <GLSLShader.h>
#include <ParamInit.h>
#include <LoadingResource.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <LibResource.h>
#include <libPicture.h>
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
#include <PhotoFiltreEffectParameter.h>
#include <RgbEffectParameter.h>
#include <FilterData.h>
#include <RegardsBitmap.h>
using namespace Regards::Window;

CRenderBitmapInterfaceOpenGL::CRenderBitmapInterfaceOpenGL(wxGLCanvas * canvas)
	: CRenderOpenGL(canvas)
{
	textureArrowRight= nullptr;
	textureArrowLeft= nullptr;   
}



void CRenderBitmapInterfaceOpenGL::LoadingResource(const double & scale_factor)
{
   
	if(textureArrowRight == nullptr && textureArrowLeft == nullptr)
	{
		wxColor colorToReplace = wxColor(0, 0, 0);
		wxColor colorActifReplacement = wxColor(255, 255, 255);
		wxImage arrowPrevious = CLibResource::CreatePictureFromSVG("IDB_ARROWLPNG" , 32 * scale_factor, 32 * scale_factor);
		arrowPrevious.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
			colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());
		wxImage arrowNext = CLibResource::CreatePictureFromSVG("IDB_ARROWRPNG", 32 * scale_factor, 32 * scale_factor);
		arrowNext.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
			colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());

		CLibPicture libPicture;
		CRegardsBitmap * arrowLeft = libPicture.ConvertwxImageToRegardsBitmap(arrowPrevious);
		CRegardsBitmap * arrowRight = libPicture.ConvertwxImageToRegardsBitmap(arrowNext);

		//arrowLeft->SaveToBmp(L"e:\\test.bmp");
		//arrowRight->SaveToBmp(L"e:\\tes2t.bmp");

		if(textureArrowRight == nullptr)
			textureArrowRight = new GLTexture();
		textureArrowRight->Create(arrowRight->GetBitmapHeight(), arrowRight->GetBitmapWidth(), arrowRight->GetPtBitmap());

		if(textureArrowLeft == nullptr)
			textureArrowLeft = new GLTexture();
		textureArrowLeft->Create(arrowLeft->GetBitmapHeight(), arrowLeft->GetBitmapWidth(), arrowLeft->GetPtBitmap());

		delete arrowLeft;
		delete arrowRight;
	}
}

void CRenderBitmapInterfaceOpenGL::ShowTransitionBitmap(GLTexture * textureTransition, const int &width, const int &height, const int &left, const int &top, const float &alphaValue)
{
	textureTransition->Enable();
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
        static_cast<GLfloat>(width) + static_cast<GLfloat>(left), static_cast<GLfloat>(height) + static_cast<GLfloat>(top),
        static_cast<GLfloat>(left), static_cast<GLfloat>(height) + static_cast<GLfloat>(top)};
    
    GLfloat texVertices[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1 };

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

	glDisable (GL_BLEND);

	textureTransition->Disable();

}


CRenderBitmapInterfaceOpenGL::~CRenderBitmapInterfaceOpenGL()
{

	if (textureArrowRight != NULL)
		delete(textureArrowRight);

	if (textureArrowLeft != NULL)
		delete(textureArrowLeft);


}


void CRenderBitmapInterfaceOpenGL::ShowArrowNext()
{

	int left = width - textureArrowLeft->GetWidth();
	int top = (height - textureArrowRight->GetHeight()) / 2;

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textureArrowRight->Enable();
	RenderQuad(textureArrowRight, left, top);
	textureArrowRight->Disable();
	glDisable(GL_BLEND);

}

void CRenderBitmapInterfaceOpenGL::ShowArrowPrevious()
{

	int left = 0;
	int top = (height - textureArrowLeft->GetHeight()) / 2;

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textureArrowLeft->Enable();
	RenderQuad(textureArrowLeft, left, top);
	textureArrowLeft->Disable();

	glDisable(GL_BLEND);


}

void CRenderBitmapInterfaceOpenGL::PhotoFiltre(const int &left, const int &top, CEffectParameter * effectParameter)
{

	CPhotoFiltreEffectParameter * photoFiltreParameter = (CPhotoFiltreEffectParameter *)effectParameter;

	GLSLShader m_pShader;

	textureDisplay->Enable();

	m_pShader.EnableShader();

	m_pShader.CreateProgram(L"IDR_GLSL_SHADER_PHOTOFILTRE", GL_FRAGMENT_PROGRAM_ARB);

	if (!m_pShader.SetTexture("textureScreen", textureDisplay->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("widthScreen", (float)textureDisplay->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader.SetParam("heightScreen", (float)textureDisplay->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("red", photoFiltreParameter->red))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("green", photoFiltreParameter->green))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("blue", photoFiltreParameter->blue))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("intensity", (float)photoFiltreParameter->intensity))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("left", 0))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("top", 0))
	{
		//TRACE( L"SetTexture failed" );
	}

	RenderQuad(textureDisplay, left, top);

	textureDisplay->Disable();
}



void CRenderBitmapInterfaceOpenGL::RGBFiltre(const int &left, const int &top, CEffectParameter * effectParameter)
{
	CRgbEffectParameter * rgbParameter = (CRgbEffectParameter *)effectParameter;

	GLSLShader m_pShader;

	textureDisplay->Enable();

	m_pShader.CreateProgram(L"IDR_GLSL_SHADER_RGB", GL_FRAGMENT_PROGRAM_ARB);

	m_pShader.EnableShader();

	if (!m_pShader.SetTexture("textureScreen", textureDisplay->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("widthScreen", (float)textureDisplay->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader.SetParam("heightScreen", (float)textureDisplay->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("red", (float)rgbParameter->red))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("green", (float)rgbParameter->green))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("blue", (float)rgbParameter->blue))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("left", 0))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader.SetParam("top", 0))
	{
		//TRACE( L"SetTexture failed" );
	}
	RenderQuad(textureDisplay, left, top);

	textureDisplay->Disable();
}




void  CRenderBitmapInterfaceOpenGL::ApplyPreviewEffect(const int &left, const int &top, const int &numEffect, CEffectParameter * effectParameter)
{
	
	switch(numEffect)
	{
	case ID_AJUSTEMENT_PHOTOFILTRE:
		PhotoFiltre(left, top, effectParameter);
		break;
	case IDM_COLOR_BALANCE:
		RGBFiltre(left, top, effectParameter);
		break;
	}
}


