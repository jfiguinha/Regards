#include "GLTextureYUV.h"

using namespace Regards::OpenGL;


GLTextureYUV::GLTextureYUV(void)
{
	imageTextureY = 0;
	imageTextureUV = 0;
}

GLTextureYUV::~GLTextureYUV(void)
{
	Delete();
}

bool GLTextureYUV::Create(const int &nWidth, const int &nHeight, const int &nWidthRect, const int &nHeightRect, uint8_t * pbyData)
{
	GLuint m_uPlanePos = nWidth * nHeight;
	uint8_t * cbcr = &pbyData[m_uPlanePos];

	width = nWidthRect;
	height = nHeightRect;

    int nError = glGetError();
	if (0 != imageTextureY && imageTextureUV != 0)
	{
		// if this texture already exists then delete it.
		Delete();
	}
	if( 0 == pbyData )
	{
		return false;
	}

	// Dealing with the Y portion of the YCbCr 
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &imageTextureY);
	glBindTexture(GL_TEXTURE_2D, imageTextureY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// This is necessary for non-power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, nWidthRect, nHeightRect, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pbyData);


	// Dealing with the CbCr potion of the YCbCr
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &imageTextureUV);
	//Bind CbCr texture
	glBindTexture(GL_TEXTURE_2D, imageTextureUV);
	// For fitting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// This is necessary for non-power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, nWidthRect / 2, nHeightRect / 2, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, cbcr);


    return ( GL_NO_ERROR == glGetError() );
}


void GLTextureYUV::SetFilterType( const GLint FilterType_i, const GLint FilterValue_i )
{
    //glBindTexture( GL_TEXTURE_2D, m_nTextureID );
    //glTexParameteri( GL_TEXTURE_2D, FilterType_i, FilterValue_i );
}

void GLTextureYUV::Delete()
{
	int nError = glGetError();
	if (0 != imageTextureY)
	{
		glDeleteTextures(1, &imageTextureY);
		nError = glGetError();
		if (GL_NO_ERROR == nError)
			imageTextureY = 0;
	}

	if (0 != imageTextureUV)
	{
		glDeleteTextures(1, &imageTextureUV);
		nError = glGetError();
		if (GL_NO_ERROR == nError)
			imageTextureUV = 0;
	}
}


int GLTextureYUV::GetWidth()
{
	return width;
}

int GLTextureYUV::GetHeight()
{
	return height;
}


void GLTextureYUV::Enable()
{
    glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, imageTextureY);
	glBindTexture(GL_TEXTURE_2D, imageTextureUV);
}