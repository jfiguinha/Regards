#pragma once
#include <Theme.h>
#include <TreeWindow.h>
#include <TreeElementControlInterface.h>
#include <wx/wxprec.h>
#include <thread>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
using namespace std;

class CTreeElementTriangle;
class CTreeElementTexte;

namespace Regards
{
	namespace Control
	{
		class CExifGpsTree : public Regards::Window::CTreeControl
		{
		public:

			CExifGpsTree(CThemeTree * theme, Regards::Window::CTreeElementControlInterface * interfaceControl);
			~CExifGpsTree(void){};
			void SetFile(const wxString & filename);
            void SetFile(const wxString & filename, const float &latitude, const float &longitude);
			wxString GetFilename();
            void UpdateScreenRatio();
            void CreateElement();
            
		private:

			void MouseOver(wxDC * dc, CPositionElement * element, const int &x, const int &y, const int& posLargeur, const int &posHauteur);
			void CreateChildTree(tree<CTreeData *>::sibling_iterator &parent);  
			void AddTreeInfos(const wxString &exifKey, const wxString &exifValue, const int &index, tree<CTreeData *>::iterator &top, tree<CTreeData *>::iterator &child);
                  

			int yPos;
			int cxMax;
			int cyMax;
			int xMaxPos;
			int xMaxPosValue;
			int xMinPos;

			wxString filename = L"";
			int widthPosition = 0;
            tree<CTreeData *>::iterator top;
		};

	}
}