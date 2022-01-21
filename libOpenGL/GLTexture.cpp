#include <header.h>
#include "GLTexture.h"
#include <RegardsBitmap.h>
using namespace Regards::OpenGL;

GLTexture::~GLTexture(void)
{
	Delete();
}

GLTexture::GLTexture()
{
	//texture = new cv::ogl::Texture2D();
}

GLTexture::GLTexture(const int& nWidth, const int& nHeight, GLenum format)
{
	//Create(nWidth, nHeight, nullptr);
	texture.create(nHeight, nWidth, cv::ogl::Texture2D::Format::RGBA);
}

int GLTexture::GetWidth()
{
	return texture.cols();
}

int GLTexture::GetHeight()
{
	return texture.rows();
}

void GLTexture::SetData(CRegardsBitmap* bitmap)
{
	if (bitmap != nullptr)
	{
		glEnable(GL_TEXTURE_2D);
		texture.copyFrom(bitmap->GetMatrix());
	}
}

cv::ogl::Texture2D* GLTexture::GetGLTexture()
{
	return &texture;
}

void GLTexture::SetFilterType(const GLint FilterType_i, const GLint FilterValue_i)
{
	glBindTexture(GL_TEXTURE_2D, texture.texId());
	glTexParameteri(GL_TEXTURE_2D, FilterType_i, FilterValue_i);
}

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
	texture.release();
}

void GLTexture::Enable()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.texId());
}
