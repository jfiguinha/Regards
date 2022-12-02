#pragma once
class CRegardsBitmap;

namespace Regards
{
	namespace Picture
	{

		class CAvif
		{
		public:
			CAvif();
			~CAvif();
			static cv::Mat GetPicture(const string &filename);
			static cv::Mat GetPicture(const string &filename, int &delay, const int &numPicture);
			static vector<cv::Mat> GetAllPicture(const string &filename, int &delay);
			static void GetPictureDimension(const string &filename, int &width, int &height);
			static int GetNbFrame(const string &filename);
			static bool HasExifMetaData(const string &filename);
			static void GetMetadata(const string &filename, uint8_t * & data, unsigned int& size);
			static int GetDelay(const string &filename);
			static cv::Mat GetThumbnailPicture(const string &filename);
			static void SavePicture(const string& filename, cv::Mat & source, uint8_t* data,const long& size, const int &compression = 0, const bool& hasExif = false);
			static void SetMetadata(const string& filename, uint8_t*& data, long& size);
		};
	}
}