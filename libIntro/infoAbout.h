#pragma once
#include <Theme.h>
#include <TreeWindow.h>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

using namespace Regards::Window;

namespace Regards
{
	namespace Introduction
	{
		class CInfoAbout : public CTreeControl
		{
		public:

			CInfoAbout(CThemeTree * theme);
			~CInfoAbout();

			void Init();

		private:

            void ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur);
			void CreateElement();
			void CreateChildTree(tree<CTreeData *>::sibling_iterator &parent);
			void AddLibelle(const wxString &libelle, const wxString &key);

			int index = 0;
			int yPos = 0;
			int widthPosition = 0;
			tree<CTreeData *>::iterator top;
			tree<CTreeData *>::iterator child;

		};
	}
}

