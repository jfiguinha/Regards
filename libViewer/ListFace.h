#pragma once
#ifndef __NOFACE_DETECTION__
/*
#include <ScrollbarWnd.h>
#include "ThumbnailFace.h"
#include "ThumbnailFaceToolBar.h"
#include "ThumbnailFacePertinenceToolBar.h"
*/
#include <TitleBarInterface.h>
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
		class CTitleBar;
	}

	namespace Viewer
	{
		class CThumbnailFacePertinenceToolBar;
		class CThumbnailFace;
		class CThumbnailFaceToolBar;

		class CListFace : public CWindowMain, public CTitleBarInterface
		{
		public:
			CListFace(wxWindow* parent, wxWindowID idCTreeWithScrollbar);
			~CListFace() override;
			void UpdateScreenRatio() override;
			void SetActifItem(const int& numItem, const bool& move);
			int GetThumbnailHeight();
			void Resize() override;
			void ClosePane() override;
			void RefreshPane() override;
			void FacialRecognitionReload();
			int ImageSuivante();
			wxString GetFilename(const int& numItem);
			int ImagePrecedente();
			int GetNumItem();
			vector<int> GetFaceSelectID();
			CThumbnailFace* GetThumbnailFace();

		private:
			static void FacialDetectionRecognition(void* param);

			void OnIdle(wxIdleEvent& evt) override;
			void ThumbnailDatabaseRefresh(wxCommandEvent& event);
			void ThumbnailFolderAdd(wxCommandEvent& event);

			void ThumbnailZoomOn(wxCommandEvent& event);
			void ThumbnailZoomOff(wxCommandEvent& event);
			void ThumbnailZoomPosition(wxCommandEvent& event);
			void ThumbnailRefresh(wxCommandEvent& event);
			void ThumbnailMove(wxCommandEvent& event);
			void OnFacePhotoAdd(wxCommandEvent& event);
			void OnResourceLoad(wxCommandEvent& event);
			void ProcessIdle() override;
			bool GetProcessEnd() override;
			static void FindFaceCompatible(const vector<int>& listFace);
			static void FacialRecognition(void* param);
			static void LoadResource(void* param);
			void OnFaceVideoAdd(wxCommandEvent& event);

			CWindowManager* windowManager = nullptr;
			CScrollbarWnd* thumbscrollbar = nullptr;
			CThumbnailFaceToolBar* thumbFaceToolbar = nullptr;
			CThumbnailFacePertinenceToolBar* thumbFacePertinenceToolbar = nullptr;
			CThumbnailFace* thumbnailFace = nullptr;
			int nbProcessFacePhoto = 0;
			bool isLoadingResource;
			int nbProcessFaceRecognition = 0;
			bool resourceLoaded = false;
			bool cleanDatabase = false;
			//std::thread * threadResource = nullptr;
		};
	}
}

#endif
