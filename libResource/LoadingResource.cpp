#include "header.h"
#include "LoadingResource.h"
#include <libPicture.h>
#include <LibResource.h>
#include <PictureData.h>
#include <ConvertUtility.h>
using namespace Regards::Picture;

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

cv::Mat CLoadingResource::LoadResourceCV(const wxString& resourceName)
{
    cv::Mat out;
    wxString resourcePath = CLibResource::LoadBitmapFromResource(resourceName);
    if (wxFileExists(resourcePath))
    {
        out = cv::imread(resourcePath.ToStdString());
    }
    return out;
}