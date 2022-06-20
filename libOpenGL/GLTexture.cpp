#include <header.h>
#include "GLTexture.h"
#include <RegardsBitmap.h>
#ifdef __APPLE__
#include <OpenCL/cl_gl.h>
#include <OpenCL/cl_gl_ext.h>
#endif
using namespace Regards::OpenGL;

GLTexture::GLTexture(void)
{
	m_nTextureID = 0;
	width = 0;
	height = 0;
	format = GL_BGRA_EXT;
    isOpenCLCompatible = false;
}

GLTexture::~GLTexture(void)
{
	Delete();
}


GLTexture* GLTexture::CreateTextureOutput(int width, int height, const bool& isOpenCLOpenGLInterop, GLenum format)
{
	GLTexture* glTextureDest = new GLTexture(width, height, format, isOpenCLOpenGLInterop);
	return glTextureDest;
}

GLTexture::GLTexture(const int& nWidth, const int& nHeight, const bool& isOpenCLOpenGLInterop, GLenum format)
{
	m_nTextureID = 0;
	width = nWidth;
	height = nHeight;
	this->format = format;
	Create(nWidth, nHeight, nullptr, isOpenCLOpenGLInterop);

}

int GLTexture::GetWidth()
{
	return width;
}

int GLTexture::GetHeight()
{
	return height;
}

uint8_t* GLTexture::GetData()
{
	GLuint m_nTextureSize = width * height << 2;
	uint8_t* m_bData = nullptr;
	if (m_nTextureID)
	{
		m_bData = new uint8_t[m_nTextureSize];
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, m_bData);
	}

	return m_bData;
}

void GLTexture::GetData(uint8_t* data)
{
	if (m_nTextureID && data != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, data);
	}
}


void GLTexture::SetData(cv::UMat& bitmap)
{
	cv::Mat bitmapMatrix;
	if (bitmap.channels() == 3)
	{
		cvtColor(bitmap, bitmapMatrix, cv::COLOR_BGR2BGRA);
	}
	else if (bitmap.channels() == 1)
		cvtColor(bitmap, bitmapMatrix, cv::COLOR_GRAY2BGRA);
	else
		bitmap.copyTo(bitmapMatrix);

	SetTextureData(bitmapMatrix);
}

void GLTexture::SetTextureData(const cv::Mat& bitmapMatrix)
{
	if (!m_nTextureID)
	{
		glGenTextures(1, &m_nTextureID);
		//glActiveTexture(GL_TEXTURE0 + m_nTextureID);
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	if (m_nTextureID)
	{
		glEnable(GL_TEXTURE_2D);
		width = bitmapMatrix.size().width;
		height = bitmapMatrix.size().height;
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, bitmapMatrix.data);
	}
}

void GLTexture::SetData(cv::Mat & bitmap)
{
	cv::Mat bitmapMatrix;
	if (bitmap.channels() == 3)
	{
		cvtColor(bitmap, bitmapMatrix, cv::COLOR_BGR2BGRA);
	}
	else if (bitmap.channels() == 1)
		cvtColor(bitmap, bitmapMatrix, cv::COLOR_GRAY2BGRA);
	else
		bitmapMatrix = bitmap;

	SetTextureData(bitmapMatrix);
}


void GLTexture::SetData(CRegardsBitmap * bitmap)
{
	SetTextureData(bitmap->GetMatrix());
}

//bool GLTexture::Create(const int &nWidth, const int &nHeight, void *pbyData, const int & nFormat_i, const int & nInternalFormat_i)
bool GLTexture::Create(const int& nWidth, const int& nHeight, uint8_t* pbyData, const bool & isOpenCLOpenGLInterop)
{
	width = nWidth;
	height = nHeight;
    isOpenCLCompatible = false;
	//GLuint m_nTextureSize = nWidth * nHeight << 2;

	//int nError = glGetError();
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, format, GL_UNSIGNED_BYTE, pbyData);


    if(isOpenCLOpenGLInterop)
    {
       	using namespace cv::ocl;
        Context& ctx = Context::getDefault();
        cl_context context = (cl_context)ctx.ptr();

        cl_int status = 0;
        clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, m_nTextureID, &status);
        if (status == CL_SUCCESS)
        {
            cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

            status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, NULL, NULL);
        }

         if (status == CL_SUCCESS)
            isOpenCLCompatible = true;           
    }


	return (GL_NO_ERROR == glGetError());
}

bool GLTexture::IsOpenCLCompatible()
{
    return isOpenCLCompatible;
}

cl_mem GLTexture::GetOpenCLTexture()
{
	return clImage;
}

void GLTexture::SetFilterType(const GLint FilterType_i, const GLint FilterValue_i)
{
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);
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
	if(isOpenCLCompatible)
    {
        cl_int status = 0;
        cl_command_queue q = (cl_command_queue)cv::ocl::Queue::getDefault().ptr();

        status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, NULL, NULL);
        if (status != CL_SUCCESS)
           cout << "OpenCL: clEnqueueReleaseGLObjects failed" << endl;

        status = clReleaseMemObject(clImage); // TODO RAII
        if (status != CL_SUCCESS)
            cout << "OpenCL: clReleaseMemObject failed"  << endl;
	}

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


}

void GLTexture::Enable()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);
}
