#pragma once
#include <ScrollbarWnd.h>
#include "ThumbnailFolder.h"
#include "ThumbnailToolBar.h"
#include "ThumbnailToolBarZoom.h"
#include <WindowMain.h>
#include "ImageList.h"
#include <ExportFile.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CListPicture : public CWindowMain
		{
		public:
			CListPicture(wxWindow* parent, wxWindowID id);
			~CListPicture();
            void UpdateScreenRatio();
			void SetActifItem(const int &numItem, const bool &move);	
			int GetThumbnailHeight();
			void SetListeFile(PhotosVector * photoVector);
			void ChangeTypeAffichage(PhotosVector * photoVector, const long & typeAffichage);
			void Resize();

		private:
			
			void ThumbnailZoomOn(wxCommandEvent& event);
			void ThumbnailZoomOff(wxCommandEvent& event);
			void ThumbnailZoomPosition(wxCommandEvent& event);
			void ExportFile(wxCommandEvent& event);
			void DeleteFile(wxCommandEvent& event);
			void CopyFile(wxCommandEvent& event);
			void GeolocalizeFile(wxCommandEvent& event);
			void GenerateIndexFile(wxCommandEvent& event);
			void ChangeDateFile(wxCommandEvent& event);
			

			CScrollbarWnd * thumbscrollbar;
			CThumbnailToolBar * thumbToolbar;
			CThumbnailToolBarZoom * thumbToolbarZoom;
			CThumbnailFolder * thumbnailFolder;

			void GeolocalizeFile(const wxString & filename, const float &latitude, const float &longitude, const wxString &lat, const wxString &lng, const wxString &geoInfos);
			void ChangeDateFile(const wxString & filename, const wxDateTime &newDate, const wxString &selectDate);
			void ExportFile(const wxString & filename, CThumbnailData * data, InfoExportFile infoFile, wxString destinationFolder, int optionPicture, int qualityPicture);
			wxString GenerateFileName(const InfoExportFile & infoFile, const wxString &dateFile, const wxString &gpsFile);
			wxString CreateExportFolder(const InfoExportFile & infoFile, const wxString &folderDestination, const wxString &dateFile, const wxString &gpsFile);
			void CreateFolder(const wxString &newFolder);

			int typeAffichage;
			bool showToolbar;
		};
	}
}

