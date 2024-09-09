#pragma once
#ifdef LIBHEIC
class CRegardsBitmap;
struct heif_image_handle;
struct heif_context;

namespace Regards
{
	namespace Picture
	{
		class CHeic
		{
		public:
			CHeic();
			~CHeic();

			static int GetNbFrame(const char * filename);
			static uint32_t GetDelay(const char * filename);
			static cv::Mat GetPicture(const char * filename, int& delay,
			                          const int& numPicture);
			static vector<cv::Mat> GetAllPicture(const char * filename, int& delay);
			static cv::Mat GetPicture(const char * filename, int& orientation, const bool & isThumbnail);
			static cv::Mat GetThumbnailPicture(const char * filename, int& orientation);
			static void GetPictureDimension(const char * filename, int& width, int& height);

			static void SavePicture(const char * filenameOut, const int& format, cv::Mat& source, uint8_t*& data_exif, unsigned int& size,
			                        const int& compression = 50, const bool& hasExif = false);

            
		};
        
		class CHeicExif
		{
		public:
			static void GetMetadataHeic(const char * filename, uint8_t * & buffer, unsigned int& size);
           
		};
        

	}
}
#endif