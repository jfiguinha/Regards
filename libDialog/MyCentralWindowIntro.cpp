#include <header.h>
#include "MyCentralWindowIntro.h"
#include <wx/dcbuffer.h>
#include "IntroTheme.h"
using namespace Regards::Introduction;

CMyCentralWindowIntro::CMyCentralWindowIntro(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll,
                                             const CThemeTree& theme) :
	CTreeWithScrollbar("CMyCentralWindowIntro", parent, id, themeScroll, theme)
{

	infoAbout = std::unique_ptr<CInfoAbout>(new CInfoAbout(theme));
	treeWindow->SetTreeControl(infoAbout.get());
	infoAbout->Init();
}


CMyCentralWindowIntro::~CMyCentralWindowIntro()
{

}
