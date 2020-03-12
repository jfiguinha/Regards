#pragma once

#define TOP 1
#define BOTTOM 2

namespace Regards
{
	namespace Window
	{
		class CToolbarInterface
		{
			public:
				CToolbarInterface(){};
				virtual ~CToolbarInterface(){};
				virtual void ClickShowButton(const int &id, const int &refresh = 1) = 0;
			protected:
				void EventManager(const int &id);
		};
	}
}