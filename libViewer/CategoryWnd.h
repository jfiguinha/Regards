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
			void AddCategorie(const int& numCatalog, const int& numCategorie, tree<CTreeData*>::iterator parent,
			                  const int& numParent, const wxString& catlibelle, const int& id);

			tree<CTreeData*>::iterator FindChild(tree<CTreeData*>::iterator parent, const wxString& catlibelle);
			tree<CTreeData*>::iterator FindExifKey(const wxString& exifkey);
			tree<CTreeData*>::iterator FindFolderKey(const wxString& exifkey);

			wxString GetCatalogLibelle(const int& numCatalog);

			void LoadCategorie(const int& numCategorie, tree<CTreeData*>::iterator parent, int numParent);
			void InitCatalogCategorie(tree<CTreeData*>::iterator parent, int numParent);
			void LoadFolder(const int& numCatalog, tree<CTreeData*>::iterator parent, int numParent);
			void LoadFace(const int& numCatalog, tree<CTreeData*>::iterator parent, int numParent);
			//Draw Tree
			void CreateElement();
			void CreateChildTree(tree<CTreeData*>::sibling_iterator& parent);
			void UpdateElement(const bool& init = false);
			void UpdateChildTree(tree<CTreeData*>::sibling_iterator& parent, const bool& init);

			//Gestion des check box
			void GestionCheckBox(CTreeDataCategory* treeData, CTreeElementCheckBox* checkBox);
			CPositionElement* VerifChildParentCheckBox(const int& numParent, bool& isAllUnchecked, const bool& checkOn);
			void VerifParentCheckBox(CTreeDataCategory* treeData, const bool& checkOn);
			void GestionCheckBoxParent(const int& numParent, const bool& check);



			std::unique_ptr<CategoryRepository>           repo_;
			std::unique_ptr<CategoryQueryService>         queryService_;
			std::unique_ptr<CategoryTreeStatePersistence> persistence_;
			//Variable
			int numCatalog;
			int idElement;
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
