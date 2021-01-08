#pragma once
#include <map>
class CDenoise3DPimpl;
class CRegardsBitmap;

class CFFmpegDenoise
{
public:
	CFFmpegDenoise();
	~CFFmpegDenoise();
	CRegardsBitmap * ApplyDenoise3D(double LumSpac, double ChromSpac, double LumTmp, double ChromTmp, CRegardsBitmap * bitmapIn);

private:
	int Coefs[4][512 * 16];
	unsigned int * Line;
	unsigned short * Frame[3];
	int restart_lap = 0;
	int prev_frame;
	int w, cw, h, ch;

	void deNoise(unsigned short * Frame,  // mpi->planes[x]
		unsigned char *FrameDest,    // dmpi->planes[x]
		unsigned int *LineAnt,       // vf->priv->Line (width bytes)
		unsigned short *FrameAnt,
		int W, int H, int sStride, int dStride,
		int *Horizontal, int *Vertical, int *Temporal);
};