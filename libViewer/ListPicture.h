#pragma once
#include <ExportFile.h>
#include <WindowMain.h>
#include <vector>
#include <functional>
using namespace Regards::Window;

class CThumbnailData;

namespace Regards
{
	namespace Window
	{
		class CWindowManager;
		class CScrollbarWnd;
	}

	namespace Viewer
	{
		class CThumbnailToolBar;
		class CThumbnailToolBarZoom;
		class CThumbnailFolder;

		class CListPicture : public CWindowMain
		{
		public:
			CListPicture(wxWindow* parent, wxWindowID id);
			~CListPicture() override;
			void UpdateScreenRatio() override;
			int GetNumItem();
			void SetActifItem(const int& numItem, const bool& move);
			int GetThumbnailHeight();
			void SetListeFile(const int& typeAffichage, const bool& isDeleteFolder, const bool& isSqlUpdate);
			void ChangeTypeAffichage(const long& typeAffichage);
			void Resize() override;

			int ImageSuivante();
			int ImagePrecedente();
			int ImageFin();
			int ImageDebut();
			wxString GetFilename(const int& numItem);

			CThumbnailFolder* GetPtThumbnailFolder()
			{
				return thumbnailFolder;
			}


		private:
			void ThumbnailZoomOn(wxCommandEvent& event);
			void ThumbnailZoomOff(wxCommandEvent& event);
			void ThumbnailZoomPosition(wxCommandEvent& event);
			void ExportFileCmd(wxCommandEvent& event);
			void DeleteFile(wxCommandEvent& event);
			void CopyFile(wxCommandEvent& event);
			void GeolocalizeFileCmd(wxCommandEvent& event);
			void GenerateIndexFile(wxCommandEvent& event);
			void ChangeDateFileCmd(wxCommandEvent& event);


			void ExportFile(const wxString& filename, const int& numPhotoId, const InfoExportFile& infoFile,
				const wxString& destinationFolder, int optionPicture, int qualityPicture);
			wxString GenerateFileName(const InfoExportFile& infoFile, const wxString& dateFile,
				const wxString& gpsFile);
			wxString CreateExportFolder(const InfoExportFile& infoFile, const wxString& folderDestination,
				const wxString& dateFile, const wxString& gpsFile);
			void CreateFolder(const wxString& newFolder);

			// --- Helpers de factorisation (naming/export) ---
			// Construit la liste des jetons de date (année / mois / jour) selon dateInfoSelection (0..3).
			std::vector<wxString> BuildDateTokens(const int& dateInfoSelection, const wxString& dateFile);
			// Construit la liste des jetons geo (pays / ville / lieu) selon geoInfoSelection (0..3),
			// ou le libellé "non geolocalise" si applicable.
			std::vector<wxString> BuildGeoTokens(const int& geoInfoSelection, const wxString& gpsFile);
			// Concatene les jetons dans "result" avec le separateur donne.
			// - createFolder == false (nom de fichier) : le tout premier jeton du premier groupe traite
			//   n'a pas de separateur en tete (comportement d'origine conserve tel quel).
			// - createFolder == true (arborescence de dossiers) : chaque jeton est prefixe du separateur
			//   puis CreateFolder() est appele progressivement (comportement d'origine conserve tel quel).
			void AppendTokens(wxString& result, const std::vector<wxString>& tokens, const wxString& separator,
				bool isFirstGroup, bool createFolder);

			// --- Helpers de factorisation (commandes sur la selection) ---
			// Recupere les fichiers selectionnes ; si la selection est vide, affiche le message
			// "NoItemSelected" / noItemInfoKey et retourne false.
			bool GetSelectedItems(std::vector<wxString>& listItem, const wxString& noItemInfoKey);
			// Factorise la boucle "wxProgressDialog + wxProgressDialog::Update" commune a toutes
			// les commandes qui traitent la selection courante.
			void RunWithProgressDialog(const wxString& caption, const wxString& text,
				const std::vector<wxString>& listItem,
				const std::function<void(const wxString&, int)>& action);

			int typeAffichage;
			CWindowManager* windowManager;
			CScrollbarWnd* thumbscrollbar;
			CThumbnailToolBar* thumbToolbar;
			CThumbnailToolBarZoom* thumbToolbarZoom;
			CThumbnailFolder* thumbnailFolder;
		};
	}
}