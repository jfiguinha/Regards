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
			static bool IsOccupied();
			static cv::Mat GetPicture(const char* filename, int& rotation);
			static cv::Mat GetPictureThumb(const char* filename, const int& width, const int& heigth);
		};
	}
}