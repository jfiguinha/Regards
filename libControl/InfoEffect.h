#pragma once
#include <TreeControl.h>
#include "ModificationManager.h"
using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CInfoEffect : public CTreeControl
		{
		public:
			CInfoEffect(CTreeElementControlInterface* interfaceControl, CModificationManager* modificationManager,
			            int bitmapWindowId);
			~CInfoEffect() override;

			wxString GetFilename();
			void Init(CImageLoadingFormat* bitmap, const wxString& libelle, const wxString& key);
			void AddEvent(const wxString& libelle, const wxString& key);
			void SetActifElement(const wxString& key);
			void AddModification(CImageLoadingFormat* bitmap, const wxString& libelle);
			void UpdateScreenRatio() override;

			void SlidePosChange(CTreeElement* treeElement, const int& position, CTreeElementValue* value,
			                    const wxString& key) override
			{
			};

		private:
			void InitTree(const wxString& libelle, const wxString& key);
			wxString GetNumModification();

			void ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
			                    const int& posLargeur, const int& posHauteur) override;
			void MouseOver(wxDC* deviceContext, CPositionElement* element, const int& x, const int& y,
			               const int& posLargeur, const int& posHauteur, bool& update) override;

			void CreateElement();
			void UpdateElement();
			void UpdateChildTree(tree<CTreeData*>::sibling_iterator& parent);

			int numEvent;
			int yPos;
			int index;
			tree<CTreeData*>::iterator top;
			tree<CTreeData*>::iterator child;
			//tree<CTreeData *>::iterator childStart;
			CModificationManager* modificationManager;
			int widthPosition;
			wxString filename;
			int bitmapWindowId;
		};
	}
}
