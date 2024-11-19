#pragma once
#ifdef __WXGTK__
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <CL/cl_gl.h>
#endif

class CTextureGLPriv;
class CTextureCudaPriv;

namespace Regards
{
	namespace OpenGL
	{
		// Handle texture related opertions in this class.
		class GLTexture
		{
		public:

			GLTexture(void);
			~GLTexture(void);

			void SetFilterType(GLint FilterType_i, GLint FilterValue_i);
			void Delete();
			void DeleteInteropTexture();
			void Enable();
			void SetData(cv::Mat& bitmap);
			bool SetData(cv::UMat& bitmap);
			bool SetData(cv::cuda::GpuMat& bitmap);

			void Disable()
			{
				//glDisable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			int GetTextureID()
			{
				return m_nTextureID;
			}

			cl_mem GetOpenCLTexture();

			int GetWidth();
			int GetHeight();


		protected:

			void checkErrors(std::string desc);
			GLuint m_nTextureID;
            
            bool SetTextureData(cv::InputArray& bitmap);

			CTextureGLPriv* pimpl_ = nullptr;
			
			cv::ogl::Texture2D * tex = nullptr;

			bool openclOpenGLInterop = false;
			int width;
			int height;
			GLenum format;
			bool pboSupported = false;
            GLenum dataformat;
			GLuint pboIds[1];
		};
	}
}
