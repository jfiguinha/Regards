#pragma once
#include "TreeElement.h"

namespace Regards::Window
{
	class CTreeElementControlInterface
	{
	public:
		//virtual void UpdateElement(CTreeElement * treeElement) = 0;
		virtual void UpdateTreeControl() = 0;
	};
}
