#pragma once
#include <InfosSeparationBar.h>
#include <FaceName.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CInfosSeparationBarFace : public CInfosSeparationBar
		{
		public:
			CInfosSeparationBarFace(const CThemeInfosSeparationBar &theme);
			virtual ~CInfosSeparationBarFace(void);

			void RenderIcone(wxDC * deviceContext, const int &posLargeur, const int &posHauteur);
			void OnClick(const int &x, const int &y);
			bool GetSelected();
			void SetNumFace(const CFaceName &faceName);
			void SetParentWindow(wxWindow * parentWindow);
			int GetNumFace();

		protected:

			wxImage bitmapCheckOn;
			wxImage bitmapCheckOff;
			wxImage bitmapEdit;
            
			wxString libelleSelectIcone;
			wxString libelleSelectAll;
			bool isSelected;
			bool isSelectIcone;
            int numFace;
			wxRect rcSelect;
			wxRect rcSelectIcone;

			int xPosEdit;
			int yPosEdit;
			int xPosText;
			int yPosText;
			wxWindow * parentWindow;
		};
	}
}

