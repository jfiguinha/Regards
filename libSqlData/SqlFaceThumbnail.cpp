#include <header.h>
#include "SqlFaceThumbnail.h"
#include "SqlFacePhoto.h"
using namespace Regards::Sqlite;

CSqlFaceThumbnail::CSqlFaceThumbnail(const wxString& filename, const int& numFace)
	: CThumbnailData(filename)
{
	this->numFace = numFace;
}

CSqlFaceThumbnail::~CSqlFaceThumbnail(void)
{
}

cv::Mat CSqlFaceThumbnail::GetImage(bool& isDefault)
{

	CSqlFacePhoto sqlThumbnail;
	// printf("Filename : %s \n",CConvertUtility::ConvertToUTF8(filename));
	cv::Mat image= sqlThumbnail.GetFace(numFace, isDefault);
	if (isDefault)
		image = GetDefaultPicture();
    else
    {
        
    }
	return image;
}

bool CSqlFaceThumbnail::TestBitmap()
{
	return true;
}
