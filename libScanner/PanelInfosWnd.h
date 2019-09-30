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
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
using namespace Regards::Internet;

class CImageLoadingFormat;


namespace Regards
{
	namespace Viewer
	{
        class CInfoEffectWnd;
        class CFiltreEffectScrollWnd;
        class CThumbnailViewerEffectWnd;
        
		class CPanelInfosWnd : public CTabWindow
		{
		public:
			CPanelInfosWnd(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface);
			~CPanelInfosWnd();
			void SetFile(const wxString &filename);
			wxString GetFilename();

		protected:

			void InfosUpdate();
			void LoadInfo();
			void DisplayURL(const wxString &url);
            
			CInfosFileWnd * infosFileWnd;
			CToolbarInfos * infosToolbar;

			wxString filename;
            wxString url;
	
		};

	}
}