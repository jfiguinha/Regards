#pragma once
#include "TreeElement.h"

namespace Regards
{
	namespace Window
	{
		class CTreeElementSlideInterface
		{
		public:
			CTreeElementSlideInterface(){};
			~CTreeElementSlideInterface(){};

			virtual void SlidePosChange(CTreeElement * treeElement, const int &position, const int &value, const wxString &key) = 0;
		};
	}
}

