#pragma once
#include "RegardsBitmap.h"

namespace Regards
{
	namespace FiltreEffet
	{
		class CMotionBlur
		{
		public:
			CMotionBlur(void);
			~CMotionBlur(void);

			bool MotionBlur(CRegardsBitmap * m_CRegardsBitmap, const double &radius,const double &sigma,const double &angle);

		private:
			int GetMotionBlurKernel(int width,const double sigma,double **kernel);
	
		};
	}
}

