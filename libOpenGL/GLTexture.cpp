#include "GLTexture.h"

using namespace Regards::OpenGL;

GLTexture::GLTexture(void)
{
	m_nTextureID = 0;
	width = 0;
	height = 0;

}

GLTexture::~GLTexture(void)
{
	Delete();
}


GLTexture * GLTexture::CreateTextureOutput(int width, int height)
{
	GLTexture * glTextureDest = nullptr;
	glTextureDest = new GLTexture(width, height);
	return glTextureDest;
}

GLTexture::GLTexture(const int &nWidth, const int &nHeight)
{
	m_nTextureID = 0;
	width = nWidth;
	height = nHeight;
	Create(nWidth, nHeight, 0);
}

int GLTexture::GetWidth()
{
	return width;
}

int GLTexture::GetHeight()
{
	return height;
}

uint8_t * GLTexture::GetData()
{
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
	GLuint m_nTextureSize = width * height << 2;
	uint8_t * m_bData = nullptr;
	if (m_nTextureID)
	{
		m_bData = new uint8_t[m_nTextureSize];
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_bData);
	}

	return m_bData;
}

uint8_t * GLTexture::GetRGBData()
{
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    GLuint m_nTextureSize = width * height * 3;
    uint8_t * data = nullptr;
    if (m_nTextureID)
    {
        data = new uint8_t[m_nTextureSize];
        glBindTexture(GL_TEXTURE_2D, m_nTextureID);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    return data;
}

void GLTexture::GetData(uint8_t * data)
{
	if (m_nTextureID && data != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	}
}

void GLTexture::SetData(uint8_t * data, const int &nWidth, const int &nHeight, const GLenum & format)
{
	if (m_nTextureID && data != nullptr)
	{
		glEnable(GL_TEXTURE_2D);
		width = nWidth;
		height = nHeight;
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}
}

//bool GLTexture::Create(const int &nWidth, const int &nHeight, void *pbyData, const int & nFormat_i, const int & nInternalFormat_i)
bool GLTexture::Create(const int &nWidth, const int &nHeight, uint8_t * pbyData, const GLenum & format, int byteParPixel)
{
	width = nWidth;
	height = nHeight;

	//GLuint m_nTextureSize = nWidth * nHeight << 2;

    //int nError = glGetError();
	if( 0 != m_nTextureID )
	{
		// if this texture already exists then delete it.
		Delete();
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_nTextureID);
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, byteParPixel, nWidth, nHeight, 0, format, GL_UNSIGNED_BYTE, pbyData);


    return ( GL_NO_ERROR == glGetError() );
}

void GLTexture::SetFilterType( const GLint FilterType_i, const GLint FilterValue_i )
{
    glBindTexture( GL_TEXTURE_2D, m_nTextureID );
    glTexParameteri( GL_TEXTURE_2D, FilterType_i, FilterValue_i );
}

void GLTexture::checkErrors(const char * desc) {
	GLenum e = glGetError();
	if (e != GL_NO_ERROR) {
		//printf( "OpenGL error in \"%s\": %s (%d)\n", desc, gluErrorString(e), e);
		//string data = message;
	}
}

void GLTexture::Delete()
{
	glDisable(GL_TEXTURE_2D);

	if( 0 != m_nTextureID )
	{
		glDeleteTextures(1, &m_nTextureID);
		m_nTextureID = 0;
	}
}

void GLTexture::Enable()
{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, m_nTextureID );
}