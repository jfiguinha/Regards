#pragma once
#include <TreeControl.h>
#include <TreeWithScrollbar.h>
/*
#include <theme.h>
#include <Criteria.h>
#include <TreeWindow.h>
#include "ViewerParam.h"
#include <TreeDataCategory.h>
*/
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

		class CKeywordWnd : public CTreeControl
		{
		public:
			CKeywordWnd(CWindowMain* windowMain, CThemeTree* theme, CTreeElementControlInterface* interfaceControl);
			~CKeywordWnd() override;
			void Init(const wxString& filename);
			void UpdateScreenRatio() override;

			void SlidePosChange(CTreeElement* treeElement, const int& position, CTreeElementValue* value,
			                    const wxString& key) override
			{
			};

		protected:
			void Resize();
			void ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
			                    const int& posLargeur, const int& posHauteur) override;

		private:
			tree<CTreeData*>::iterator FindChild(tree<CTreeData*>::iterator parent, const wxString& catlibelle);

			//Draw Tree
			void RenderElement(RenderMode mode);
			void RenderChildTree(tree<CTreeData*>::sibling_iterator& parent, RenderMode mode);

			CPositionElement* RenderDelete(
				CTreeDataCategory* data,
				int& xPos,
				int& yPos,
				bool visible,
				RenderMode mode);

			void InitKeyWordCategorie(tree<CTreeData*>::iterator parent, int numParent);
			//Gestion des check box
			bool GetCheckState(const wxString& exifKey, const wxString& key);
			bool GetTriangleState(const wxString& exifKey, const wxString& key);

			//Variable
			int numCatalog;
			int idElement;
			int yPos;
			wxString sqlRequest;
			CTreeDataCategory* treeDataModify;
			wxString stateValue;
			wxString stateTriangleValue;
			int widthPosition;
			CWindowMain* windowMain;
			int numPhotoId;
		};
	}
}
