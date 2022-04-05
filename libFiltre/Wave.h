#pragma once
class CRegardsBitmap;

namespace Regards
{
	namespace FiltreEffet
	{
		class CWaveFilter
		{
		public:
			CWaveFilter();
			~CWaveFilter();
			void ProcessEffect(CRegardsBitmap * bitmap, int x, int y, short height, int radius, int scale);

		private:
			void WaterEffect(CRegardsBitmap * bitma);
			void ProcessWaves();
			void PutDrop(int x, int y, short height, int radius);
			inline void SetWaveData(int x, int y, int buffer, short data);
			inline short GetWaveData(int x, int y, int buffer);

			int _waveWidth;
			int _waveHeight;
			bool _weHaveWaves;
			int _scale;
			short * _waves;
			int _activeBuffer;
		};
	}
}
