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

// Dessine un quad (x,y,w,h) avec UV [0..1].
// Ordre TRIANGLE_STRIP : TL, TR, BL, BR.
void CRenderBitmapOpenGL::DrawRect(int x, int y, int w, int h)
{
	EnsureVAO();

	const float fx = static_cast<float>(x);
	const float fy = static_cast<float>(y);
	const float fw = static_cast<float>(w);
	const float fh = static_cast<float>(h);

	const float verts[16] = {
		fx,      fy,      0.f, 0.f,   // TL
		fx + fw, fy,      1.f, 0.f,   // TR
		fx,      fy + fh, 0.f, 1.f,   // BL
		fx + fw, fy + fh, 1.f, 1.f,   // BR
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao_);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

GLSLShader* CRenderBitmapOpenGL::FindShader(const wxString& shaderName, GLenum glSlShaderType_i)
{
	return renderOpenGL->FindShader(shaderName, glSlShaderType_i);
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
	textureArrowRight->SetData(mat);

	if (textureArrowLeft == nullptr)
		textureArrowLeft = std::make_unique<GLTexture>();

	mat = arrowLeft.GetMatrix();
	textureArrowLeft->SetData(mat);
}



void CRenderBitmapOpenGL::RenderWithAlphaChannel(GLTexture* glTexture, const int& alpha, const int& left,
	const int& top, const bool& flipH, const bool& flipV,
	const bool& inverted)
{
	if (!glTexture) return;

	glTexture->Enable();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLSLShader* shader = renderOpenGL->FindShader(L"IDR_GLSL_ALPHA_SHADER");
	if (shader)
	{
		shader->EnableShader();
		shader->SetTexture("textureScreen", glTexture->GetTextureID());
		shader->SetParam("intensity", alpha);
	}

	renderOpenGL->RenderQuad(glTexture, flipH, flipV, left, top, inverted);

	if (shader) shader->DisableShader();
	glDisable(GL_BLEND);
	glTexture->Disable();
}

void CRenderBitmapOpenGL::ShowSecondBitmap(GLTexture* textureTransition, const int& width, const int& height,
	const int& left, const int& top, const bool& blend)
{
	if (!textureTransition) return;

	if (blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	textureTransition->Enable();
	DrawRect(left, top, width, height);
	textureTransition->Disable();

	if (blend)
		glDisable(GL_BLEND);
}


void CRenderBitmapOpenGL::ShowSecondBitmapWithAlpha(GLTexture* textureTransition, const int& alpha, const int& width,
	const int& height, const int& left, const int& top)
{
	if (!textureTransition) return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textureTransition->Enable();

	GLSLShader* shader = renderOpenGL->FindShader(L"IDR_GLSL_ALPHA_SHADER");
	if (shader)
	{
		shader->EnableShader();
		shader->SetTexture("textureScreen", textureTransition->GetTextureID());
		shader->SetParam("intensity", alpha);
	}

	DrawRect(left, top, width, height);

	textureTransition->Disable();
	if (shader) shader->DisableShader();

	glDisable(GL_BLEND);
}


CRenderBitmapOpenGL::~CRenderBitmapOpenGL()
{
	if (vao_)
		glDeleteVertexArrays(1, &vao_);
	if (vbo_)
		glDeleteBuffers(1, &vbo_);
}

void CRenderBitmapOpenGL::ShowArrowNext()
{
	if (!textureArrowRight) return;

	const int left = renderOpenGL->GetWidth() - textureArrowRight->GetWidth();
	const int top = (renderOpenGL->GetHeight() - textureArrowRight->GetHeight()) / 2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textureArrowRight->Enable();
	renderOpenGL->RenderQuad(textureArrowRight.get(), left, top);
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
	renderOpenGL->RenderQuad(textureArrowLeft.get(), left, top);
	textureArrowLeft->Disable();

	glDisable(GL_BLEND);
}

// ═══════════════════════════════════════════════════════════════════════════
//  VAO/VBO local (pour ShowSecondBitmap / DrawRect)
// ═══════════════════════════════════════════════════════════════════════════

void CRenderBitmapOpenGL::EnsureVAO()
{
	if (vao_) return;    // déjà créé

	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	// 4 sommets × (vec2 pos + vec2 uv) = 16 floats
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	// location 0 : vec2 aPos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	// location 1 : vec2 aTexCoord
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
		(void*)(2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}