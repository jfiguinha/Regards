#include <header.h>
#include "ThumbnailDataFace.h"
#include "SqlFacePhoto.h"
using namespace Regards::Sqlite;

CThumbnailDataFace::CThumbnailDataFace(const wxString& filename, const int& numFace)
	: CThumbnailData(filename)
{
	this->numFace = numFace;
}

CThumbnailDataFace::~CThumbnailDataFace(void)
{
}

cv::Mat CThumbnailDataFace::GetImage(bool& isDefault)
{

	CSqlFacePhoto sqlThumbnail;
	// printf("Filename : %s \n",CConvertUtility::ConvertToStdString(filename));
	cv::Mat image= sqlThumbnail.GetFace(numFace, isDefault);
	if (isDefault)
		image = GetDefaultPicture();
	return image;
}

bool CThumbnailDataFace::TestBitmap()
{
	return true;
}
