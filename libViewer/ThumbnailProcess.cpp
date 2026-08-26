#include <header.h>
#include "ThumbnailProcess.h"
#include <SqlThumbnail.h>
#include <libPicture.h>
#include <ImageLoadingFormat.h>
#include <ConvertUtility.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>

#include <window_id.h>
#include <opencv2/imgcodecs.hpp>
#include <wx/filename.h>
using namespace Regards::Sqlite;
using namespace Regards::Picture;
using namespace Regards::Viewer;

CThumbnailProcess::CThumbnailProcess(CMainWindow* parent, int maxConcurrent)
{
    this->parent = parent;
    if (maxConcurrent <= 0)
    {
        int cfg = 1;
        if (CRegardsConfigParam* config = CParamInit::getInstance(); config != nullptr)
            cfg = config->GetThumbnailProcess() + 1;
        m_maxConcurrent = cfg;
    }
    else
    {
        m_maxConcurrent = maxConcurrent;
    }
}

void CThumbnailProcess::ProcessThumbnail(wxString filename, int type, long longWindow, int& nbProcess)
{
    if (nbProcess >= m_maxConcurrent)
        return;

    if (filename.empty())
        return;

    
    auto pLoadBitmap = new CThreadLoadingBitmap();
    pLoadBitmap->filename = filename;
    pLoadBitmap->window = parent;
    pLoadBitmap->longWindow = longWindow;
    pLoadBitmap->type = type;
    pLoadBitmap->_thread = std::make_unique<std::thread>(LoadPicture, pLoadBitmap);

    nbProcess++;
}

void CThumbnailProcess::LoadPicture(void* param)
{

    //std::thread* t1 = nullptr;
    CLibPicture libPicture;
    auto threadLoadingBitmap = static_cast<CThreadLoadingBitmap*>(param);
    if (threadLoadingBitmap == nullptr)
        return;

    CImageLoadingFormat* imageLoad = libPicture.LoadThumbnail(threadLoadingBitmap->filename);
    if (imageLoad != nullptr)
    {
        threadLoadingBitmap->bitmapIcone = imageLoad->GetMatrix().getMat();
        delete imageLoad;
    }

    if (!threadLoadingBitmap->bitmapIcone.empty())
    {
        CSqlThumbnail sqlThumbnail;
        wxFileName    file(threadLoadingBitmap->filename);
        wxULongLong   sizeFile = file.GetSize();
        wxString      hash = sizeFile.ToString();

        wxString localName = sqlThumbnail.InsertThumbnail(
            threadLoadingBitmap->filename,
            threadLoadingBitmap->bitmapIcone.size().width,
            threadLoadingBitmap->bitmapIcone.size().height,
            hash);

        if (!localName.empty())
            cv::imwrite(CConvertUtility::ConvertToStdString(localName), threadLoadingBitmap->bitmapIcone);
    }

    // Notify UI — ownership of CThumbnailTask transferred to the event.
    auto event = new wxCommandEvent(wxEVENT_ICONEUPDATE);
    event->SetClientData(threadLoadingBitmap); // caller must keep task alive; use shared_ptr refcount
    wxQueueEvent(threadLoadingBitmap->window, event);
}
