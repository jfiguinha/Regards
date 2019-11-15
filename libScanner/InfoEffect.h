#pragma once
#include <TreeWindow.h>
#include <BitmapWndViewer.h>
#include "ModificationManager.h"
using namespace Regards::Control;
using namespace Regards::Window;

namespace Regards
{
	namespace Scanner
	{
		class CInfoEffect : public CTreeControl
		{
		public:

			CInfoEffect(CTreeElementControlInterface * interfaceControl, CModificationManager * modificationManager);
			~CInfoEffect();

			wxString GetFilename();
			void Init(CRegardsBitmap * bitmap, const wxString &libelle, const wxString &key);
			void AddEvent(const wxString &libelle, const wxString &key);
			void SetActifElement(const wxString &key);
			void AddModification(CRegardsBitmap * bitmap, const wxString &libelle);
            void UpdateScreenRatio();
			void SlidePosChange(CTreeElement * treeElement, const int &position, CTreeElementValue * value, const wxString &key){};

		private:

            void InitTree(const wxString &libelle, const wxString &key);
			wxString GetNumModification();

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
			//tree<CTreeData *>::iterator childStart;
			CModificationManager * modificationManager;
			int widthPosition;
			wxString filename;
		};

	}
}