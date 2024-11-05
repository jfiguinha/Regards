#pragma once
#ifdef __WXGTK__
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <CL/cl_gl.h>
#endif

class CTextureGLPriv;

namespace Regards
{
	namespace OpenGL
	{
		// Handle texture related opertions in this class.
		class GLTexture
		{
		public:
			GLTexture(const int& nWidth, const int& nHeight, const bool& openclOpenGLInterop,
			          GLenum format = GL_BGRA_EXT, GLenum formatTexture = GL_RGBA8);
			GLTexture(void);
			~GLTexture(void);

			static GLTexture* CreateTextureOutput(int width, int height, const bool& openclOpenGLInterop,
			                                      GLenum format = GL_BGRA_EXT, GLenum formatTexture = GL_RGBA8);
			bool Create(const int& nWidth, const int& nHeight, uint8_t* pbyData);
			void SetFilterType(GLint FilterType_i, GLint FilterValue_i);
			void Delete();
			void DeleteInteropTexture();
			void Enable();
			void SetData(cv::Mat& bitmap);
			bool SetData(cv::UMat& bitmap);

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
			uint8_t* GetData();
			void GetData(uint8_t* data);

		protected:
			void SetTextureData(const cv::Mat& bitmapMatrix);
			void checkErrors(std::string desc);
			GLuint m_nTextureID;
			void SetDataToPBO(const cv::Mat& bitmapMatrix);
			void InitPbo(const cv::Mat& bitmapMatrix);
			CTextureGLPriv* pimpl_ = nullptr;

			bool openclOpenGLInterop = false;
			int width;
			int height;
			GLenum format;
			GLenum formatTexture;
			bool pboSupported = false;
			GLuint pboIds[1];
		};
	}
}
