#pragma once

#include <ExportFile.h>
#include <WindowMain.h>
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
			void SetListeFile();
			void ChangeTypeAffichage(const long& typeAffichage);
			void Resize() override;

			int ImageSuivante();
			int ImagePrecedente();
			int ImageFin();
			int ImageDebut();
			wxString GetFilename(const int& numItem);

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

			CWindowManager* windowManager;
			CScrollbarWnd* thumbscrollbar;
			CThumbnailToolBar* thumbToolbar;
			CThumbnailToolBarZoom* thumbToolbarZoom;
			CThumbnailFolder* thumbnailFolder;

			void GeolocalizeFile(const wxString& filename, const float& latitude, const float& longitude,
			                     const wxString& lat, const wxString& lng, const wxString& geoInfos);
			void ChangeDateFile(const wxString& filename, const wxDateTime& newDate, const wxString& selectDate);
			void ExportFile(const wxString& filename, CThumbnailData* data, InfoExportFile infoFile,
			                wxString destinationFolder, int optionPicture, int qualityPicture);
			wxString GenerateFileName(const InfoExportFile& infoFile, const wxString& dateFile,
			                          const wxString& gpsFile);
			wxString CreateExportFolder(const InfoExportFile& infoFile, const wxString& folderDestination,
			                            const wxString& dateFile, const wxString& gpsFile);
			void CreateFolder(const wxString& newFolder);

			int typeAffichage;
		};
	}
}
