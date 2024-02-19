#pragma once
#include <InfosSeparationBar.h>
#include <FaceName.h>
using namespace Regards::Window;

namespace Regards::Window
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

		void RenderTitle(wxDC* dc) override;

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
		wxSize sizeDelete;

		int xPosEdit{};
		int yPosEdit{};
		int xPosText{};
		int yPosText{};
		wxWindow* parentWindow;
		const CThemeInfosSeparationBar& theme_;
	};
}
