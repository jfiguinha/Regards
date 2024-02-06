#include "header.h"
#include "LoadingResource.h"
#include <libPicture.h>
#include <LibResource.h>
#include <PictureData.h>
#include <ConvertUtility.h>
using namespace Regards::Picture;
/*
cv::Mat CLoadingResource::LoadRegardsBmpResource(const wxString& resourceName)
{
	wxString resourcePath = CLibResource::LoadBitmapFromResource(resourceName);
	return cv::imread(CConvertUtility::ConvertToStdString(resourcePath));
}
*/
wxImage CLoadingResource::LoadImageResource(const wxString& resourceName)
{
    wxImage out;
	wxString resourcePath = CLibResource::LoadBitmapFromResource(resourceName);
    if(wxFileExists(resourcePath))
    {
        out.LoadFile(resourcePath, wxBITMAP_TYPE_ANY);
    }
	return out;
}
