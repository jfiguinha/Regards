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

			~CCriteriaTree(void) = default;

			// --- Public interface --------------------------------------------------
			void SetFile(const wxString& filename, const int& numPhotoId);
			wxString GetFilename();
			void UpdateScreenRatio() override;

			// Kept for API compatibility with existing callers; forwards to RenderTree().
			void CreateElement();

			void SlidePosChange(CTreeElement* treeElement, const int& position, CTreeElementValue* value,
			                    const wxString& key) override
			{
			};

		private:
			// --- Input handling ------------------------------------------------------
			void MouseOver(wxDC* dc, CPositionElement* element, const int& x, const int& y, const int& posLargeur,
			               const int& posHauteur, bool& update) override;
			void ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
			                    const int& posLargeur, const int& posHauteur) override;

			// --- Category loading / dispatch ------------------------------------------
			void LoadCategories();
			void ProcessCategory(const wxString& picture, CPhotoCategorie& photoCategorie,
			                     tree<CTreeData*>::iterator& child);

			// --- Category handlers -----------------------------------------------------
			void AddFaces(const wxString& picture, CPhotoCategorie& photoCategorie,
			             tree<CTreeData*>::iterator& child);
			void AddFolder(const wxString& picture, CPhotoCategorie& photoCategorie,
			              tree<CTreeData*>::iterator& child);
			void AddGPS(const wxString& picture, CPhotoCategorie& photoCategorie,
			           tree<CTreeData*>::iterator& child);
			void AddDate(const wxString& picture, CPhotoCategorie& photoCategorie,
			            tree<CTreeData*>::iterator& child);
			void AddStars(const wxString& picture, CPhotoCategorie& photoCategorie,
			             tree<CTreeData*>::iterator& child);
			void AddKeywords(const wxString& picture, CPhotoCategorie& photoCategorie,
			                tree<CTreeData*>::iterator& child);

			// --- Tree node factories ----------------------------------------------------
			void CreateLinkNode(tree<CTreeData*>::iterator& parent, const wxString& key, const wxString& value,
			                    const int& linkType, const int& id);
			void CreateStarNode(tree<CTreeData*>::iterator& parent, const wxString& key, const wxString& value);
			void CreateTextNode(tree<CTreeData*>::iterator& parent, const wxString& key, const wxString& value);

			// --- Rendering ---------------------------------------------------------------
			void RenderTree();
			void RenderNode(tree<CTreeData*>::sibling_iterator& parent);

			CPositionElement* RenderTextLinkValue(
				CTreeData* data,
				int& xPos,
				int& yPos,
				bool visible);

			// --- Legacy / currently unused ------------------------------------------------
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
