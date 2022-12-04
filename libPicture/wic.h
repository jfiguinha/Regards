#ifdef WIN32
#include <wincodec.h>


namespace Regards
{
	namespace Picture
	{
		class CWic
		{
		public:
			CWic();
			~CWic();
			cv::Mat GetPicture(const string& filename, const int &numPicture);
			cv::Mat GetThumbnailMetadata(const string& filename);
			int GetNbFrame(const string& filename, bool & error);
			void GetPictureDimension(const string& filename, int& width, int& height);
		private:

			IWICImagingFactory* m_pIWICFactory = nullptr;
			HRESULT hr = S_OK;
		};
	}
}
#endif