#pragma once

class CRegardsBitmap;

namespace Regards
{
	namespace FiltreEffet
	{
		class CHistogramme
		{
		public:

			CHistogramme(void);
			~CHistogramme(void);

			static bool HistogramLog(CRegardsBitmap * bitmap);
			static bool HistogramNormalize(CRegardsBitmap * bitmap);
			static bool HistogramEqualize(CRegardsBitmap * bitmap);
			void ParcoursBitmap(CRegardsBitmap * bitmap);

		private:
	
			int GetValue(int iColor, int iYMax, int iPos);

			long m_lMoyenneValue[256];
			long m_lMaxMoyenneValue;

			long m_lRedValue[256];
			long m_lMaxRedValue;

			long m_lGreenValue[256];
			long m_lMaxGreenValue;

			long m_lBlueValue[256];
			long m_lMaxBlueValue;

		};
	}
}

