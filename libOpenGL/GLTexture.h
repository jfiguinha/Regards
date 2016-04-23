#pragma once
#include "GLcontext.h"
#include <stdint.h>
#include <vector>
using namespace std;

namespace Regards
{
	namespace OpenGL
	{
		
		// Handle texture related opertions in this class.
		class GLTexture
		{
		public:
			GLTexture(const int &nWidth, const int &nHeight);
			GLTexture(void);
			~GLTexture(void);

			static GLTexture * CreateTextureOutput(int width, int height);
			bool Create(const int &nWidth, const int &nHeight, uint8_t * pbyData, const GLenum & format = GL_BGRA_EXT, int byteParPixel = 4);
			void SetFilterType( const GLint FilterType_i, const GLint FilterValue_i );
			void Delete();
			void Enable();
			void SetData(uint8_t * data, const int &nWidth, const int &nHeight, const GLenum & format = GL_BGRA_EXT);
			void Disable()
			{
				glDisable( GL_TEXTURE_2D );
			}
			int GetTextureID()
			{
				return m_nTextureID;
			}

			int GetWidth();
			int GetHeight();
			uint8_t * GetData();
            uint8_t * GetRGBData();
			void GetData(uint8_t * data);
		protected:

			void checkErrors(const char * desc);
			GLuint m_nTextureID;
			int width;
			int height;

		};

		

	}
}