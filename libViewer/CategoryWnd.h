#pragma once
#include <theme.h>
#include <Criteria.h>
#include <TreeWindow.h>
#include "ViewerParam.h"
#include <TreeDataCategory.h>
using namespace Regards::Window;
using namespace Regards::Viewer;

#define WM_ADDCRITERIA 1
#define WM_REFRESHCRITERIA 2

namespace Regards
{
	namespace Viewer
	{

		class CCategoryWnd :public CTreeControl
		{
		public:
			CCategoryWnd(CWindowMain * windowMain, CThemeTree * theme, CTreeElementControlInterface * interfaceControl);
			~CCategoryWnd();
			void Init();
            void RefreshCriteriaSearch();
            void UpdateScreenRatio();
			void RefreshListPhoto();
            wxString GetSqlRequest();
			void SlidePosChange(CTreeElement * treeElement, const int &position, CTreeElementValue * value, const wxString &key){};
		protected:

            
			void SaveState();
			void LoadState();
			void SaveStateTriangle();
			void ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur);

		private:

			void AddCategorie(const int &numCatalog, const int &numCategorie, tree<CTreeData *>::iterator parent, const int &numParent, const wxString &catlibelle, const int &id);

			tree<CTreeData *>::iterator FindChild(tree<CTreeData *>::iterator parent, const wxString &catlibelle);
			tree<CTreeData *>::iterator FindExifKey(const wxString & exifkey);
			tree<CTreeData *>::iterator FindFolderKey(const wxString & exifkey);
			void UpdateSQLSearchCriteria();
			wxString GetCatalogLibelle(const int &numCatalog);

			void LoadCategorie(const int &numCategorie, tree<CTreeData *>::iterator parent, int numParent);
			void InitCatalogCategorie(tree<CTreeData *>::iterator parent, int numParent);
			void LoadFolder(const int &numCatalog, tree<CTreeData *>::iterator parent, int numParent);
			void LoadFace(const int &numCatalog, tree<CTreeData *>::iterator parent, int numParent);
			//Draw Tree
			void CreateElement();
			void CreateChildTree(tree<CTreeData *>::sibling_iterator &parent);
			void UpdateElement(const bool &init = false);
			void UpdateChildTree(tree<CTreeData *>::sibling_iterator &parent, const bool &init);

			//Gestion des check box
			void GestionCheckBox(CTreeDataCategory * treeData, CTreeElementCheckBox * checkBox);
			CPositionElement * VerifChildParentCheckBox(const int &numParent, bool & isAllUnchecked, const bool &checkOn);
			void VerifParentCheckBox(CTreeDataCategory * treeData, const bool &checkOn);
			void GestionCheckBoxParent(const int &numParent, const bool &check);
			bool GetCheckState(const wxString &exifKey, const wxString &key);
			bool GetTriangleState(const wxString &exifKey, const wxString &key);

			//Variable
			int numCatalog;
			int idElement;
			int yPos;
            wxString sqlRequest;
			CTreeDataCategory * treeDataModify;
			wxString stateValue;
			wxString stateTriangleValue;
			vector<wxString> MonthName;
			int widthPosition;
			vector<int> listPhoto;
			CMainParam * config;
			CWindowMain * windowMain;

		};


	}
}
