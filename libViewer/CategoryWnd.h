#pragma once
#include <TreeControl.h>
using namespace Regards::Window;

#define WM_ADDCRITERIA 1
#define WM_REFRESHCRITERIA 2

class CThemeTree;

namespace Regards
{
	namespace Window
	{
		class CTreeElementControlInterface;
		class CTreeDataCategory;
		class CWindowMain;
	}

	namespace Viewer
	{
		class CMainParam;
		class CategoryQueryService;
		class CategoryRepository;
		class CategoryTreeStatePersistence;
		class CategoryHierarchyIndex;
		class CategorySelectionController;

		class CCategoryWnd : public CTreeControl
		{
		public:
			CCategoryWnd(CWindowMain* windowMain, CThemeTree* theme, CTreeElementControlInterface* interfaceControl);
			~CCategoryWnd() override;
			void Init();
			void RefreshCriteriaSearch();
			void UpdateScreenRatio() override;
			void RefreshListPhoto();
			wxString GetSqlRequest();

			void SlidePosChange(CTreeElement* treeElement, const int& position, CTreeElementValue* value,
			                    const wxString& key) override
			{
			};

		protected:

			void ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
			                    const int& posLargeur, const int& posHauteur) override;

		private:
			//Draw Tree
			void CreateElement();
			void CreateChildTree(tree<CTreeData*>::sibling_iterator& parent);
			void UpdateElement(const bool& init = false);
			void UpdateChildTree(tree<CTreeData*>::sibling_iterator& parent, const bool& init);

			std::unique_ptr<CategoryHierarchyIndex>       index_;
			std::unique_ptr<CategoryRepository>           repo_;
			std::unique_ptr<CategoryQueryService>         queryService_;
			std::unique_ptr<CategoryTreeStatePersistence> persistence_;
			std::unique_ptr<CategorySelectionController>  selectionCtrl_;
			//Variable
			int numCatalog;
			int yPos;


			std::vector<wxString> monthNames_;
			int widthPosition;
			std::vector<int> listPhoto;
			CMainParam* config;
			CWindowMain* windowMain;


			wxString     sqlRequest_;
			wxString     oldSqlRequest_;
		};
	}
}
