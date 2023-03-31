#pragma once
class CRegardsBitmap;

class Chqdn3d
{
public:
	/*
#define HQDN3D_SPATIAL_LUMA_DEFAULT    4.0f
#define HQDN3D_SPATIAL_CHROMA_DEFAULT  3.0f
#define HQDN3D_TEMPORAL_LUMA_DEFAULT   6.0f
*/
	Chqdn3d(const int& w, const int& h, const double& LumSpac = 4, const double& temporalLumaDefault=6.0, const double& temporalSpatialLumaDefault=4.0);
	~Chqdn3d();
	void UpdateParameter(const int& w, const int& h, const double& LumSpac = 4, const double& temporalLumaDefault = 6.0, const double& temporalSpatialLumaDefault = 4.0);
	int ApplyDenoise3D(cv::Mat& bitmapIn);
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
	double spatial_luma = 0;
	double temporal_luma = 0;
};
