#pragma once

#include <theme.h>

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


namespace Regards
{
	namespace Window
	{
		class CScrollInterface;

		class CTreeWindow : public CWindowMain, public CTreeElementSlideInterface, public CTreeElementControlInterface
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
			virtual void SlidePosChange(CTreeElement * treeElement, const int &position, CTreeElementValue * value, const wxString &key){};
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
			void OnLeftPosition(wxCommandEvent& event);
			void OnTopPosition(wxCommandEvent& event);
			void OnMoveLeft(wxCommandEvent& event);
			void OnMoveRight(wxCommandEvent& event);
			void OnMoveTop(wxCommandEvent& event);
			void OnMoveBottom(wxCommandEvent& event);
			void OnScrollMove(wxCommandEvent& event);

			void Resize();
			void DrawBackgroundRectangle(wxDC * deviceContext, const int &y, const int &rowHeight, const wxColour &color);
			void CalculControlSize();
			void UpdateTreeControl();
			void GenerateBackgroundBitmap(wxDC * deviceContext, const int &posLargeur, const int &posHauteur);
			
			void MoveTop();
			void MoveLeft();
			void MoveBottom();
			void MoveRight();

			void Update()
			{
				Refresh();
			}

			void TestMaxX();
			void TestMaxY();
            wxColour GetBackgroundColour(const int &yPos);
            
			wxBitmap * renderBitmap;
			int bitmapWidth;
			int bitmapHeight;
			int oldPosLargeur;
			int oldPosHauteur;

			int controlWidth;
			int controlHeight;

			CThemeTree themeTree;
			CTreeControl * treeControl;
            
            //Buffer
            wxBitmap backgroundBuffer; 
            bool bufferUpdate;
			int posHauteur;
			int posLargeur;

			int isMoving;
		};
	}
}

