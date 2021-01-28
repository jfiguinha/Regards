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
class CRegardsBitmap;
class CFaceLoadData;
class CFaceDetectorElement;
class CThreadFace;

namespace Regards
{
	namespace OpenCV
	{
		class CFaceDetector;
	}
}

using namespace Regards::OpenCV;

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
			~CListFace();
            void UpdateScreenRatio();
			void SetActifItem(const int &numItem, const bool &move);	
			int GetThumbnailHeight();
			void Resize();
			void ForceRefresh();
			void ClosePane();
			void RefreshPane();
			void FacialRecognitionReload();

		private:
			void SetFreeFaceDetector(const int & numElement);
			CFaceDetector * GetFreeFaceDetector(int & numElement);
			static void FacialDetectionRecognition(void * param);
			static int DetectFaceOnBitmap(CRegardsBitmap * pictureData, CThreadFace * path);
			void OnIdle(wxIdleEvent& evt);
			void ThumbnailFolderAdd(wxCommandEvent& event);
			void ThumbnailZoomOn(wxCommandEvent& event);
			void ThumbnailZoomOff(wxCommandEvent& event);
			void ThumbnailZoomPosition(wxCommandEvent& event);
			void ThumbnailRefresh(wxCommandEvent& event);
			void ThumbnailMove(wxCommandEvent& event);
			void OnFacePhotoAdd(wxCommandEvent& event);
			void OnResourceLoad(wxCommandEvent& event);
			void OnFaceVideoAdd(wxCommandEvent& event);
			void ProcessIdle();
			bool GetProcessEnd();
			static void FindFaceCompatible(const vector<int> & listFace);
			static void FacialRecognition(void * param);
			static int DetectFaceOnBitmap(CRegardsBitmap * pictureData, CThreadFace * path, const int &position);
			static void LoadResource(void * param);

			CWindowManager * windowManager;
			CScrollbarWnd * thumbscrollbar;
			CThumbnailFaceToolBar * thumbFaceToolbar;
			CThumbnailFacePertinenceToolBar * thumbFacePertinenceToolbar;
			CThumbnailFace * thumbnailFace;

			int nbProcessFacePhoto;
			bool isLoadingResource;
			int nbProcessFaceRecognition;
			//std::thread * threadResource;
			std::thread * threadFacialRecognition;
			vector<CFaceDetectorElement *> listFaceDetector;
			bool isLoad = false;
		};
	}
}

#endif