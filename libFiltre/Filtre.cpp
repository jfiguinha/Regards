#include <header.h>
#include "Filtre.h"

using namespace Regards::FiltreEffet;

CFiltre::CFiltre(): bmWidth(0), bmHeight(0)
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
	ProcessOpenCV(pBitsSrc);
}

CMatrixConvolution::CMatrixConvolution(const short* kernel,
	int kernelSize,
	int kernelFactor,
	int kernelOffset)
	: offset(kernelOffset)
{
	CV_Assert(kernel != nullptr);
	CV_Assert(kernelSize > 0);

	kernelMat.create(kernelSize, kernelSize, CV_32F);

	float factor = 0.f;

	for (int y = 0; y < kernelSize; y++)
	{
		float* dst = kernelMat.ptr<float>(y);

		for (int x = 0; x < kernelSize; x++)
		{
			float value = static_cast<float>(kernel[y * kernelSize + x]);

			dst[x] = value;

			factor += value;
		}
	}

	//
	// Si l'utilisateur fournit explicitement un facteur,
	// on l'utilise.
	//
	if (kernelFactor != 0)
	{
		kernelMat /= static_cast<float>(kernelFactor);
	}
	else
	{
		//
		// Sinon on normalise automatiquement lorsque c'est possible.
		//
		if (std::abs(factor) > FLT_EPSILON)
			kernelMat /= factor;
	}
}

/////////////////////////////////////////////////////////////////////////////

void CMatrixConvolution::ProcessOpenCV(cv::Mat& image)
{
	filter2D(image,
		image,
		-1,
		kernelMat,
		cv::Point(-1, -1),
		static_cast<double>(offset),
		cv::BORDER_REPLICATE);
}

float CNoise::Noise2d(int x, int y)
{
	int n = ((x + (y << 6)) << 13) ^ (x + (y << 6));
	return 255 * (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

cv::Mat CNoise::CreateNoise(const cv::Size& size)
{
	cv::Mat noise(size, CV_32F);

	for (int y = 0; y < size.height; y++)
	{
		float* dst = noise.ptr<float>(y);

		for (int x = 0; x < size.width; x++)
			dst[x] = Noise2d(x, y);
	}

	return noise;
}

void CNoise::ProcessOpenCV(cv::Mat& image)
{
	cv::Mat noise = CreateNoise(image.size());

	cv::Mat noise8;

	noise.convertTo(noise8, CV_8U, 128.0, 128.0);

	cv::Mat noise3;

	cvtColor(noise8, noise3, cv::COLOR_GRAY2BGR);

	add(image,
		noise3,
		image);
}

void CMosaic::ProcessOpenCV(cv::Mat& image)
{
	if (image.empty())
		return;

	const int width = std::max(1, image.cols / blockSize);
	const int height = std::max(1, image.rows / blockSize);

	cv::Mat small;

	resize(image,
		small,
		cv::Size(width, height),
		0.0,
		0.0,
		cv::INTER_LINEAR);

	resize(small,
		image,
		image.size(),
		0.0,
		0.0,
		cv::INTER_NEAREST);
}

void CSwirl::BuildMaps(const cv::Size& size,
	cv::Mat& mapX,
	cv::Mat& mapY)
{
	mapX.create(size, CV_32F);
	mapY.create(size, CV_32F);

	const float cx = size.width * 0.5f;
	const float cy = size.height * 0.5f;

	const float maxRadius =
		radius * std::min(size.width, size.height) * 0.5f;

	for (int y = 0; y < size.height; y++)
	{
		float* mx = mapX.ptr<float>(y);
		float* my = mapY.ptr<float>(y);

		for (int x = 0; x < size.width; x++)
		{
			float dx = x - cx;
			float dy = y - cy;

			float r = std::sqrt(dx * dx + dy * dy);

			if (r > maxRadius)
			{
				mx[x] = x;
				my[x] = y;
				continue;
			}

			float theta = atan2(dy, dx);

			float t =
				(maxRadius - r) / maxRadius;

			theta += angle * t;

			mx[x] = cx + r * cos(theta);
			my[x] = cy + r * sin(theta);
		}
	}
}

void CSwirl::ProcessOpenCV(cv::Mat& image)
{
	cv::Mat mapX;
	cv::Mat mapY;

	BuildMaps(image.size(),
		mapX,
		mapY);

	cv::Mat dst;

	cv::remap(image,
		dst,
		mapX,
		mapY,
		cv::INTER_LINEAR,
		cv::BORDER_REFLECT101);

	image = std::move(dst);
}