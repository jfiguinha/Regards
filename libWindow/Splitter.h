#pragma once
#include "IMoveWindow.h"
#include "WindowMain.h"

namespace Regards::Window
{
	class CSplitter : public CWindowMain, public IMoveWindow
	{
	public:
		CSplitter(wxWindow* parent, wxWindowID id, const CThemeSplitter& theme, const bool& horizontal = false);
		~CSplitter() override;
		void SetWindow(wxWindow* window1, wxWindow* window2);
		void SetWindow1FixPosition(const bool& fixPosition, const int& windowSize);
		void SetWindow2FixPosition(const bool& fixPosition, const int& windowSize);
		bool IsAllClose();
		void SetFixPosition(const bool& value);
		virtual void SetHorizontal(const bool& horizontal);
		void SetPosition(const int& pos);
		int GetPosition();
		void SetSeparationBarVisible(const bool& visible);
		bool GetSeparationVisibility();

		void UpdateScreenRatio() override = 0;
		//virtual void OnPaint(wxPaintEvent& event);

		void GenerateHorizontalRenderBitmap();
		void GenerateVerticalRenderBitmap();

		bool OnLButtonDown() override;
		void OnLButtonUp() override;

		void SetNewPosition(CSeparationBar* separationBar) override;
		void Resize() override;
		void Resize(wxWindow* window);

	protected:
		virtual void ResizeEvent();

		int posBar;
		bool horizontal;
		int oldWidth;
		int oldHeight;
		wxWindow* window1;
		wxWindow* window2;
		CSeparationBar* separationBar;

	private:
		void ResizeVertical();
		void ResizeHorizontal();
		void DrawSeparationBar(const int& x, const int& y, const int& width, const int& height);


		bool fixPosition;

		wxBitmap renderBitmap;

		int taille;
		int posWindow;
		int diff;

		bool fixWindow1Size;
		int window1Size;
		bool fixWindow2Size;
		int window2Size;
		bool windowResize;
		bool fastRender;
		bool moving;
		int windowMinimalSize;
		bool showSeparationBar;
		CThemeSplitter themeSplitter;
	};
}
