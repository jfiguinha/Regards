#pragma once

class CImageLoadingFormat;

namespace Regards
{
	namespace Picture
	{
		class CRaw
		{
		public:
			static void GetDimensions(const wxString& fileName, int& width, int& height);
			static CImageLoadingFormat* GetThumbnail(const wxString& fileName, const bool& thumbnail, bool & isFromExif);
			static bool LoadPicture(const wxString& fileName, CImageLoadingFormat* imageLoadingFormat);
		};
	}
}
