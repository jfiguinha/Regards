#include <header.h>
#include "IntroTheme.h"
#include <rapidxml.hpp>
using namespace rapidxml;
using namespace Regards::Introduction;

/**
 * \brief 
 */
CIntroTheme::CIntroTheme()
{
}

CIntroTheme::~CIntroTheme()
{
}


void CIntroTheme::GetAboutTexteTheme(CThemeIntroLogo* theme) const
{
	*theme = themeIntroLogo;
}
