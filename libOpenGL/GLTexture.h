#pragma once
#ifdef __WXGTK__
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <CL/cl_gl.h>
#endif

#include <PictureArray.h>
#include <memory>
#include <string>

// FIX : forward declarations suffisent, pas besoin d'inclure les headers lourds ici
class CTextureGLPriv;

namespace cv
{
    namespace ogl
    {
        class Texture2D;
    }
}

namespace Regards
{
    namespace OpenGL
    {
        class GLTexture
        {
        public:
            GLTexture();
            GLTexture(const int& textureId, const int& width, const int& height);
            ~GLTexture();

            // FIX : non copiable — une texture OpenGL est une ressource GPU unique.
            // Copier l'ID sans dupliquer la ressource provoque un double-delete GL.
            GLTexture(const GLTexture&)            = delete;
            GLTexture& operator=(const GLTexture&) = delete;

            // Move autorisé : transfert de propriété sans copie GPU
            GLTexture(GLTexture&& other) noexcept;
            GLTexture& operator=(GLTexture&& other) noexcept;

            void SetFilterType(GLint filterType, GLint filterValue);
            void Delete();
            void DeleteInteropTexture();
            void Enable();
            bool SetData(Regards::Picture::CPictureArray& bitmap);

            void Disable()
            {
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            GLuint GetTextureID() const { return m_nTextureID; }

            // FIX : GetOpenCLTexture() était déclaré dans le .h mais jamais défini
            // dans le .cpp → supprimé pour éviter le link error.
            // Si nécessaire, l'exposer via CTextureGLPriv.

            int GetWidth()  const { return width;  }
            int GetHeight() const { return height; }

        protected:
            void checkErrors(const std::string& desc);

            bool SetTextureData(Regards::Picture::CPictureArray& bitmap);

            GLuint  m_nTextureID = static_cast<GLuint>(-1);
            int     width        = 0;
            int     height       = 0;
            GLenum  format       = GL_BGRA_EXT;
            GLenum  dataformat   = GL_RGBA8;
            bool    pboSupported = false;
            GLuint  pboIds[1]    = {0};

            // FIX : unique_ptr remplace les raw pointers — destruction automatique
            // garantie même en cas d'exception, et ownership explicite.
            std::unique_ptr<CTextureGLPriv>    pimpl_;
            std::unique_ptr<cv::ogl::Texture2D> tex_;
        };
    }
}