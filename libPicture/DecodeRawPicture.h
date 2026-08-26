#pragma once
#include <string>
using namespace std;
class LibRaw;
class CDecodeRawParameter;
class CImageLoadingFormat;
class CDecodeRawPicturePimpl;

namespace Regards
{
	namespace Filter
	{
		
		class CDecodeRawPicture
		{
		public:
			CDecodeRawPicture(const string& fileName);
			~CDecodeRawPicture();
			CImageLoadingFormat* DecodePicture(CDecodeRawParameter* decodeRawParameter);

		private:
			std::unique_ptr<CDecodeRawPicturePimpl> pimpl;
		};
	}
}
