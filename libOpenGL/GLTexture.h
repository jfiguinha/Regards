#pragma once
#include "GLcontext.h"

class CRegardsBitmap;

namespace Regards
{
	namespace OpenGL
	{
		// Handle texture related opertions in this class.
		class GLTexture
		{
		public:
			GLTexture(const int& nWidth, const int& nHeight, GLenum format = GL_BGRA_EXT);
			GLTexture(void);
			~GLTexture(void);

			static GLTexture* CreateTextureOutput(int width, int height, GLenum format = GL_BGRA_EXT);
			bool Create(const int& nWidth, const int& nHeight, uint8_t* pbyData);
			void SetFilterType(GLint FilterType_i, GLint FilterValue_i);
			void Delete();
			void Enable();
			void SetData(CRegardsBitmap * bitmap);
			void SetData(cv::Mat & bitmap);
			void SetData(cv::UMat& bitmap);
			void Disable()
			{
				glDisable(GL_TEXTURE_2D);
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
			int width;
			int height;
			GLenum format;
		};
	}
}
