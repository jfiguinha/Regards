#pragma once
#include <theme.h>
#include <TreeWindow.h>
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
			void SlidePosChange(CTreeElement * treeElement, const int &position, CTreeElementValue * value, const wxString &key){};
            void UpdateScreenRatio();
            
            
		private:

            void ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur);
			void CreateElement();
			void CreateChildTree(tree<CTreeData *>::sibling_iterator &parent);
			void AddLibelle(const wxString &libelle, const wxString &key);

			int index;
			int yPos;
			int widthPosition;
			tree<CTreeData *>::iterator top;
			tree<CTreeData *>::iterator child;

		};
	}
}

