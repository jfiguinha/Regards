#include <header.h>
#include "ffmpeg_denoise.h"
#include <RegardsBitmap.h>
#include <algorithm>
extern float value[256];

#define ABS(A) ( (A) > 0 ? (A) : -(A) )

static void PrecalcCoefs(int *Ct, double Dist25)
{
	int i;
	double Gamma, Simil, C;

	Gamma = log(0.25) / log(1.0 - Dist25 / 255.0 - 0.00001);

	for (i = -255 * 16; i < 256 * 16; i++)
	{
		Simil = 1.0 - ABS(i) / (16 * 255.0);
		C = pow(Simil, Gamma) * 65536.0 * (double)i / 16.0;
		Ct[16 * 256 + i] = (int)((C < 0) ? (C - 0.5) : (C + 0.5));
	}
}

static inline unsigned int LowPassMul(unsigned int PrevMul, unsigned int CurrMul, int* Coef) {
	//	int dMul= (PrevMul&0xFFFFFF)-(CurrMul&0xFFFFFF);
	int dMul = PrevMul - CurrMul;
	int d = ((dMul + 0x10007FF) / (65536 / 16));
	return CurrMul + Coef[d];
}

void CFFmpegDenoise::deNoise(unsigned short * Frame,  // mpi->planes[x]
	unsigned char *FrameDest,    // dmpi->planes[x]
	unsigned int *LineAnt,       // vf->priv->Line (width bytes)
	unsigned short *FrameAnt,
	int W, int H, int sStride, int dStride,
	int *Horizontal, int *Vertical, int *Temporal)
{
	int X, Y;
	int sLineOffs = 0, dLineOffs = 0;
	unsigned int PixelAnt;
	int PixelDst;

	/* First pixel has no left nor top neightbour. Only previous frame */
	LineAnt[0] = PixelAnt = Frame[0] << 16;
	PixelDst = LowPassMul(FrameAnt[0] << 8, PixelAnt, Temporal);
	FrameAnt[0] = ((PixelDst + 0x1000007F) / 256);
	FrameDest[0] = ((PixelDst + 0x10007FFF) / 65536);

	/* Fist line has no top neightbour. Only left one for each pixel and
		* last frame */
	for (X = 1; X < W; X++) {
		LineAnt[X] = PixelAnt = LowPassMul(PixelAnt, Frame[X] << 16, Horizontal);
		PixelDst = LowPassMul(FrameAnt[X] << 8, PixelAnt, Temporal);
		FrameAnt[X] = ((PixelDst + 0x1000007F) / 256);
		FrameDest[X] = ((PixelDst + 0x10007FFF) / 65536);
	}

	for (Y = 1; Y < H; Y++) {
		unsigned int PixelAnt;
		unsigned short* LinePrev = &FrameAnt[Y*W];
		sLineOffs += sStride, dLineOffs += dStride;
		/* First pixel on each line doesn't have previous pixel */
		PixelAnt = Frame[sLineOffs] << 16;
		LineAnt[0] = LowPassMul(LineAnt[0], PixelAnt, Vertical);
		PixelDst = LowPassMul(LinePrev[0] << 8, LineAnt[0], Temporal);
		LinePrev[0] = ((PixelDst + 0x1000007F) / 256);
		FrameDest[dLineOffs] = ((PixelDst + 0x10007FFF) / 65536);

		for (X = 1; X < W; X++) {
			int PixelDst;
			/* The rest are normal */
			PixelAnt = LowPassMul(PixelAnt, Frame[sLineOffs + X] << 16, Horizontal);
			LineAnt[X] = LowPassMul(LineAnt[X], PixelAnt, Vertical);
			PixelDst = LowPassMul(LinePrev[X] << 8, LineAnt[X], Temporal);
			LinePrev[X] = ((PixelDst + 0x1000007F) / 256);
			FrameDest[dLineOffs + X] = ((PixelDst + 0x10007FFF) / 65536);
		}
	}
}


CFFmpegDenoise::CFFmpegDenoise()
{

}

CFFmpegDenoise::~CFFmpegDenoise()
{

}

CRegardsBitmap * CFFmpegDenoise::ApplyDenoise3D(double LumSpac, double ChromSpac, double LumTmp, double ChromTmp, CRegardsBitmap * bitmapIn)
{

	CRegardsBitmap * bitmapOut = new CRegardsBitmap(bitmapIn->GetBitmapWidth(), bitmapIn->GetBitmapHeight());
	unsigned short * picture_y = new unsigned short[bitmapIn->GetBitmapWidth() * bitmapIn->GetBitmapHeight()];
	unsigned short * picture_u = new unsigned short[bitmapIn->GetBitmapWidth() * bitmapIn->GetBitmapHeight()];
	unsigned short * picture_v = new unsigned short[bitmapIn->GetBitmapWidth() * bitmapIn->GetBitmapHeight()];


	if (LumSpac < 0)
		LumSpac = 4.0;
	if (ChromSpac < 0)
		ChromSpac = .75 * LumSpac;
	if (LumTmp < 0)
		LumTmp = 1.5 * LumSpac;
	if (ChromTmp < 0) {
		if (LumSpac == 0)
			ChromTmp = ChromSpac * 1.5;
		else
			ChromTmp = LumTmp * ChromSpac / LumSpac;
	}
	LumSpac = __min(254.9, LumSpac);
	ChromSpac = __min(254.9, ChromSpac);
	LumTmp = __min(254.9, LumTmp);
	ChromTmp = __min(254.9, ChromTmp);

	if (restart_lap < 0)
		restart_lap = __max(2, (int)(1 + __max(LumTmp, ChromTmp)));

	PrecalcCoefs(Coefs[0], LumSpac);
	PrecalcCoefs(Coefs[1], LumTmp);
	PrecalcCoefs(Coefs[2], ChromSpac);
	PrecalcCoefs(Coefs[3], ChromTmp);

	w = bitmapIn->GetBitmapWidth();
	h = bitmapIn->GetBitmapHeight();
	cw = w >> 1;
	ch = h >> 1;

	unsigned int * Line = new unsigned int[w];
	Frame[0] = new unsigned short[w*h];
	Frame[1] = new unsigned short[cw*ch];
	Frame[2] = new unsigned short[cw*ch];

	uint8_t* data = bitmapIn->GetPtBitmap();
	int posPicture = 0;
	int posLocal = 0;
	for (int posY = 0; posY < bitmapIn->GetBitmapHeight(); posY++)
	{
		for (int posX = 0; posX < bitmapIn->GetBitmapWidth(); posX++)
		{
			picture_y[posLocal] = 0.257f * value[data[posPicture + 2]] + 0.504f * value[data[posPicture + 1]] + 0.098f * value[data[posPicture]] + 16;
			picture_v[posLocal] = (0.439 * data[posPicture + 2]) - (0.368 * data[posPicture + 1]) - (0.071 * data[posPicture]) + 128;
			picture_u[posLocal] = -(0.148 * data[posPicture + 2]) - (0.291 * data[posPicture + 1]) + (0.439 * data[posPicture]) + 128;

			posLocal++;
			posPicture += 4;
		}
	}

	uint8_t * y_out = new uint8_t[posLocal];
	uint8_t * u_out = new uint8_t[posLocal];
	uint8_t * v_out = new uint8_t[posLocal];

	deNoise(picture_y,
		y_out,
		Line, Frame[0], w, h,
		bitmapIn->GetBitmapWidth(),
		bitmapIn->GetBitmapWidth(),
		Coefs[0], Coefs[0], Coefs[1]);
	deNoise(picture_u,
		u_out,
		Line, Frame[1], cw, ch,
		bitmapIn->GetBitmapWidth(),
		bitmapIn->GetBitmapWidth(),
		Coefs[2], Coefs[2], Coefs[3]);
	deNoise(picture_v,
		v_out,
		Line, Frame[2], cw, ch,
		bitmapIn->GetBitmapWidth(),
		bitmapIn->GetBitmapWidth(),
		Coefs[2], Coefs[2], Coefs[3]);

	posLocal = 0;
	posPicture = 0;
	data = bitmapOut->GetPtBitmap();

	for (int posY = 0; posY < bitmapOut->GetBitmapHeight(); posY++)
	{
		for (int posX = 0; posX < bitmapOut->GetBitmapWidth(); posX++)
		{
			float Y = picture_y[posLocal];
			float V = picture_v[posLocal];
			float U = picture_u[posLocal];

			double B = 1.164 * (Y - 16) + 2.018 * (U - 128);
			double G = 1.164 * (Y - 16) - 0.813 * (V - 128) - 0.391 * (U - 128);
			double R = 1.164 * (Y - 16) + 1.596 * (V - 128);

			data[posPicture + 2] = min(max(R,0.0),255.0);//min(Y + 1.13983f * V, 255.0f); //R
			data[posPicture + 1] = min(max(G, 0.0), 255.0);//min(Y - 0.39465f * U - 0.5806f * V, 255.0f); //G
			data[posPicture] = min(max(B, 0.0), 255.0);//min(Y + 2.03211f * U, 255.0f); //B
			posPicture += 4;
			posLocal++;
		}
	}

	return bitmapOut;
}