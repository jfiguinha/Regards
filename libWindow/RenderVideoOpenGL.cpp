#include <header.h>
// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderVideoOpenGL.h"
#include <GLSLShader.h>

#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#include <GLUT/glut.h>
#endif
#include <EffectVideoParameter.h>

using namespace Regards::OpenGL;
//#define RENDER_TO_TEXTURE

CRenderVideoOpenGL::CRenderVideoOpenGL(CRenderOpenGL* renderOpenGL)
{
	textureSubtitle = nullptr;
	textureVideo = nullptr;
	fboId = 0;
	this->renderOpenGL = renderOpenGL;
}


CRenderVideoOpenGL::~CRenderVideoOpenGL()
{
	if (textureVideo != nullptr)
		delete(textureVideo);
	textureVideo = nullptr;

	if (textureSubtitle != nullptr)
		delete(textureSubtitle);
	textureSubtitle = nullptr;
}


void CRenderVideoOpenGL::RenderWithInterpolationAndEffect(CVideoEffectParameter* effectParameter,
	const wxFloatRect& rect, const float& iTime, const bool& inverted)
{
	GLTexture* glTexture = renderOpenGL->GetGLTexture();
	glTexture->Enable();
	textureVideo->Enable();

	int width_local = glTexture->GetWidth();
	int height_local = glTexture->GetHeight();

	int left_local = (renderOpenGL->GetWidth() - width_local) / 2;
	int top_local = (renderOpenGL->GetHeight() - height_local) / 2;

	if (effectParameter->effectEnable)
	{
		GLSLShader* m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_VIDEO_INTERPOLATION");
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
				printf("SetParam fWidth failed \n ");
			}
			if (!m_pShader->SetParam("fHeight", height_local))
			{
				printf("SetParam fHeight failed \n ");
			}
			if (!m_pShader->SetTexture("texVideo", textureVideo->GetTextureID()))
			{
				printf("SetTexture texVideo failed \n ");
			}
			if (!m_pShader->SetParam("videoWidth", textureVideo->GetWidth()))
			{
				printf("SetParam videoWidth failed \n ");
			}
			if (!m_pShader->SetParam("videoHeight", textureVideo->GetHeight()))
			{
				printf("SetParam videoHeight failed \n ");
			}
			if (!m_pShader->SetParam("interpolation", effectParameter->interpolation))
			{
				printf("SetParam interpolation failed \n ");
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
				printf("SetParam effectenable failed \n ");
			}
			if (!m_pShader->SetIntegerParam("vhsEffect", effectParameter->vhsEnable))
			{
				printf("SetParam vhsEffect failed \n ");
			}
			if (!m_pShader->SetParam("iTime", iTime))
			{
				printf("SetParam vhsEffect failed \n ");
			}
			if (!m_pShader->SetIntegerParam("grayscale", effectParameter->grayEnable))
			{
				printf("SetParam grayscale failed \n ");
			}
			if (!m_pShader->SetIntegerParam("sepia", effectParameter->sepiaEnable))
			{
				printf("SetParam sepia failed \n ");
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

	textureVideo->Disable();
	glTexture->Disable();
}

void CRenderVideoOpenGL::RenderOpenGLEffect(GLSLShader* m_pShader, GLTexture* glTexture, CVideoEffectParameter* effectParameter, const wxFloatRect& rect, const float& iTime)
{
	srand(time(nullptr));
	float timer = rand() % 1000 + 1;

	int width_local = glTexture->GetWidth();
	int height_local = glTexture->GetHeight();

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
		printf("SetParam effectenable failed \n ");
	}
	if (!m_pShader->SetIntegerParam("vhsEffect", effectParameter->vhsEnable))
	{
		printf("SetParam vhsEffect failed \n ");
	}
	if (!m_pShader->SetParam("iTime", iTime))
	{
		printf("SetParam vhsEffect failed \n ");
	}
	if (!m_pShader->SetIntegerParam("grayscale", effectParameter->grayEnable))
	{
		printf("SetParam grayscale failed \n ");
	}
	if (!m_pShader->SetIntegerParam("sepia", effectParameter->sepiaEnable))
	{
		printf("SetParam sepia failed \n ");
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


void CRenderVideoOpenGL::RenderWithEffect(CVideoEffectParameter* effectParameter,
                                          const wxFloatRect& rect, const float& iTime, const bool& inverted)
{
	GLTexture* glTexture = renderOpenGL->GetGLTexture();
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
			m_pShader->EnableShader();
			RenderOpenGLEffect(m_pShader, glTexture, effectParameter, rect, iTime);

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


void CRenderVideoOpenGL::RenderWithOpenGLInterpolationAndEffect(CVideoEffectParameter* effectParameter, const wxFloatRect& rect, const float& iTime, int& widthOut, const int& heightOut, const bool& flipH, const bool& flipV, const int& angle, wxRect& rc, const bool& inverted)
{
	textureVideo->Enable();

	if (effectParameter->effectEnable)
	{
		GLSLShader* m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_VIDEO");
		if (m_pShader != nullptr)
		{
			RenderOpenGLEffect(m_pShader, textureVideo, effectParameter, rect, iTime);
		}

		if (effectParameter->interpolationQuality > 0)
		{
			int width_local = textureVideo->GetWidth();
			int height_local = textureVideo->GetHeight();

			int left_local = (renderOpenGL->GetWidth() - width_local) / 2;
			int top_local = (renderOpenGL->GetHeight() - height_local) / 2;

			renderOpenGL->RenderQuad(textureVideo, left_local, top_local, inverted);
		}
		else
			RenderWithInterpolation(widthOut, heightOut, flipH, flipV, angle, rc, inverted);
		if (m_pShader != nullptr)
			m_pShader->DisableShader();
	}
	else if (effectParameter->interpolationQuality > 0)
	{
		int width_local = textureVideo->GetWidth();
		int height_local = textureVideo->GetHeight();

		int left_local = (renderOpenGL->GetWidth() - width_local) / 2;
		int top_local = (renderOpenGL->GetHeight() - height_local) / 2;

		renderOpenGL->RenderQuad(textureVideo, left_local, top_local, inverted);
	}
	else
	{
		RenderWithInterpolation(widthOut, heightOut, flipH, flipV, angle, rc, inverted);
	}

	textureVideo->Disable();
}

void CRenderVideoOpenGL::RenderWithInterpolation(const int& widthOut, const int& heightOut, const bool& flipH, const bool& flipV, const int &angle, wxRect & rc, const bool& inverted)
{
	
	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glEnableClientState(GL_VERTEX_ARRAY);

	bool _flipH = flipH;
	bool _flipV = flipV;

	//float xFactor = (float)rc.width / (float)textureVideo->GetWidth();
	//float yFactor = (float)rc.height / (float)textureVideo->GetHeight();

	int left = 0;
	int top = 0;

	if (renderOpenGL->GetWidth() > rc.width)
	{
		left = (renderOpenGL->GetWidth() - rc.width) / 2;
	}
	else
	{
		int diff = rc.width - renderOpenGL->GetWidth();
		left = (diff - rc.x) - diff;
	}

	if (renderOpenGL->GetHeight() > rc.height)
	{
		top = (renderOpenGL->GetHeight() - rc.height) / 2;
	}
	else
	{
		top = rc.y - (rc.height - renderOpenGL->GetHeight());
	}

	GLfloat vertices[] = {
		static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(rc.width) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(rc.width) + static_cast<GLfloat>(left),
		static_cast<GLfloat>(rc.height) + static_cast<GLfloat>(top),
		static_cast<GLfloat>(left), static_cast<GLfloat>(rc.height) + static_cast<GLfloat>(top)
	};

	if (angle == 180)
	{
		_flipH = !_flipH;
		_flipV = !_flipV;
	}

	GLfloat texVertices[8];

	if (inverted)
	{
		GLfloat vertices[] = {
			0, 1,
			1, 1,
			1, 0,
			0, 0
		};

		if (_flipV)
		{
			vertices[1] = 0;
			vertices[3] = 0;
			vertices[5] = 1;
			vertices[7] = 1;
		}

		if (_flipH)
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

		if (_flipV)
		{
			vertices[1] = 1;
			vertices[3] = 1;
			vertices[5] = 0;
			vertices[7] = 0;
		}

		if (_flipH)
		{
			vertices[0] = 1;
			vertices[2] = 0;
			vertices[4] = 0;
			vertices[6] = 1;
		}

		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}

	// If a rotation is requested (90/180/270), rotate the texture coordinates
// so the displayed texture is rotated accordingly. The texVertices array
// contains 4 pairs: (u0,v0),(u1,v1),(u2,v2),(u3,v3) matching the quad
// vertices order. We remap these pairs by index rotations.
	if (angle == 90 || angle == 270)
	{
		GLfloat rotated[8];
		int offset = 0;
		if (angle == 90)
		{
			// 90 deg clockwise -> new[i] = old[(i+3)%4]
			//offset = 3;
			offset = 3;
		}
		else if (angle == 270)
		{
			// 270 deg (or 90 ccw) -> new[i] = old[(i+1)%4]
			//offset = 1;
			offset = 1;
		}

		for (int i = 0; i < 4; ++i)
		{
			int src = (i + offset) % 4;
			rotated[i * 2] = texVertices[src * 2];
			rotated[i * 2 + 1] = texVertices[src * 2 + 1];
		}

		memcpy(texVertices, rotated, sizeof(rotated));
	}

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texVertices);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();

	glFlush();

	
}


void CRenderVideoOpenGL::SetSubtitle(cv::Mat& subtitle)
{
	//if (textureSubtitle != nullptr)
	//
	//textureSubtitle = nullptr;

	if(textureSubtitle == nullptr)
		textureSubtitle = new GLTexture();
        
    Regards::Picture::CPictureArray mat = Regards::Picture::CPictureArray(subtitle);
	textureSubtitle->SetData(mat);
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

GLTexture* CRenderVideoOpenGL::GetVideoTexture(const int& width, const int& height)
{
	if (textureVideo == nullptr)
		textureVideo = new GLTexture();

	return textureVideo;
}

void CRenderVideoOpenGL::SetVideoTexture(Regards::Picture::CPictureArray & pictureArray)
{
	if (textureVideo == nullptr)
		textureVideo = new GLTexture();
	textureVideo->SetData(pictureArray);
}

GLTexture* CRenderVideoOpenGL::GetVideoTexturePt()
{
	return textureVideo;
}
