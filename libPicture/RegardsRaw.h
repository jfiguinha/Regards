#pragma once
#include <string>
using namespace std;

#define JPEGOUTPUT 1
#define BITMAPOUTPUT 2
#define NOTHUMBNAIL 3

class DataStorage
{
public:
	DataStorage()
	{

	}
	~DataStorage()
	{
		if (dataPt != nullptr)
			delete[] dataPt;
		dataPt = nullptr;
	}

	uint8_t* dataPt = nullptr;
	int size = 0;
};

namespace Regards
{
	namespace Picture
	{
		class CRegardsRaw
		{
		public:
			CRegardsRaw();
			~CRegardsRaw();

			static void GetDimensions(const string& fileName, int& width, int& height);
			static DataStorage * GetThumbnail(const string& fileName, int& outputFormat);
		};
	}
}
