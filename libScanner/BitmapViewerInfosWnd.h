#pragma once
#include <Splitter.h>
#include <PanelWithClickToolbar.h>
#include <ToolbarInterface.h>
#include "PanelInfosWnd.h"
using namespace Regards::Window;

class CImageLoadingFormat;
class CScannerFrame;

namespace Regards
{
	namespace Scanner
	{
		class CViewerPDF;

		class CBitmapViewerInfosWnd :public CSplitter
		{
		public:
			CBitmapViewerInfosWnd(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface, CScannerFrame * frame, const CThemeSplitter & theme, const bool &horizontal = true);
			~CBitmapViewerInfosWnd();

			void ShowInfos();
			bool IsPanelInfosVisible();
			void ResizeWindow();
            void UpdateScreenRatio();

			int GetAnimationPosition();
			void LoadFile(const wxString &filename);
			CImageLoadingFormat * GetImage();
			wxString GetFilename();
            

		protected:

			void OnResize(wxCommandEvent& event);
			void RedrawBarPos();

			CPanelWithClickToolbar * paneInfos;
			CPanelInfosWnd * panelInfosWindow;
			CViewerPDF * previewWindow;

			int posBarInfos;
            bool clickToobarShow;
            bool paneInfosShow;
			wxString filename;
		};

	}
}