#include <header.h>
#include "GLTexture.h"
#include <opencv2/core.hpp>
#include <epoxy/gl.h>
#ifdef __APPLE__
#include <OpenCL/cl_gl.h>
#include <OpenCL/cl_gl_ext.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <appcontext.h>
#include <OpenCLContext.h>
extern AppContext application_context;
using namespace Regards::OpenGL;
using namespace cv::ocl;


// ===========================================================================
// CTextureGLPriv — interop OpenCL / OpenGL
// ===========================================================================
class CTextureGLPriv
{
public:
	CTextureGLPriv(Regards::OpenCL::COpenCLContext* openCLContext) :openCLContext(openCLContext)
	{};
	~CTextureGLPriv() { DeleteTextureInterop(); }


	bool   convertToGLTexture2D(cv::UMat& u, GLTexture* glTexture);
	cl_int CreateTextureInterop(GLTexture* glTexture);
	void   DeleteTextureInterop();

	cl_mem clImage = nullptr;
	bool isAcquired = false;
	bool   isOpenCLCompatible = true;
	bool   isBGRATexture = false;
	Regards::OpenCL::COpenCLContext* openCLContext = nullptr;
};

// ---------------------------------------------------------------------------
cl_int CTextureGLPriv::CreateTextureInterop(GLTexture* glTexture)
{
	if (clImage != nullptr)
		return CL_SUCCESS;

	cl_context       context = openCLContext->GetContext();
	cl_int status = 0;
	clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D,
		0, glTexture->GetTextureID(), &status);
	if (status != CL_SUCCESS)
	{
		clImage = nullptr;
		isOpenCLCompatible = false;
		return status;
	}

	isOpenCLCompatible = true;
	return CL_SUCCESS;
}



bool CTextureGLPriv::convertToGLTexture2D(cv::UMat& u, GLTexture* glTexture)
{
	bool isOk = true;
	cl_int status = 0;

	if (isOpenCLCompatible)
	{
        try
        {
			cv::UMat bitmapMatrix;
			if (u.channels() == 3)
			{
				cvtColor(u, bitmapMatrix, cv::COLOR_BGR2RGBA);
			}
			else if (u.channels() == 1)
			{
				cvtColor(u, bitmapMatrix, cv::COLOR_GRAY2RGBA);
			}
			else
			{
				cvtColor(u, bitmapMatrix, cv::COLOR_BGRA2RGBA);
			}

			if (!bitmapMatrix.isContinuous())
				bitmapMatrix = bitmapMatrix.clone();
            
			cl_command_queue q = openCLContext->GetCommandQueue();
            
			status = CreateTextureInterop(glTexture);

			if (status != CL_SUCCESS)
				CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromGLTexture failed");

			status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, nullptr, nullptr);
			if (status != CL_SUCCESS)
				CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireGLObjects failed");
			isAcquired = true;

			auto clBuffer = static_cast<cl_mem>(bitmapMatrix.handle(cv::ACCESS_READ));
			size_t offset = 0;
			size_t dst_origin[3] = {0, 0, 0};
			size_t region[3] = {static_cast<size_t>(bitmapMatrix.cols), static_cast<size_t>(bitmapMatrix.rows), 1};
			status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, nullptr, nullptr);

			if (status != CL_SUCCESS)
				CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");

			status = clFinish(q);
			if (status != CL_SUCCESS)
				CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish after copy failed");

			status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, nullptr, nullptr);
			if (status != CL_SUCCESS)
				CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseGLObjects failed");
			// Mark as released as soon as the release command has been successfully enqueued,
			// to avoid double-release if clFinish later fails.
			isAcquired = false;

			status = clFinish(q);
			if (status != CL_SUCCESS)
				CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish after release failed");
		}
		catch (cv::Exception& e)
		{
			if (isAcquired)
				DeleteTextureInterop();
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "convertToGLTexture2D OpenCL OpenGL Interop no work" << std::endl;
			status = -1;
			isOk = false;
			//printf("convertToGLTexture2D isOpenCLOpenGLInterop is FALSE \n");
		}
	}

	return isOk;
}

// ---------------------------------------------------------------------------
void CTextureGLPriv::DeleteTextureInterop()
{
	if (clImage == nullptr)
		return;

	cl_command_queue q = openCLContext->GetCommandQueue();

	cl_int status = CL_SUCCESS;
	if (isAcquired)
	{
		status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, nullptr, nullptr);
		if (status != CL_SUCCESS)
			std::cerr << "OpenCL: clEnqueueReleaseGLObjects failed (" << status << ")\n";
		else
		{
			status = clFinish(q);
			if (status != CL_SUCCESS)
				std::cerr << "OpenCL: clFinish after release failed (" << status << ")\n";
		}
		isAcquired = false;
	}

	status = clReleaseMemObject(clImage);
	if (status != CL_SUCCESS)
		std::cerr << "OpenCL: clReleaseMemObject failed (" << status << ")\n";

	clImage = nullptr;
}

GLTexture::GLTexture(void)
{
	m_nTextureID = -1;
	width = 0;
	height = 0;
	format = GL_BGRA_EXT;
    dataformat = GL_BGRA;
	pboSupported = false;//epoxy_has_gl_extension("GL_ARB_pixel_buffer_object");

}

GLTexture::GLTexture(const int& textureId, const int& width, const int& height)
    : m_nTextureID(textureId), width(width), height(height), format(GL_BGRA_EXT), dataformat(GL_BGRA), pboSupported(false), pimpl_(nullptr)

{

}

GLTexture::~GLTexture(void)
{
	Delete();

	if (pboSupported)
	{
		glDeleteBuffers(1, pboIds);
	}       
}


int GLTexture::GetWidth()
{
	return width;
}

int GLTexture::GetHeight()
{
	return height;
}

void GLTexture::DeleteInteropTexture()
{
	if (pimpl_ != nullptr)
	{
		pimpl_->DeleteTextureInterop();
	}
}

bool GLTexture::SetData(Regards::Picture::CPictureArray& bitmap, Regards::OpenCL::COpenCLContext* openCLContext, const bool &deleteOldData)
{   
   
    //openclOpenGLInterop = 0;

	int kind = bitmap.Kind();

	bool isOk = false;

	if (deleteOldData)
	{
		Delete();
		m_nTextureID = -1;
	}

	if(kind == cv::_InputArray::KindFlag::UMAT && application_context.openclOpenGLInterop && openCLContext != nullptr)
	{
		if (pimpl_ != nullptr && pimpl_->openCLContext != openCLContext)
		{
			pimpl_->DeleteTextureInterop();
			pimpl_.reset();
		}

		if (pimpl_ == nullptr && application_context.openclOpenGLInterop)
			pimpl_ = std::make_unique<CTextureGLPriv>(openCLContext);

		cv::UMat umatBitmap = bitmap.getUMat();

		if (umatBitmap.channels() == 4)
		{
			format = GL_BGRA;
			dataformat = GL_BGRA;
			pimpl_->isBGRATexture = true;
		}
		else
		{
			pimpl_->isBGRATexture = false;
		}


		if (pimpl_ != nullptr && pimpl_->isOpenCLCompatible && application_context.openclOpenGLInterop)
		{
			if (bitmap.getWidth() != width || height != bitmap.getHeight())
			{
				Delete();
				m_nTextureID = -1;

			}

			if (m_nTextureID == -1)
			{
				width = bitmap.getWidth();
				height = bitmap.getHeight();
				glGenTextures(1, &m_nTextureID);
				//glActiveTexture(GL_TEXTURE0 + m_nTextureID);
				glBindTexture(GL_TEXTURE_2D, m_nTextureID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
            
            isOk = pimpl_->convertToGLTexture2D(umatBitmap, this);


			if (!isOk)
			{
				CRegardsConfigParam* regardsParam = CParamInit::getInstance();
				application_context.openclOpenGLInterop = false;
				pimpl_->DeleteTextureInterop();
				regardsParam->SetIsOpenCLOpenGLInteropSupport(application_context.openclOpenGLInterop);
			}
		}
	}
	if (!isOk)
	{
		isOk = SetTextureData(bitmap);// SetTextureData(bitmap);
		if (!isOk)
		{
			Delete();
			isOk = SetTextureData(bitmap);// SetTextureData(bitmap);
		}
	}

	return isOk;
}


bool GLTexture::SetTextureData(Regards::Picture::CPictureArray& bitmap)
{
	// Récupération du cv::Mat CPU (download depuis GPU si UMat)
	cv::Mat mat = bitmap.getMat();
	if (mat.empty())
	{
		std::cerr << "GLTexture::SetTextureDataCPU: bitmap is empty\n";
		return false;
	}

	// Conversion vers RGBA si nécessaire
	cv::Mat rgba;
	const int ch = mat.channels();
	if (ch == 3)
	{
		cv::cvtColor(mat, rgba, cv::COLOR_BGR2BGRA);
	}
	else if (ch == 1)
		cv::cvtColor(mat, rgba, cv::COLOR_GRAY2RGBA);
	else if (ch == 4)
		rgba = mat; // déjà 4 canaux, on suppose RGBA ou BGRA selon le pipeline
	else
	{
		std::cerr << "GLTexture::SetTextureDataCPU: unsupported channel count " << ch << "\n";
		return false;
	}

	// S'assurer que les lignes sont contiguës (requis par glTexImage2D)
	if (!rgba.isContinuous())
		rgba = rgba.clone();

	const int newW = rgba.cols;
	const int newH = rgba.rows;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLenum dataformat = GL_BGRA;

	if (m_nTextureID == static_cast<GLuint>(-1))
	{
		// Première création
		glGenTextures(1, &m_nTextureID);
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			newW, newH, 0,
			dataformat, GL_UNSIGNED_BYTE, rgba.data);
		checkErrors("SetTextureDataCPU glTexImage2D");
	}
	else if (newW != width || newH != height)
	{
		// Dimensions changées : réallocation du storage
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			newW, newH, 0,
			dataformat, GL_UNSIGNED_BYTE, rgba.data);
		checkErrors("SetTextureDataCPU glTexImage2D resize");
	}
	else
	{
		// Mêmes dimensions : mise à jour partielle, plus rapide
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0,
			0, 0, newW, newH,
			dataformat, GL_UNSIGNED_BYTE, rgba.data);
		checkErrors("SetTextureDataCPU glTexSubImage2D");
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	width = newW;
	height = newH;
	return true;
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
		std::cerr << "OpenGL error in \"" << desc << "\": "
			<< reinterpret_cast<const char*>(gluErrorString(e))
			<< " (" << e << ")\n";
	}
}

// ---------------------------------------------------------------------------
void GLTexture::Delete()
{
	checkErrors("GLTexture::Delete() entry");

	if (m_nTextureID != static_cast<GLuint>(-1) && m_nTextureID != 0)
	{
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);

		if (pimpl_)
			pimpl_->DeleteTextureInterop();

		glDeleteTextures(1, &m_nTextureID);
		m_nTextureID = static_cast<GLuint>(-1);
		width = 0;
		height = 0;

		glBindTexture(GL_TEXTURE_2D, 0);
		checkErrors("GLTexture::Delete() glDeleteTextures");
	}
}

void GLTexture::Enable()
{
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);
}
