#pragma once
#include <InfosSeparationBar.h>
using namespace Regards::Window;

namespace Regards::Scanner
{
	class CInfosSeparationBarExplorer : public CInfosSeparationBar
	{
	public:
		CInfosSeparationBarExplorer(const CThemeInfosSeparationBar& theme);
		~CInfosSeparationBarExplorer(void) override;

		void RenderIcone(wxDC* deviceContext, const int& posLargeur, const int& posHauteur) override;
		void OnClick(const int& x, const int& y) override;
		bool GetSelected();

	protected:
		wxImage bitmapCheckOn;
		wxImage bitmapCheckOff;
		wxString libelleSelectAll;
		bool isSelected;
		wxRect rcSelect;
	};
}
