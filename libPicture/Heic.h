#pragma once
#ifdef LIBHEIC
class CRegardsBitmap;

namespace Regards
{
	namespace Picture
	{
		class CHeic
		{
		public:
			CHeic();
			~CHeic();

			static void Initx265Decoder();
			static void Uninitx265Decoder();
			static int GetNbFrame(const char * filename);
			static uint32_t GetDelay(const char * filename);
			static cv::Mat GetPicture(const char * filename, bool& isMasterSequence, int& delay,
			                          const int& numPicture);
			static vector<cv::Mat> GetAllPicture(const char * filename, bool& isMasterSequence, int& delay);
			static cv::Mat GetPicture(const char * filename, int& orientation);
			static cv::Mat GetThumbnailPicture(const char * filename, int& orientation);
			static void GetPictureDimension(const char * filename, int& width, int& height);
			static void GetMetadata(const char * filename, uint8_t* & data, unsigned int& size);
			static void SavePicture(const char * filenameOut, cv::Mat& source, uint8_t*& data_exif, unsigned int& size,
			                        const int& compression = 50, const bool& hasExif = false);

		private:
			static void DecodePictureMultiThread(void* parameter);
		};
	}
}
#endif
