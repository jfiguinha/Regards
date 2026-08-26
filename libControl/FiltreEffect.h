#pragma once
#include <TreeControl.h>
#include "EffectParameter.h"
#include <FiltreEffectInterface.h>
#include <FiltreUpdate.h>
#include "FilterWindowParam.h"
using namespace Regards::Window;

;

#define TYPE_SLIDE 1
#define TYPE_CHECKBOX 2
#define TYPE_LISTBOX 3

namespace Regards::Window
{
	class CTreeDataEffect;

	class CFiltreEffect : public CTreeControl, public IFiltreEffectInterface
	{
	public:
		CFiltreEffect(IFiltreUpdate* bitmapViewer, CTreeElementControlInterface* interfaceControl, bool isVideo,
		              int bitmapWindowId);
		~CFiltreEffect(void) = default;
		virtual void Init(CEffectParameter* effectParameter, cv::Mat source, const wxString& filename,
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
		void AddTreeInfos(const wxString& exifKey, CTreeElementValue* position, void* value, int typeValue,
			const int& index, tree<CTreeData*>::iterator& top, tree<CTreeData*>::iterator& child,
			const int& type);
		//void UpdateElement();
		void UpdateBitmapToViewer(CImageLoadingFormat* bitmap);
		void RenderElement(RenderMode mode);
		void UpdateElement(tree<CTreeData*>::sibling_iterator& parent, RenderMode mode);
		void UpdateElementChild(tree<CTreeData*>::sibling_iterator& it, CTreeDataEffect* data, int widthPosition, int profondeur, bool isVisible, RenderMode mode);
		void CreateElementChild(tree<CTreeData*>::sibling_iterator& it, CTreeDataEffect* data, int widthPosition, int profondeur, bool isVisible, RenderMode mode);

		CPositionElement* RenderSlide(
			CTreeData* data,
			int& xPos,
			int& yPos,
			bool visible,
			RenderMode mode);

		CPositionElement* RenderCheckbox(
			CTreeData* dataEffect,
			int& xPos,
			int& yPos,
			bool visible,
			RenderMode mode);

		CPositionElement* RenderList(
			CTreeData* dataEffect,
			int& xPos,
			int& yPos,
			bool visible,
			RenderMode mode);

		int filtre;

		int yPos;
		int cxMax;
		int cyMax;
		int xMaxPos;
		int xMaxPosValue;
		int xMinPos;
		int widthPosition;

		CEffectParameter* effectParameter;
		std::unique_ptr<CFilterWindowParam> filterEffect;

		cv::Mat source;
		IFiltreUpdate* bitmapViewer;

		tree<CTreeData*>::iterator top;
		tree<CTreeData*>::iterator child;

		bool isVideo = false;
		int index;
		int bitmapWindowId;
	};
}
