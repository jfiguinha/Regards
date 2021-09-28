#pragma once
#ifdef LIBHEIC
class CRegardsBitmap;
class CxMemFile;

namespace Regards
{
	namespace Picture
	{
		class CHeic
		{
		public:
			CHeic();
			~CHeic();

			static void Initx265Decoder();
			static void Uninitx265Decoder();
			static int GetNbFrame(const string &filename);
			static uint32_t GetDelay(const string &filename);
			static CRegardsBitmap * GetPicture(const string &filename, bool &isMasterSequence, int &delay, const int &numPicture);
			static vector<CRegardsBitmap *> GetAllPicture(const string &filename, bool &isMasterSequence, int &delay);
			static CRegardsBitmap * GetPicture(const string &filename);
			static CRegardsBitmap * GetThumbnailPicture(const string &filename);
			static void GetPictureDimension(const string &filename, int &width, int &height);
			static void GetMetadata(const string &filename, uint8_t * & data, unsigned int& size);
			static void SavePicture(const string &filenameOut, CRegardsBitmap * source, const int &compression = 50);
			static void SetMetadata(const string& filename, uint8_t*& data, unsigned int & size);
		private:

			static void DecodePictureMultiThread(void * parameter);
		};
	}
}
#endif
