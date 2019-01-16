#pragma once
#include "TreeElement.h"
#include <TreeElementValue.h>


namespace Regards
{
	namespace Window
	{


		class CTreeElementSlideInterface
		{
		public:
			CTreeElementSlideInterface(){};
			~CTreeElementSlideInterface(){};

			virtual void SlidePosChange(CTreeElement * treeElement, const int &position, CTreeElementValue * value, const wxString &key) = 0;
		};
	}
}

