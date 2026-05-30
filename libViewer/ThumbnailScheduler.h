#pragma once
#include <unordered_set>

namespace Regards::Viewer
{
    class CThumbnailProcess;

    /**
     * ThumbnailScheduler
     *
     * Responsabilités :
     *   - Maintenir la file des photos sans miniature (photoList / listFile)
     *   - Soumettre les travaux à CThumbnailProcess en respectant le quota
     *     de processeurs (nbProcess < nbProcesseur)
     *   - Recharger la file depuis SQL quand elle est vide
     *   - Exposer les compteurs utilisés par la barre de statut
     */
    class ThumbnailScheduler
    {
    public:
        struct WxStringHash
        {
            std::size_t operator()(const wxString& v) const
            {
                return std::hash<std::wstring>{}(v.ToStdWstring());
            }
        };

        explicit ThumbnailScheduler(wxEvtHandler* eventSink,
                                    CThumbnailProcess* thumbnailProcess);

        // ── File d'attente ───────────────────────────────────────────────

        /** Recharge complète depuis la base (appelé après un scan). */
        void ReloadFromDatabase();

        /** Prioritise un fichier unique en tête de file. */
        void PrioritizeFile(const wxString& filename);

        /** Prioritise une liste de fichiers en tête de file (ordre inversé). */
        void PrioritizeFiles(const std::vector<wxString>& files);

        // ── Scheduling ──────────────────────────────────────────────────

        /**
         * À appeler depuis ProcessIdle() de CMainWindow.
         * Retourne true s'il reste du travail en cours ou en attente.
         */
        bool Tick(int nbProcesseur, int nbElementInIconeList);

        /** Appelé quand CThumbnailProcess termine un item (décrémente nbProcess). */
        void OnThumbnailDone();

        /** Vide complètement la file et remet les compteurs à zéro. */
        void Reset();

        // ── Accesseurs (lecture seule) ───────────────────────────────────
        int  GetNbProcess()     const { return nbProcess.load(); }
        int  GetThumbnailPos()  const { return thumbnailPos; }

    private:
        bool TryDispatchOne(int nbElementInIconeList);
        void ReloadIfEmpty();

        wxEvtHandler*    eventSink;        ///< CMainWindow — destinataire wxEVENT_UPDATEMESSAGE
        CThumbnailProcess* thumbnailProcess;

        mutable std::mutex photoListMutex;
        std::deque<wxString>                              photoList;
        std::unordered_set<wxString, WxStringHash>        listFile;

        std::atomic<int> nbProcess{0};
        int              thumbnailPos  = 0;
    };

} // namespace Regards::Viewer
