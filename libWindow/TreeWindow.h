#pragma once
#include <mutex>
#include <Theme.h>
#include "ScrollInterface.h"
#include "PositionElement.h"
#include "TreeElement.h"
#include <TreeData.h>
#include <Tree.h>
#include "TreeControl.h"
#include "TreeElementTriangle.h"
#include "TreeElementTexte.h"
#include "TreeElementSlide.h"
#include "TreeElementCheckBox.h"
#include "TreeElementSlideInterface.h"
#include "TreeElementControlInterface.h"
#include "WindowMain.h"
using namespace std;

namespace Regards
{
	namespace Window
	{
		class CScrollInterface;

		class CTreeWindow : public CWindowMain, public CScrollInterface, public CTreeElementSlideInterface, public CTreeElementControlInterface
		{
		public:
			CTreeWindow(wxWindow* parent, wxWindowID id, const CThemeTree & theme);
			~CTreeWindow();

			int GetWidth();
			int GetHeight();

			wxWindow * GetWindow(){
				return this;
			}
            
            void UpdateScreenRatio();

			void SetTreeControl(CTreeControl * treeControl);
			virtual void SlidePosChange(CTreeElement * treeElement, const int &position, const int &value, const wxString &key){};
			void UpdateElement(CTreeElement * treeElement);
			CThemeTree * GetTheme()
			{
				return &themeTree;
			}

		protected:

			void OnEraseBackground(wxEraseEvent& event){};
			void OnPaint(wxPaintEvent& event);
			void OnMouseMove(wxMouseEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void OnLButtonUp(wxMouseEvent& event);
			void OnLDoubleClick(wxMouseEvent& event);
			void OnMouseWheel(wxMouseEvent& event);
			void OnKeyDown(wxKeyEvent& event);
			void OnMouseCaptureLost(wxMouseEvent& event){};

			void Resize();
			void DrawBackgroundRectangle(wxDC * deviceContext, const int &y, const int &rowHeight, const wxColour &color);
			void CalculControlSize();
			void UpdateTreeControl();
			void GenerateBackgroundBitmap(wxDC * deviceContext, const int &posLargeur, const int &posHauteur);
			
			void Update()
			{
				this->Refresh();
			}

			void TestMaxX();
			void TestMaxY();
            wxColour GetBackgroundColour(const int &yPos);
            
			wxBitmap * renderBitmap = nullptr;
			int bitmapWidth = 0;
			int bitmapHeight = 0;
			int oldPosLargeur = 0;
			int oldPosHauteur = 0;

			int controlWidth;
			int controlHeight;

			std::mutex m_lock;
			CThemeTree themeTree;
			CTreeControl * treeControl = nullptr;
		};
	}
}

