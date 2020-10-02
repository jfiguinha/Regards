#include <header.h>
#include "IntroTheme.h"
#include <rapidxml.hpp>
using namespace rapidxml;
using namespace Regards::Introduction;

CIntroTheme::CIntroTheme()
{

}


void CIntroTheme::GetAboutTexteTheme(CThemeIntroLogo * theme)
{
	*theme = themeIntroLogo;
}


