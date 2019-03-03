#pragma once
#include <InfosSeparationBar.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CInfosSeparationBarExplorer : public CInfosSeparationBar
		{
		public:
			CInfosSeparationBarExplorer(const CThemeInfosSeparationBar &theme);
			virtual ~CInfosSeparationBarExplorer(void);

			void RenderIcone(wxDC * deviceContext, const int &posLargeur, const int &posHauteur);
			void OnClick(const int &x, const int &y);
			bool GetSelected();

		protected:

            wxImage CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vector);
            
			wxImage bitmapCheckOn;
			wxImage bitmapCheckOff;
            
            wxString checkOnVector;
            wxString checkOffVector;
            
			wxString libelleSelectAll;
			bool isSelected;
            wxRect rcSelect;
		};
	}
}

