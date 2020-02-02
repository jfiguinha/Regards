#pragma once
#include "SeparationBar.h"
#include "WindowMain.h"
#include "WindowOpenGLMain.h"

namespace Regards
{
	namespace Window
	{
		enum class Pos { wxCENTRAL, wxTOP, wxBOTTOM, wxLEFT, wxRIGHT};
		class CPanelWithClickToolbar;

		class CSeparationBarToAdd
		{
		public:
			wxRect rect;
			CSeparationBar * separationBar;
			int separationBarId;
			bool isHorizontal;
			int posBar;
			int size;
		};

		class CWindowToAdd
		{
		public:

			CWindowToAdd()
			{
				window = nullptr;
				windowOpengl = nullptr;
			}

			wxWindow * GetWindow()
			{
				if (window != nullptr)
					return window;
				return windowOpengl;
			}
			CMasterWindow * GetMasterWindowPt()
			{
				if (window != nullptr)
					return window;
				return windowOpengl;
			}

			CPanelWithClickToolbar * GetPanel()
			{
				if (isPanel)
				{
					if (window != nullptr)
						return (CPanelWithClickToolbar *)window;
				}
				return nullptr;
			}

			void SetWindow(CWindowMain * window)
			{
				this->window = nullptr;
				this->windowOpengl = nullptr;
				this->window = window;
			}

			void SetWindow(CWindowOpenGLMain * window)
			{
				this->window = nullptr;
				this->windowOpengl = nullptr;
				this->windowOpengl = window;
			}

			wxRect rect;
			wxRect rect_old;
			Pos position;
			int size;
			bool fixe;
			bool fixe_old;
			int id;
			bool isTop;
			bool isHide;
			bool isPanel;
			int diffWidth;
			int diffHeight;

			CSeparationBarToAdd * separationBar;

		private:
			CWindowMain * window;
			CWindowOpenGLMain * windowOpengl;
		};



		class CWindowManager : public CWindowMain, public IMoveWindow
		{
		public:
			CWindowManager(wxWindow* parent, wxWindowID id, const CThemeSplitter & theme);
			virtual ~CWindowManager();

			void AddPanel(CWindowMain * window, const Pos &pos, bool fixe, int size, wxRect rect, const wxString &panelLabel, const wxString &windowName, const bool &isVisible, const int &idPanel, const bool &refreshButton);
			void AddWindow(CWindowMain * window, Pos position, bool fixe, int size, wxRect rect, int id, bool isPanel);
			void AddWindow(CWindowOpenGLMain * window, Pos position, bool fixe, int size, wxRect rect, int id, bool isPanel);
			void SetSeparationBarVisible(const bool& visible);
			bool GetSeparationVisibility();
			void ChangeWindow(CWindowMain * window, Pos position);
			void GenerateRenderBitmap();
			void HideWindow(Pos position, const bool &refresh = true);
			void ShowWindow(Pos position, const bool &refresh = true);
			void HidePaneWindow(Pos position);
			void ShowPaneWindow(Pos position);
			bool OnLButtonDown();
			void OnLButtonUp();
			void UpdateScreenRatio();

			void SetNewPosition(CSeparationBar * separationBar);
			virtual void Resize();
			void UnInit();
			void Init();

		protected:

			vector<CWindowToAdd *> listWindow;

		private:
			void AddWindow(CWindowToAdd * windowToAdd, Pos position, bool fixe, int size, wxRect rect, int id, bool isPanel);
			void OnRefreshData(wxCommandEvent& event);
			void AddDifference(const int & diffWidth, const int &diffHeight, Pos position);
			void OnResize(wxCommandEvent& event);
			CWindowToAdd * FindWindow(Pos position);
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

			void SetWindowRightSize(CWindowToAdd * right, int width, int y, int right_height);
			void SetWindowLeftSize(CWindowToAdd * left, int width, int y, int left_height);
			void SetWindowTopSize(CWindowToAdd * top, int height, int x, int top_width);
			void SetWindowBottomSize(CWindowToAdd * bottom, int height, int x, int y, int width);

			void DrawSeparationBar(const int &x, const int &y, const int &width, const int &height, const bool &horizontal);
			
			
			bool init;
			int oldWidth;
			int oldHeight;
			bool moving;
			wxBitmap renderBitmap;
			bool showSeparationBar;
			CThemeSplitter themeSplitter;
		};
	}
}

