#pragma once
#include "infoAbout.h"
#include <TreeWindow.h>
#include <TreeWithScrollbar.h>

using namespace Regards::Window;

namespace Regards
{
	namespace Introduction
	{
		class CMyCentralWindowIntro : public CTreeWithScrollbar
		{
		public:
			CMyCentralWindowIntro(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll,
			                      const CThemeTree& theme);
			~CMyCentralWindowIntro() override;

		private:
			CInfoAbout* infoAbout;
		};
	}
}
