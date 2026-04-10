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
    frameBufferSupport = epoxy_has_gl_extension("GL_EXT_framebuffer_object");
    //printf("CRenderVideoOpenGL frameBufferSupport %b \n", frameBufferSupport);
}


CRenderVideoOpenGL::~CRenderVideoOpenGL()
{
	Cleanup();
}

void CRenderVideoOpenGL::Cleanup()
{
	if (textureVideo != nullptr)
		delete(textureVideo);
	textureVideo = nullptr;

	if (textureSubtitle != nullptr)
		delete(textureSubtitle);
	textureSubtitle = nullptr;

	if (FFrameBuffer != 0)
	{

		// cleanup
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &FFrameBuffer);
	}

	FFrameBuffer = 0;
	widthBuffer = 0;
	heightBuffer = 0;
}

bool CRenderVideoOpenGL::RenderShaderInterpolation(const wxRect& rc, const bool& flipH, const bool& flipV, const int& angle, const bool& inverted, const int &interpolation)
{
	GLTexture* glTexture = renderOpenGL->GetGLTexture();
	GLSLShader* m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_INTERPOLATION");
    bool isOk = false;
	if (m_pShader != nullptr)
	{
        if(m_pShader->IsOk())
        {
            if(m_pShader->EnableShader() && m_pShader->IsOk())
            {
                if (!m_pShader->SetTexture("ImageTexture", textureVideo->GetTextureID()))
                {
                    //printf("SetTexture ImageTexture failed \n ");
                }
                if (!m_pShader->SetParam("widthTex", textureVideo->GetWidth()))
                {
                    //printf("SetParam sharpness failed \n ");
                }
                if (!m_pShader->SetParam("heightTex", textureVideo->GetHeight()))
                {
                    //printf("SetParam sharpness failed \n ");
                }
                if (!m_pShader->SetIntegerParam("widthIn", widthBuffer))
                {
                    //printf("SetParam widthIn failed \n ");
                }
                if (!m_pShader->SetIntegerParam("heightIn", heightBuffer))
                {
                    //printf("SetParam heightIn failed \n ");
                }
                if (!m_pShader->SetIntegerParam("widthOut", rc.width))
                {
                    //printf("SetParam widthOut failed \n ");
                }
                if (!m_pShader->SetIntegerParam("heightOut", rc.height))
                {
                    //printf("SetParam heightOut failed \n ");
                }
                if (!m_pShader->SetIntegerParam("flipH", flipH))
                {
                    //printf("SetParam flipH failed \n ");
                }
                if (!m_pShader->SetIntegerParam("flipV", flipV))
                {
                    //printf("SetParam flipV failed \n ");
                }
                if (!m_pShader->SetIntegerParam("angle", angle))
                {
                    //printf("SetParam angle failed \n ");
                }
                if (!m_pShader->SetIntegerParam("left", rc.x))
                {
                    //printf("SetParam left failed \n ");
                }
                if (!m_pShader->SetIntegerParam("top", rc.y))
                {
                    //printf("SetParam top failed \n ");
                }
                if (!m_pShader->SetIntegerParam("interpolation", interpolation))
                {
                    //printf("SetParam interpolation failed \n ");
                }
                
                isOk = true;
            }
        }
	}

    if(isOk)
    {
        int left_local = 0;// (glTexture->GetWidth() - widthOut) / 2;
        int top_local = 0;//(glTexture->GetHeight() - heightOut) / 2;

        renderOpenGL->RenderQuad(glTexture->GetWidth(), glTexture->GetHeight(), left_local, top_local, !inverted);
        
        m_pShader->DisableShader();
    }
	
    
    return isOk;
    
}

void CRenderVideoOpenGL::RenderShader(GLSLShader* m_pShader, GLTexture* glTexture, CVideoEffectParameter* effectParameter, const wxFloatRect& rect, const float& iTime)
{
	srand(time(nullptr));
	float timer = rand() % 1000 + 1;

	int width_local = glTexture->GetWidth();
	int height_local = glTexture->GetHeight();

	if (!m_pShader->SetTexture("texUnit", glTexture->GetTextureID()))
	{
		//printf("SetTexture texUnit failed \n ");
	}
	if (!m_pShader->SetParam("fWidth", width_local))
	{
		//printf("SetParam sharpness failed \n ");
	}
	if (!m_pShader->SetParam("fHeight", height_local))
	{
		//printf("SetParam sharpness failed \n ");
	}
	if (!m_pShader->SetParam("top", rect.top))
	{
		//printf("SetParam top failed \n ");
	}
	if (!m_pShader->SetParam("left", rect.left))
	{
		//printf("SetParam left failed \n ");
	}
	if (!m_pShader->SetParam("right", rect.right))
	{
		//printf("SetParam right failed \n ");
	}
	if (!m_pShader->SetParam("bottom", rect.bottom))
	{
		//printf("SetParam bottom failed \n ");
	}
	if (!m_pShader->SetIntegerParam("effectenable", effectParameter->effectEnable))
	{
		//printf("SetParam effectenable failed \n ");
	}
	if (!m_pShader->SetIntegerParam("vhsEffect", effectParameter->vhsEnable))
	{
		//printf("SetParam vhsEffect failed \n ");
	}
	if (!m_pShader->SetParam("iTime", iTime))
	{
		//printf("SetParam vhsEffect failed \n ");
	}
	if (!m_pShader->SetIntegerParam("grayscale", effectParameter->grayEnable))
	{
		//printf("SetParam grayscale failed \n ");
	}
	if (!m_pShader->SetIntegerParam("sepia", effectParameter->sepiaEnable))
	{
		//printf("SetParam sepia failed \n ");
	}
	if (!m_pShader->SetIntegerParam("sharpenMasking", effectParameter->SharpenEnable))
	{
		//printf("SetParam sharpenMasking failed \n ");
	}
	if (!m_pShader->SetIntegerParam("tone", effectParameter->bandcEnable))
	{
		//printf("SetParam tone failed \n ");
	}
	if (!m_pShader->SetIntegerParam("colorboost", effectParameter->ColorBoostEnable))
	{
		//printf("SetParam colorboost failed \n ");
	}
	if (!m_pShader->SetIntegerParam("denoise", effectParameter->openglDenoise))
	{
		//printf("SetParam colorboost failed \n ");
	}
	if (!m_pShader->SetIntegerParam("filmgrain", effectParameter->filmgrainenable))
	{
		//printf("SetParam colorboost failed \n ");
	}
	if (!m_pShader->SetParam("sharpness", effectParameter->sharpness))
	{
		//printf("SetParam sharpness failed \n ");
	}
	if (!m_pShader->SetParam("contrast", effectParameter->contrast))
	{
		//printf("SetParam contrast failed \n ");
	}
	if (!m_pShader->SetParam("brightness", effectParameter->brightness))
	{
		//printf("SetParam brightness failed \n ");
	}
	if (!m_pShader->SetParam("red", effectParameter->color_boost[0]))
	{
		//printf("SetParam red failed \n ");
	}
	if (!m_pShader->SetParam("green", effectParameter->color_boost[1]))
	{
		//printf("SetParam green failed \n ");
	}
	if (!m_pShader->SetParam("blue", effectParameter->color_boost[2]))
	{
		//printf("SetParam blue failed \n ");
	}
	if (!m_pShader->SetParam("sigma", effectParameter->uSigma))
	{
		//printf("SetParam uSigma failed \n ");
	}
	if (!m_pShader->SetParam("threshold", effectParameter->uThreshold / 100.0f))
	{
		//printf("SetParam uThreshold failed \n ");
	}
	if (!m_pShader->SetParam("kSigma", effectParameter->uKSigma))
	{
		//printf("SetParam uKSigma failed \n ");
	}
	if (!m_pShader->SetParam("timer", timer))
	{
		//printf("SetParam uKSigma failed \n ");
	}
}

void CRenderVideoOpenGL::Render(CVideoEffectParameter* effectParameter, wxFloatRect& rect, const float& iTime, int& widthOut, const int& heightOut, const bool& flipH, const bool& flipV, const int& angle, wxRect& rc, const bool& inverted)
{
	GLTexture* glTexture = renderOpenGL->GetGLTexture();
	GLSLShader* m_pShader = nullptr;
	
    
    
	glTexture->Enable();
	textureVideo->Enable();
	if (frameBufferSupport && effectParameter->interpolationQuality == 0 && (effectParameter->interpolation > 0 || effectParameter->effectEnable))
	{       
        bool updateViewport = false;
		if (FFrameBuffer == 0)
		{
			widthBuffer = glTexture->GetWidth();
			heightBuffer = glTexture->GetHeight();
		}

		if (FFrameBuffer != 0 && (widthBuffer != glTexture->GetWidth() || heightBuffer != glTexture->GetHeight()))
		{
			// cleanup
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteFramebuffers(1, &FFrameBuffer);
			FFrameBuffer = 0;
		}

		// setup FBO
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

		// render to FBO
        bool renderInterpolationOk = false;
		glBindFramebuffer(GL_FRAMEBUFFER, FFrameBuffer);
        if(updateViewport)
            glViewport(0, 0, glTexture->GetWidth(), glTexture->GetHeight());
		if (effectParameter->interpolation > 0)
		{
			textureVideo->Enable();
			renderInterpolationOk = RenderShaderInterpolation(rc, flipH, flipV, angle, inverted, effectParameter->interpolation);
			textureVideo->Disable();
		}
		if(!renderInterpolationOk)
		{
			textureVideo->Enable();
			RenderWithInterpolation(renderOpenGL->GetWidth(), renderOpenGL->GetHeight(), flipH, flipV, angle, rc, !inverted);
			textureVideo->Disable();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

        bool enableTexture= true;
		if (effectParameter->effectEnable)
		{            
			m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_VIDEO");
			if (m_pShader != nullptr)
			{
                if(m_pShader->IsOk())
                {
                    if(m_pShader->EnableShader())
                    {                    
                        rect.top = (float)((glTexture->GetHeight() - rc.height) / 2) / (float)glTexture->GetHeight();
                        rect.bottom = 1.0f - rect.top;

                        rect.left = (float)((glTexture->GetWidth() - rc.width) / 2) / (float)glTexture->GetWidth();
                        rect.right = 1.0f - rect.left;

                        RenderShader(m_pShader, glTexture, effectParameter, rect, iTime);
                        
                        enableTexture = false;
                    }
                }
			}
		}
		if(enableTexture)
		{
			glTexture->Enable();
		}

		renderOpenGL->RenderQuad(glTexture, 0, 0, inverted);


	}
	else if(effectParameter->interpolationQuality > 0)
	{
		GLSLShader* m_pShader = nullptr;
		m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_VIDEO");

		if (m_pShader != nullptr)
		{
			if(m_pShader->EnableShader())
            {
                rect.top = (float)((textureVideo->GetHeight() - heightOut) / 2) / (float)textureVideo->GetHeight();
                rect.bottom = 1.0f - rect.top;

                rect.left = (float)((textureVideo->GetWidth() - widthOut) / 2) / (float)textureVideo->GetWidth();
                rect.right = 1.0f - rect.left;

                RenderShader(m_pShader, textureVideo, effectParameter, rect, iTime);
            }

		}

		int width_local = textureVideo->GetWidth();
		int height_local = textureVideo->GetHeight();

		int left_local = (renderOpenGL->GetWidth() - width_local) / 2;
		int top_local = (renderOpenGL->GetHeight() - height_local) / 2;

		renderOpenGL->RenderQuad(textureVideo, left_local, top_local, inverted);
	}
	else
	{
		RenderWithInterpolation(renderOpenGL->GetWidth(), renderOpenGL->GetHeight(), flipH, flipV, angle, rc, inverted);
	}

	if (m_pShader != nullptr)
		m_pShader->DisableShader();

	textureVideo->Disable();
	glTexture->Disable();

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

void CRenderVideoOpenGL::SetVideoTexture(Regards::Picture::CPictureArray & pictureArray, const bool &deleteTexture)
{
	if (textureVideo == nullptr)
		textureVideo = new GLTexture();
	if (deleteTexture)
		textureVideo->Delete();
	textureVideo->SetData(pictureArray);
}

GLTexture* CRenderVideoOpenGL::GetVideoTexturePt()
{
	return textureVideo;
}
