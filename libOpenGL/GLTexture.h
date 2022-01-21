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

			cv::ogl::Texture2D* GetGLTexture();

			int GetWidth();
			int GetHeight();

		protected:

			void checkErrors(std::string desc);
			cv::ogl::Texture2D * texture;
		};
	}
}
