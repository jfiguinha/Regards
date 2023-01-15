#pragma once
#include <theme.h>
#include <TreeControl.h>
#include <PhotoCategorie.h>

namespace Regards
{
	namespace Window
	{
		class CTreeElementControlInterface;
		class CTreeElementValue;
		class CPositionElement;
	}

	using namespace Window;

	namespace Control
	{
		class CCriteriaTree : public CTreeControl
		{
		public:
			CCriteriaTree(CThemeTree* theme, CTreeElementControlInterface* interfaceControl);

			~CCriteriaTree(void) override
			{
			};
			void SetFile(const wxString& filename, const int& numPhotoId);
			wxString GetFilename();
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
			void AddTreeInfos(const wxString& exifKey, const wxString& exifValue, const int& index,
			                  tree<CTreeData*>::iterator& top, tree<CTreeData*>::iterator& child);


			int yPos;
			int rotation;
			int numPhotoId;
			wxString filename;
			int widthPosition;
			static PhotoCategorieVector photoCategorieVector;
			tree<CTreeData*>::iterator top;
			tree<CTreeData*>::iterator child;
		};
	}
}
