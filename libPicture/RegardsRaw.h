#pragma once
#include <string>
using namespace std;

#define JPEGOUTPUT 1
#define BITMAPOUTPUT 2
#define NOTHUMBNAIL 3


namespace Regards
{
	namespace Picture
	{
		class CRegardsRaw
		{
		public:
			static void GetDimensions(const string& fileName, int& width, int& height);
			static int GetOrientation(const string& fileName);
			static std::vector<uint8_t> GetThumbnail(const string& fileName, int& outputFormat);

		};
	}
}
