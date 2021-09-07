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
			/**
			 * \brief 
			 * \param theme 
			 */
			explicit CInfosSeparationBarFace(const CThemeInfosSeparationBar& theme);
			~CInfosSeparationBarFace(void) override;

			void RenderIcone(wxDC* deviceContext, const int& posLargeur, const int& posHauteur) override;
			void OnClick(const int& x, const int& y) override;
			bool GetSelected();
			void SetNumFace(const CFaceName& faceName);
			void SetParentWindow(wxWindow* parentWindow);
			int GetNumFace();

		protected:
			wxImage bitmapCheckOn;
			wxImage bitmapCheckOff;
			wxImage bitmapDelete;
			wxImage bitmapEdit;
			
			wxString libelleDelete;
			wxString libelleSelectIcone;
			wxString libelleSelectAll;
			bool isSelected;
			bool isSelectIcone;
			int numFace{};
			wxRect rcSelect;
			wxRect rcSelectIcone;
			wxRect rcDeleteIcone;

			int xPosEdit{};
			int yPosEdit{};
			int xPosText{};
			int yPosText{};
			wxWindow* parentWindow;
			const CThemeInfosSeparationBar& theme_;
		};
	}
}
