#pragma once
#include "ExifGpsTree.h"
#include <ScrollbarWnd.h>
#include <WindowMain.h>
#include <FileGeolocation.h>
#include <thread>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
using namespace Regards::Window;
using namespace Regards::Internet;
using namespace std;
class CTreeElementTriangle;
class CTreeElementTexte;

namespace Regards
{
	namespace Control
	{
		class CExifGpsTreeWnd : public CWindowMain
		{
		public:

			CExifGpsTreeWnd(wxWindow* parent, wxWindowID id, const CThemeTree & theme,const CThemeScrollBar & themeScroll);
            ~CExifGpsTreeWnd(void);
            void UpdateScreenRatio();
            void SetFile(const wxString & filename);
            void SetFile(const wxString & filename, const float &latitude, const float &longitude);
            int GetHeight();
            int GetWidth();
            void SaveChange();
            
		private:

            void Resize();
            
            CScrollbarWnd * InfosFileScroll = nullptr;
            CTreeWindow * treeWindow = nullptr;
            CExifGpsTree * criteriaTree = nullptr;
            CExifGpsTree * oldCriteriaTree = nullptr;
            wxString filename;
            float latitude;
            float longitude;
		};

	}
}