#pragma once
#include <string>
#include <ScrollbarWnd.h>
#include "thumbnailCriteria.h"
#include "ThumbnailToolBar.h"
#include "ThumbnailToolBarZoom.h"
#include <WindowMain.h>
using namespace Regards::Window;
using namespace std;

namespace Regards
{
	namespace Explorer
	{
		class CListPicture : public CWindowMain
		{
		public:
			CListPicture(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar);
			~CListPicture();

			void PictureClick(const wxString &filepath, const int &numElement);

            void UpdateScreenRatio();
            
			void ThumbnailZoomOn();
			void ThumbnailZoomOff();
			void ThumbnailZoomPosition(const int &position);

			void SetNumElement(const int &numElement);
			int GetNbElement();

			void SetTypeOrderPhoto(const int &typeOrder);
			void RefreshList();
            void UpdateList();
            void RefreshThumbnail();

			void ExportFile();
			void DeleteFile();
			void CopyFile();
            
            void StopLoadingPicture();
            void GeolocalizeFile();
            void ChangeDateFile();
            
		private:

            //void RefreshThumbnail(wxCommandEvent& event);
			void RefreshViewer(wxCommandEvent& event);
			void Resize();

			CScrollbarWnd * thumbscrollbar = nullptr;
			CThumbnailToolBar * thumbToolbar = nullptr;
			CThumbnailToolBarZoom * thumbToolbarZoom = nullptr;
			CThumbnailCriteria * thumbnailWindow = nullptr;

			int typeAffichage = SHOW_ALL;
			bool update = true;
		};
	}
}

