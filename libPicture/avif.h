#pragma once
class CRegardsBitmap;
struct avifDecoder;

namespace Regards
{
	namespace Picture
	{
		class CAvif
		{
		public:
			CAvif();
			~CAvif();
            static void CreateDecoder();
			static cv::Mat GetPicture(const char * filename, bool isThumb = false);
			static cv::Mat GetPicture(const char * filename, int& delay, const int& numPicture);
			static vector<cv::Mat> GetAllPicture(const char * filename, int& delay);
			static void GetPictureDimension(const char * filename, int& width, int& height);
			static int GetNbFrame(const char * filename);
			static bool HasExifMetaData(const char * filename);
			static void GetMetadata(const char * filename, uint8_t* & data, unsigned int& size);
			static int GetDelay(const char * filename);
			static cv::Mat GetThumbnailPicture(const char * filename);
			static void SavePicture(const char * filename, cv::Mat& source, uint8_t* data, const long& size,
			                        const int& compression = 0, const bool& hasExif = false);
			static void SetMetadata(const char * filename, uint8_t*& data, long& size);

		private:
			static std::mutex mudecoder;
			static avifDecoder* decoder;
            static std::mutex mudecoder_thumb;
			static avifDecoder* decoder_thumb;
		};
	}
}