#pragma once
#include "GLcontext.h"
#include <stdint.h>

namespace Regards
{
	namespace OpenGL
	{
		// Handle texture related opertions in this class.
		class GLTextureYUV
		{
		public:
			GLTextureYUV(void);
			~GLTextureYUV(void);
			bool Create(const int &nWidth, const int &nHeight, const int &nWidthRect, const int &nHeightRect, uint8_t * pbyData);
			void SetFilterType( const GLint FilterType_i, const GLint FilterValue_i );
			void Delete();
			void Enable();
			int GetWidth();
			int GetHeight();
			void Disable()
			{
				glDisable( GL_TEXTURE_2D );
			}
			int GetTextureIDY()
			{
				return imageTextureY;
			}
			int GetTextureIDUV()
			{
				return imageTextureUV;
			}

		private:

			GLuint imageTextureY;
			GLuint imageTextureUV;
			int width;
			int height;
		};

	}
}