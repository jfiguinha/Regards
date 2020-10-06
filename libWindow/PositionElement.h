#pragma once

namespace Regards
{
	namespace Window
	{
		class CTreeData;
		class CTreeElement;

		#define ELEMENT_TRIANGLE 1
		#define ELEMENT_TEXTE 2
		#define ELEMENT_CLOSE 3
		#define ELEMENT_DELETE 4
		#define ELEMENT_CHECKBOX 5
		#define ELEMENT_TEXTEVALUE 6
		#define ELEMENT_SLIDE 7
		#define ELEMENT_THUMBNAIL 8
		#define ELEMENT_LISTBOX 9
		#define ELEMENT_STAR 10

		class CPositionElement
		{
		public:
			CPositionElement()
			{
				x = 0;
				y = 0;
				width = 0;
				height = 0;
				type = 0;
				numColumn = 0;
				numRow = 0;
			}
			~CPositionElement();

			int GetType();
			void SetType(const int &type);

			int GetX();
			void SetX(const int &x);

			int GetY();
			void SetY(const int &y);

			int GetWidth();
			void SetWidth(const int &width);

			int GetHeight();
			void SetHeight(const int &height);

			int GetRow();
			void SetRow(const int &row);

			int GetColumn();
			void SetColumn(const int &column);

			void SetColorBackground(const wxColour &colorBackground);
			wxColour GetColorBackground();

			CTreeData * GetTreeData();
			void SetTreeData(CTreeData * treeData);

			CTreeElement * GetTreeElement();
			void SetTreeElement(CTreeElement * treeElement);

			void DrawElement(wxDC * deviceContext, const int &xStartElement, const int &xposLargeur, const int &posHauteur);
			void DrawElement(wxDC * deviceContext);

			void SetValue(const int &x, const int &y, const int &numColumn, const int &numRow, const int &width, const int &height, const int &type, CTreeElement * treeElement, CTreeData * treeData);

		private:

			int type;
			int x;
			int y;
			int width;
			int height;
			int numColumn;
			int numRow;
			CTreeData * treeData;
			CTreeElement * treeElement;
			wxColour colorBackground;
		};


		typedef tbb::concurrent_vector<CPositionElement *> PositionElementVector;
	}
}
