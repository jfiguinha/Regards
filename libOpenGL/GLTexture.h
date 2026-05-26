#pragma once
#ifdef __WXGTK__
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <CL/cl_gl.h>
#endif

#include <PictureArray.h>
#include <memory>
#include <string>

class CTextureGLPriv;

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

            // Non copiable — une texture OpenGL est une ressource GPU unique.
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
            int    GetWidth()     const { return width;  }
            int    GetHeight()    const { return height; }

        protected:
            void checkErrors(const std::string& desc);

            // Chemin CPU : upload direct via glTexImage2D / glTexSubImage2D.
            // Remplace l'ancien SetTextureData qui dépendait de cv::ogl::Texture2D.
            bool SetTextureDataCPU(Regards::Picture::CPictureArray& bitmap);

            GLuint m_nTextureID = static_cast<GLuint>(-1);
            int    width        = 0;
            int    height       = 0;
            GLenum format       = GL_BGRA_EXT;
            GLenum dataformat   = GL_RGBA8;
            bool   pboSupported = false;
            GLuint pboIds[1]    = {0};

            // cv::ogl::Texture2D supprimé — plus de dépendance opencv_highgui/opengl
            std::unique_ptr<CTextureGLPriv> pimpl_;
        };
    }
}