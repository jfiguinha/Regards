#pragma once
#include <theme.h>
#include <TreeControl.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Introduction
	{
		class CInfoAbout : public CTreeControl
		{
		public:
			CInfoAbout(const CThemeTree& theme);
			~CInfoAbout() override;

			void Init();

			void SlidePosChange(CTreeElement* treeElement, const int& position, CTreeElementValue* value,
			                    const wxString& key) override
			{
			};
			void UpdateScreenRatio() override;


		private:
			void ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
			                    const int& posLargeur, const int& posHauteur) override;
			void CreateElement();
			void CreateChildTree(tree<CTreeData*>::sibling_iterator& parent);

			int index;
			int yPos;
			int widthPosition;
			tree<CTreeData*>::iterator top;
			tree<CTreeData*>::iterator child;
		};
	}
}
