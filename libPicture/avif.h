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

		private:
			static std::mutex mudecoder;
			static avifDecoder* decoder;
            static std::mutex mudecoder_thumb;
			static avifDecoder* decoder_thumb;
		};
	}
}