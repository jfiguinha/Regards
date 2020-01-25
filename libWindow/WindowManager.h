#pragma once
#include "SeparationBar.h"
#include "WindowMain.h"

namespace Regards
{
	namespace Window
	{
		enum class Pos { wxCENTRAL, wxTOP, wxBOTTOM, wxLEFT, wxRIGHT };

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
			wxRect rect;
			wxRect rect_old;
			Pos position;
			int size;
			bool fixe;
			bool fixe_old;
			int id;
			bool isHide;
			int diffWidth;
			int diffHeight;
			CWindowMain * window;
			CSeparationBarToAdd * separationBar;
		};



		class CWindowManager : public CWindowMain, public IMoveWindow
		{
		public:
			CWindowManager(wxWindow* parent, wxWindowID id, const CThemeSplitter & theme);
			virtual ~CWindowManager();

			void AddPanel(CWindowMain * window, const Pos &pos, bool fixe, int size, wxRect rect, const wxString &panelLabel, const wxString &windowName, const bool &isVisible, const int &idPanel);
			void AddWindow(CWindowMain * window, Pos position, bool fixe, int size, wxRect rect, int id = 0);
			void SetSeparationBarVisible(const bool& visible);
			bool GetSeparationVisibility();
			void UpdatePosition(CSeparationBar * separationBar) {};

			void GenerateRenderBitmap();
			void HideWindow(Pos position);
			void ShowWindow(Pos position);
			bool OnLButtonDown(CSeparationBar * separationBar);
			void OnLButtonUp(CSeparationBar * separationBar);

			void SetNewPosition(CSeparationBar * separationBar);
			virtual void Resize();
			void UnInit();
			void Init();

		protected:

			vector<CWindowToAdd *> listWindow;

		private:

			void AddDifference(const int & diffWidth, const int &diffHeight, Pos position);
			void OnResize(wxCommandEvent& event);
			CWindowToAdd * FindWindow(Pos position);
			void MoveTop(int difference);
			void MoveBottom(int difference);
			void MoveRight(int difference);
			void MoveLeft(int difference);

			void OnPaint(wxPaintEvent& event);

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

