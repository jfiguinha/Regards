#include "header.h"
// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#ifdef RENDEROPENGL
#include "RenderBitmapInterfaceOpenGL.h"
#include <ConfigParam.h>
#include <GLSLShader.h>
#include <RegardsBitmap.h>
#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <utility.h>
using namespace Regards::Video;

//#define RENDER_TO_TEXTURE

CRenderBitmapInterfaceOpenGL::CRenderBitmapInterfaceOpenGL(wxGLCanvas * canvas)
	: CRenderOpenGL(canvas)
{
	textureSubtitle = nullptr;
	textureVideo = nullptr;
	textureVideoCopy = nullptr;
	cl_textureVideoCopy = nullptr;
	fboId = 0;

}



CRenderBitmapInterfaceOpenGL::~CRenderBitmapInterfaceOpenGL()
{
	DeleteVideoTexture();

}

void CRenderBitmapInterfaceOpenGL::RenderWithEffect(const int &left, const int &top, GLTexture * glTexture, CVideoEffectParameter * effectParameter, const bool & flipH,const bool & flipV, const bool & inverted)
{

	glTexture->Enable();
    if(effectParameter->effectEnable)
    {
        GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_VIDEO");
        if(m_pShader != nullptr)
        {
            m_pShader->EnableShader(); 
            if (!m_pShader->SetTexture("texUnit", glTexture->GetTextureID()))
            {
                printf( "SetTexture texUnit failed \n " );
            } 
            if (!m_pShader->SetParam("fWidth", glTexture->GetWidth()))
            {
                printf( "SetParam sharpness failed \n " );
            }
            if (!m_pShader->SetParam("fHeight", glTexture->GetHeight()))
            {
                printf( "SetParam sharpness failed \n " );
            }        
            if (!m_pShader->SetIntegerParam("effectenable", effectParameter->effectEnable))
            {
                printf( "SetParam grayscale failed \n " );
            }  
            if (!m_pShader->SetIntegerParam("grayscale", effectParameter->grayEnable))
            {
                printf( "SetParam grayscale failed \n " );
            }
            if (!m_pShader->SetIntegerParam("sharpenMasking", effectParameter->SharpenEnable))
            {
                printf("SetParam sharpenMasking failed \n " );
            }
            if (!m_pShader->SetIntegerParam("tone", effectParameter->bandcEnable))
            {
                printf( "SetParam tone failed \n " );
            }
            if (!m_pShader->SetIntegerParam("colorboost", effectParameter->ColorBoostEnable))
            {
                printf( "SetParam colorboost failed \n " );
            }
            if (!m_pShader->SetIntegerParam("denoise", effectParameter->denoiseEnable))
            {
                printf( "SetParam colorboost failed \n " );
            } 
            if (!m_pShader->SetParam("sharpness", effectParameter->sharpness))
            {
                printf( "SetParam sharpness failed \n " );
            }
            if (!m_pShader->SetParam("contrast", effectParameter->contrast))
            {
                printf( "SetParam contrast failed \n " );
            }
            if (!m_pShader->SetParam("brightness", effectParameter->brightness))
            {
                printf( "SetParam brightness failed \n " );
            }        
            if (!m_pShader->SetParam("red", effectParameter->color_boost[0]))
            {
                printf( "SetParam red failed \n " );
            }        
            if (!m_pShader->SetParam("green", effectParameter->color_boost[1]))
            {
                printf( "SetParam green failed \n " );
            }        
            if (!m_pShader->SetParam("blue",  effectParameter->color_boost[2]))
            {
                printf( "SetParam blue failed \n " );
            } 
            if (!m_pShader->SetIntegerParam("exponent", effectParameter->exponent))
            {
                printf( "SetParam colorboost failed \n " );
            } 
        }

        RenderQuad(glTexture, flipH, flipV, left, top, inverted);
        if(m_pShader != nullptr)
            m_pShader->DisableShader();
    }
    else
    {
        GLSLShader * m_pShader = FindShader(L"IDR_GLSL_ALPHA_SHADER");
        if(m_pShader != nullptr)
        {
            m_pShader->EnableShader(); 
            if (!m_pShader->SetTexture("textureScreen", glTexture->GetTextureID()))
            {
                printf( "SetTexture textureScreen failed \n " );
            } 
            if (!m_pShader->SetParam("intensity",  100))
            {
                printf( "SetParam intensity failed \n " );
            }                
        }
        RenderQuad(glTexture, flipH, flipV, left, top, inverted);
        if(m_pShader != nullptr)
            m_pShader->DisableShader();
    }

	glTexture->Disable();
}

void CRenderBitmapInterfaceOpenGL::SetSubtitle(CRegardsBitmap * subtitle)
{
	if(textureSubtitle != nullptr)
		delete(textureSubtitle);
	textureSubtitle = nullptr;

	textureSubtitle = new GLTexture();
	textureSubtitle->Create(subtitle->GetBitmapWidth(), subtitle->GetBitmapHeight(), subtitle->GetPtBitmap());
}

void CRenderBitmapInterfaceOpenGL::ShowSubtitle()
{
	if(textureSubtitle != nullptr)
	{
		int left = (width - textureSubtitle->GetWidth()) / 2;
		int top = textureSubtitle->GetHeight();

		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		textureSubtitle->Enable();
		RenderQuad(textureSubtitle, left, top, true);
		textureSubtitle->Disable();

		glDisable(GL_BLEND);
	}
}

void CRenderBitmapInterfaceOpenGL::DeleteSubtitle()
{
	if(textureSubtitle != nullptr)
		delete(textureSubtitle);
	textureSubtitle = nullptr;
}

void CRenderBitmapInterfaceOpenGL::DeleteVideoTexture()
{
	if(textureVideo != nullptr)
		delete(textureVideo);
	textureVideo = nullptr;

	if(textureVideoCopy != nullptr)
		delete(textureVideoCopy);
	textureVideoCopy = nullptr;

	if(cl_textureVideoCopy != nullptr)
	{
		cl_int err;
		err = clReleaseMemObject(cl_textureVideoCopy);
		//Error::CheckError(err);
		cl_textureVideoCopy = nullptr;
	}
}

GLTexture * CRenderBitmapInterfaceOpenGL::GetVideoTexture(const int &width, const int &height)
{
	if (textureVideo != NULL)
		delete(textureVideo);

	textureVideo = new GLTexture(width, height);

	return textureVideo;
}
#ifdef WIN32
cl_mem CRenderBitmapInterfaceOpenGL::GetCopyVideoTexture(cl_context context)
{
	if(textureVideo != nullptr)
	{
		cl_int err;

		if(textureVideoCopy == nullptr || (textureVideoCopy->GetWidth() != textureVideo->GetWidth() || textureVideoCopy->GetHeight() != textureVideo->GetHeight()))
		{
			
			if (textureVideoCopy != NULL)
				delete(textureVideoCopy);
			textureVideoCopy = nullptr;

			if(cl_textureVideoCopy != nullptr)
			{
				err = clReleaseMemObject(cl_textureVideoCopy);
				Error::CheckError(err);
			}

			textureVideoCopy = GLTexture::CreateTextureOutput(textureVideo->GetWidth(), textureVideo->GetHeight());

			glBindTexture( GL_TEXTURE_2D, textureVideoCopy->GetTextureID() );
			//cl_textureVideoCopy = clCreateFromGLTexture2D(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureVideoCopy->GetTextureID(), &err);
           cl_textureVideoCopy = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureVideoCopy->GetTextureID(), &err);
			Error::CheckError(err);
		}
		

        CopyTexture();

		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			printf("error");
		}


	}
	return cl_textureVideoCopy;
}

 void CRenderBitmapInterfaceOpenGL::CopyTexture()
{
    bool isCopyError = true;
    if(__glewCopyImageSubData != NULL && directcopytexture)
    {
        
        isCopyError = false;
        glCopyImageSubData(textureVideo->GetTextureID(), GL_TEXTURE_2D, 0, 0, 0, 0,
                           textureVideoCopy->GetTextureID(), GL_TEXTURE_2D, 0, 0, 0, 0,
                           textureVideo->GetWidth(), textureVideo->GetHeight(), 1);
                           
        // check OpenGL error
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) 
        {
            isCopyError = true;
            cout << "OpenGL error: " << err << endl;
        }
    }


    if(isCopyError)
    {
        directcopytexture = false;
        // create a framebuffer object
        glGenFramebuffers(1, &fboId);
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, textureVideo->GetTextureID(), 0);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                               GL_TEXTURE_2D, textureVideoCopy->GetTextureID(), 0);
        glDrawBuffer(GL_COLOR_ATTACHMENT1);
        glBlitFramebuffer(0, 0, textureVideo->GetWidth(), textureVideo->GetHeight(), 0, 0, textureVideo->GetWidth(), textureVideo->GetHeight(),
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &fboId);
        fboId = 0;
    }
    else
        directcopytexture = true;
}

cl_mem CRenderBitmapInterfaceOpenGL::GetOpenCLVideoTexturePt()
{
	return cl_textureVideoCopy;
}

#endif

GLTexture * CRenderBitmapInterfaceOpenGL::GetVideoTexturePt()
{
	return textureVideo;
}


#endif