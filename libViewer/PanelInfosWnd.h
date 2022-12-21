#pragma once
#include <FileGeolocation.h>
#include "ToolbarInfos.h"
#include "ModificationManager.h"
#if wxUSE_WEBVIEW_IE
#include "wx/msw/webview_ie.h"
#endif
#if wxUSE_WEBVIEW_EDGE
#include "wx/msw/webview_edge.h"
#endif
#include <wx/webview.h>
#include <wx/animate.h>
#include "CriteriaWindow.h"
#include "PicturePanel.h"
#include <InfosFileWnd.h>
#include "FiltreEffect.h"
#include "TabWindow.h"
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

	namespace Viewer
	{
		class CPanelInfosWnd : public CTabWindow
		{
		public:
			CPanelInfosWnd(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface);
			~CPanelInfosWnd() override;
			void OnFiltreOk(const int& numFiltre);
			void OnFiltreCancel();
			void SetBitmapFile(const wxString& filename, const bool& isThumbnail);
			void SetVideoFile(const wxString& filename);
			void SetAnimationFile(const wxString& filename);
			void ShowFiltre(const wxString& title);

			CFiltreEffect* GetFilterWindow(int& numFiltre);
			wxString GetFilename();

		protected:
			void ApplyEffect(wxCommandEvent& event);
			void ShowFiltreEvent(wxCommandEvent& event);

			wxString MapsUpdate();
			void EffectUpdate();
			void HistoryUpdate();
			void VideoEffectUpdate();
			void InfosUpdate();
			void LoadInfo() override;
			void AudioVideoUpdate();
			void DisplayURL(const wxString& url);
			void HistogramUpdate();

			CInfosFileWnd* infosFileWnd;
			CInfoEffectWnd* historyEffectWnd;
			CThumbnailViewerEffectWnd* thumbnailEffectWnd;
			CFiltreEffectScrollWnd* filtreEffectWnd;
			CCriteriaWindow* criteriaTreeWnd;
			CPicturePanel* picturePanel;

			wxWebView* webBrowser = nullptr;
			CToolbarInfos* infosToolbar;
			CModificationManager* modificationManager;

			bool isThumbnail;
			bool isVideo;
			wxString filename;
			wxString url;
			bool firstTime;
			wxWindowID id_;
		};
	}
}
