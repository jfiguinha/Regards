#pragma once
#include "InfoAbout.h"
#include "TitleIntro.h"
#include <TreeWindow.h>
#include <ScrollbarWnd.h>
#include "WindowMain.h"
using namespace Regards::Window;

namespace Regards
{
	namespace Introduction
	{
		class CMyCentralWindowIntro : public CWindowMain
		{
		public:
			CMyCentralWindowIntro(wxWindow* parent, wxWindowID id);
			~CMyCentralWindowIntro();

		private:
			void Resize();

			CTitleIntro * titleIntro = nullptr;
			CInfoAbout * infoAbout = nullptr;
			CTreeWindow * treeAbout = nullptr;
			CScrollbarWnd * aboutWndScroll = nullptr;
		};
	}
}
