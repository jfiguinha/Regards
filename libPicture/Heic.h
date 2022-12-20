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
			static int GetNbFrame(const string &filename);
			static uint32_t GetDelay(const string &filename);
			static cv::Mat GetPicture(const string &filename, bool &isMasterSequence, int &delay, const int &numPicture);
			static vector<cv::Mat> GetAllPicture(const string &filename, bool &isMasterSequence, int &delay);
			static cv::Mat GetPicture(const string &filename, int & orientation);
			static cv::Mat GetThumbnailPicture(const string &filename, int& orientation);
			static void GetPictureDimension(const string &filename, int &width, int &height);
			static void GetMetadata(const string &filename, uint8_t * & data, unsigned int& size);
			static void SavePicture(const string &filenameOut, cv::Mat & source, uint8_t*& data_exif, unsigned int& size, const int &compression = 50, const bool& hasExif = false);
		private:

			static void DecodePictureMultiThread(void * parameter);
		};
	}
}
#endif
