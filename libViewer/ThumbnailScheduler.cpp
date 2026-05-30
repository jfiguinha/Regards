#include "ThumbnailScheduler.h"
#include "ThumbnailProcess.h"          // CThumbnailProcess
#include <SqlPhotosWithoutThumbnail.h> // CSqlPhotosWithoutThumbnail
#include <ThumbnailBuffer.h>           // CThumbnailBuffer
#include <window_id.h>
#include <wx/event.h>

using namespace Regards::Viewer;
using namespace Regards::Sqlite;

ThumbnailScheduler::ThumbnailScheduler(wxEvtHandler*    eventSink,
                                       CThumbnailProcess* thumbnailProcess)
    : eventSink(eventSink)
    , thumbnailProcess(thumbnailProcess)
{}

// ── File d'attente ───────────────────────────────────────────────────────────

void ThumbnailScheduler::ReloadFromDatabase()
{
    std::lock_guard<std::mutex> lock(photoListMutex);
    CSqlPhotosWithoutThumbnail sqlPhoto;
    sqlPhoto.GetPhotoList(&photoList, 0);
}

void ThumbnailScheduler::PrioritizeFile(const wxString& filename)
{
    std::lock_guard<std::mutex> lock(photoListMutex);
    auto it = std::find(photoList.begin(), photoList.end(), filename);
    if (it != photoList.end())
        photoList.erase(it);
    photoList.push_front(filename);
}

void ThumbnailScheduler::PrioritizeFiles(const std::vector<wxString>& files)
{
    std::lock_guard<std::mutex> lock(photoListMutex);
    // Insertion en ordre inversé pour que files[0] se retrouve en tête
    for (int i = static_cast<int>(files.size()) - 1; i >= 0; --i)
    {
        const wxString& name = files[i];
        auto it = std::find(photoList.begin(), photoList.end(), name);
        if (it != photoList.end())
            photoList.erase(it);
        photoList.push_front(name);
    }
}

// ── Scheduling ───────────────────────────────────────────────────────────────

bool ThumbnailScheduler::Tick(int nbProcesseur, int nbElementInIconeList)
{
    bool hasPendingWork = false;

    // 1. Essaie de dispatcher un item si le quota n'est pas atteint
    {
        std::lock_guard<std::mutex> lock(photoListMutex);

        const bool canProcess =
            nbElementInIconeList > 0 &&
            !photoList.empty()        &&
            nbProcess < nbProcesseur;

        if (canProcess)
        {
            wxString path = photoList.front();
            photoList.pop_front();

            if (listFile.insert(path).second)   // pas encore en cours
            {
                // Notifie la barre de statut
                auto* evt = new wxCommandEvent(wxEVENT_UPDATEMESSAGE);
                evt->SetExtraLong(static_cast<long>(photoList.size()));
                wxQueueEvent(eventSink, evt);

                int nb = nbProcess.load();
                thumbnailProcess->ProcessThumbnail(path, 0, 0, nb);
                nbProcess.store(nb);

                hasPendingWork = true;
            }
        }
    }

    // 2. Recharge la file si elle est vide
    {
        std::unique_lock<std::mutex> lock(photoListMutex);

        if (photoList.empty())
        {
            CSqlPhotosWithoutThumbnail sqlPhoto;
            sqlPhoto.GetPhotoList(&photoList, 0);
            const bool stillEmpty = photoList.empty();
            lock.unlock();

            if (stillEmpty)
            {
                // Plus rien à générer — informe la barre de statut
                auto* evt = new wxCommandEvent(wxEVENT_UPDATEMESSAGE);
                evt->SetExtraLong(0L);
                wxQueueEvent(eventSink, evt);
                // hasPendingWork reste false
            }
            else
            {
                hasPendingWork = true;
            }
        }
        else
        {
            hasPendingWork = true;
        }
    }

    return hasPendingWork;
}

void ThumbnailScheduler::OnThumbnailDone()
{
    --nbProcess;
}

void ThumbnailScheduler::Reset()
{
    std::lock_guard<std::mutex> lock(photoListMutex);
    nbProcess   = 0;
    thumbnailPos = 0;
    photoList.clear();
    listFile.clear();
}
