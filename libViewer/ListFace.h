#pragma once
#include <ScrollbarWnd.h>
#include "ThumbnailFace.h"
#include "ThumbnailFaceToolBar.h"
#include "ThumbnailFacePertinenceToolBar.h"
#include <WindowMain.h>
using namespace Regards::Window;
using namespace std;

namespace Regards
{
	namespace Viewer
	{
		class CListFace : public CWindowMain
		{
		public:
			CListFace(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar);
			~CListFace();

			void PictureClick(const wxString &filepath, const int &numElement);
            void UpdateScreenRatio();
			void SetActifItem(const int &numItem, const bool &move);	
			int GetThumbnailHeight();

		private:

			void ThumbnailZoomOn(wxCommandEvent& event);
			void ThumbnailZoomOff(wxCommandEvent& event);
			void ThumbnailZoomPosition(wxCommandEvent& event);
			void ThumbnailUpdate(wxCommandEvent& event);
			void ThumbnailAdd(wxCommandEvent& event);
			void ThumbnailRemove(wxCommandEvent& event);
			void ThumbnailMove(wxCommandEvent& event);
			void Resize();
			void SetListeFile(wxCommandEvent& event);
			CScrollbarWnd * thumbscrollbar;
			CThumbnailFaceToolBar * thumbFaceToolbar;
			CThumbnailFacePertinenceToolBar * thumbFacePertinenceToolbar;
			//CThumbnailFacePertinenceToolBar * thumbFacePertinenceToolbar;
			CThumbnailFace * thumbnailFace;
			bool update;
		};
	}
}

