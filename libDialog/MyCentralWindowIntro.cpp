#include <header.h>
#include "MyCentralWindowIntro.h"
#include <wx/dcbuffer.h>
#include "IntroTheme.h"
using namespace Regards::Introduction;

CMyCentralWindowIntro::CMyCentralWindowIntro(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll,
                                             const CThemeTree& theme) :
	CTreeWithScrollbar("CMyCentralWindowIntro", parent, id, themeScroll, theme)
{
	infoAbout = nullptr;
	infoAbout = new CInfoAbout(theme);
	treeWindow->SetTreeControl(infoAbout);
	infoAbout->Init();
}


CMyCentralWindowIntro::~CMyCentralWindowIntro()
{
	if (infoAbout != nullptr)
		delete(infoAbout);
}
