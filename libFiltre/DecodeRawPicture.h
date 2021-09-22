#pragma once
#include <string>
using namespace std;
class LibRaw;
class CDecodeRawParameter;
class CImageLoadingFormat;
namespace Regards
{
	namespace Filter
	{
		class CDecodeRawPicture
		{
		public:
			CDecodeRawPicture(const string & fileName, CDecodeRawParameter* & decodeRawParameter);
			~CDecodeRawPicture();
			CImageLoadingFormat * DecodePicture(CDecodeRawParameter * decodeRawParameter);

		private:
			LibRaw * rawProcessor;
			int result;
			//unsigned char *buffer;

		};
	}
}