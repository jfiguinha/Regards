#include <header.h>
#include "GLTexture.h"

using namespace Regards::OpenGL;

GLTexture::GLTexture(void)
{
	m_nTextureID = 0;
	width = 0;
	height = 0;
	format = GL_BGRA_EXT;
}

GLTexture::~GLTexture(void)
{
	Delete();
}


GLTexture * GLTexture::CreateTextureOutput(int width, int height, GLenum format)
{
	GLTexture * glTextureDest = NULL;
	glTextureDest = new GLTexture(width, height, format);
	return glTextureDest;
}

GLTexture::GLTexture(const int &nWidth, const int &nHeight, GLenum format)
{
	m_nTextureID = 0;
	width = nWidth;
	height = nHeight;
	this->format = format;
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
	GLuint m_nTextureSize = width * height << 2;
	uint8_t * m_bData = NULL;
	if (m_nTextureID)
	{
		m_bData = new uint8_t[m_nTextureSize];
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, m_bData);
	}

	return m_bData;
}

void GLTexture::GetData(uint8_t * data)
{
	if (m_nTextureID && data != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, data);
	}
}

void GLTexture::SetData(float * data, const int &nWidth, const int &nHeight)
{
	if (m_nTextureID && data != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		width = nWidth;
		height = nHeight;
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, format, GL_FLOAT, data);
	}
}

void GLTexture::SetData(uint8_t * data, const int &nWidth, const int &nHeight)
{
	if (m_nTextureID && data != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		width = nWidth;
		height = nHeight;
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}
}

//bool GLTexture::Create(const int &nWidth, const int &nHeight, void *pbyData, const int & nFormat_i, const int & nInternalFormat_i)
bool GLTexture::Create(const int &nWidth, const int &nHeight, uint8_t * pbyData)
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

	/*
	//generate the texture ID
	glGenTextures(1, &m_nTextureID);
	//binnding the texture
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);
	//regular sampler params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//need to set GL_NEAREST
	//(not GL_NEAREST_MIPMAP_* which would cause CL_INVALID_GL_OBJECT later)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//specify texture dimensions, format etc
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pbyData);
	*/
	
	
	glEnable(GL_TEXTURE_2D);
	
	glGenTextures(1, &m_nTextureID);
	//glActiveTexture(GL_TEXTURE0 + m_nTextureID);
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, format, GL_UNSIGNED_BYTE, pbyData);
	
	/*
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_nTextureID);
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	*/
    return ( GL_NO_ERROR == glGetError() );
}

void GLTexture::SetFilterType( const GLint FilterType_i, const GLint FilterValue_i )
{
    glBindTexture( GL_TEXTURE_2D, m_nTextureID );
    glTexParameteri( GL_TEXTURE_2D, FilterType_i, FilterValue_i );
}

void GLTexture::checkErrors(std::string desc) {
	GLenum e = glGetError();
	if (e != GL_NO_ERROR) {
		char message[512];
		sprintf(message, "OpenGL error in \"%s\": %s (%d)\n", desc.c_str(), gluErrorString(e), e);
		string data = message;
	}
}

void GLTexture::Delete()
{
	//glDisable(GL_TEXTURE_2D);
    printf("Delete Texture id : %d \n",m_nTextureID);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, m_nTextureID );   
    
	if( 0 != m_nTextureID )
	{
		glDeleteTextures(1, &m_nTextureID);
		m_nTextureID = 0;
	}
    
    checkErrors("GLTexture::Delete()");
}

void GLTexture::Enable()
{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, m_nTextureID );
}
