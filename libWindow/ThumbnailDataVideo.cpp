#include "ThumbnailDataVideo.h"
#if defined(__WXMSW__)
#include "../include/config_id.h"
#else
#include <config_id.h>
#endif
#include <libPicture.h>
#include <wx/filename.h>
#include <ConvertUtility.h>
#include <ImageLoadingFormat.h>

CThumbnailDataVideo::CThumbnailDataVideo(const wxString & filename, const bool &testValidity)
	: CThumbnailData(filename)
{
	//Loading Video Thumbnail
    CLibPicture libPicture;
    listVideo = libPicture.LoadAllVideoThumbnail(filename);
}

CThumbnailDataVideo::~CThumbnailDataVideo(void)
{
    for(CImageVideoThumbnail * video : listVideo)
    {
        if(video->image != nullptr)
            delete video->image;
        delete video;
    }
        
}


wxImage CThumbnailDataVideo::GetwxImage(const int &numImage)
{
    wxImage test;
    if(numImage < listVideo.size())
    {   
        listVideo[numImage]->image->GetwxImage();
    }
    return test;
}

void CThumbnailDataVideo::SetBitmap(CImageLoadingFormat * bitmap)
{
	
}

