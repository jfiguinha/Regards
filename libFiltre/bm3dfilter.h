#pragma once
#include <RegardsBitmap.h>

class CBm3DFilter
{
public:
	CBm3DFilter(CRegardsBitmap * bitmapSource, const float &sigma);
	~CBm3DFilter();
    int DetermineData(const int &nbProcess);
	int ExecuteFilter();
	CRegardsBitmap * GetResult();
    wxString FilterName()
    {
        return "Bm3D Filter";
    };
    
private:

    CRegardsBitmap * bitmap = nullptr;
    float fSigma = 25;
    vector<wxPoint> listPicture;
    //! Variables initialization
    const char *_tau_2D_hard = "bior";
    const char *_tau_2D_wien = "dct";
    const char *_color_space = "opp";
    const char *_patch_size = "0"; // >0: overrides default
    //const char *_nb_threads = "1";
    const bool useSD_1 = false;
    const bool useSD_2 = false;
    const bool verbose = false;
   // unsigned int width, height, chnls = 1;
    unsigned int chnls = 1;
    //int nbProcess = 1;
    int patch_size;
    int nb_threads;
    unsigned int tau_2D_wien;
    unsigned int tau_2D_hard;
    unsigned int color_space;
};