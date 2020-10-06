#pragma once
class CRegardsBitmap;

namespace Regards
{
	namespace Picture
	{
		class CHeifAvif
		{
		public:
			CHeifAvif();
			~CHeifAvif();
			static CRegardsBitmap * GetPicture(const string &filename);
			static void SavePicture(const string &filenameOut, const int & type, CRegardsBitmap * source, const int &compression = 50);
			static void GetPictureDimension(const string &filename, int &width, int &height);
			static int GetNbFrame(const string &filename);
			static CRegardsBitmap * GetThumbnailPicture(const string &filename);
			static bool HasExifMetaData(const string &filename);
			static void GetMetadata(const string &filename, uint8_t * & data, long & size);
		};
	}
}