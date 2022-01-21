#pragma once
namespace cv
{
    namespace ogl
    {
        class Texture2D;
    }
}

class CRegardsBitmap;

namespace Regards
{
	namespace OpenGL
	{
		// Handle texture related opertions in this class.
		class GLTexture
		{
		public:
			GLTexture(const int& nWidth, const int& nHeight);
			GLTexture();
			~GLTexture(void);

			void Delete();
			void Enable();
			void SetData(CRegardsBitmap* bitmap);

			void Disable();

			int GetTextureID();

#ifndef __WXGTK__
			cv::ogl::Texture2D* GetGLTexture();
#endif
			int GetWidth();
			int GetHeight();

		protected:

			void checkErrors(std::string desc);
#ifdef __WXGTK__
            bool Create(const int& nWidth, const int& nHeight, uint8_t* pbyData);
			uint m_nTextureID;
			int width;
			int height;
#else
			cv::ogl::Texture2D * texture;
#endif
		};
	}
}
