#pragma once
#include <ConfigParam.h>
#include <ThumbnailData.h>
#include <SqlThumbnail.h>
#include "ScrollInterface.h"
#include "StatusBarInterface.h"
#include "Icone.h"
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include "WindowMain.h"
#include <wx/animate.h>

using namespace std;
using namespace Regards::Sqlite;
using namespace Regards::Window;

typedef std::vector<CThumbnailData *> ThumbnailDataVector;

#ifdef LOCALTHREAD
class MyThread;
// declare a new type of event, to be used by our MyThread class:
wxDECLARE_EVENT(wxEVT_COMMAND_MYTHREAD_COMPLETED, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_PAUSETHREAD, wxCommandEvent);
#endif

namespace Regards
{
	namespace Window
	{
		class CScrollbarHorizontalWnd;
		class CScrollbarVerticalWnd;

		class CThumbnail : public CWindowMain, public CScrollInterface
		{
		public:
			CThumbnail(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar, const CThemeThumbnail & themeThumbnail);
			virtual ~CThumbnail();
			
            void StartLoadingPicture(const int &numItem);
            void StopLoadingPicture();
			
			void EraseThumbnail();
			void ZoomOn();
			void ZoomOff();
			void ZoomPosition(const int &position);

			int GetWidth();
			int GetHeight();

			wxWindow * GetWindow(){
				return this;
			}

			int GetIconeHeight();
			int GetIconeWidth();

			wxString SetActifItem(const int &numItem);
			virtual void GetSelectItem(vector<CThumbnailData *> &vectorData);

			void SetTheme(CThemeThumbnail * theme);
            
            void UpdateScreenRatio();
#ifdef LOCALTHREAD			
			MyThread *m_pThread;
			wxCriticalSection m_pThreadCS;    // protects the m_pThread pointer
#endif			
			void BackgroundManager();
			
		protected:
		
#ifdef LOCALTHREAD		
			//----------------------------------------------------------------			
			//Thread Event
			//----------------------------------------------------------------
		    void DoStartThread();
			void DoPauseThread();
			// a resume routine would be nearly identic to DoPauseThread()
			void DoResumeThread();
			void OnThreadUpdate(wxThreadEvent&);
			void OnThreadCompletion(wxThreadEvent&);
			void OnThreadPause(wxCommandEvent& event);
#endif
			//----------------------------------------------------------------
			
            void OnLoading(wxTimerEvent& event);
			virtual void FindOtherElement(wxDC * dc, const int &x, const int &y){};

			void OpenFileViewer(const wxString &filename);
			void Update(){ this->Refresh(); };
			void TestMaxX();
			void TestMaxY();
			void SetIconeSize(const int &width, const int &height);
			virtual void CreateOrLoadStorageFile(){};
			virtual void SetNbFiles(const int &nbFiles){};
			virtual CIcone * FindElement(const int &xPos, const int &yPos);

			void Resize();

			void onIdle(wxIdleEvent& evt);
			void OnPaint(wxPaintEvent& event);
			void OnMouseMove(wxMouseEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void OnLDoubleClick(wxMouseEvent& event);
			void OnMouseWheel(wxMouseEvent& event);
			void OnKeyDown(wxKeyEvent& event);
			

			void CalculControlSize();
			void UpdateVideoIcone(const int &thumbnailPos, CRegardsBitmap * bitmap, const int &timePosition);
			void UpdateBitmapIcone(const int &thumbnailPos, CRegardsBitmap * bitmap);
			virtual void OnPictureClick(CThumbnailData * data) = 0;
			void InitScrollingPos();
			void CreateBitmapFolder();
			virtual void EraseThumbnailList();
			void RenderBitmap(wxDC * deviceContext, CIcone * pBitmapIcone);

			virtual void RenderIcone(wxDC * dc) = 0;
			virtual void UpdateScroll() = 0;
            virtual void ResizeThumbnail(){};
			static void LoadPicture(void * param);

			void KillThreadIcone(wxCommandEvent& event);
			void UpdateRenderIcone(wxCommandEvent& event);
			//------------------------------------------------------------
			//Variable
			//------------------------------------------------------------
			//vector<std::thread *> threadVector;
			int thumbnailPos = 0;
			int nbProcesseur = 0;
			int nbProcess = 0;

			//Variable
			IconeVector pIconeList;

			int xNewSize;
			int yNewSize;
			int thumbnailSizeX;
			int thumbnailSizeY;

			bool isStoragePt;
			int positionYOld;
			int positionXOld;

			int nbLigneX;
			int nbLigneY;

			//std::mutex m_lock;

			int numStartElement;
			int nbElementToShow;

			int controlWidth = 0;
			int controlHeight = 0;

			CIcone * numActif = nullptr;
			CIcone * numSelect = nullptr;

			std::thread * threadMain = nullptr;
			std::mutex muThreadMain;
			//vector<std::thread *> listThread;
			//vector<CIcone *> listSelect;

			int positionSize;
			
			bool threadDataProcess = false;
			bool allThreadEnd = true;

			//wxBitmap bitmapFolder;
			wxBitmap bitmapPhoto;
			static const int TabSize[];
			static const int Max;

			IStatusBarInterface * statusbar;
			CThemeThumbnail themeThumbnail;
            
            wxTimer * loadingTimer = nullptr;
            bool showLoadingBitmap = false;
            int stepLoading = 0;
            CIcone * loadingIcone = nullptr;
            wxAnimation * m_animation = nullptr;
		};
	}
}

