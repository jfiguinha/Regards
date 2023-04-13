#include <header.h>
#include "GLTexture.h"
#ifdef __APPLE__
#include <OpenCL/cl_gl.h>
#include <OpenCL/cl_gl_ext.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace Regards::OpenGL;

using namespace cv::ocl;

class CTextureGLPriv
{
public:
	CTextureGLPriv()
	{
		context = static_cast<cl_context>(Context::getDefault().ptr());
		q = static_cast<cl_command_queue>(Queue::getDefault().ptr());
	}

	bool convertToGLTexture2D(cv::UMat& inputData, GLTexture* glTexture);
	cl_int CreateTextureInterop(GLTexture* glTexture);
	void DeleteTextureInterop();

	cl_mem clImage = nullptr;
	bool isOpenCLCompatible = true;
	cl_context context;
	cl_command_queue q;
};


cl_int CTextureGLPriv::CreateTextureInterop(GLTexture* glTexture)
{
	cl_int status = 0;
	if (clImage == nullptr)
	{
		clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, glTexture->GetTextureID(),
		                                &status);
		if (status == CL_SUCCESS)
		{
			status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, nullptr, nullptr);
		}

		if (status == CL_SUCCESS)
			isOpenCLCompatible = true;
		else
		{
			DeleteTextureInterop();
			isOpenCLCompatible = false;
		}
	}
	return status;
}

bool CTextureGLPriv::convertToGLTexture2D(cv::UMat& u, GLTexture* glTexture)
{
	//printf("convertToGLTexture2D \n");
	bool isOk = true;

	//printf("convertToGLTexture2D isOpenCLOpenGLInterop \n");
	cl_int status = 0;

	if (isOpenCLCompatible)
	{
		try
		{
			cv::Size srcSize = u.size();
			status = CreateTextureInterop(glTexture);

			if (status != CL_SUCCESS)
				CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromGLTexture failed");

			auto clBuffer = static_cast<cl_mem>(u.handle(cv::ACCESS_READ));
			size_t offset = 0; // TODO
			size_t dst_origin[3] = {0, 0, 0};
			size_t region[3] = {static_cast<size_t>(u.cols), static_cast<size_t>(u.rows), 1};
			status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, nullptr, nullptr);

			if (status == CL_SUCCESS)
			{
				status = clFinish(q); // TODO Use events
			}
			else
			{
				CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
				isOk = false;
			}
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "convertToGLTexture2D OpenCL OpenGL Interop no work" << std::endl;
			status = -1;
			isOk = false;
			printf("convertToGLTexture2D isOpenCLOpenGLInterop is FALSE \n");
		}
	}

	return isOk;
}

void CTextureGLPriv::DeleteTextureInterop()
{
	if (clImage != nullptr)
	{
		cl_int status = 0;
		status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, nullptr, nullptr);
		if (status != CL_SUCCESS)
			cout << "OpenCL: clEnqueueReleaseGLObjects failed" << endl;

		status = clReleaseMemObject(clImage); // TODO RAII
		if (status != CL_SUCCESS)
			cout << "OpenCL: clReleaseMemObject failed" << endl;
	}
}

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

	if (pimpl_ != nullptr)
		delete pimpl_;
}

GLTexture* GLTexture::CreateTextureOutput(int width, int height, const bool& openclOpenGLInterop, GLenum format)
{
	auto glTextureDest = new GLTexture(width, height, openclOpenGLInterop, format);
	return glTextureDest;
}

GLTexture::GLTexture(const int& nWidth, const int& nHeight, const bool& openclOpenGLInterop, GLenum format)
{
	m_nTextureID = 0;
	width = nWidth;
	height = nHeight;
	this->format = format;
	this->openclOpenGLInterop = openclOpenGLInterop;
	Create(nWidth, nHeight, nullptr);
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

void GLTexture::DeleteInteropTexture()
{
	if (pimpl_ != nullptr && pimpl_->isOpenCLCompatible && openclOpenGLInterop)
	{
		pimpl_->DeleteTextureInterop();
	}
}

bool GLTexture::SetData(cv::UMat& bitmap)
{
	bool isOk = false;

	if (pimpl_ == nullptr && openclOpenGLInterop)
		pimpl_ = new CTextureGLPriv();

	if (pimpl_ != nullptr && pimpl_->isOpenCLCompatible && openclOpenGLInterop)
	{
		cv::UMat bitmapMatrix;
		if (bitmap.channels() == 3)
		{
			cvtColor(bitmap, bitmapMatrix, cv::COLOR_BGR2RGBA);
			isOk = pimpl_->convertToGLTexture2D(bitmapMatrix, this);
		}
		else if (bitmap.channels() == 1)
		{
			cvtColor(bitmap, bitmapMatrix, cv::COLOR_GRAY2RGBA);
			isOk = pimpl_->convertToGLTexture2D(bitmapMatrix, this);
		}
		else
		{
			cvtColor(bitmap, bitmapMatrix, cv::COLOR_BGRA2RGBA);
			isOk = pimpl_->convertToGLTexture2D(bitmapMatrix, this);
		}


		if (!isOk)
		{
			openclOpenGLInterop = false;
			pimpl_->DeleteTextureInterop();
		}
	}


	if (!isOk)
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

	return isOk;
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

void GLTexture::SetData(cv::Mat& bitmap)
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


//bool GLTexture::Create(const int &nWidth, const int &nHeight, void *pbyData, const int & nFormat_i, const int & nInternalFormat_i)
bool GLTexture::Create(const int& nWidth, const int& nHeight, uint8_t* pbyData)
{
	width = nWidth;
	height = nHeight;

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


	return (GL_NO_ERROR == glGetError());
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
	checkErrors("GLTexture::Delete()");
	//glDisable(GL_TEXTURE_2D);
	printf("Delete Texture id : %d \n", m_nTextureID);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);

	if (pimpl_ && openclOpenGLInterop)
	{
		pimpl_->DeleteTextureInterop();
	}

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
