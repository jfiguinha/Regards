#include <header.h>
#include "Filtre.h"

using namespace Regards::FiltreEffet;

#ifdef USE_TBB

struct myFiltreTask
{
	myFiltreTask(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest, CFiltre* pt)
	{
		this->x = x;
		this->y = y;
		this->pBitsSrc = pBitsSrc;
		this->pBitsDest = pBitsDest;
		this->filtre = pt;
	}

	void operator()()
	{
		filtre->PixelCompute(x, y, pBitsSrc, pBitsDest);
	}

	void ExecuteFilter()
	{
		filtre->PixelCompute(x, y, pBitsSrc, pBitsDest);
	}

	int x;
	int y;
	cv::Mat pBitsSrc;
	cv::Mat pBitsDest;
	CFiltre* filtre;
};
#endif

CFiltre::CFiltre(): bmWidth(0), bmHeight(0)
{
}


CFiltre::~CFiltre()
{
}

void CFiltre::SetParameter(cv::Mat& pBitmap, CRgbaquad color)
{
	this->pBitsSrc = pBitmap;
	this->color = color;
	bmWidth = pBitmap.size().width;
	bmHeight = pBitmap.size().height;
}

//Effet GrayScale
void CFiltre::Compute()
{
	cv::Mat pBitsDest;
	pBitsSrc.copyTo(pBitsDest);

#ifdef USE_TBB
	//tbb::task_scheduler_init init;  // Automatic number of threads
	//tbb::task_scheduler_init init(tbb::task_scheduler_init::default_num_threads());  // Explicit number of threads

	std::vector<myFiltreTask> tasks;
	for (auto y = 0; y < bmHeight; y++)
	{
		for (auto x = 0; x < bmWidth; x++)
		{
			tasks.push_back(myFiltreTask(x, y, pBitsSrc, pBitsDest, this));
		}
	}


	tbb::parallel_for(
		0, bmHeight * bmWidth, 1, [=](int y)
		{
			//for (size_t i = r.begin(); i < r.end(); ++i) 
			myFiltreTask task = tasks[y];
			task.ExecuteFilter();
		}
	);

#else


		for (auto y = 0; y < bmHeight; y++)
		{

			for (auto x = 0; x < bmWidth; x++)
			{
				PixelCompute(x, y, pBitsSrc, pBitsDest);
			}
		}
#endif

	pBitsDest.copyTo(pBitsSrc);
}


void CMatrixConvolution::PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest)
{
	float red = 0.0;
	float green = 0.0;
	float blue = 0.0;
	int k = 0;
	int start = -Ksize / 2;
	int end = Ksize / 2;
	int Kfactor = 0;

	for (auto i = start; i <= end; i++)
	{
		for (auto j = start; j <= end; j++)
		{
			Kfactor += kernel[k];
			int localX = x + j;
			int localY = y + i;
			if (localX >= 0 && localX < bmWidth && localY < bmHeight && localY >= 0)
			{
				red += static_cast<float>(pBitsSrc.at<cv::Vec3b>(y + i, x + j)[0]) * kernel[k];
				green += static_cast<float>(pBitsSrc.at<cv::Vec3b>(y + i, x + j)[1]) * kernel[k];
				blue += static_cast<float>(pBitsSrc.at<cv::Vec3b>(y + i, x + j)[2]) * kernel[k];
			}
			k++;
		}
	}

	red = (red / Kfactor) + Koffset;
	green = (green / Kfactor) + Koffset;
	blue = (blue / Kfactor) + Koffset;

	red = red < 0.0 ? 0.0 : red;
	uint8_t r = red > 255.0 ? 255 : red;

	green = green < 0 ? 0 : green;
	uint8_t g = green > 255 ? 255 : green;

	blue = blue < 0 ? 0 : blue;
	uint8_t b = blue > 255 ? 255 : blue;

	pBitsDest.at<cv::Vec3b>(y, x)[0] = r;
	pBitsDest.at<cv::Vec3b>(y, x)[1] = g;
	pBitsDest.at<cv::Vec3b>(y, x)[2] = b;
}

float CNoise::Noise2d(int x, int y)
{
	int n = ((x + (y << 6)) << 13) ^ (x + (y << 6));
	return 255 * (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

void CNoise::PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest)
{
	//int pos = GetPosition(x, y);
	//uint8_t alpha = pBitsSrc.data[pos + 3];
	float n = Noise2d(x, y);

	int r = pBitsSrc.at<cv::Vec3b>(y, x)[0] + n;
	int g = pBitsSrc.at<cv::Vec3b>(y, x)[1] + n;
	int b = pBitsSrc.at<cv::Vec3b>(y, x)[2] + n;

	r = max(0, min(255, r));
	g = max(0, min(255, g));
	b = max(0, min(255, b));


	pBitsDest.at<cv::Vec3b>(y, x)[0] = r;
	pBitsDest.at<cv::Vec3b>(y, x)[1] = g;
	pBitsDest.at<cv::Vec3b>(y, x)[2] = b;

	//uint8_t data[4] = { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), alpha };
	//memcpy(pBitsDest.data + pos, data, 4);
}

void CMosaic::PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest)
{
	//uint8_t r = 0;
	//uint8_t g = 0;
	//uint8_t b = 0;

	float w = size;
	float h = size;

	float s = floor(x / w);
	float t = floor(y / h);

	pBitsDest.at<cv::Vec3b>(y, x)[0] = pBitsSrc.at<cv::Vec3b>(t * h, s * w)[0];
	pBitsDest.at<cv::Vec3b>(y, x)[1] = pBitsSrc.at<cv::Vec3b>(t * h, s * w)[1];
	pBitsDest.at<cv::Vec3b>(y, x)[2] = pBitsSrc.at<cv::Vec3b>(t * h, s * w)[2];
}

float CSwirl::EuclideanDist(FLOATPOINT p)
{
	return abs(p.x - p.y);
}

float CSwirl::EuclideanDist(FLOATPOINT p, FLOATPOINT q)
{
	FLOATPOINT diff;
	diff.x = p.x - q.x;
	diff.y = p.y - q.y;
	return sqrt(diff.x * diff.x + diff.y * diff.y);
}

float CSwirl::DotProduct(FLOATPOINT p, FLOATPOINT q)
{
	float dot;
	dot = p.x * q.x + p.y * q.y;
	return dot;
}

wxPoint CSwirl::PostFX(int x, int y, int width, int height, float radius, float angleDegree)
{
	//Calcul du centre
	FLOATPOINT pt;
	FLOATPOINT pttest;
	wxPoint ptOut;
	FLOATPOINT ptCentre;

	ptCentre.x = static_cast<float>(width) / 2.0;
	ptCentre.y = static_cast<float>(height) / 2.0;

	pttest.x = x;
	pttest.y = y;

	pt.x = x - ptCentre.x;
	pt.y = y - ptCentre.y;

	float angle = angleDegree * 0.0174532925;

	float dist = EuclideanDist(pttest, ptCentre);
	if (dist < radius)
	{
		float percent = (radius - dist) / radius;
		float theta = percent * percent * angle * 8.0;
		float s = sin(theta);
		float c = cos(theta);
		FLOATPOINT pt1;
		pt1.x = c;
		pt1.y = -s;

		FLOATPOINT pt2;
		pt2.x = s;
		pt2.y = c;

		FLOATPOINT pt_out;
		pt_out.x = DotProduct(pt, pt1);
		pt_out.y = DotProduct(pt, pt2);

		pt = pt_out;

		//tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
	}
	pt.x += ptCentre.x;
	pt.y += ptCentre.y;


	ptOut.x = pt.x;
	ptOut.y = pt.y;
	return ptOut;
}

void CSwirl::PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest)
{
	//uint8_t data[4];
	wxPoint pt = PostFX(x, y, bmWidth, bmHeight, radius, angle);
	if (pt.x >= 0 && pt.x < bmWidth && pt.y >= 0 && pt.y < bmHeight)
	{
		pBitsDest.at<cv::Vec3b>(y, x)[0] = pBitsSrc.at<cv::Vec3b>(pt.y, pt.x)[0];
		pBitsDest.at<cv::Vec3b>(y, x)[1] = pBitsSrc.at<cv::Vec3b>(pt.y, pt.x)[1];
		pBitsDest.at<cv::Vec3b>(y, x)[2] = pBitsSrc.at<cv::Vec3b>(pt.y, pt.x)[2];
	}
	else
	{
		pBitsDest.at<cv::Vec3b>(y, x)[0] = color.GetRed();
		pBitsDest.at<cv::Vec3b>(y, x)[1] = color.GetGreen();
		pBitsDest.at<cv::Vec3b>(y, x)[2] = color.GetBlue();
	}
}

void CPosterize::PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest)
{
	int r = static_cast<float>(pBitsSrc.at<cv::Vec3b>(y, x)[0]) / _offset;
	int g = static_cast<float>(pBitsSrc.at<cv::Vec3b>(y, x)[1]) / _offset;
	int b = static_cast<float>(pBitsSrc.at<cv::Vec3b>(y, x)[2]) / _offset;

	pBitsDest.at<cv::Vec3b>(y, x)[0] = posterize[r];
	pBitsDest.at<cv::Vec3b>(y, x)[1] = posterize[g];
	pBitsDest.at<cv::Vec3b>(y, x)[2] = posterize[b];
}

void CSolarize::PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest)
{
	pBitsDest.at<cv::Vec3b>(y, x)[0] = solarize[pBitsSrc.at<cv::Vec3b>(y, x)[0]];
	pBitsDest.at<cv::Vec3b>(y, x)[1] = solarize[pBitsSrc.at<cv::Vec3b>(y, x)[1]];
	pBitsDest.at<cv::Vec3b>(y, x)[2] = solarize[pBitsSrc.at<cv::Vec3b>(y, x)[2]];
}
