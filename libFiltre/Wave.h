#pragma once
class CRegardsBitmap;

namespace Regards::FiltreEffet
{
	class CWaveFilter
	{
	public:
		CWaveFilter();
		~CWaveFilter();
		void ProcessEffect(cv::Mat& image, int x, int y, short height, int radius, int scale);

	private:
		void WaterEffect(cv::Mat& image);
		void ProcessWaves();
		void PutDrop(int x, int y, short height, int radius);
		inline void SetWaveData(int x, int y, int buffer, short data);
		inline short GetWaveData(int x, int y, int buffer);

		int _waveWidth;
		int _waveHeight;
		bool _weHaveWaves;
		int _scale;
		int CalculPosition(int x, int y, int buffer);
		map<int, short> _waves;
		int _activeBuffer;
	};
}
