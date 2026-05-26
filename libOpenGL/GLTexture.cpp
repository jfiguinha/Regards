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
#include <opengl.hpp>
#include <opencv2/core/opengl.hpp>

using namespace Regards::OpenGL;
using namespace cv::ocl;

extern string                      platformName;
extern cv::ocl::OpenCLExecutionContext clExecCtx;
extern int                         openclOpenGLInterop;

// ---------------------------------------------------------------------------
// Helper : log GL error — ne quitte plus le processus (exit(-1) supprimé)
// FIX : CHECK_ERROR_GL appelait exit(-1), ce qui est inacceptable dans une
// application GUI (pas de cleanup, pas de log propre).
// ---------------------------------------------------------------------------
static void CHECK_ERROR_GL(const std::string& location = "")
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "GL Error"
                  << (location.empty() ? "" : " in " + location)
                  << ": " << gluErrorString(err)
                  << " (0x" << std::hex << err << std::dec << ")"
                  << std::endl;
        // FIX : on loggue sans tuer le processus
    }
}

// ===========================================================================
// CTextureGLPriv — gestion de l'interop OpenCL/OpenGL
// ===========================================================================
class CTextureGLPriv
{
public:
    CTextureGLPriv()  = default;
    ~CTextureGLPriv() { DeleteTextureInterop(); }

    // FIX : non copiable — cl_mem est une ressource OpenCL non partageable naïvement
    CTextureGLPriv(const CTextureGLPriv&)            = delete;
    CTextureGLPriv& operator=(const CTextureGLPriv&) = delete;

    bool   convertToGLTexture2D(cv::UMat& u, GLTexture* glTexture);
    cl_int CreateTextureInterop(GLTexture* glTexture);
    void   DeleteTextureInterop();

    cl_mem clImage           = nullptr;
    bool   isOpenCLCompatible = true;
    bool   isBGRATexture      = false;
};

// ---------------------------------------------------------------------------
cl_int CTextureGLPriv::CreateTextureInterop(GLTexture* glTexture)
{
    if (clImage != nullptr)
        return CL_SUCCESS; // déjà acquis

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
// FIX : séparation claire acquire/release — clFinish avant Release pour
// garantir que le GPU a terminé avant de libérer l'objet partagé.
// ---------------------------------------------------------------------------
void CTextureGLPriv::DeleteTextureInterop()
{
    if (clImage == nullptr)
        return;

    cl_command_queue q = static_cast<cl_command_queue>(clExecCtx.getQueue().ptr());

    // Attendre la fin des opérations en cours avant de relâcher
    clFinish(q);

    cl_int status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, nullptr, nullptr);
    if (status != CL_SUCCESS)
        std::cerr << "OpenCL: clEnqueueReleaseGLObjects failed (" << status << ")\n";

    // FIX : clFinish après Release pour que la queue ait bien traité le Release
    clFinish(q);

    status = clReleaseMemObject(clImage);
    if (status != CL_SUCCESS)
        std::cerr << "OpenCL: clReleaseMemObject failed (" << status << ")\n";

    clImage = nullptr;
}

// ---------------------------------------------------------------------------
bool CTextureGLPriv::convertToGLTexture2D(cv::UMat& u, GLTexture* glTexture)
{
    if (!isOpenCLCompatible)
        return false;

    // --- Détermination du format de sortie ---
    std::string color = "RGBA"; // défaut sûr

#ifndef __APPLE__
    try
    {
        GLint type = 0;
        glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_TYPE, 1, &type);
        // FIX : la constante magique 35863 == GL_UNSIGNED_INT_8_8_8_8_REV
        // qui correspond à l'ordre BGRA natif sur certains drivers.
        // On utilise la constante symbolique pour la lisibilité.
        color = (type == GL_UNSIGNED_INT_8_8_8_8_REV) ? "BGRA" : "RGBA";
    }
    catch (...)
    {
        color = "RGBA";
    }
#endif

    // --- Conversion de format de pixel ---
    cv::UMat bitmapMatrix;
    try
    {
        const int ch = u.channels();
        if (ch == 3)
        {
            cv::cvtColor(u, bitmapMatrix,
                         color == "BGRA" ? cv::COLOR_BGR2BGRA : cv::COLOR_BGR2RGBA);
        }
        else if (ch == 1)
        {
            cv::cvtColor(u, bitmapMatrix, cv::COLOR_GRAY2RGBA);
        }
        else // ch == 4
        {
            // FIX : condition simplifiée — isBGRATexture indique que u est déjà BGRA
            if (color == "BGRA" && !isBGRATexture)
                cv::cvtColor(u, bitmapMatrix, cv::COLOR_BGRA2RGBA);
            else
                bitmapMatrix = u; // pas de copie, partage le buffer UMat
        }
    }
    catch (const cv::Exception& e)
    {
        std::cerr << "convertToGLTexture2D cvtColor failed: " << e.what() << "\n";
        return false;
    }

    // --- Copie vers la texture GL via OpenCL ---
    cl_command_queue q = static_cast<cl_command_queue>(clExecCtx.getQueue().ptr());

    cl_int status = CreateTextureInterop(glTexture);
    if (status != CL_SUCCESS)
    {
        std::cerr << "OpenCL: clCreateFromGLTexture failed (" << status << ")\n";
        return false;
    }

    try
    {
        auto   clBuffer    = static_cast<cl_mem>(bitmapMatrix.handle(cv::ACCESS_READ));
        size_t dst_origin[3] = {0, 0, 0};
        size_t region[3]     = {
            static_cast<size_t>(bitmapMatrix.cols),
            static_cast<size_t>(bitmapMatrix.rows),
            1
        };

        // FIX : offset retiré du TODO — clEnqueueCopyBufferToImage attend un
        // offset en octets dans le buffer source. 0 est correct ici car handle()
        // retourne déjà le pointeur de début de la UMat.
        status = clEnqueueCopyBufferToImage(q, clBuffer, clImage,
                                            0, dst_origin, region,
                                            0, nullptr, nullptr);
        if (status != CL_SUCCESS)
        {
            std::cerr << "OpenCL: clEnqueueCopyBufferToImage failed (" << status << ")\n";
            return false;
        }

        // FIX : clFinish nécessaire ici pour synchroniser avant que GL utilise
        // la texture. Le TODO "Use events" reste valide pour une optimisation
        // future (clEnqueueWaitForEvents éviterait le blocage total de la queue).
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

// ===========================================================================
// GLTexture
// ===========================================================================

GLTexture::GLTexture()
    : m_nTextureID(static_cast<GLuint>(-1))
    , width(0)
    , height(0)
    , format(GL_BGRA_EXT)
    , dataformat(GL_RGBA8)
    , pboSupported(false)
    , pboIds{0}
    , pimpl_(nullptr)
    , tex_(nullptr)
{}

GLTexture::GLTexture(const int& textureId, const int& w, const int& h)
    : m_nTextureID(static_cast<GLuint>(textureId))
    , width(w)
    , height(h)
    , format(GL_BGRA_EXT)
    , dataformat(GL_RGBA8)
    , pboSupported(false)
    , pboIds{0}
    , pimpl_(nullptr)
    , tex_(nullptr)
{}

// FIX : Move constructor — transfert de propriété sans toucher aux ressources GPU
GLTexture::GLTexture(GLTexture&& other) noexcept
    : m_nTextureID(other.m_nTextureID)
    , width(other.width)
    , height(other.height)
    , format(other.format)
    , dataformat(other.dataformat)
    , pboSupported(other.pboSupported)
    , pboIds{other.pboIds[0]}
    , pimpl_(std::move(other.pimpl_))
    , tex_(std::move(other.tex_))
{
    other.m_nTextureID = static_cast<GLuint>(-1);
    other.width        = 0;
    other.height       = 0;
}

GLTexture& GLTexture::operator=(GLTexture&& other) noexcept
{
    if (this != &other)
    {
        Delete();
        m_nTextureID   = other.m_nTextureID;
        width          = other.width;
        height         = other.height;
        format         = other.format;
        dataformat     = other.dataformat;
        pboSupported   = other.pboSupported;
        pboIds[0]      = other.pboIds[0];
        pimpl_         = std::move(other.pimpl_);
        tex_           = std::move(other.tex_);

        other.m_nTextureID = static_cast<GLuint>(-1);
        other.width        = 0;
        other.height       = 0;
    }
    return *this;
}

// FIX : destructeur simplifié — unique_ptr détruit pimpl_ et tex_ automatiquement
GLTexture::~GLTexture()
{
    Delete();

    if (pboSupported)
        glDeleteBuffers(1, pboIds);

    // pimpl_ et tex_ sont des unique_ptr : détruits automatiquement ici
}

// ---------------------------------------------------------------------------
int GLTexture::GetWidth()  const { return width;  }
int GLTexture::GetHeight() const { return height; }

// ---------------------------------------------------------------------------
void GLTexture::DeleteInteropTexture()
{
    if (pimpl_ && pimpl_->isOpenCLCompatible && openclOpenGLInterop)
        pimpl_->DeleteTextureInterop();
}

// ---------------------------------------------------------------------------
bool GLTexture::SetData(Regards::Picture::CPictureArray& bitmap)
{
    const int kind = bitmap.Kind();
    bool isOk = false;

    // --- Chemin OpenCL/GL interop (UMat sur GPU) ---
    if (kind == cv::_InputArray::KindFlag::UMAT && openclOpenGLInterop)
    {
        // FIX : création de pimpl_ sans double vérification redondante
        if (!pimpl_)
            pimpl_ = std::make_unique<CTextureGLPriv>();

        cv::UMat umatBitmap = bitmap.getUMat();

        // Mise à jour du format selon le nombre de canaux
        if (umatBitmap.channels() == 4)
        {
            format   = GL_BGRA;
            dataformat = GL_BGRA;
            pimpl_->isBGRATexture = true;
        }
        else
        {
            format   = GL_BGRA_EXT;
            dataformat = GL_RGBA8;
            pimpl_->isBGRATexture = false;
        }

        if (pimpl_->isOpenCLCompatible)
        {
            // Recréation de la texture si les dimensions ont changé
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
                // L'interop ne fonctionne pas sur ce driver : désactivation globale
                CRegardsConfigParam* regardsParam = CParamInit::getInstance();
                openclOpenGLInterop = 0;
                pimpl_->DeleteTextureInterop();
                regardsParam->SetIsOpenCLOpenGLInteropSupport(openclOpenGLInterop);
            }
        }
    }

    // --- Fallback : chemin CPU via cv::ogl::Texture2D ---
    if (!isOk)
    {
        isOk = SetTextureData(bitmap);
        if (!isOk)
        {
            // FIX : une seule tentative de récupération, pas de boucle infinie
            Delete();
            isOk = SetTextureData(bitmap);
        }
    }

    return isOk;
}

// ---------------------------------------------------------------------------
// FIX : tex_ est un unique_ptr, plus de new/delete manuels
// ---------------------------------------------------------------------------
bool GLTexture::SetTextureData(Regards::Picture::CPictureArray& bitmap)
{
    bool isOk = false;
    try
    {
        if (!tex_)
            tex_ = std::make_unique<cv::ogl::Texture2D>();

        bitmap.CopyFrom(tex_.get());
        tex_->bind();
        m_nTextureID = tex_->texId();
        width  = bitmap.getWidth();
        height = bitmap.getHeight();
        isOk   = true;
    }
    catch (const cv::Exception& e)
    {
        std::cerr << "GLTexture::SetTextureData exception: " << e.what() << "\n";
        isOk = false;
    }
    return isOk;
}

// ---------------------------------------------------------------------------
void GLTexture::SetFilterType(const GLint filterType, const GLint filterValue)
{
    glBindTexture(GL_TEXTURE_2D, m_nTextureID);
    glTexParameteri(GL_TEXTURE_2D, filterType, filterValue);
}

// FIX : checkErrors ne construit plus de string avec sprintf — utilise std::string
void GLTexture::checkErrors(const std::string& desc)
{
    GLenum e = glGetError();
    if (e != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error in \"" << desc
                  << "\": " << gluErrorString(e)
                  << " (0x" << std::hex << e << std::dec << ")\n";
    }
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

    // FIX : unique_ptr — reset() suffit, pas de delete manuel
    tex_.reset();
}

// ---------------------------------------------------------------------------
void GLTexture::Enable()
{
    glBindTexture(GL_TEXTURE_2D, m_nTextureID);
}