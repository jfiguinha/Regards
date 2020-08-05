#pragma once
#ifndef __NOFACE_DETECTION__
/*
#include <ScrollbarWnd.h>
#include "ThumbnailFace.h"
#include "ThumbnailFaceToolBar.h"
#include "ThumbnailFacePertinenceToolBar.h"
*/
#include <WindowMain.h>
using namespace Regards::Window;
using namespace std;

class CFaceLoadData;

namespace Regards
{
	namespace Window
	{
		class CWindowManager;
		class CScrollbarWnd;
	}

	namespace Viewer
	{
		class CThumbnailFacePertinenceToolBar;
		class CThumbnailFace;
		class CThumbnailFaceToolBar;

		class CListFace : public CWindowMain
		{
		public:
			CListFace(wxWindow* parent, wxWindowID idCTreeWithScrollbar);
			~CListFace();
            void UpdateScreenRatio();
			void SetActifItem(const int &numItem, const bool &move);	
			int GetThumbnailHeight();

		private:
			void OnIdle(wxIdleEvent& evt);
			void ThumbnailFolderAdd(wxCommandEvent& event);
			void ThumbnailZoomOn(wxCommandEvent& event);
			void ThumbnailZoomOff(wxCommandEvent& event);
			void ThumbnailZoomPosition(wxCommandEvent& event);
			void ThumbnailRefresh(wxCommandEvent& event);
			void ThumbnailMove(wxCommandEvent& event);
			void OnFacePhotoAdd(wxCommandEvent& event);
			void OnResourceLoad(wxCommandEvent& event);
			void Resize();
			void ProcessIdle();
			bool GetProcessEnd();
			static void FacialRecognition(void * param);
			static void LoadResource(void * param);

			CWindowManager * windowManager;
			CScrollbarWnd * thumbscrollbar;
			CThumbnailFaceToolBar * thumbFaceToolbar;
			CThumbnailFacePertinenceToolBar * thumbFacePertinenceToolbar;
			CThumbnailFace * thumbnailFace;

			int nbProcessFacePhoto;
			bool isLoadingResource;
			std::thread * threadResource;
		};
	}
}

#endif