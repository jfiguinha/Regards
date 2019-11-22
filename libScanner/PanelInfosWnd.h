#pragma once
#include <FileGeolocation.h>
#include <RegardsBitmap.h>
#include "ToolbarInfos.h"
#include <wx/webview.h>
#include <ScrollbarWnd.h>
#include <wx/animate.h>
#include <CriteriaTreeWnd.h>
#include <FilterWindowParam.h>
#include <InfosFileWnd.h>
#include "TabWindow.h"
#include "FiltreEffect.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
using namespace Regards::Internet;

class CImageLoadingFormat;


namespace Regards
{
	namespace Control
	{
		class CInfoEffectWnd;
		class CFiltreEffectScrollWnd;
		class CThumbnailViewerEffectWnd;
	}

	namespace Scanner
	{
		class COcrWnd;

		class CPanelInfosWnd : public CTabWindow
		{
		public:
			CPanelInfosWnd(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface);
			~CPanelInfosWnd();
			void OnFiltreOk(const int &numFiltre);
			void OnFiltreCancel();
			void SetFile(wxString filename);
			wxString GetFilename();
			
			void ShowFiltre(const wxString &title);
			CFiltreEffect * GetFilterWindow(int &numFiltre);

		protected:
			void ApplyEffect(wxCommandEvent& event);
			void ShowFiltreEvent(wxCommandEvent& event);
			void InfosUpdate();
			void LoadInfo();
			void DisplayURL(const wxString &url);
			void EffectUpdate();
			void HistoryUpdate();

			CInfoEffectWnd * historyEffectWnd;
			CThumbnailViewerEffectWnd * thumbnailEffectWnd;
			CFiltreEffectScrollWnd * filtreEffectWnd;

			CInfosFileWnd * infosFileWnd;
			CToolbarInfos * infosToolbar;
			COcrWnd * ocrWnd;
			wxString _filename;
            wxString url;
			wxWebView * webBrowser = nullptr;
		};

	}
}