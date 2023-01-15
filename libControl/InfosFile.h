#pragma once
#include <theme.h>
#include <TreeControl.h>
//#include <TreeElementControlInterface.h>
//#include <PositionElement.h>


namespace Regards
{
	namespace Window
	{
		class CTreeElementTriangle;
		class CTreeElementTexte;
		class CPositionElement;
		class CTreeElementControlInterface;
	}

	using namespace Window;

	namespace Control
	{
		class CInfosFile : public CTreeControl
		{
		public:
			CInfosFile(CThemeTree* theme, CTreeElementControlInterface* interfaceControl);

			~CInfosFile(void) override
			{
			};
			void SetFile(const wxString& filename);
			const wxString GetFilename();
			void UpdateScreenRatio() override;
			void CreateElement();

			void SlidePosChange(CTreeElement* treeElement, const int& position, CTreeElementValue* value,
			                    const wxString& key) override
			{
			};

		private:
			void MouseOver(wxDC* dc, CPositionElement* element, const int& x, const int& y, const int& posLargeur,
			               const int& posHauteur, bool& update) override;
			void ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
			                    const int& posLargeur, const int& posHauteur) override;
			void CreateChildTree(tree<CTreeData*>::sibling_iterator& parent);
			void UpdateElement();
			void UpdateChildTree(tree<CTreeData*>::sibling_iterator& parent);
			void AddTreeInfos(const wxString& exifKey, const wxString& exifValue, const int& index,
			                  tree<CTreeData*>::iterator& top, tree<CTreeData*>::iterator& child);


			int yPos;
			//int xMargePos;
			int cxMax;
			//int cyMax;
			int xMaxPos;
			int xMaxPosValue;
			//int xMinPos;
			int rotation;
			wxString filename;
			int widthPosition;
		};
	}
}
