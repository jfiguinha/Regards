#pragma once
#ifndef __NOFACE_DETECTION__
#include <ScrollbarWnd.h>
#include "ThumbnailFace.h"
#include "ThumbnailFaceToolBar.h"
#include "ThumbnailFacePertinenceToolBar.h"
#include <WindowMain.h>
using namespace Regards::Window;
using namespace std;

class CFaceLoadData;

namespace Regards
{
	namespace Viewer
	{
		

		class CListFace : public CWindowMain
		{
		public:
			CListFace(wxWindow* parent, wxWindowID idCTreeWithScrollbar);
			~CListFace();
            void UpdateScreenRatio();
			void SetActifItem(const int &numItem, const bool &move);	
			int GetThumbnailHeight();

		private:
			void ThumbnailZoomOn(wxCommandEvent& event);
			void ThumbnailZoomOff(wxCommandEvent& event);
			void ThumbnailZoomPosition(wxCommandEvent& event);
			void ThumbnailRefresh(wxCommandEvent& event);
			void ThumbnailMove(wxCommandEvent& event);
			void Resize();

			CScrollbarWnd * thumbscrollbar;
			CThumbnailFaceToolBar * thumbFaceToolbar;
			CThumbnailFacePertinenceToolBar * thumbFacePertinenceToolbar;
			//CThumbnailFacePertinenceToolBar * thumbFacePertinenceToolbar;
			CThumbnailFace * thumbnailFace;
			bool update;
			bool updateFaceList;

		};
	}
}

#endif