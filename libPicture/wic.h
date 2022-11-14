#ifdef WIN32
#include <wincodec.h>
class CRegardsBitmap;

namespace Regards
{
	namespace Picture
	{
		class CWic
		{
		public:
			CWic();
			~CWic();
			CRegardsBitmap* GetPicture(const string& filename, const int &numPicture);
			CRegardsBitmap* GetThumbnailMetadata(const string& filename);
			int GetNbFrame(const string& filename, bool & error);
			void GetPictureDimension(const string& filename, int& width, int& height);
		private:

			IWICImagingFactory* m_pIWICFactory = nullptr;
			HRESULT hr = S_OK;
		};
	}
}
#endif