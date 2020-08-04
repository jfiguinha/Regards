#pragma once
#include <TreeControl.h>
#include "BboxText.h"
using namespace Regards::Window;

namespace Regards
{
	namespace Window
	{
		class CTreeElementControlInterface;
		class CTreeElementValue;
		class CPositionElement;
	}

	namespace Scanner
	{
		class COcrLabel : public CTreeControl
		{
		public:

			COcrLabel(CTreeElementControlInterface * interfaceControl, int idWindow);
			~COcrLabel();

			void Init(vector<ChOcrElement *> &labelList);
			void SetActifElement(const wxString &key);
            void UpdateScreenRatio();
			void SlidePosChange(CTreeElement * treeElement, const int &position, CTreeElementValue * value, const wxString &key){};
			void Init();

		private:

			void SaveState();
			bool GetCheckState(const wxString &exifKey, const wxString &key);
			void ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur);
			void MouseOver(wxDC * deviceContext, CPositionElement * element, const int &x, const int &y, const int& posLargeur, const int &posHauteur);

			void CreateElement();
			void UpdateElement();
			void UpdateChildTree(tree<CTreeData *>::sibling_iterator &parent);

			int numEvent;
			int yPos;
			int index;
			tree<CTreeData *>::iterator top;
			tree<CTreeData *>::iterator child;
			int widthPosition;
			int item = 0;
			wxString stateValue;
			int idWindow;
		};

	}
}