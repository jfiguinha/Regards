#pragma once

#include <theme.h>
#include "WindowMain.h"
#include "TreeElementSlideInterface.h"
#include "TreeElementControlInterface.h"

namespace Regards
{
	namespace Window
	{
		class CScrollInterface;
		class CTreeControl;

		class CTreeWindow : public CWindowMain, public CTreeElementSlideInterface, public CTreeElementControlInterface
		{
		public:
			CTreeWindow(wxWindow* parent, wxWindowID id, const CThemeTree& theme);
			~CTreeWindow() override;

			int GetWidth() override;
			int GetHeight() override;

			wxWindow* GetWindow()
			{
				return this;
			}

			void UpdateScreenRatio() override;

			void SetTreeControl(CTreeControl* treeControl);

			void SlidePosChange(CTreeElement* treeElement, const int& position, CTreeElementValue* value,
				const wxString& key) override
			{
			};
			//void UpdateElement(CTreeElement * treeElement);
			CThemeTree* GetTheme()
			{
				return &themeTree;
			}

		protected:
			void OnEraseBackground(wxEraseEvent& event) override
			{
			};
			void on_paint(wxPaintEvent& event);
			void OnMouseMove(wxMouseEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void OnLButtonUp(wxMouseEvent& event);
			void OnLDoubleClick(wxMouseEvent& event);
			void OnMouseWheel(wxMouseEvent& event);
			void OnKeyDown(wxKeyEvent& event);
			void OnMouseLeave(wxMouseEvent& event);

			void OnMouseCaptureLost(wxMouseEvent& event)
			{
			};
			void OnLeftPosition(wxCommandEvent& event);
			void OnTopPosition(wxCommandEvent& event);
			void OnMoveLeft(wxCommandEvent& event);
			void OnMoveRight(wxCommandEvent& event);
			void OnMoveTop(wxCommandEvent& event);
			void OnMoveBottom(wxCommandEvent& event);
			void OnScrollMove(wxCommandEvent& event);

			void Resize() override;
			void DrawBackgroundRectangle(wxDC* deviceContext, const int& y, const int& rowHeight, const wxColour& color);
			void CalculControlSize();
			void UpdateTreeControl() override;
			void GenerateBackgroundBitmap(wxDC* deviceContext, const int& posLargeur, const int& posHauteur);

			void MoveTop();
			void MoveLeft();
			void MoveBottom();
			void MoveRight();

			void TestMaxX();
			void TestMaxY();
			wxColour GetBackgroundColour(const int& yPos);
			void GenerateScreenBuffer();

			wxBitmap* renderBitmap;
			int bitmapWidth;
			int bitmapHeight;
			int oldPosLargeur;
			int oldPosHauteur;

			int controlWidth;
			int controlHeight;

			CThemeTree themeTree;
			CTreeControl* treeControl;

			//Buffer
			wxBitmap backgroundBuffer;
			// bool bufferUpdate;
			int posHauteur;
			int posLargeur;

			int isMoving;
		};
	}
}
