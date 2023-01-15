#pragma once
#include "SeparationBar.h"
#include "WindowMain.h"
#include "WindowOpenGLMain.h"
#include "WindowToAdd.h"

namespace Regards::Window
{
	class CPanelWithClickToolbar;

	class CWindowToAdd;


	class CWindowManager : public CWindowMain, public IMoveWindow
	{
	public:
		CWindowManager(wxWindow* parent, wxWindowID id, const CThemeSplitter& theme);
		~CWindowManager() override;

		CPanelWithClickToolbar* AddPanel(CWindowMain* window, const Pos& pos, bool fixe, int size, wxRect rect,
		                                 const wxString& panelLabel, const wxString& windowName, const bool& isVisible,
		                                 const int& idPanel, const bool& refreshButton, const bool& isTop = false);
		void AddWindow(CWindowMain* window, Pos position, bool fixe, int size, wxRect rect, int id, bool isPanel,
		               const bool& isTop = false);
		void AddWindow(CWindowOpenGLMain* window, Pos position, bool fixe, int size, wxRect rect, int id, bool isPanel,
		               const bool& isTop = false);
		void SetSeparationBarVisible(const bool& visible);
		bool GetSeparationVisibility();
		void ChangeWindow(CWindowMain* window, Pos position, bool isPanel);
		void GenerateRenderBitmap();
		void HideWindow(Pos position, const bool& refresh = true);
		void ShowWindow(Pos position, const bool& refresh = true);
		void HidePaneWindow(Pos position, const int& refresh = 1);
		void ShowPaneWindow(Pos position, const int& refresh = 1);
		int GetPaneState(Pos position);
		bool OnLButtonDown() override;
		void OnLButtonUp() override;
		void UpdateScreenRatio() override;
		bool IsWindowVisible(Pos position);
		void SetNewPosition(CSeparationBar* separationBar) override;
		void Resize() override;
		void UnInit();
		void Init();
		void ResetPosition();
		wxRect GetWindowSize(Pos position);
		bool GetWindowIsShow(Pos position);

	protected:
		vector<CWindowToAdd*> listWindow;

	private:
		void AddWindow(CWindowToAdd* windowToAdd, Pos position, bool fixe, int size, wxRect rect, int id, bool isPanel,
		               const bool& isTop);
		void OnRefreshData(wxCommandEvent& event);
		void AddDifference(const int& diffWidth, const int& diffHeight, Pos position);
		void OnResize(wxCommandEvent& event) override;
		CWindowToAdd* FindWindow(Pos position);
		void MoveTop(int difference);
		void MoveBottom(int difference);
		void MoveRight(int difference);
		void MoveLeft(int difference);

		//Init Window
		void Init_top();
		void Init_bottom();
		void Init_right();
		void Init_left();
		void Init_Central();

		void SetWindowRightSize(CWindowToAdd* right, int width, int y, int right_height);
		void SetWindowLeftSize(CWindowToAdd* left, int width, int y, int left_height);
		void SetWindowTopSize(CWindowToAdd* top, int height, int x, int top_width);
		void SetWindowBottomSize(CWindowToAdd* bottom, int height, int x, int y, int width);

		void DrawSeparationBar(const int& x, const int& y, const int& width, const int& height, const bool& horizontal);

		bool fastRender;
		bool init;
		int oldWidth;
		int oldHeight;
		bool moving;
		wxBitmap renderBitmap;
		bool showSeparationBar;
		CThemeSplitter themeSplitter;
		int separationBarSize = 0;
	};
}
