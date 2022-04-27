#include <header.h>
// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderVideoOpenGL.h"
#include <GLSLShader.h>
#include <RegardsBitmap.h>
#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <EffectVideoParameter.h>
#include <utility.h>

using namespace Regards::OpenGL;
//#define RENDER_TO_TEXTURE

CRenderVideoOpenGL::CRenderVideoOpenGL(CRenderOpenGL* renderOpenGL)
{
	textureSubtitle = nullptr;
	textureVideo = nullptr;
	textureVideoCopy = nullptr;
	cl_textureVideoCopy = nullptr;
	fboId = 0;
	this->renderOpenGL = renderOpenGL;
}


CRenderVideoOpenGL::~CRenderVideoOpenGL()
{
	DeleteVideoTexture();
}


void CRenderVideoOpenGL::RenderWithEffect(GLTexture* glTexture, CVideoEffectParameter* effectParameter,
                                          const wxFloatRect& rect, const bool& inverted)
{
	glTexture->Enable();

	int width_local = glTexture->GetWidth();
	int height_local = glTexture->GetHeight();

	int left_local = (renderOpenGL->GetWidth() - width_local) / 2;
	int top_local = (renderOpenGL->GetHeight() - height_local) / 2;

	if (effectParameter->effectEnable)
	{
		GLSLShader* m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_VIDEO");
		if (m_pShader != nullptr)
		{
			srand(time(nullptr));
			float timer = rand() % 1000 + 1;

			m_pShader->EnableShader();
			if (!m_pShader->SetTexture("texUnit", glTexture->GetTextureID()))
			{
				printf("SetTexture texUnit failed \n ");
			}
			if (!m_pShader->SetParam("fWidth", width_local))
			{
				printf("SetParam sharpness failed \n ");
			}
			if (!m_pShader->SetParam("fHeight", height_local))
			{
				printf("SetParam sharpness failed \n ");
			}
			if (!m_pShader->SetParam("top", rect.top))
			{
				printf("SetParam colorboost failed \n ");
			}
			if (!m_pShader->SetParam("left", rect.left))
			{
				printf("SetParam colorboost failed \n ");
			}
			if (!m_pShader->SetParam("right", rect.right))
			{
				printf("SetParam colorboost failed \n ");
			}
			if (!m_pShader->SetParam("bottom", rect.bottom))
			{
				printf("SetParam colorboost failed \n ");
			}
			if (!m_pShader->SetIntegerParam("effectenable", effectParameter->effectEnable))
			{
				printf("SetParam grayscale failed \n ");
			}
			if (!m_pShader->SetIntegerParam("grayscale", effectParameter->grayEnable))
			{
				printf("SetParam grayscale failed \n ");
			}
			if (!m_pShader->SetIntegerParam("sharpenMasking", effectParameter->SharpenEnable))
			{
				printf("SetParam sharpenMasking failed \n ");
			}
			if (!m_pShader->SetIntegerParam("tone", effectParameter->bandcEnable))
			{
				printf("SetParam tone failed \n ");
			}
			if (!m_pShader->SetIntegerParam("colorboost", effectParameter->ColorBoostEnable))
			{
				printf("SetParam colorboost failed \n ");
			}
			if (!m_pShader->SetIntegerParam("denoise", effectParameter->openglDenoise))
			{
				printf("SetParam colorboost failed \n ");
			}
			if (!m_pShader->SetIntegerParam("filmgrain", effectParameter->filmgrainenable))
			{
				printf("SetParam colorboost failed \n ");
			}
			if (!m_pShader->SetParam("sharpness", effectParameter->sharpness))
			{
				printf("SetParam sharpness failed \n ");
			}
			if (!m_pShader->SetParam("contrast", effectParameter->contrast))
			{
				printf("SetParam contrast failed \n ");
			}
			if (!m_pShader->SetParam("brightness", effectParameter->brightness))
			{
				printf("SetParam brightness failed \n ");
			}
			if (!m_pShader->SetParam("red", effectParameter->color_boost[0]))
			{
				printf("SetParam red failed \n ");
			}
			if (!m_pShader->SetParam("green", effectParameter->color_boost[1]))
			{
				printf("SetParam green failed \n ");
			}
			if (!m_pShader->SetParam("blue", effectParameter->color_boost[2]))
			{
				printf("SetParam blue failed \n ");
			}
			if (!m_pShader->SetParam("sigma", effectParameter->uSigma))
			{
				printf("SetParam uSigma failed \n ");
			}
			if (!m_pShader->SetParam("threshold", effectParameter->uThreshold / 100.0f))
			{
				printf("SetParam uThreshold failed \n ");
			}
			if (!m_pShader->SetParam("kSigma", effectParameter->uKSigma))
			{
				printf("SetParam uKSigma failed \n ");
			}
			if (!m_pShader->SetParam("timer", timer))
			{
				printf("SetParam uKSigma failed \n ");
			}
		}

		renderOpenGL->RenderQuad(glTexture, left_local, top_local, inverted);
		if (m_pShader != nullptr)
			m_pShader->DisableShader();
	}
	else
	{
		renderOpenGL->RenderQuad(glTexture, left_local, top_local, inverted);
	}

	glTexture->Disable();
}

void CRenderVideoOpenGL::SetSubtitle(CRegardsBitmap* subtitle)
{
	if (textureSubtitle != nullptr)
		delete(textureSubtitle);
	textureSubtitle = nullptr;

	textureSubtitle = new GLTexture();
	textureSubtitle->SetData(subtitle);
}

void CRenderVideoOpenGL::ShowSubtitle()
{
	if (textureSubtitle != nullptr)
	{
		int left = (renderOpenGL->GetWidth() - textureSubtitle->GetWidth()) / 2;
		int top = textureSubtitle->GetHeight();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		textureSubtitle->Enable();
		renderOpenGL->RenderQuad(textureSubtitle, left, top, true);
		textureSubtitle->Disable();

		glDisable(GL_BLEND);
	}
}

void CRenderVideoOpenGL::DeleteSubtitle()
{
	if (textureSubtitle != nullptr)
		delete(textureSubtitle);
	textureSubtitle = nullptr;
}

void CRenderVideoOpenGL::DeleteVideoTexture()
{
	if (textureVideo != nullptr)
		delete(textureVideo);
	textureVideo = nullptr;

	if (textureVideoCopy != nullptr)
		delete(textureVideoCopy);
	textureVideoCopy = nullptr;

	if (textureSubtitle != nullptr)
		delete(textureSubtitle);
	textureSubtitle = nullptr;

	if (cl_textureVideoCopy != nullptr)
	{
		cl_int err = clReleaseMemObject(cl_textureVideoCopy);
		Error::CheckError(err);
		cl_textureVideoCopy = nullptr;
	}
}

GLTexture* CRenderVideoOpenGL::GetVideoTexture(const int& width, const int& height)
{
	if (textureVideo != nullptr)
		delete(textureVideo);

	textureVideo = new GLTexture(width, height);

	return textureVideo;
}



GLTexture* CRenderVideoOpenGL::GetVideoTexturePt()
{
	return textureVideo;
}


GLTexture* CRenderVideoOpenGL::GetVideoTextureCopyPt()
{
	if (textureVideoCopy != nullptr)
		delete(textureVideoCopy);
	textureVideoCopy = nullptr;

	textureVideoCopy = new GLTexture(textureVideo->GetWidth(), textureVideo->GetHeight());

	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	                       GL_TEXTURE_2D, textureVideo->GetTextureID(), 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
	                       GL_TEXTURE_2D, textureVideoCopy->GetTextureID(), 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(0, 0, textureVideo->GetWidth(), textureVideo->GetHeight(), 0, 0, textureVideo->GetWidth(),
	                  textureVideo->GetHeight(),
	                  GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fboId);
	//CopyTexture();
	return textureVideoCopy;
}
