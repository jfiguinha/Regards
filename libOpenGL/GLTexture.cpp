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

#include <opencv2/core/opengl.hpp>
using namespace Regards::OpenGL;

using namespace cv::ocl;
extern string platformName;
extern cv::ocl::OpenCLExecutionContext clExecCtx;


void CHECK_ERROR_GL() {
    GLenum err = glGetError();
    if(err != GL_NO_ERROR) {
        std::cerr << "GL Error: " << gluErrorString(err) << std::endl;
        exit(-1);
    }
}

class CTextureGLPriv
{
public:
	CTextureGLPriv()
	{
		//context = static_cast<cl_context>(Context::getDefault().ptr());
		//q = static_cast<cl_command_queue>(Queue::getDefault().ptr());
	}

	bool convertToGLTexture2D(cv::UMat& u, GLTexture* glTexture);
	cl_int CreateTextureInterop(GLTexture* glTexture);
	void DeleteTextureInterop();


	cl_mem clImage = nullptr;
	bool isOpenCLCompatible = true;
	//cl_context context;
	//cl_command_queue q;
};


cl_int CTextureGLPriv::CreateTextureInterop(GLTexture* glTexture)
{
    printf("CreateTextureInterop 1 \n");
	cl_int status = 0;
	if (clImage == nullptr)
	{
        printf("CreateTextureInterop 2 : GLTexture ID : %i \n", glTexture->GetTextureID());
    
        cl_context context = (cl_context)clExecCtx.getContext().ptr();
        cl_command_queue q = (cl_command_queue)clExecCtx.getQueue().ptr();
        
		clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, glTexture->GetTextureID(),
		                                &status);
		if (status == CL_SUCCESS)
		{
            printf("CreateTextureInterop CL_SUCCESS \n");
			status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, nullptr, nullptr);
		}

		if (status == CL_SUCCESS)
			isOpenCLCompatible = true;
		else
		{
            printf("CreateTextureInterop CL_ERROR : %i \n", status);
			DeleteTextureInterop();
			isOpenCLCompatible = false;
		}
	}
	return status;
}

bool CTextureGLPriv::convertToGLTexture2D(cv::UMat& u, GLTexture* glTexture)
{
	bool isOk = true;
	cl_int status = 0;

	if (isOpenCLCompatible)
	{
		try
		{
            CRegardsConfigParam* regardsParam = CParamInit::getInstance();
            wxString color = regardsParam->GetOpenGLOutputColor();

			GLint format, type;
			glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_FORMAT, 1, &format);
			glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_TYPE, 1, &type);

			if (type == 35863)
				color = "BGRA";
			else
				color = "RGBA";
    
			cv::UMat bitmapMatrix;
			if (u.channels() == 3)
			{
				if (color == "BGRA")
				{
					cvtColor(u, bitmapMatrix, cv::COLOR_BGR2BGRA);
				}
				else
				{
					cvtColor(u, bitmapMatrix, cv::COLOR_BGR2RGBA);
				}
			}
			else if (u.channels() == 1)
			{
				cvtColor(u, bitmapMatrix, cv::COLOR_GRAY2RGBA);
			}
			else
			{
				if (color == "BGRA")
				{
					cvtColor(u, bitmapMatrix, cv::COLOR_BGRA2RGBA);
				}
                else
                    bitmapMatrix = u;
			}
            
            
            cl_context context = (cl_context)clExecCtx.getContext().ptr();
            cl_command_queue q = (cl_command_queue)clExecCtx.getQueue().ptr();
            
			cv::Size srcSize = u.size();
			status = CreateTextureInterop(glTexture);

			if (status != CL_SUCCESS)
				CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromGLTexture failed");

			auto clBuffer = static_cast<cl_mem>(bitmapMatrix.handle(cv::ACCESS_READ));
			size_t offset = 0; // TODO
			size_t dst_origin[3] = {0, 0, 0};
			size_t region[3] = {static_cast<size_t>(bitmapMatrix.cols), static_cast<size_t>(bitmapMatrix.rows), 1};
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
        cl_context context = (cl_context)clExecCtx.getContext().ptr();
        cl_command_queue q = (cl_command_queue)clExecCtx.getQueue().ptr();
        
		cl_int status = 0;
		status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, nullptr, nullptr);
		if (status != CL_SUCCESS)
			cout << "OpenCL: clEnqueueReleaseGLObjects failed" << endl;

		status = clReleaseMemObject(clImage); // TODO RAII
		if (status != CL_SUCCESS)
			cout << "OpenCL: clReleaseMemObject failed" << endl;
	}
	clImage = nullptr;
}

GLTexture::GLTexture(void)
{
	m_nTextureID = -1;
	width = 0;
	height = 0;
	format = GL_BGRA_EXT;
    dataformat = GL_RGBA8;
	pboSupported = false;//epoxy_has_gl_extension("GL_ARB_pixel_buffer_object");

}

GLTexture::~GLTexture(void)
{
	Delete();

	if (pboSupported)
	{
		glDeleteBuffers(1, pboIds);
	}

	if (pimpl_ != nullptr)
		delete pimpl_;
        
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
	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	int openclOpenGLInterop = regardsParam->GetIsOpenCLOpenGLInteropSupport();

	if (pimpl_ != nullptr && pimpl_->isOpenCLCompatible && openclOpenGLInterop)
	{
		pimpl_->DeleteTextureInterop();
	}
}

bool GLTexture::SetData(Regards::Picture::CPictureArray& bitmap)
{   
	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	int openclOpenGLInterop = regardsParam->GetIsOpenCLOpenGLInteropSupport();

	int kind = bitmap.Kind();

	bool isOk = false;

	if(kind == cv::_InputArray::KindFlag::UMAT && openclOpenGLInterop)
	{
		cv::UMat umatBitmap = bitmap.getUMat();

		if (pimpl_ == nullptr && openclOpenGLInterop)
			pimpl_ = new CTextureGLPriv();

		if (pimpl_ != nullptr && pimpl_->isOpenCLCompatible && openclOpenGLInterop)
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
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexImage2D(GL_TEXTURE_2D, 0, dataformat, width, height, 0, format, GL_UNSIGNED_BYTE, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
            
            isOk = pimpl_->convertToGLTexture2D(umatBitmap, this);


			if (!isOk)
			{
				openclOpenGLInterop = false;
				pimpl_->DeleteTextureInterop();
				regardsParam->SetIsOpenCLOpenGLInteropSupport(openclOpenGLInterop);
			}
		}
	}
	if (!isOk)
	{
		isOk = SetTextureData(bitmap);
	}

	return isOk;
}


bool GLTexture::SetTextureData(Regards::Picture::CPictureArray& bitmap)
{
     bool isOk = false;
    try
    {
        if (tex == nullptr)
            tex = new cv::ogl::Texture2D();

		bitmap.CopyFrom(tex);
        //tex->copyFrom(bitmap, true);
        tex->bind();
        //tex->setAutoRelease(false);
        m_nTextureID = tex->texId();
        width = tex->size().width;
        height = tex->size().height;
        isOk = true;
    }
    catch (cv::Exception& e)
    {
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
        std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
        isOk = false;
    }
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
		char message[512];
		sprintf(message, "OpenGL error in \"%s\": %p (%d)\n", desc.c_str(), gluErrorString(e), e);
		string data = message;
	}
}

void GLTexture::Delete()
{
	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	int openclOpenGLInterop = regardsParam->GetIsOpenCLOpenGLInteropSupport();

	checkErrors("GLTexture::Delete()");

	if (m_nTextureID != -1)
	{
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
		glBindTexture(GL_TEXTURE_2D, 0);
		checkErrors("GLTexture::Delete()");
	}


}

void GLTexture::Enable()
{
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);
}
