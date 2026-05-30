#pragma once
#include <Photos.h>
#include <wx/string.h>
#include <wx/event.h>

namespace Regards::Viewer
{
    class CCentralWindow;
    class CCategoryFolderWindow;

    /**
     * FolderRefreshService
     *
     * Responsabilités :
     *   - Interroger la base SQL pour construire la liste filtrée de photos
     *   - Détecter si la liste a réellement changé (HasPictureListChanged)
     *   - Mettre à jour CThumbnailBuffer
     *   - Résoudre le fichier courant (ResolveCurrentFilename) après un refresh
     *   - Propager le refresh aux fenêtres dépendantes (face, catégorie)
     */
    class FolderRefreshService
    {
    public:
        /**
         * @param centralWnd  Fenêtre centrale — pour GetFilename / SetListeFile /
         *                    ImageSuivante / ImagePrecedente / ImageDebut / ImageFin
         * @param eventSink   Fenêtre parente — pour FindWindowById
         */
        FolderRefreshService(CCentralWindow* centralWnd,
                             wxWindow*       eventSink,
                             int             faceDetection);

        /**
         * Rafraîchit la liste de photos visible.
         *
         * @param isDeleteFolder  true si un dossier vient d'être supprimé
         * @param refreshPhotos   true pour forcer un rechargement SQL même si
         *                        la requête n'a pas changé
         */
        void UpdateFolderStatic(bool isDeleteFolder,
                                bool refreshPhotos = false);

        // ── Fichier courant ──────────────────────────────────────────────
        void      SetFirstFileToShow(const wxString& path);
        void      SetLocalFilename(const wxString& path);
        wxString  GetLocalFilename() const;

        /** Retourne true si la liste SQL a changé depuis la dernière requête. */
        bool      GetInit() const { return init; }

    private:
        bool HasPictureListChanged(const PhotosVector* newPictures) const;
        void ResolveCurrentFilename();

        bool FindNextValidFile(const wxString& filename);
        bool FindNextValidFile();
        bool FindPreviousValidFile();

        void RefreshDependentWindows(CCategoryFolderWindow* categoryFolder);

        CCentralWindow* centralWnd;
        wxWindow*       eventSink;
        int             faceDetection;

        wxString localFilename;
        wxString firstFileToShow;
        wxString oldRequest;
        bool     init = true;
    };

} // namespace Regards::Viewer
