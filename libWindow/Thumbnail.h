#pragma once
#include <ConfigParam.h>
#include <ThumbnailData.h>
#include <SqlThumbnail.h>

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

		class CThumbnail : public CWindowMain
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
			static void LoadPicture(void * param);

		protected:
			void ProcessThumbnail(CThumbnailData * pThumbnailData);
			virtual void EraseThumbnail(wxCommandEvent& event);
			void StartLoadingPicture(wxCommandEvent& event);
			void StopLoadingPicture(wxCommandEvent& event);
			void UpdateMessage(wxCommandEvent& event);
			void OnScrollMove(wxCommandEvent& event);
			virtual void ProcessThumbnailIdle(){};

			virtual void FindOtherElement(wxDC * dc, const int &x, const int &y){};

			void OpenFileViewer(const wxString &filename);
			void Update(){ Refresh(); };
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
			void OnKeyDown(wxKeyEvent& event);
            void OnEnterWindow(wxMouseEvent& event);
            void OnLeaveWindow(wxMouseEvent& event);
			bool GetProcessEnd();
			void OnLeftPosition(wxCommandEvent& event);
			void OnTopPosition(wxCommandEvent& event);
			void OnMoveLeft(wxCommandEvent& event);
			void OnMoveRight(wxCommandEvent& event);
			void OnMoveTop(wxCommandEvent& event);
			void OnMoveBottom(wxCommandEvent& event);

			void CalculControlSize();
			virtual void OnPictureClick(CThumbnailData * data) = 0;
			void InitScrollingPos();
			virtual void EraseThumbnailList();
			virtual void AfterSetList();
			void RenderBitmap(wxDC * deviceContext, CIcone * pBitmapIcone, const int &posLargeur, const int &posHauteur);

            void OnRefreshIcone(wxTimerEvent& event);
			virtual void RenderIcone(wxDC * dc) = 0;
			virtual void UpdateScroll() = 0;
			

			void UpdateRenderIcone(wxCommandEvent& event);
			CIcone *  FindIcone(const wxString &filename);
			//------------------------------------------------------------
			//Variable
			//------------------------------------------------------------
			void MoveTop();
			void MoveLeft();
			void MoveBottom();
			void MoveRight();

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
			bool preprocess_thumbnail = false;

			int positionSize;
			


			//wxBitmap bitmapFolder;
			wxBitmap bitmapPhoto;
			static const int TabSize[];
			static const int Max;

            wxTimer * refreshTimer;
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
            bool showLoadingBitmap;
            int stepLoading;
            CIcone * loadingIcone;
            wxAnimation * m_animation;
			bool render;
			bool check;
			bool testValidity;
            wxBitmap bitmapMemory;
            int oldPosLargeur;
            int oldPosHauteur;

			wxString selectFilename;
			wxString actifFilename;

			int posHauteur;
			int posLargeur;
			int isMoving;

		};
	}
}

