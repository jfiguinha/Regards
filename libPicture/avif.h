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
			static cv::Mat GetPicture(const char * filename, bool isThumb = false);

		};
	}
}