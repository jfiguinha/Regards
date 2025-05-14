#include <header.h>
#include "InterpolationBicubic.h"


CInterpolationBicubic::CInterpolationBicubic(const double& dWidth)
	: wX(nullptr), wY(nullptr), m_dWidth(dWidth)
{
}


CInterpolationBicubic::~CInterpolationBicubic()
{
	// Libérer la mémoire existante pour éviter les fuites
	if (wX != nullptr)
	{
		delete[] wX;
		wX = nullptr;
	}

	if (wY != nullptr)
	{
		delete[] wY;
		wY = nullptr;
	}
}

void CInterpolationBicubic::CalculWeight(const int32_t &width, const int32_t &height, const float &ratioY, const float &ratioX, const float &posTop, const float &posLeft)
{
	// Libérer la mémoire existante pour éviter les fuites
	if (wX != nullptr)
	{
		delete[] wX;
		wX = nullptr;
	}

	if (wY != nullptr)
	{
		delete[] wY;
		wY = nullptr;
	}

	wX = new weightX[width];
	wY = new weightX[height];

#pragma omp parallel for
	for (auto y = 0; y < height; y++)
	{
		float posY = (float)y * ratioY + posTop;
		int valueB = (int)posY;
		float realB = posY - valueB;
		wY[y].tabF[0] = Filter(-(-1.0f - realB));
		wY[y].tabF[1] = Filter(-(0.0f - realB));
		wY[y].tabF[2] = Filter(-(1.0f - realB));
		wY[y].tabF[3] = Filter(-(2.0f - realB));
	}
#pragma omp parallel for
	for (auto x = 0; x < width; x++)
	{
		float posX = (float)x * ratioX + posLeft;
		int valueA = (int)posX;
		float realA = posX - valueA;
		wX[x].tabF[0] = Filter((-1.0f - realA));
		wX[x].tabF[1] = Filter((0.0f - realA));
		wX[x].tabF[2] = Filter((1.0f - realA));
		wX[x].tabF[3] = Filter((2.0f - realA));
	}
}

void CInterpolationBicubic::Execute(const cv::Mat & in, cv::Mat & out)
{
	// Vérification des dimensions des matrices
	if (in.empty() || out.empty() || in.rows <= 0 || in.cols <= 0 || out.rows <= 0 || out.cols <= 0)
	{
		throw std::invalid_argument("Les matrices d'entrée ou de sortie sont invalides.");
	}

	float ratioX = static_cast<float>(in.cols) / static_cast<float>(out.cols);
	float ratioY = static_cast<float>(in.rows) / static_cast<float>(out.rows);

	// Calcul des poids pour l'interpolation
	CalculWeight(out.cols, out.rows, ratioY, ratioX, 0.0f, 0.0f);

	// Allocation des tableaux pour les positions
	std::unique_ptr<int[]> posTabY(new int[out.rows]);
	std::unique_ptr<int[]> posTabX(new int[out.cols]);

	// Pré-calcul des positions Y
	for (int y = 0; y < out.rows; y++)
	{
		posTabY[y] = static_cast<int>(y * ratioY);
	}

	// Pré-calcul des positions X
	for (int x = 0; x < out.cols; x++)
	{
		posTabX[x] = static_cast<int>(x * ratioX);
	}

	// Interpolation bicubique
#pragma omp parallel for
	for (int y = 0; y < out.rows; y++)
	{
		for (int x = 0; x < out.cols; x++)
		{
			int i = x * 3 + (y * out.step);
			uchar* data = out.data + i;

			// Vérification des limites des positions
			if (posTabX[x] >= 0 && posTabX[x] < in.cols && posTabY[y] >= 0 && posTabY[y] < in.rows)
			{
				Bicubic(in, data, posTabX[x], posTabY[y], wY[y].tabF, wX[x].tabF);
			}
			else
			{
				// Valeurs par défaut en cas de position invalide
				data[0] = data[1] = data[2] = 0;
				data[3] = 255; // Alpha par défaut (opaque)
			}
		}
	}

}

void CInterpolationBicubic::Bicubic(const cv::Mat& in, uchar * & data, const int& x, const int& y, float* tabF1, float* tabF)
{

	float nDenom = 0.0f;
	float r = 0.0f, g = 0.0f, b = 0.0f;

	// Définir les limites pour les boucles
	int debutN = (y == 0) ? 0 : -1;
	int finN = (y >= in.rows - 2) ? (in.rows - y - 1) : 2;

	int debutM = (x == 0) ? 0 : -1;
	int finM = (x >= in.cols - 2) ? (in.cols - x - 1) : 2;

	int posX = x + debutM;
	int posY = y + debutN;

	for (int n = debutN; n <= finN; n++) {
		for (int m = debutM; m <= finM; m++) {
			int position = (posX + m) * 3 + (posY + n) * in.step;

			// Vérifier que la position est valide
			if (position < 0 || position >= in.total() * in.elemSize()) {
				continue;
			}

			float f = tabF1[n + 1] * tabF[m + 1];
			nDenom += f;
			r += in.data[position] * f;
			g += in.data[position + 1] * f;
			b += in.data[position + 2] * f;
		}
	}

	// Éviter la division par zéro
	if (nDenom > 0.0f) {
		data[0] = clamp(static_cast<int>(r / nDenom), 0, 255);
		data[1] = clamp(static_cast<int>(g / nDenom), 0, 255);
		data[2] = clamp(static_cast<int>(b / nDenom), 0, 255);
	}
	else {
		data[0] = data[1] = data[2] = 0; // Valeurs par défaut en cas d'erreur
	}
	data[3] = 255; // Alpha par défaut (opaque)

}

double CInterpolationBicubic::Filter(const double &f)
{
	
	return (f < -2.0 || f > 2.0) ? 0.0 : (
		(f < -1.0) ? (2.0 + f)*(2.0 + f)*(2.0 + f) / 6.0 : (
		(f < 0.0) ? (4.0 + f*f*(-6.0 - 3.0*f)) / 6.0 : (
		(f < 1.0) ? (4.0 + f*f*(-6.0 + 3.0*f)) / 6.0 : (2.0 - f)*(2.0 - f)*(2.0 - f) / 6.0)));
	
}