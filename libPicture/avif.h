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
            static void CreateDecoder();
			static cv::Mat GetPicture(const char * filename, bool isThumb = false);
        private:
            static std::mutex muPicture;
            static bool isOccupied;
            static avifDecoder* decoderThumb;
		};
	}
}