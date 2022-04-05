#pragma once
class CRegardsBitmap;

class Chqdn3d
{
public:
	Chqdn3d(const int& w, const int& h, const double& LumSpac = 4, const double& LumTmp = 6);
	~Chqdn3d();
	int ApplyDenoise3D(CRegardsBitmap* bitmapIn);
	uint8_t* ApplyDenoise3D(uint8_t* picture_y, const int& w, const int& h);

private:
	//unsigned int hqdn3d_lowpass_mul(int prev_mul,
	//                                int curr_mul,
	//                                short* coef);
	void hqdn3d_precalc_coef(short* ct,
	                         double dist25);
	void hqdn3d_denoise_temporal(unsigned char* frame_src,
	                             unsigned char* frame_dst,
	                             unsigned short* frame_ant,
	                             int w, int h,
	                             short* temporal);
	void hqdn3d_denoise_spatial(unsigned char* frame_src,
	                            unsigned char* frame_dst,
	                            unsigned short* line_ant,
	                            unsigned short* frame_ant,
	                            int w, int h,
	                            short* spatial,
	                            short* temporal);
	void hqdn3d_denoise(unsigned char* frame_src,
	                    unsigned char* frame_dst,
	                    unsigned short* line_ant,
	                    unsigned short** frame_ant_ptr,
	                    int w,
	                    int h,
	                    short* spatial,
	                    short* temporal);
	short hqdn3d_coef[2][512 * 16];
	int h, w;
	unsigned short* Frame = nullptr;
	uint8_t* y_out = nullptr;
	unsigned short* Line = nullptr;
	uint8_t* picture_y = nullptr;
};
