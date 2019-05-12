#pragma once
#include <ScrollbarWnd.h>
#include "ThumbnailFolder.h"
#include "ThumbnailToolBar.h"
#include "ThumbnailToolBarZoom.h"
#include <WindowMain.h>
#include "ImageList.h"
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CListPicture : public CWindowMain
		{
		public:
			CListPicture(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar);
			~CListPicture();

			void PictureClick(const wxString &filepath, const int &numElement);
            void UpdateScreenRatio();
			void SetActifItem(const int &numItem, const bool &move);	
			int GetThumbnailHeight();
			void SetListeFile(PhotosVector * photoVector);
			void ChangeTypeAffichage(PhotosVector * photoVector, const long & typeAffichage);

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
			void Resize();

			CScrollbarWnd * thumbscrollbar;
			CThumbnailToolBar * thumbToolbar;
			CThumbnailToolBarZoom * thumbToolbarZoom;
			CThumbnailFolder * thumbnailFolder;
			
			int typeAffichage;
			bool showToolbar;
		};
	}
}

