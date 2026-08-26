#pragma once
#include <WindowMain.h>
#include <WindowOpenGLMain.h>
#include "WindowManager.h"
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarVerticalWnd.h>
namespace Regards::Window
{

	class CControlSize
	{
	public:
		int controlWidth;
		int controlHeight;
		bool useScaleFactor = false;
	};

	class CScrollbarWnd : public CWindowMain
	{
	public:
		CScrollbarWnd(wxWindow* parent, CWindowMain* centralWindow, wxWindowID id,
			const wxString& windowName = "ScrollBar");
		CScrollbarWnd(wxWindow* parent, CWindowOpenGLMain* centralWindow, wxWindowID id,
			const wxString& windowName = "ScrollBar");
		~CScrollbarWnd() = default;

		void UpdateScreenRatio() override;

		void SetPageSize(int pageSize);
		void SetLineSize(int lineSize);

		void HideVerticalScroll();
		void HideHorizontalScroll();
		void ShowVerticalScroll();
		void ShowHorizontalScroll();

		// [CRITIQUE] GetShowingScrollV/H retournaient int alors que showV/showH sont bool
		bool GetShowingScrollV() const;
		bool GetShowingScrollH() const;

		// [QUALITE] Ajout de const sur tous les accesseurs purs
		int GetBarWidth()   const;
		int GetBarHeight()  const;

		void Resize() override;

		int GetHeight() override;
		int GetWidth()  override;

		int GetPosLargeur() const;
		int GetPosHauteur() const;

	private:
		void DefaultConstructor();

		// [IMPORTANT] Helper pour éviter la duplication des handlers d'événements
		void ForwardScrollEvent(wxEventType type, int pos);

	protected:
		void SetPosition(int posX, int posY);
		void RefreshData(wxCommandEvent& event);
		void OnLeftPosition(wxCommandEvent& event);
		void OnTopPosition(wxCommandEvent& event);
		void OnControlSize(wxCommandEvent& event);
		void OnSetPosition(wxCommandEvent& event);

		void OnMoveLeft(wxCommandEvent& event);
		void OnMoveRight(wxCommandEvent& event);
		void OnMoveTop(wxCommandEvent& event);
		void OnMoveBottom(wxCommandEvent& event);

		void OnScrollMove(wxCommandEvent& event);

		// Owned by wxWidgets window tree via windowManager — do not delete manually
		CScrollbarHorizontalWnd * scrollHorizontal;
		CScrollbarVerticalWnd * scrollVertical;

		bool showV;
		bool showH;
		// [QUALITE] _showV/_showH supprimés : membres jamais utilisés (fantômes)
		bool _useScaleFactor = false;

		int controlHeight;
		int controlWidth;



		int defaultPageSize;
		int defaultLineSize;

		// [QUALITE] loadingTimer supprimé : alloué mais jamais démarré ni géré
		CWindowManager * windowManager;
		std::unique_ptr<CWindowToAdd> centralWindow;
		CWindowToAdd * windowToaddCentral;

		bool oldshowScrollH = false;
		bool oldshowScrollV = false;
	};
}