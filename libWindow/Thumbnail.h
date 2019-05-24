#pragma once
#include <ConfigParam.h>
#include <ThumbnailData.h>
#include <SqlThumbnail.h>
#include "ScrollInterface.h"
#include "IconeList.h"
#include "WindowMain.h"
#include <wx/animate.h>
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;
using namespace std;
using namespace Regards::Sqlite;
using namespace Regards::Window;

//typedef std::vector<CThumbnailData *> ThumbnailDataVector;

namespace Regards
{
	namespace Window
	{
		class CScrollbarHorizontalWnd;
		class CScrollbarVerticalWnd;

		class CThumbnail : public CWindowMain, public CScrollInterface
		{
		public:
			CThumbnail(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity);
			virtual ~CThumbnail();
			
			
			void ZoomOn();
			void ZoomOff();
			void ZoomPosition(const int &position);
			void SetCheck(const bool &check);
			virtual int GetWidth();
			virtual int GetHeight();

			wxWindow * GetWindow(){
				return this;
			}

			int GetIconeHeight();
			int GetIconeWidth();

			virtual void SetActifItem(const int &numItem, const bool &move);
			virtual void GetSelectItem(vector<CThumbnailData *> &vectorData);

			void SetTheme(CThemeThumbnail * theme);
           
            void UpdateScreenRatio();
			wxString GetWaitingMessage();

		protected:
			void EraseThumbnail(wxCommandEvent& event);
			void StartLoadingPicture(wxCommandEvent& event);
			void StopLoadingPicture(wxCommandEvent& event);
			void UpdateMessage(wxCommandEvent& event);
            void OnLoading(wxTimerEvent& event);
            void OnRefreshIcone(wxTimerEvent& event);
			virtual void ProcessThumbnailIdle(){};

			virtual void FindOtherElement(wxDC * dc, const int &x, const int &y){};

			void OpenFileViewer(const wxString &filename);
			void Update(){ this->FastRefresh(this); };
			void TestMaxX();
			void TestMaxY();
			void SetIconeSize(const int &width, const int &height);
			virtual void CreateOrLoadStorageFile(){};
			virtual void SetNbFiles(const int &nbFiles){};
			virtual CIcone * FindElement(const int &xPos, const int &yPos);
			virtual void ResizeThumbnail(){};
			void Resize();
			virtual void ProcessIdle();
			void OnIdle(wxIdleEvent& evt);
			void OnPaint(wxPaintEvent& event);
			void OnMouseMove(wxMouseEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void OnLDoubleClick(wxMouseEvent& event);
			void OnMouseWheel(wxMouseEvent& event);
            void OnEnterWindow(wxMouseEvent& event);
            void OnLeaveWindow(wxMouseEvent& event);
			void OnKeyDown(wxKeyEvent& event);
			bool GetProcessEnd();

			void CalculControlSize();
			virtual void OnPictureClick(CThumbnailData * data) = 0;
			void InitScrollingPos();
			virtual void EraseThumbnailList();
			void RenderBitmap(wxDC * deviceContext, CIcone * pBitmapIcone, const int &posLargeur, const int &posHauteur);

			virtual void RenderIcone(wxDC * dc) = 0;
			virtual void UpdateScroll() = 0;
			static void LoadPicture(void * param);

			void UpdateRenderIcone(wxCommandEvent& event);
			CIcone *  FindIcone(const wxString &filename);
			//------------------------------------------------------------
			//Variable
			//------------------------------------------------------------


			//Variable
            CIconeList * iconeList;
			//IconeVector pIconeList;

			int xNewSize;
			int yNewSize;
			int thumbnailSizeX;
			int thumbnailSizeY;

			bool isStoragePt;
			int positionYOld;
			int positionXOld;

			int nbLigneX;
			int nbLigneY;

			//mutex m_lock;

			int numStartElement;
			int nbElementToShow;


			int positionSize;
			


			//wxBitmap bitmapFolder;
			wxBitmap bitmapPhoto;
			static const int TabSize[];
			static const int Max;


			CThemeThumbnail themeThumbnail;

			int controlWidth;
			int controlHeight;
			int thumbnailPos;
			int nbProcess;
            std::mutex muActif;
            std::mutex muSelect;
            std::mutex muLoadIcone;
			CIcone * numActif;
			CIcone * numSelect;
			bool threadDataProcess;
			bool allThreadEnd;
            wxTimer * loadingTimer;
            wxTimer * refreshTimer;
            bool showLoadingBitmap;
            int stepLoading;
            CIcone * loadingIcone;
            wxAnimation * m_animation;
			bool render;
			bool check;
			bool testValidity;
            wxBitmap bitmapMemory;
            bool bufferUpdate;
            int oldPosLargeur;
            int oldPosHauteur;
		};
	}
}

