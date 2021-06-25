#pragma once
#include "TreeData.h"
using namespace std;

namespace Regards
{
	namespace Window
	{
		class CTreeDataStars : public CTreeData
		{
		public:
			CTreeDataStars(void);
			~CTreeDataStars(void);
			void SetNumPhotoId(const int& numPhotoId);
			int GetNumPhotoId();
		protected:
			int numPhotoId;
		};
	}
}
