#pragma once
#include <InfosSeparationBar.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CInfosSeparationBarEffect : public CInfosSeparationBar
		{
		public:
			CInfosSeparationBarEffect(const CThemeInfosSeparationBar& theme)
				: CInfosSeparationBar(theme)
			{
			};

			~CInfosSeparationBarEffect(void) override
			{
			};

			void AddPhotoToList(const int& numElement);
		};
	}
}
