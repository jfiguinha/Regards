#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

namespace Regards
{
	namespace Window
	{
		class CScrollbarWnd;

		class CScrollInterface
		{
		public:
			CScrollInterface();
			~CScrollInterface();
			void SetTopPosition(const int &top);
			void SetLeftPosition(const int &left);
			void SetScrollbar(CScrollbarWnd * scrollbar);
			virtual wxWindow * GetWindow() = 0;

			virtual int GetWidth(){
				return 0;
			};

			virtual int GetHeight()
			{
				return 0;
			};

			void MoveLeft();
			void MoveRight();
			void MoveTop();
			void MoveBottom();



		protected:

			virtual void TestMaxX() = 0;
			virtual void TestMaxY() = 0;
			virtual void Update() = 0;


			int posLargeur;
			int posHauteur;
			int defaultPageSize;
			int defaultLineSize;
			CScrollbarWnd * scrollbar;

		};
	}
}

