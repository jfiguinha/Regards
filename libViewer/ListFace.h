#pragma once
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
			CListFace(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar);
			~CListFace();

			void PictureClick(const wxString &filepath, const int &numElement);
            void UpdateScreenRatio();
			void SetActifItem(const int &numItem, const bool &move);	
			int GetThumbnailHeight();

		private:

			void ProcessIdle();
			void OnRefreshProcess(wxCommandEvent& event);
			void OnFacePhotoAdd(wxCommandEvent& event);
			void OnAddFacePhoto(wxCommandEvent& event);
			void OnIdle(wxIdleEvent& evt);
			void ThumbnailZoomOn(wxCommandEvent& event);
			void ThumbnailZoomOff(wxCommandEvent& event);
			void ThumbnailZoomPosition(wxCommandEvent& event);
			void ThumbnailRefresh(wxCommandEvent& event);
			void ThumbnailMove(wxCommandEvent& event);
			void Resize();
			bool GetProcessEnd();
			static void FacialRecognition(void * param);

			CScrollbarWnd * thumbscrollbar;
			CThumbnailFaceToolBar * thumbFaceToolbar;
			CThumbnailFacePertinenceToolBar * thumbFacePertinenceToolbar;
			//CThumbnailFacePertinenceToolBar * thumbFacePertinenceToolbar;
			CThumbnailFace * thumbnailFace;
			bool update;
			int nbProcessFacePhoto;
			bool updateFaceList;
			CFaceLoadData * faceData;
		};
	}
}

