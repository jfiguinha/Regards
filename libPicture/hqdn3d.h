#pragma once
class CRegardsBitmap;

class Chqdn3d
{
public:
	Chqdn3d(const int &w, const int &h, const double & LumSpac, const double & ChromSpac = 4, const double & LumTmp = 3, const double & ChromTmp = 3);
	~Chqdn3d();
	int ApplyDenoise3D(CRegardsBitmap * bitmapIn);
	uint8_t * ApplyDenoise3D(uint8_t * picture_y, const int &w, const int &h);

private:

	void deNoise(uint8_t * Frame,  // mpi->planes[x]
		uint8_t * FrameDest,    // dmpi->planes[x]
		unsigned int *LineAnt,       // vf->priv->Line (width bytes)
		uint8_t *FrameAnt,
		int W, int H, int sStride, int dStride,
		int *Horizontal, int *Vertical, int *Temporal);

	int Coefs[4][512 * 16];

	//int restart_lap = 0;
	//int prev_frame;
	int h, w;
	uint8_t * Frame = nullptr;
	uint8_t * y_out = nullptr;
	unsigned int * Line = nullptr;
	uint8_t * picture_y = nullptr;

};