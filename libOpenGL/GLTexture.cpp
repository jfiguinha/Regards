#include <header.h>
#include "GLcontext.h"
#include "GLTexture.h"
#include <RegardsBitmap.h>
using namespace Regards::OpenGL;

GLTexture::~GLTexture(void)
{

	Delete();
#ifndef __WXGTK__
    delete texture;
#endif
    
}

GLTexture::GLTexture()
{
#ifndef __WXGTK__
	texture = new cv::ogl::Texture2D();
#else
	m_nTextureID = 0;
	width = 0;
	height = 0;
#endif
}

void GLTexture::Disable()
{
    glDisable(GL_TEXTURE_2D);
}


int GLTexture::GetTextureID()
{
#ifndef __WXGTK__
    return texture->texId();
#else
    return m_nTextureID;
#endif
}


GLTexture::GLTexture(const int& nWidth, const int& nHeight)
{
#ifndef __WXGTK__
	texture = new cv::ogl::Texture2D();
	texture->create(nHeight, nWidth, cv::ogl::Texture2D::Format::RGBA, true);
#else
	m_nTextureID = 0;
	width = nWidth;
	height = nHeight;
	Create(nWidth, nHeight, nullptr);
#endif
}

int GLTexture::GetWidth()
{
#ifndef __WXGTK__
	return texture->cols();
#else
    return width;
#endif
}

int GLTexture::GetHeight()
{
#ifndef __WXGTK__
	return texture->rows();
#else
    return height;
#endif
}

#ifdef __WXGTK__
bool GLTexture::Create(const int& nWidth, const int& nHeight, uint8_t* pbyData)
{
	width = nWidth;
	height = nHeight;

	if (0 != m_nTextureID)
	{
		// if this texture already exists then delete it.
		Delete();
	}

	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &m_nTextureID);
	//glActiveTexture(GL_TEXTURE0 + m_nTextureID);
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pbyData);

	return (GL_NO_ERROR == glGetError());
}
#endif

void GLTexture::SetData(CRegardsBitmap* bitmap)
{
#ifndef __WXGTK__
	if (bitmap != nullptr)
	{
		glEnable(GL_TEXTURE_2D);
		texture->copyFrom(bitmap->GetMatrix(), true);
	}
#else
    if (bitmap != nullptr)
    {
		glEnable(GL_TEXTURE_2D);
		width = bitmap->GetBitmapWidth();
		height = bitmap->GetBitmapHeight();
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, bitmap->GetMatrix().data);
    }
#endif

}

#ifndef __WXGTK__
cv::ogl::Texture2D* GLTexture::GetGLTexture()
{
	return texture;
}
#endif

void GLTexture::checkErrors(std::string desc)
{
	GLenum e = glGetError();
	if (e != GL_NO_ERROR)
	{
		char message[512];
		sprintf(message, "OpenGL error in \"%s\": %p (%d)\n", desc.c_str(), gluErrorString(e), e);
		string data = message;
	}
}

void GLTexture::Delete()
{
#ifndef __WXGTK__
	texture->release();
#else
	checkErrors("GLTexture::Delete()");
	//glDisable(GL_TEXTURE_2D);
	printf("Delete Texture id : %d \n", m_nTextureID);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);

	if (0 != m_nTextureID)
	{
		glDeleteTextures(1, &m_nTextureID);
		m_nTextureID = 0;
	}

	checkErrors("GLTexture::Delete()");
#endif
}

void GLTexture::Enable()
{
#ifndef __WXGTK__
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->texId());
#else
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);
#endif
}
