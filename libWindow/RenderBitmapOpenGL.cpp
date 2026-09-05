#include "header.h"

// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderBitmapOpenGL.h"
//#include <ConfigParam.h>
#include <GLSLShader.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <GLUT/glut.h>
#endif
#include <ImageLoadingFormat.h>
#include <LibResource.h>
using namespace Regards::OpenGL;
using namespace Regards::Picture;

CRenderBitmapOpenGL::CRenderBitmapOpenGL(CRenderOpenGL* renderOpenGL)
{
	textureArrowRight = nullptr;
	textureArrowLeft = nullptr;
	this->renderOpenGL = renderOpenGL;
}

COpenGLShader * CRenderBitmapOpenGL::FindShader(const wxString& shaderName, GLenum glSlShaderType_i, const wxString& vertexName)
{
	return renderOpenGL->FindShader(shaderName, glSlShaderType_i, vertexName);
}

void CRenderBitmapOpenGL::LoadingResource(const double& scale_factor, wxColor& colorActifReplacement)
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
		textureArrowRight = std::make_unique<GLTexture>();

	Regards::Picture::CPictureArray mat = arrowRight.GetMatrix();
	textureArrowRight->SetData(mat, nullptr);

	if (textureArrowLeft == nullptr)
		textureArrowLeft = std::make_unique<GLTexture>();

	mat = arrowLeft.GetMatrix();
	textureArrowLeft->SetData(mat, nullptr);
}



void CRenderBitmapOpenGL::RenderTexture(GLTexture* pictureNext, const int& width, const int& height, const int& left, const int& top)
{
	if (!pictureNext) return;

	pictureNext->Enable();

	COpenGLShader* defaultShader = renderOpenGL->FindShader(L"IDR_GLSL_TEXTURE", GL_FRAGMENT_SHADER);
	if (defaultShader != nullptr)
	{
		defaultShader->EnableShader(renderOpenGL->projectionMatrix);
		defaultShader->m_pShader->SetTexture("sourceTex", pictureNext->GetTextureID(), 0);
	}

	renderOpenGL->RenderQuad(pictureNext, width, height, false, false, left, top, false);

	if (defaultShader != nullptr)
		defaultShader->DisableShader();

	pictureNext->Disable();
}


float* CRenderBitmapOpenGL::GetProjectionMatrix()
{
	return renderOpenGL->projectionMatrix;
}

void CRenderBitmapOpenGL::RenderTextureWithAlpha(GLTexture* textureTransition, const int& alpha, const int& width,
	const int& height, const int& left, const int& top)
{
	if (!textureTransition) return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textureTransition->Enable();

	COpenGLShader* shader = renderOpenGL->FindShader(L"IDR_GLSL_ALPHA_SHADER", GL_FRAGMENT_SHADER);
	if (shader)
	{
		shader->EnableShader(renderOpenGL->projectionMatrix);
		shader->m_pShader->SetTexture("textureScreen", textureTransition->GetTextureID(), 0);
		shader->m_pShader->SetParam("intensity", static_cast<float>(alpha));
	}

	// CORRECTION CONFORME À VOS SIGNATURES :
	// Arguments : (texture, width, height, flipH=false, flipV=false, left, top, inverted=false)
	renderOpenGL->RenderQuad(textureTransition, width, height, false, false, left, top, false);

	textureTransition->Disable();
	if (shader)
		shader->DisableShader();

	glDisable(GL_BLEND);
}

void CRenderBitmapOpenGL::ShowArrowNext()
{
	if (!textureArrowRight) return;

	const int left = renderOpenGL->GetWidth() - textureArrowRight->GetWidth();
	const int top = (renderOpenGL->GetHeight() - textureArrowRight->GetHeight()) / 2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textureArrowRight->Enable();

	// Activer le shader par défaut pour dessiner la texture en Core Profile
	COpenGLShader* defaultShader = renderOpenGL->FindShader(L"IDR_GLSL_TEXTURE", GL_FRAGMENT_SHADER);
	if (defaultShader != nullptr)
	{
		defaultShader->EnableShader(renderOpenGL->projectionMatrix);
		defaultShader->m_pShader->SetTexture("sourceTex", textureArrowRight->GetTextureID(), 0);
	}

	renderOpenGL->RenderQuad(textureArrowRight.get(), left, top);

	if (defaultShader != nullptr) defaultShader->DisableShader();
	textureArrowRight->Disable();
	glDisable(GL_BLEND);
}

void CRenderBitmapOpenGL::ShowArrowPrevious()
{
	if (!textureArrowLeft) return;

	const int left = 0;
	const int top = (renderOpenGL->GetHeight() - textureArrowLeft->GetHeight()) / 2;


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textureArrowLeft->Enable();

	// Activer le shader par défaut pour dessiner la texture en Core Profile
	COpenGLShader* defaultShader = renderOpenGL->FindShader(L"IDR_GLSL_TEXTURE", GL_FRAGMENT_SHADER);
	if (defaultShader != nullptr)
	{
		defaultShader->EnableShader(renderOpenGL->projectionMatrix);
		defaultShader->m_pShader->SetTexture("sourceTex", textureArrowLeft->GetTextureID(), 0);
	}

	renderOpenGL->RenderQuad(textureArrowLeft.get(), left, top);

	if (defaultShader != nullptr) defaultShader->DisableShader();
	textureArrowLeft->Disable();
	glDisable(GL_BLEND);
}