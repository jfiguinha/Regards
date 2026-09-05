#include <header.h>
#include "RenderVideoOpenGL.h"
#include <GLSLShader.h>

#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#include <GLUT/glut.h>
#endif
#include <EffectVideoParameter.h>

using namespace Regards::OpenGL;

CRenderVideoOpenGL::CRenderVideoOpenGL(CRenderOpenGL* renderOpenGL)
{
	textureSubtitle = nullptr;
	textureVideo = nullptr;
	fboId = 0;
	this->renderOpenGL = renderOpenGL;
	frameBufferSupport = epoxy_has_gl_extension("GL_EXT_framebuffer_object");

	// Initialisation UNIQUE du générateur de nombres aléatoires pour l'application
	srand(static_cast<unsigned>(time(nullptr)));
}

CRenderVideoOpenGL::~CRenderVideoOpenGL()
{
	Cleanup();
}

void CRenderVideoOpenGL::Cleanup()
{
	if (FFrameBuffer != 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &FFrameBuffer);
	}

	FFrameBuffer = 0;
	widthBuffer = 0;
	heightBuffer = 0;
}

void CRenderVideoOpenGL::RenderShader(GLSLShader* shader, GLTexture* glTexture, CVideoEffectParameter* ep, const wxFloatRect& rect, const float& iTime)
{
	// PLUS de srand() ici ! Gain CPU majeur.
	float timer = static_cast<float>(rand() % 1000 + 1);

	shader->SetTexture("texUnit", glTexture->GetTextureID(), 0);
	shader->SetParam("fWidth", static_cast<float>(glTexture->GetWidth()));
	shader->SetParam("fHeight", static_cast<float>(glTexture->GetHeight()));
	shader->SetParam("top", rect.top);
	shader->SetParam("left", rect.left);
	shader->SetParam("right", rect.right);
	shader->SetParam("bottom", rect.bottom);
	shader->SetIntegerParam("effectenable", ep->effectEnable);
	shader->SetIntegerParam("vhsEffect", ep->vhsEnable);
	shader->SetParam("iTime", iTime);
	shader->SetIntegerParam("grayscale", ep->grayEnable);
	shader->SetIntegerParam("sepia", ep->sepiaEnable);
	shader->SetIntegerParam("sharpenMasking", ep->SharpenEnable);
	shader->SetIntegerParam("tone", ep->bandcEnable);
	shader->SetIntegerParam("colorboost", ep->ColorBoostEnable);
	shader->SetIntegerParam("denoise", ep->openglDenoise);
	shader->SetIntegerParam("filmgrain", ep->filmgrainenable);
	shader->SetParam("sharpness", ep->sharpness);
	shader->SetParam("contrast", ep->contrast);
	shader->SetParam("brightness", ep->brightness);
	shader->SetParam("red", ep->color_boost[0]);
	shader->SetParam("green", ep->color_boost[1]);
	shader->SetParam("blue", ep->color_boost[2]);
	shader->SetParam("sigma", ep->uSigma);
	shader->SetParam("threshold", ep->uThreshold / 100.f);
	shader->SetParam("kSigma", ep->uKSigma);
	shader->SetParam("timer", timer);
}

bool CRenderVideoOpenGL::RenderShaderInterpolation(const wxRect& rc, const bool& flipH, const bool& flipV, const int& angle, const bool& inverted, const int& interpolation)
{
	GLTexture* glTexture = renderOpenGL->GetGLTexture();
	COpenGLShader* shader = renderOpenGL->FindShader(L"IDR_GLSL_INTERPOLATION");
	if (!shader || !shader->m_pShader->IsOk()) return false;
	if (!shader->EnableShader(renderOpenGL->projectionMatrix) || !shader->m_pShader->IsOk()) return false;

	shader->m_pShader->SetTexture("ImageTexture", textureVideo->GetTextureID(), 0);
	shader->m_pShader->SetParam("widthTex", static_cast<float>(textureVideo->GetWidth()));
	shader->m_pShader->SetParam("heightTex", static_cast<float>(textureVideo->GetHeight()));
	shader->m_pShader->SetIntegerParam("widthIn", widthBuffer);
	shader->m_pShader->SetIntegerParam("heightIn", heightBuffer);
	shader->m_pShader->SetIntegerParam("widthOut", rc.width);
	shader->m_pShader->SetIntegerParam("heightOut", rc.height);
	shader->m_pShader->SetIntegerParam("flipH", flipH ? 1 : 0);
	shader->m_pShader->SetIntegerParam("flipV", flipV ? 1 : 0);
	shader->m_pShader->SetIntegerParam("angle", angle);
	shader->m_pShader->SetIntegerParam("left", rc.x);
	shader->m_pShader->SetIntegerParam("top", rc.y);
	shader->m_pShader->SetIntegerParam("interpolation", interpolation);

	renderOpenGL->RenderQuad(glTexture->GetWidth(), glTexture->GetHeight(), 0, 0, !inverted);
	shader->DisableShader();
	return true;
}

void CRenderVideoOpenGL::Render(CVideoEffectParameter* effectParameter, wxFloatRect& rect, const float& iTime, int& widthOut, const int& heightOut, const bool& flipH, const bool& flipV, const int& angle, wxRect& rc, const bool& inverted)
{
	GLTexture* glTexture = renderOpenGL->GetGLTexture();
	COpenGLShader* shader = nullptr;

	glTexture->Enable();
	textureVideo->Enable();

	if (effectParameter->interpolationQuality > 0 && effectParameter->effectEnable)
	{
		shader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_VIDEO");
		if (shader != nullptr && shader->m_pShader->IsOk())
		{
			if (shader->EnableShader(renderOpenGL->projectionMatrix))
			{
				rect.top = (float)((textureVideo->GetHeight() - heightOut) / 2) / (float)textureVideo->GetHeight();
				rect.bottom = 1.0f - rect.top;
				rect.left = (float)((textureVideo->GetWidth() - widthOut) / 2) / (float)textureVideo->GetWidth();
				rect.right = 1.0f - rect.left;

				RenderShader(shader->m_pShader.get(), textureVideo.get(), effectParameter, rect, iTime);
			}
		}

		int width_local = textureVideo->GetWidth();
		int height_local = textureVideo->GetHeight();
		int left_local = (renderOpenGL->GetWidth() - width_local) / 2;
		int top_local = (renderOpenGL->GetHeight() - height_local) / 2;

		renderOpenGL->RenderQuad(textureVideo.get(), left_local, top_local, inverted);
	}
	else
	{       
		bool updateViewport = false;

		// 1. Détermination des dimensions cibles pour la détection de changement d'angle
		if (FFrameBuffer == 0)
		{
			widthBuffer = glTexture->GetWidth();
			heightBuffer = glTexture->GetHeight();
		}

		if (angle == 90 || angle == 270)
		{
			widthBuffer = glTexture->GetHeight();
			heightBuffer = glTexture->GetWidth();
		}
		else
		{
			widthBuffer = glTexture->GetWidth();
			heightBuffer = glTexture->GetHeight();
		}

		// 2. Recréation du FBO si l'angle ou les dimensions de la texture changent
		if (FFrameBuffer != 0 && (widthBuffer != glTexture->GetWidth() || heightBuffer != glTexture->GetHeight()))
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteFramebuffers(1, &FFrameBuffer);
			FFrameBuffer = 0;
		}

		// 3. Initialisation du FBO
		if (FFrameBuffer == 0)
		{
			glGenFramebuffers(1, &FFrameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, FFrameBuffer);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glTexture->GetTextureID(), 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			widthBuffer = glTexture->GetWidth();
			heightBuffer = glTexture->GetHeight();
			updateViewport = true;
		}

		// 4. Rendu de l'interpolation et de la rotation dans le FBO
		glBindFramebuffer(GL_FRAMEBUFFER, FFrameBuffer);
		if (updateViewport)
		{
			glViewport(0, 0, glTexture->GetWidth(), glTexture->GetHeight());
		}

		textureVideo->Enable();
		RenderShaderInterpolation(rc, flipH, flipV, angle, inverted, effectParameter->interpolation);
		textureVideo->Disable();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 5. Application des effets ou rendu final sur l'écran
		if (effectParameter->effectEnable)
		{            
			shader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_VIDEO");
			if (shader != nullptr && shader->m_pShader->IsOk())
			{
				if (shader->EnableShader(renderOpenGL->projectionMatrix))
				{                    
					rect.top = (float)((glTexture->GetHeight() - rc.height) / 2) / (float)glTexture->GetHeight();
					rect.bottom = 1.0f - rect.top;
					rect.left = (float)((glTexture->GetWidth() - rc.width) / 2) / (float)glTexture->GetWidth();
					rect.right = 1.0f - rect.left;

					RenderShader(shader->m_pShader.get(), glTexture, effectParameter, rect, iTime);
				}
			}
		}
		else
		{
			shader = renderOpenGL->FindShader(L"IDR_GLSL_TEXTURE", GL_FRAGMENT_SHADER);
			if (shader != nullptr)
			{
				shader->EnableShader(renderOpenGL->projectionMatrix);
				shader->m_pShader->SetTexture("sourceTex", glTexture->GetTextureID(), 0);
			}
		}

		glTexture->Enable();
		renderOpenGL->RenderQuad(glTexture, 0, 0, inverted);
	}

	if (shader != nullptr)
		shader->DisableShader();

	textureVideo->Disable();
	glTexture->Disable();
}


void CRenderVideoOpenGL::SetSubtitle(cv::Mat& subtitle)
{
	if (textureSubtitle == nullptr)
		textureSubtitle = std::make_unique<GLTexture>();

	Regards::Picture::CPictureArray mat = Regards::Picture::CPictureArray(subtitle);
	textureSubtitle->SetData(mat, nullptr);
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
		renderOpenGL->RenderQuad(textureSubtitle.get(), left, top, true);
		textureSubtitle->Disable();

		glDisable(GL_BLEND);
	}
}

GLTexture* CRenderVideoOpenGL::GetVideoTexture(const int& width, const int& height)
{
	if (textureVideo == nullptr)
		textureVideo = std::make_unique<GLTexture>();

	return textureVideo.get();
}

void CRenderVideoOpenGL::SetVideoTexture(Regards::Picture::CPictureArray& pictureArray, const bool& deleteTexture)
{
	if (textureVideo == nullptr)
		textureVideo = std::make_unique<GLTexture>();
	if (deleteTexture)
		textureVideo->Delete();
	textureVideo->SetData(pictureArray, renderOpenGL->GetOpenCLContext(), deleteTexture);
}

GLTexture* CRenderVideoOpenGL::GetVideoTexturePt()
{
	return textureVideo.get();
}