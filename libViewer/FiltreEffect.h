#pragma once
#include <TreeControl.h>
#include <RegardsBitmap.h>
#include <BitmapWndViewer.h>
#include "EffectParameter.h"
#include "FiltreEffectInterface.h"
#include "FilterWindowParam.h"

using namespace Regards::Window;
using namespace Regards::Control;

namespace Regards
{
	namespace Viewer
	{

		class CFiltreEffect : public CTreeControl, public IFiltreEffectInterface
		{
		public:

			CFiltreEffect(CBitmapWndViewer * bitmapViewer, CThemeTree * theme, CTreeElementControlInterface * interfaceControl);
			~CFiltreEffect(void);
			virtual void Init(CEffectParameter * effectParameter, CRegardsBitmap * source, const int &filtre);
			virtual void SlidePosChange(CTreeElement * treeElement, const int &position, const int &value, const wxString &key);

            void UpdateScreenRatio();

            void AddTreeInfos(const wxString &exifKey, const int &position, const vector<int> & value);
            
		protected:

			void MouseOver(wxDC * deviceContext, CPositionElement * element, const int &x, const int &y, const int& posLargeur, const int &posHauteur);
			void ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur);
			void UnclickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur);

			void UpdateElement();
			void UpdateChildTree(tree<CTreeData *>::sibling_iterator &parent);
			void CreateElement();
			void CreateChildTree(tree<CTreeData *>::sibling_iterator &parent);
			void AddTreeInfos(const wxString &exifKey, const int &position, const vector<int> & value, const int &index, tree<CTreeData *>::iterator &top, tree<CTreeData *>::iterator &child);

			int filtre = 0;

			int yPos;
			int cxMax;
			int cyMax;
			int xMaxPos;
			int xMaxPosValue;
			int xMinPos;
			int widthPosition = 0;

			CEffectParameter * effectParameter;
            CFilterWindowParam * filterEffect = nullptr;

			CRegardsBitmap * source = nullptr;
			CBitmapWndViewer * bitmapViewer = nullptr;
            
            tree<CTreeData *>::iterator top;
            tree<CTreeData *>::iterator child;
            int index = 0;
		};
	}
}