#pragma once
#include "IMoveWindow.h"
#include "SeparationBar.h"
#include "WindowMain.h"

namespace Regards
{
	namespace Window
	{
		class CSplitter : public CWindowMain, public IMoveWindow
		{
		public:
			CSplitter(wxWindow* parent, wxWindowID id, const CThemeSplitter & theme, const bool &horizontal = false);
			~CSplitter();
			void SetWindow(wxWindow * window1, wxWindow * window2);
			void SetWindow1FixPosition(const bool &fixPosition, const int &windowSize);
			void SetWindow2FixPosition(const bool &fixPosition, const int &windowSize);
			bool IsAllClose();
			void SetFixPosition(const bool &value);
			void SetHorizontal(const bool &horizontal);
			void SetPosition(const int &pos);
			int GetPosition();
			void SetSeparationBarVisible(const bool& visible);
			bool GetSeparationVisibility();
			void UpdatePosition(){};

			virtual void OnPaint(wxPaintEvent& event);

			void GenerateHorizontalRenderBitmap();
			void GenerateVerticalRenderBitmap();

			bool OnLButtonDown();
			void OnLButtonUp();

			void SetNewPosition();
			void Resize();
			void Resize(wxWindow * window);

			int posBar = 0;
			bool horizontal;
			int oldWidth = 0;
			int oldHeight = 0;
			wxWindow * window1 = nullptr;
			wxWindow * window2 = nullptr;

		private:

			void ResizeVertical();
			void ResizeHorizontal();
			void DrawSeparationBar(const int &x, const int &y, const int &width, const int &height);

			bool fixWindow1Size = false;
			int window1Size = 0;
			bool fixWindow2Size = false;
			int window2Size = 0;
			bool fixPosition;
			bool windowResize = false;
			bool fastRender = true;
			wxBitmap renderBitmap;
			CSeparationBar * separationBar = nullptr;
			int taille;
			int posWindow;
			int diff;
			bool moving = false;
			int windowMinimalSize = 100;
			bool showSeparationBar = true;
			CThemeSplitter themeSplitter;
		};
	}
}

