#ifdef RENDEROPENGL
#pragma once
#include "GLcontext.h"

namespace Regards
{
	namespace OpenGL
	{
		
		// Handle texture related opertions in this class.
		class GLTexture
		{
		public:
			GLTexture(const int &nWidth, const int &nHeight, GLenum format = GL_BGRA_EXT);
			GLTexture(void);
			~GLTexture(void);

			static GLTexture * CreateTextureOutput(int width, int height, GLenum format = GL_BGRA_EXT);
			bool Create(const int &nWidth, const int &nHeight, uint8_t * pbyData);
			void SetFilterType( const GLint FilterType_i, const GLint FilterValue_i );
			void Delete();
			void Enable();
			void SetData(uint8_t * data, const int &nWidth, const int &nHeight);
            void SetData(float * data, const int &nWidth, const int &nHeight);
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
			void GetData(uint8_t * data);
		protected:

			void checkErrors(std::string desc);
			GLuint m_nTextureID;
			int width;
			int height;
			GLenum format;
		};

		

	}
}
#endif