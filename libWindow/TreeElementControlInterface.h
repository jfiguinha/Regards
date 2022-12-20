#pragma once
#include "TreeElement.h"

namespace Regards
{
	namespace Window
	{
		class CTreeElementControlInterface
		{
		public:
			CTreeElementControlInterface()
			{
			};

			virtual ~CTreeElementControlInterface()
			{
			};

			//virtual void UpdateElement(CTreeElement * treeElement) = 0;
			virtual void UpdateTreeControl() = 0;
		};
	}
}
