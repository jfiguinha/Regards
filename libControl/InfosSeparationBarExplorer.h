#pragma once
#include <InfosSeparationBar.h>
using namespace Regards::Window;

namespace Regards::Window
{
	class CInfosSeparationBarExplorer : public CInfosSeparationBar
	{
	public:
		CInfosSeparationBarExplorer(const CThemeInfosSeparationBar& theme);
		~CInfosSeparationBarExplorer(void) override;

		void RenderIcone(wxDC* deviceContext, const int& posLargeur, const int& posHauteur) override;
		void OnClick(const int& x, const int& y) override;
		bool GetSelected();
		bool GetShow();

	protected:

		void CreateIcone(wxImage& bitmap, const wxString& name);
		CThemeTreeTriangle themeTriangle;
		bool open = true;
		wxImage bitmapCheckOn;
		wxImage bitmapCheckOff;
		wxImage bitmapFolderExpand;
		wxImage bitmapFolderContract;
		wxString libelleSelectAll;
		bool isSelected;
		wxRect rcSelect;
		wxRect rcShowSelect;
	};
}
