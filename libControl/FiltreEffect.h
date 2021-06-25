#pragma once
#include <TreeControl.h>
#include "EffectParameter.h"
#include <FiltreEffectInterface.h>
#include <FiltreUpdate.h>
#include "FilterWindowParam.h"
using namespace Regards::Window;

class CRegardsBitmap;

#define TYPE_SLIDE 1
#define TYPE_CHECKBOX 2
#define TYPE_LISTBOX 3

namespace Regards
{
	namespace Window
	{
		class CFiltreEffect : public CTreeControl, public IFiltreEffectInterface
		{
		public:
			CFiltreEffect(IFiltreUpdate* bitmapViewer, CTreeElementControlInterface* interfaceControl, bool isVideo,
			              int bitmapWindowId);
			~CFiltreEffect(void) override;
			virtual void Init(CEffectParameter* effectParameter, CRegardsBitmap* source, const wxString& filename,
			                  const int& filtre);
			void SlidePosChange(CTreeElement* treeElement, const int& position, CTreeElementValue* value,
			                    const wxString& key) override;
			virtual CImageLoadingFormat* ApplyEffect();
			void UpdateScreenRatio() override;
			void UpdateMousePosition();
			void AddTreeInfos(const wxString& exifKey, CTreeElementValue* position, void* value,
			                  const int& typeValue = 1, const int& type = TYPE_SLIDE) override;

		protected:
			void MouseOver(wxDC* deviceContext, CPositionElement* element, const int& x, const int& y,
			               const int& posLargeur, const int& posHauteur, bool& update) override;
			void ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
			                    const int& posLargeur, const int& posHauteur) override;
			void UnclickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
			                      const int& posLargeur, const int& posHauteur) override;

			void UpdateElement();
			void UpdateChildTree(tree<CTreeData*>::sibling_iterator& parent);
			void CreateElement();
			void CreateChildTree(tree<CTreeData*>::sibling_iterator& parent);
			void AddTreeInfos(const wxString& exifKey, CTreeElementValue* position, void* value, int typeValue,
			                  const int& index, tree<CTreeData*>::iterator& top, tree<CTreeData*>::iterator& child,
			                  const int& type);

			int filtre;

			int yPos;
			int cxMax;
			int cyMax;
			int xMaxPos;
			int xMaxPosValue;
			int xMinPos;
			int widthPosition;

			CEffectParameter* effectParameter;
			CFilterWindowParam* filterEffect;

			CRegardsBitmap* source;
			IFiltreUpdate* bitmapViewer;

			tree<CTreeData*>::iterator top;
			tree<CTreeData*>::iterator child;

			bool isVideo = false;
			int index;
			int bitmapWindowId;
		};
	}
}
