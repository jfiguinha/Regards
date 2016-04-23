#pragma once
#include "GLcontext.h"
#include <stdint.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

namespace Regards
{
	namespace OpenGL
	{
		// Handle texture related opertions in this class.
		class GLTextureRgbWithAlpha
		{
		public:
			GLTextureRgbWithAlpha(void);
			~GLTextureRgbWithAlpha(void);
			bool Create(const wxImage &image);
			void Delete();
			void Enable();
			int GetWidth();
			int GetHeight();

			wxImage GetData();
			void Disable()
			{
				glDisable( GL_TEXTURE_2D );
			}
			int GetTextureIDRgb()
			{
				return imageTextureRgb;
			}
			int GetTextureIDAlpha()
			{
				return imageTextureAlpha;
			}

		private:

			GLuint imageTextureRgb;
			GLuint imageTextureAlpha;
			int width;
			int height;
		};

	}
}