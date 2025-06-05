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
extern std::string platformName;
extern cv::ocl::OpenCLExecutionContext clExecCtx;

void CHECK_ERROR_GL() {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "GL Error: " << gluErrorString(err) << std::endl;
        std::exit(-1);
    }
}

class CTextureGLPriv {
public:
    CTextureGLPriv() = default;
    ~CTextureGLPriv() { DeleteTextureInterop(); }

    bool convertToGLTexture2D(cv::UMat& u, GLTexture* glTexture);
    cl_int CreateTextureInterop(GLTexture* glTexture);
    void DeleteTextureInterop();

    cl_mem clImage = nullptr;
    bool isOpenCLCompatible = true;
};

cl_int CTextureGLPriv::CreateTextureInterop(GLTexture* glTexture) {
    if (clImage != nullptr)
        return CL_SUCCESS;

    cl_int status = 0;
    cl_context context = (cl_context)clExecCtx.getContext().ptr();
    cl_command_queue q = (cl_command_queue)clExecCtx.getQueue().ptr();

    clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, glTexture->GetTextureID(), &status);
    if (status == CL_SUCCESS) {
        status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, nullptr, nullptr);
    }

    isOpenCLCompatible = (status == CL_SUCCESS);
    if (!isOpenCLCompatible) {
        DeleteTextureInterop();
    }
    return status;
}

bool CTextureGLPriv::convertToGLTexture2D(cv::UMat& u, GLTexture* glTexture) {
    if (!isOpenCLCompatible)
        return false;

    try {
        auto* regardsParam = CParamInit::getInstance();
        wxString color = regardsParam->GetOpenGLOutputColor();

        GLint format = 0, type = 0;
        glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_FORMAT, 1, &format);
        glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_TYPE, 1, &type);

        if (type == 35863)
            color = "BGRA";
        else
            color = "RGBA";

        cv::UMat bitmapMatrix;
        if (u.channels() == 3) {
            cvtColor(u, bitmapMatrix, color == "BGRA" ? cv::COLOR_BGR2BGRA : cv::COLOR_BGR2RGBA);
        }
        else if (u.channels() == 1) {
            cvtColor(u, bitmapMatrix, cv::COLOR_GRAY2RGBA);
        }
        else {
            if (color == "BGRA")
                cvtColor(u, bitmapMatrix, cv::COLOR_BGRA2RGBA);
            else
                bitmapMatrix = u;
        }

        cl_context context = (cl_context)clExecCtx.getContext().ptr();
        cl_command_queue q = (cl_command_queue)clExecCtx.getQueue().ptr();

        if (CreateTextureInterop(glTexture) != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromGLTexture failed");

        auto clBuffer = static_cast<cl_mem>(bitmapMatrix.handle(cv::ACCESS_READ));
        size_t offset = 0;
        size_t dst_origin[3] = { 0, 0, 0 };
        size_t region[3] = { static_cast<size_t>(bitmapMatrix.cols), static_cast<size_t>(bitmapMatrix.rows), 1 };
        cl_int status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, nullptr, nullptr);

        if (status == CL_SUCCESS) {
            status = clFinish(q);
        }
        else {
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
            isOpenCLCompatible = false;
            return false;
        }
    }
    catch (const cv::Exception& e) {
        std::cerr << "exception caught: " << e.what() << std::endl;
        isOpenCLCompatible = false;
        return false;
    }
    return true;
}

void CTextureGLPriv::DeleteTextureInterop() {
    if (clImage != nullptr) {
        cl_context context = (cl_context)clExecCtx.getContext().ptr();
        cl_command_queue q = (cl_command_queue)clExecCtx.getQueue().ptr();

        cl_int status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, nullptr, nullptr);
        if (status != CL_SUCCESS)
            std::cerr << "OpenCL: clEnqueueReleaseGLObjects failed" << std::endl;

        status = clReleaseMemObject(clImage);
        if (status != CL_SUCCESS)
            std::cerr << "OpenCL: clReleaseMemObject failed" << std::endl;
    }
    clImage = nullptr;
}

GLTexture::GLTexture()
    : m_nTextureID(-1), width(0), height(0), format(GL_BGRA_EXT), dataformat(GL_RGBA8), pboSupported(false), pimpl_(nullptr), tex(nullptr)
{
}

GLTexture::GLTexture(const int& textureId, const int& width, const int& height)
    : m_nTextureID(textureId), width(width), height(height), format(GL_BGRA_EXT), dataformat(GL_RGBA8), pboSupported(false), pimpl_(nullptr), tex(nullptr)
{
}

GLTexture::~GLTexture() {
    Delete();
    if (pboSupported) {
        glDeleteBuffers(1, pboIds);
    }
    delete pimpl_;
}

int GLTexture::GetWidth() { return width; }
int GLTexture::GetHeight() { return height; }

void GLTexture::DeleteInteropTexture() {
    auto* regardsParam = CParamInit::getInstance();
    if (pimpl_ && pimpl_->isOpenCLCompatible && regardsParam->GetIsOpenCLOpenGLInteropSupport()) {
        pimpl_->DeleteTextureInterop();
    }
}

bool GLTexture::SetData(Regards::Picture::CPictureArray& bitmap) {
    auto* regardsParam = CParamInit::getInstance();
    int openclOpenGLInterop = regardsParam->GetIsOpenCLOpenGLInteropSupport();

    int kind = bitmap.Kind();
    bool isOk = false;

    if (kind == cv::_InputArray::KindFlag::UMAT && openclOpenGLInterop) {
        cv::UMat umatBitmap = bitmap.getUMat();

        if (!pimpl_ && openclOpenGLInterop)
            pimpl_ = new CTextureGLPriv();

        if (pimpl_ && pimpl_->isOpenCLCompatible && openclOpenGLInterop) {
            if (bitmap.getWidth() != width || height != bitmap.getHeight()) {
                Delete();
                m_nTextureID = -1;
            }

            if (m_nTextureID == -1) {
                width = bitmap.getWidth();
                height = bitmap.getHeight();
                glGenTextures(1, &m_nTextureID);
                glBindTexture(GL_TEXTURE_2D, m_nTextureID);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(GL_TEXTURE_2D, 0, dataformat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            isOk = pimpl_->convertToGLTexture2D(umatBitmap, this);

            if (!isOk) {
                openclOpenGLInterop = false;
                pimpl_->DeleteTextureInterop();
                regardsParam->SetIsOpenCLOpenGLInteropSupport(openclOpenGLInterop);
            }
        }
    }
    if (!isOk) {
        isOk = SetTextureData(bitmap);
    }
    return isOk;
}

bool GLTexture::SetTextureData(Regards::Picture::CPictureArray& bitmap) {
    try {
        if (!tex)
            tex = new cv::ogl::Texture2D();

        bitmap.CopyFrom(tex);
        tex->bind();
        m_nTextureID = tex->texId();
        width = tex->size().width;
        height = tex->size().height;
        return true;
    }
    catch (const cv::Exception& e) {
        std::cerr << "exception caught: " << e.what() << std::endl;
        std::cerr << "wrong file format, please input the name of an IMAGE file" << std::endl;
        return false;
    }
}

void GLTexture::SetFilterType(const GLint FilterType_i, const GLint FilterValue_i) {
    glBindTexture(GL_TEXTURE_2D, m_nTextureID);
    glTexParameteri(GL_TEXTURE_2D, FilterType_i, FilterValue_i);
}

void GLTexture::checkErrors(const std::string& desc) {
    GLenum e = glGetError();
    if (e != GL_NO_ERROR) {
        char message[512];
        sprintf(message, "OpenGL error in \"%s\": %p (%d)\n", desc.c_str(), gluErrorString(e), e);
        std::string data = message;
    }
}

void GLTexture::Delete() {
    auto* regardsParam = CParamInit::getInstance();
    int openclOpenGLInterop = regardsParam->GetIsOpenCLOpenGLInteropSupport();

    checkErrors("GLTexture::Delete()");

    if (m_nTextureID != -1) {
        glBindTexture(GL_TEXTURE_2D, m_nTextureID);

        if (pimpl_ && openclOpenGLInterop) {
            pimpl_->DeleteTextureInterop();
        }

        if (m_nTextureID != 0) {
            glDeleteTextures(1, &m_nTextureID);
            m_nTextureID = 0;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        checkErrors("GLTexture::Delete()");
    }
}

void GLTexture::Enable() {
    glBindTexture(GL_TEXTURE_2D, m_nTextureID);
}