#pragma once

namespace Regards::Window
{
	class CSeparationBar;
	class CMasterWindow;
	class CPanelWithClickToolbar;
	class CWindowMain;
	class CWindowOpenGLMain;

	enum class Pos { wxCENTRAL, wxTOP, wxBOTTOM, wxLEFT, wxRIGHT };

	class CSeparationBarToAdd
	{
	public:
		wxRect rect;
		CSeparationBar* separationBar;
		int separationBarId;
		bool isHorizontal;
		int posBar;
		int size;
	};

	class CWindowToAdd
	{
	public:
		CWindowToAdd();
		wxWindow* GetWindow();

		CMasterWindow* GetMasterWindowPt();

		CPanelWithClickToolbar* GetPanel();

		void SetWindow(CWindowMain* window, bool isPanel);

		void SetWindow(CWindowOpenGLMain* window, bool isPanel);

		wxSize GetSize();

		wxRect rect;
		wxRect rect_old;
		Pos position;
		int size;
		int size_old;
		bool fixe;
		bool fixe_old;
		int id;
		bool isTop;
		bool isHide;
		bool isPanel;
		int diffWidth = 0;
		int diffHeight = 0;

		CSeparationBarToAdd* separationBar;

	private:
		CWindowMain* window;
		CWindowOpenGLMain* windowOpengl;
	};
}
