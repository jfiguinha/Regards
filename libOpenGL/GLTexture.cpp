#include <header.h>
#include "GLTexture.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
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

// Supprimé : #include <opencv2/core/opengl.hpp>
// cv::ogl::Texture2D retiré — dépendance opencv_highgui avec support OpenGL
// optionnel souvent absent sur les builds Linux standard, et source de fuites
// mémoire car son cycle de vie est géré en interne par OpenCV sans RAII clair.
// Remplacement : upload CPU direct via glTexImage2D / glTexSubImage2D.

using namespace Regards::OpenGL;
using namespace cv::ocl;

extern string                          platformName;
extern cv::ocl::OpenCLExecutionContext clExecCtx;
extern int                             openclOpenGLInterop;

// ---------------------------------------------------------------------------
// Helper GL error — log sans exit()
// ---------------------------------------------------------------------------
static void CHECK_ERROR_GL(const std::string& location = "")
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        std::cerr << "GL Error in " << location
                  << ": " << gluErrorString(err)
                  << " (0x" << std::hex << err << std::dec << ")\n";
}

// ===========================================================================
// CTextureGLPriv — interop OpenCL / OpenGL
// ===========================================================================
class CTextureGLPriv
{
public:
    CTextureGLPriv()  = default;
    ~CTextureGLPriv() { DeleteTextureInterop(); }

    CTextureGLPriv(const CTextureGLPriv&)            = delete;
    CTextureGLPriv& operator=(const CTextureGLPriv&) = delete;

    bool   convertToGLTexture2D(cv::UMat& u, GLTexture* glTexture);
    cl_int CreateTextureInterop(GLTexture* glTexture);
    void   DeleteTextureInterop();

    cl_mem clImage            = nullptr;
    bool   isOpenCLCompatible = true;
    bool   isBGRATexture      = false;
};

// ---------------------------------------------------------------------------
cl_int CTextureGLPriv::CreateTextureInterop(GLTexture* glTexture)
{
    if (clImage != nullptr)
        return CL_SUCCESS;

    cl_context       context = static_cast<cl_context>(clExecCtx.getContext().ptr());
    cl_command_queue q       = static_cast<cl_command_queue>(clExecCtx.getQueue().ptr());

    cl_int status = 0;
    clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D,
                                    0, glTexture->GetTextureID(), &status);
    if (status != CL_SUCCESS)
    {
        clImage = nullptr;
        isOpenCLCompatible = false;
        return status;
    }

    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, nullptr, nullptr);
    if (status != CL_SUCCESS)
    {
        DeleteTextureInterop();
        isOpenCLCompatible = false;
    }
    else
    {
        isOpenCLCompatible = true;
    }
    return status;
}

// ---------------------------------------------------------------------------
void CTextureGLPriv::DeleteTextureInterop()
{
    if (clImage == nullptr)
        return;

    cl_command_queue q = static_cast<cl_command_queue>(clExecCtx.getQueue().ptr());

    clFinish(q);

    cl_int status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, nullptr, nullptr);
    if (status != CL_SUCCESS)
        std::cerr << "OpenCL: clEnqueueReleaseGLObjects failed (" << status << ")\n";

    clFinish(q);

    status = clReleaseMemObject(clImage);
    if (status != CL_SUCCESS)
        std::cerr << "OpenCL: clReleaseMemObject failed (" << status << ")\n";

    clImage = nullptr;
}

bool CTextureGLPriv::convertToGLTexture2D(cv::UMat& u, GLTexture* glTexture)
{
    bool isOk = true;
    cl_int status = 0;

    if (isOpenCLCompatible)
    {
        CRegardsConfigParam* regardsParam = CParamInit::getInstance();
        wxString color = regardsParam->GetOpenGLOutputColor();

#ifndef __APPLE__
        try
        {
            //printf("glGetInternalformativ test if is it available \n"); 

            GLint format, type;
           // glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_FORMAT, 1, &format);
            glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_TYPE, 1, &type);

            //printf("GL_TEXTURE_IMAGE_FORMAT Dec : %d Hex : %#08x\n", format, format);
            //printf("GL_TEXTURE_IMAGE_TYPE Dec : %d Hex : %#08x\n", type, type);

            if (type == GL_UNSIGNED_INT_8_8_8_8_REV)
                color = "BGRA";
            else
                color = "RGBA";
        }
        catch (...)
        {
            //printf("glGetInternalformativ is not available \n"); 
            color = "RGBA";
        }
#else
        color = "RGBA";
#endif 

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
                if (color == "BGRA" && !isBGRATexture)
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
            size_t dst_origin[3] = { 0, 0, 0 };
            size_t region[3] = { static_cast<size_t>(bitmapMatrix.cols), static_cast<size_t>(bitmapMatrix.rows), 1 };
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
            //printf("convertToGLTexture2D isOpenCLOpenGLInterop is FALSE \n");
        }
    }

    return isOk;
}
/*

// ---------------------------------------------------------------------------
bool CTextureGLPriv::convertToGLTexture2D(cv::UMat& u, GLTexture* glTexture)
{
    if (!isOpenCLCompatible)
        return false;

    // Détermination du format de sortie
    std::string color = "RGBA";
#ifndef __APPLE__
    try
    {
        GLint type = 0;
        glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_TYPE, 1, &type);
        color = (type == GL_UNSIGNED_INT_8_8_8_8_REV) ? "BGRA" : "RGBA";
    }
    catch (...) { color = "RGBA"; }
#endif

    // Conversion de format de pixel
    cv::UMat bitmapMatrix;
    try
    {
        const int ch = u.channels();
        if (ch == 3)
            cv::cvtColor(u, bitmapMatrix,
                         color == "BGRA" ? cv::COLOR_BGR2BGRA : cv::COLOR_BGR2RGBA);
        else if (ch == 1)
            cv::cvtColor(u, bitmapMatrix, cv::COLOR_GRAY2RGBA);
        else
        {
            if (color == "BGRA" && !isBGRATexture)
                cv::cvtColor(u, bitmapMatrix, cv::COLOR_BGRA2RGBA);
            else
                bitmapMatrix = u;
        }
    }
    catch (const cv::Exception& e)
    {
        std::cerr << "convertToGLTexture2D cvtColor failed: " << e.what() << "\n";
        return false;
    }

    // Copie vers la texture GL via OpenCL
    cl_command_queue q = static_cast<cl_command_queue>(clExecCtx.getQueue().ptr());

    cl_int status = CreateTextureInterop(glTexture);
    if (status != CL_SUCCESS)
    {
        std::cerr << "OpenCL: CreateTextureInterop failed (" << status << ")\n";
        return false;
    }

    try
    {
        auto   clBuffer      = static_cast<cl_mem>(bitmapMatrix.handle(cv::ACCESS_READ));
        size_t dst_origin[3] = {0, 0, 0};
        size_t region[3]     = {
            static_cast<size_t>(bitmapMatrix.cols),
            static_cast<size_t>(bitmapMatrix.rows),
            1
        };

        status = clEnqueueCopyBufferToImage(q, clBuffer, clImage,
                                            0, dst_origin, region,
                                            0, nullptr, nullptr);
        if (status != CL_SUCCESS)
        {
            std::cerr << "OpenCL: clEnqueueCopyBufferToImage failed (" << status << ")\n";
            return false;
        }

        status = clFinish(q);
        if (status != CL_SUCCESS)
        {
            std::cerr << "OpenCL: clFinish failed (" << status << ")\n";
            return false;
        }
    }
    catch (const cv::Exception& e)
    {
        std::cerr << "convertToGLTexture2D exception: " << e.what() << "\n";
        isOpenCLCompatible = false;
        return false;
    }

    return true;
}
*/
// ===========================================================================
// GLTexture
// ===========================================================================

GLTexture::GLTexture()
    : m_nTextureID(static_cast<GLuint>(-1))
    , width(0), height(0)
    , format(GL_BGRA_EXT), dataformat(GL_RGBA8)
    , pboSupported(false), pboIds{0}
    , pimpl_(nullptr)
{}

GLTexture::GLTexture(const int& textureId, const int& w, const int& h)
    : m_nTextureID(static_cast<GLuint>(textureId))
    , width(w), height(h)
    , format(GL_BGRA_EXT), dataformat(GL_RGBA8)
    , pboSupported(false), pboIds{0}
    , pimpl_(nullptr)
{}

GLTexture::GLTexture(GLTexture&& other) noexcept
    : m_nTextureID(other.m_nTextureID)
    , width(other.width), height(other.height)
    , format(other.format), dataformat(other.dataformat)
    , pboSupported(other.pboSupported), pboIds{other.pboIds[0]}
    , pimpl_(std::move(other.pimpl_))
{
    other.m_nTextureID = static_cast<GLuint>(-1);
    other.width = other.height = 0;
}

GLTexture& GLTexture::operator=(GLTexture&& other) noexcept
{
    if (this != &other)
    {
        Delete();
        m_nTextureID = other.m_nTextureID;
        width        = other.width;
        height       = other.height;
        format       = other.format;
        dataformat   = other.dataformat;
        pboSupported = other.pboSupported;
        pboIds[0]    = other.pboIds[0];
        pimpl_       = std::move(other.pimpl_);

        other.m_nTextureID = static_cast<GLuint>(-1);
        other.width = other.height = 0;
    }
    return *this;
}

GLTexture::~GLTexture()
{
    Delete();
    if (pboSupported)
        glDeleteBuffers(1, pboIds);
    // pimpl_ détruit automatiquement par unique_ptr
}


// ---------------------------------------------------------------------------
void GLTexture::DeleteInteropTexture()
{
    if (pimpl_ && pimpl_->isOpenCLCompatible && openclOpenGLInterop)
        pimpl_->DeleteTextureInterop();
}

// ---------------------------------------------------------------------------
// Crée ou recrée la texture GL et uploade les pixels depuis un cv::Mat CPU.
// Utilise glTexSubImage2D si les dimensions n'ont pas changé (plus rapide
// que glTexImage2D qui réalloue le storage côté driver).
// ---------------------------------------------------------------------------
bool GLTexture::SetTextureDataCPU(Regards::Picture::CPictureArray& bitmap)
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                     newW, newH, 0,
                     dataformat, GL_UNSIGNED_BYTE, rgba.data);
        CHECK_ERROR_GL("SetTextureDataCPU glTexImage2D");
    }
    else if (newW != width || newH != height)
    {
        // Dimensions changées : réallocation du storage
        glBindTexture(GL_TEXTURE_2D, m_nTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                     newW, newH, 0,
                     dataformat, GL_UNSIGNED_BYTE, rgba.data);
        CHECK_ERROR_GL("SetTextureDataCPU glTexImage2D resize");
    }
    else
    {
        // Mêmes dimensions : mise à jour partielle, plus rapide
        glBindTexture(GL_TEXTURE_2D, m_nTextureID);
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        0, 0, newW, newH,
                        dataformat, GL_UNSIGNED_BYTE, rgba.data);
        CHECK_ERROR_GL("SetTextureDataCPU glTexSubImage2D");
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    width  = newW;
    height = newH;
    return true;
}

// ---------------------------------------------------------------------------
bool GLTexture::SetData(Regards::Picture::CPictureArray& bitmap)
{
    bool isOk = false;

    // Chemin OpenCL/GL interop (UMat sur GPU)
    if (bitmap.Kind() == cv::_InputArray::KindFlag::UMAT && openclOpenGLInterop)
    {
        if (!pimpl_)
            pimpl_ = std::make_unique<CTextureGLPriv>();

        cv::UMat umatBitmap = bitmap.getUMat();

        if (umatBitmap.channels() == 4)
        {
            format     = GL_BGRA;
            dataformat = GL_BGRA;
            pimpl_->isBGRATexture = true;
        }
        else
        {
            format     = GL_BGRA_EXT;
            dataformat = GL_RGBA8;
            pimpl_->isBGRATexture = false;
        }

        if (pimpl_->isOpenCLCompatible)
        {
            if (bitmap.getWidth() != width || bitmap.getHeight() != height)
            {
                Delete();
                m_nTextureID = static_cast<GLuint>(-1);
            }

            if (m_nTextureID == static_cast<GLuint>(-1))
            {
                width  = bitmap.getWidth();
                height = bitmap.getHeight();

                glGenTextures(1, &m_nTextureID);
                glBindTexture(GL_TEXTURE_2D, m_nTextureID);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(GL_TEXTURE_2D, 0, dataformat, width, height,
                             0, format, GL_UNSIGNED_BYTE, nullptr);
                glBindTexture(GL_TEXTURE_2D, 0);
                CHECK_ERROR_GL("GLTexture::SetData glTexImage2D");
            }

            isOk = pimpl_->convertToGLTexture2D(umatBitmap, this);

            if (!isOk)
            {
                CRegardsConfigParam* regardsParam = CParamInit::getInstance();
                openclOpenGLInterop = 0;
                pimpl_->DeleteTextureInterop();
                regardsParam->SetIsOpenCLOpenGLInteropSupport(openclOpenGLInterop);
            }
        }
    }

    // Fallback CPU — remplace l'ancien chemin cv::ogl::Texture2D
    if (!isOk)
    {
        isOk = SetTextureDataCPU(bitmap);
        if (!isOk)
        {
            Delete();
            isOk = SetTextureDataCPU(bitmap);
        }
    }

    return isOk;
}

// ---------------------------------------------------------------------------
void GLTexture::SetFilterType(const GLint filterType, const GLint filterValue)
{
    glBindTexture(GL_TEXTURE_2D, m_nTextureID);
    glTexParameteri(GL_TEXTURE_2D, filterType, filterValue);
}

void GLTexture::checkErrors(const std::string& desc)
{
    GLenum e = glGetError();
    if (e != GL_NO_ERROR)
        std::cerr << "OpenGL error in \"" << desc
                  << "\": " << gluErrorString(e)
                  << " (0x" << std::hex << e << std::dec << ")\n";
}

// ---------------------------------------------------------------------------
void GLTexture::Delete()
{
    checkErrors("GLTexture::Delete() entry");

    if (m_nTextureID != static_cast<GLuint>(-1) && m_nTextureID != 0)
    {
        glBindTexture(GL_TEXTURE_2D, m_nTextureID);

        if (pimpl_ && openclOpenGLInterop)
            pimpl_->DeleteTextureInterop();

        glDeleteTextures(1, &m_nTextureID);
        m_nTextureID = static_cast<GLuint>(-1);

        glBindTexture(GL_TEXTURE_2D, 0);
        checkErrors("GLTexture::Delete() glDeleteTextures");
    }
}

// ---------------------------------------------------------------------------
void GLTexture::Enable()
{
    glBindTexture(GL_TEXTURE_2D, m_nTextureID);
}
