#pragma once
#include <ThumbnailData.h>
#include <SqlThumbnail.h>
#include "IconeList.h"
#include <wx/activityindicator.h>
#include "WindowMain.h"
#include <wx/animate.h>
#include <queue>
#include "ThreadLoadingBitmap.h"
using namespace std;
using namespace Regards::Sqlite;
using namespace Regards::Window;

class CListToClean;

namespace Regards::Window
{
	class CScrollbarHorizontalWnd;
	class CScrollbarVerticalWnd;


	class CThumbnail : public CWindowMain
	{
	public:
		CThumbnail(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail, const bool& testValidity);
		~CThumbnail() override;

		wxString GetFilename(const int& numItem);
		int ImageSuivante();
		int ImagePrecedente();
		int ImageFin();
		int ImageDebut();
		int GetNbElement();
		int GetNumPhotoId(const int& numItem);
		int GetNumItem();

		void UpdateRenderIcone(CThreadLoadingBitmap* threadLoadingBitmap);

		int GetHauteur()
		{
			return posHauteur;
		}

		int GetLargeur()
		{
			return posLargeur;
		}

		void ZoomOn();
		void ZoomOff();
		void ZoomPosition(const int& position);
		void SetCheck(const bool& check);
		int GetWidth() override;
		int GetHeight() override;

		wxWindow* GetWindow()
		{
			return this;
		}

		int GetIconeHeight();
		int GetIconeWidth();

		virtual void SetActifItem(const int& idPhoto, const bool& move);
		virtual void GetSelectItem(vector<CThumbnailData*>& vectorData);

		void SetTheme(CThemeThumbnail* theme);

		void UpdateScreenRatio() override;

		void Resize() override;
		static bool ItemCompFonct(int xPos, int yPos, CIcone *  icone, CWindowMain* parent);
		static bool ItemCompFonctPhotoId(int xPos, int yPos, CIcone *  icone, CWindowMain* parent);
		void ChangeTabValue(const vector<int>& TabNewSize, const int& positionSize);
		int GetTabValue();
        virtual void ProcessVideo() {};
		
	protected:

		void GenerateCleanupListFile(std::vector<CIcone*> & pIconeListToClean);
		//void ProcessIdle() {} override;
		void PaintNow();
		void Render(wxDC& dc);
		void RefreshIcone(const int& idPhoto);
		CIcone *  GetIconeById(const int& idPhoto);
		int GetNumItemById(const int& idPhoto);
		void EraseThumbnailList(CIconeList* iconeListLocal);
		void EraseIconeList(std::vector<CIcone*> pIconeListToClean);
		int GetNbIconSelected();

		virtual void EraseThumbnail(wxCommandEvent& event);
		void StartLoadingPicture(wxCommandEvent& event);
		void StopLoadingPicture(wxCommandEvent& event);

		void OnScrollMove(wxCommandEvent& event);
		void OnKeyUp(wxKeyEvent& event);
		void TestMaxX();
		void TestMaxY();

		virtual void DeleteIcone(CIcone *  numSelect)
		{
		};

		virtual void OnMouseRelease(const int& x, const int& y)
		{
		};

		virtual void FindOtherElement(wxDC* dc, const int& x, const int& y)
		{
		};

		void OpenFileViewer(const wxString& filename);
		void SetIconeSize(const int& width, const int& height);

		virtual void CreateOrLoadStorageFile()
		{
		};

		virtual CIcone *  FindElement(const int& xPos, const int& yPos);

		virtual void ResizeThumbnail()
		{
		};

		void OnIdle(wxIdleEvent& evt) override;
		void on_paint(wxPaintEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnLButtonDown(wxMouseEvent& event);
		void OnLButtonUp(wxMouseEvent& event);
		void OnLDoubleClick(wxMouseEvent& event);
		void OnMouseWheel(wxMouseEvent& event);
		void OnKeyDown(wxKeyEvent& event);
		void OnEnterWindow(wxMouseEvent& event);
		void OnLeaveWindow(wxMouseEvent& event);
		bool GetProcessEnd() override;
		void OnLeftPosition(wxCommandEvent& event);
		void OnTopPosition(wxCommandEvent& event);
		void OnRefreshThumbnail(wxCommandEvent& event);

		void CalculControlSize();
		virtual void OnPictureClick(CThumbnailData* data) = 0;
		void InitScrollingPos();
		virtual void AfterSetList();
		void RenderBitmap(wxDC* deviceContext, CIcone *  pBitmapIcone, const int& posLargeur, const int& posHauteur);
		void OnAnimation(wxTimerEvent& event);
		void OnRefreshIconeSelect(wxTimerEvent& event);
		void OnRefreshIconeActif(wxTimerEvent& event);
		void OnTimerClick(wxTimerEvent& event);

		void OnTimerMove(wxTimerEvent& event);
		virtual void RenderIcone(wxDC* dc) = 0;
		virtual void UpdateScroll() = 0;
		void RefreshIconeVideo(const int& idPhoto);
		static bool ItemFilenameCompFonct(wxString filename, CIcone *  icone) /* Définit une fonction. */;

		void ExecuteTimer(const int& numId, wxTimer* refresh);
		
		void update_render_icone(wxCommandEvent& event);



		CIcone *  GetIconeByPath(const wxString& filepath);
		static bool ItemCompFonctPath(wxString filepath, CIcone *  icone);

		//------------------------------------------------------------
		//Variable
		//------------------------------------------------------------
		void MoveTop();
		void MoveLeft();
		void MoveBottom();
		void MoveRight();

		bool isMouseOnWindow = false;
		bool enableTimer = true;
		int nbElement = 0;
		int nbPhotoElement = 0;
		//Variable
		//std::mutex lockIconeList;
		CIconeList* iconeList;

		bool processThumbnailVideo = false;
		wxString videoFilename = "";
		int xNewSize;
		int yNewSize;
		int thumbnailSizeX;
		int thumbnailSizeY;

		bool isStoragePt;
		int nbLigneX;
		int nbLigneY;

		//mutex m_lock;
		bool mouseClickBlock = false;
		bool mouseClickMove = false;

		int numStartElement;
		int nbElementToShow;
		bool preprocess_thumbnail = false;

		int positionSize;
		bool controlKeyPush = false;


		//wxBitmap bitmapFolder;
		wxBitmap bitmapPhoto;
		vector<int> TabSize;
		int Max;

		wxTimer* refreshMouseMove;
		wxTimer* refreshActifTimer;
		wxTimer* refreshSelectTimer;
		wxTimer* timeClick;
		CThemeThumbnail themeThumbnail;

		int controlWidth;
		int controlHeight;
		int thumbnailPos;


		int timeActif = 0;
		int timeSelect = 0;

		wxActivityIndicator* m_waitingAnimation;
		bool animationStart = false;
		int numActifPhotoId = -1;
		int numSelectPhotoId = -1;
		int numloadingIconePhotoId = -1;

		bool threadDataProcess;
		bool allThreadEnd;
		bool showLoadingBitmap;

		int stepLoading;

		wxAnimation* m_animation;
		wxTimer* timerAnimation;
		bool render;
		bool check;
		bool testValidity;
		wxBitmap bitmapMemory;
		wxBitmap bitmapIconDrag;
		int xPosDrag;
		int yPosDrag;
		int nbElementChecked = 0;
		int oldPosLargeur;
		int oldPosHauteur;

		int posHauteur;
		int posLargeur;
		int isMoving;
		int idWindowToRefresh = 0;
		bool flipVertical = false;
		bool flipHorizontal = false;
		bool isMovingScroll = false;
		int numOldItem = -1;
		bool moveOnPaint = true;
		bool enableDragAndDrop = false;
		int nbElementInIconeList = 0;
		bool firstRefresh = true;

		bool needToRefresh = false;
        bool stopToGetNbElement = false;
		std::mutex muEraseList;
		std::vector<CListToClean *> listToErrase;
		std::vector<CIcone *> pIconeList;
		wxWindowID localid;
		
	};
}
