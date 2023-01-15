#include "header.h"
#include "videostream.h"
#include "moviestreaminfo.h"
#include <ConvertUtility.h>
using namespace Regards::Video;
using namespace ffmpegthumbnailer;

CVideoStream::CVideoStream()
{
}


CVideoStream::~CVideoStream()
{
}


vector<CStreamInfo> CVideoStream::GetVideoStream(const wxString& fileName)
{
	MovieStreamInfo movieStream(CConvertUtility::ConvertToStdString(fileName));
	return movieStream.GetListStreamInfo();
}
