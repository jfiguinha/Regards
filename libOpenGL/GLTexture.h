#pragma once
#include "GLcontext.h"
#include <opencv2/core/opengl.hpp>

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
			GLTexture();
			~GLTexture(void);

			void SetFilterType(GLint FilterType_i, GLint FilterValue_i);
			void Delete();
			void Enable();
			void SetData(CRegardsBitmap* bitmap);

			void Disable()
			{
				glDisable(GL_TEXTURE_2D);
			}

			int GetTextureID()
			{
				return texture.texId();
			}

			cv::ogl::Texture2D* GetGLTexture();

			int GetWidth();
			int GetHeight();

		protected:

			void checkErrors(std::string desc);
			cv::ogl::Texture2D texture;
		};
	}
}
