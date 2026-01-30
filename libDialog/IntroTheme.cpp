#include <header.h>
#include "IntroTheme.h"
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
