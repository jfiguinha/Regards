#include "GLTextureRgbWithAlpha.h"

using namespace Regards::OpenGL;


GLTextureRgbWithAlpha::GLTextureRgbWithAlpha(void)
{
	imageTextureRgb = 0;
	imageTextureAlpha = 0;
	width = 0;
	height = 0;
}

GLTextureRgbWithAlpha::~GLTextureRgbWithAlpha(void)
{
	Delete();
}

bool GLTextureRgbWithAlpha::Create(const wxImage &image)
{
	width = image.GetWidth();
	height = image.GetHeight();
    int nError = glGetError();
	if (0 != imageTextureRgb && imageTextureAlpha != 0)
	{
		// if this texture already exists then delete it.
		Delete();
	}
	if (0 == image.GetData())
	{
		return false;
	}
	else
	{
		// Dealing with the Y portion of the YCbCr 
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &imageTextureRgb);
		glBindTexture(GL_TEXTURE_2D, imageTextureRgb);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// This is necessary for non-power-of-two textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.GetWidth(), image.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.GetData());

		nError = glGetError();
	}

	if (0 != image.GetAlpha())
	{
		// Dealing with the CbCr potion of the YCbCr
		glActiveTexture(GL_TEXTURE1);
		glGenTextures(1, &imageTextureAlpha);
		//Bind CbCr texture
		glBindTexture(GL_TEXTURE_2D, imageTextureAlpha);
		// For fitting
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// This is necessary for non-power-of-two textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, image.GetWidth(), image.GetHeight(), 0, GL_ALPHA, GL_UNSIGNED_BYTE, image.GetAlpha());

		nError = glGetError();
	}

    return ( GL_NO_ERROR == glGetError() );
}


void GLTextureRgbWithAlpha::Delete()
{
	int nError = glGetError();
	if (0 != imageTextureRgb)
	{
		glDeleteTextures(1, &imageTextureRgb);
		nError = glGetError();
		if (GL_NO_ERROR == nError)
			imageTextureRgb = 0;
	}

	if (0 != imageTextureAlpha)
	{
		glDeleteTextures(1, &imageTextureAlpha);
		nError = glGetError();
		if (GL_NO_ERROR == nError)
			imageTextureAlpha = 0;
	}
}


int GLTextureRgbWithAlpha::GetWidth()
{
	return width;
}

int GLTextureRgbWithAlpha::GetHeight()
{
	return height;
}

wxImage GLTextureRgbWithAlpha::GetData()
{
	if (width != 0 && height != 0)
	{
		uint8_t * rgbdata = new uint8_t[width * height * 4];
		uint8_t * alphadata = new uint8_t[width * height];
		if (imageTextureRgb)
		{
			glBindTexture(GL_TEXTURE_2D, imageTextureRgb);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbdata);
		}

		if (imageTextureAlpha)
		{
			glBindTexture(GL_TEXTURE_2D, imageTextureAlpha);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_ALPHA, GL_UNSIGNED_BYTE, alphadata);
		}

		return wxImage(width, height, rgbdata, alphadata);
	}
	
	return wxImage(wxSize(width, height));
}

void GLTextureRgbWithAlpha::Enable()
{
    glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, imageTextureRgb);
	glBindTexture(GL_TEXTURE_2D, imageTextureAlpha);
}