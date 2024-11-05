#include <header.h>
#include "ThumbnailViewerVideo.h"
#include <window_id.h>
#include <libPicture.h>
#include <Thumbnail.h>
#include <ThumbnailDataStorage.h>
#include <ImageVideoThumbnail.h>
using namespace Regards::VideoFFmpeg;
using namespace Regards::Picture;

extern wxImage defaultPicture;

CThumbnailViewerVideo::CThumbnailViewerVideo(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
                                             const bool& testValidity)
	: CThumbnailVideo(parent, id, themeThumbnail, testValidity)
{

	idWindowToRefresh = THUMBNAILVIDEOWINDOW;
	moveOnPaint = false;
}


CThumbnailViewerVideo::~CThumbnailViewerVideo(void)
{
}


void CThumbnailViewerVideo::InitWithDefaultPicture(const wxString& szFileName, const int& size)
{
    printf("CThumbnailViewerVideo::InitWithDefaultPicture %s \n", szFileName.ToStdString().c_str());
    
	int x = 0;
	int y = 0;
	int typeElement = TYPEVIDEO;
	threadDataProcess = false;
	auto iconeListLocal = new CIconeList();

    CLibPicture libPicture;
    vector<CImageVideoThumbnail*> listThumbnail = libPicture.LoadAllVideoThumbnail(szFileName, true, true);
    //Création de la liste des icones

    //int i = 0;
    for (auto j = 0; j < size; j++)
    {
        float percent = (static_cast<float>(j) / static_cast<float>(size)) * 100.0f;
        CImageVideoThumbnail* thumbnail = listThumbnail[j];
        auto thumbnailData = new CThumbnailDataStorage(szFileName);
        //thumbnailData->SetStorage(nullptr);
        thumbnailData->SetNumPhotoId(j);
        thumbnailData->SetNumElement(j);
        thumbnailData->SetTypeElement(typeElement);
        thumbnailData->SetPercent(percent);
        thumbnailData->SetTimePosition(thumbnail->timePosition);

        if (thumbnail->image.empty())
        {
            thumbnail->image = CLibPicture::mat_from_wx(defaultPicture);
        }

        thumbnailData->SetBitmap(thumbnail->image);
        if (typeElement == TYPEMULTIPAGE)
            thumbnailData->SetLibelle("Page : " + to_string(j + 1) + "/" + to_string(size));
            
        auto pBitmapIcone = new CIcone();
        pBitmapIcone->SetNumElement(j);
        pBitmapIcone->SetData(thumbnailData);
        pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
        pBitmapIcone->SetWindowPos(x, y);

        if (j == 0)
        {
            pBitmapIcone->SetSelected(true);
            numSelectPhotoId = j;
        }

        iconeListLocal->AddElement(pBitmapIcone);

        x += themeThumbnail.themeIcone.GetWidth();
    }


    for (auto j = 0; j < listThumbnail.size(); j++)
    {
        CImageVideoThumbnail* thumbnail = listThumbnail[j];
        if (thumbnail != nullptr)
            delete thumbnail;
    }
    
    listThumbnail.clear();




	auto oldIconeList = iconeList;
	iconeList = iconeListLocal;

	nbElementInIconeList = iconeList->GetNbElement();

    //printf("CThumbnailVideo::InitWithDefaultPicture \n");
    oldIconeList->EraseThumbnailListWithIcon();
    delete oldIconeList;
    //EraseThumbnailList(oldIconeList);

	threadDataProcess = true;

	UpdateScroll();

	processThumbnailVideo = true;

	//nbElementInIconeList = 1;

	needToRefresh = true;
}


void CThumbnailViewerVideo::OnPictureClick(CThumbnailData* data)
{
    /*
	auto mainWindow = static_cast<CMainWindow*>(this->FindWindowById(MAINVIEWERWINDOWID));
	if (mainWindow != nullptr && data != nullptr)
	{
#ifdef FFMPEG
		int timePosition = data->GetTimePosition();
#else
		int timePosition = data->GetTimePosition() * 1000;
#endif
		wxCommandEvent evt(wxEVENT_PICTUREVIDEOCLICK);
		evt.SetExtraLong(timePosition);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
    */
}
