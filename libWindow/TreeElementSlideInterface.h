#pragma once
#include "TreeElement.h"
#include <TreeElementValue.h>


namespace Regards::Window
{
	class CTreeElementSlideInterface
	{
	public:
		CTreeElementSlideInterface()
		{
		};

		virtual ~CTreeElementSlideInterface()
		{
		};

		virtual void SlidePosChange(CTreeElement* treeElement, const int& position, CTreeElementValue* value,
		                            const wxString& key) = 0;
	};
}
