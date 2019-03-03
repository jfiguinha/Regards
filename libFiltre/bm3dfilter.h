#pragma once
#include <RegardsBitmap.h>
#include "MultiThreadFiltre.h"
#include <wx/gdicmn.h>

class CBm3DFilter : public IMultiThreadFiltre
{
public:
	CBm3DFilter(CRegardsBitmap * & bitmap, const float &sigma);
	~CBm3DFilter();
    int DetermineData(const int &nbProcess);
	int ExecuteFilter();
    int NextStep(wxDialog * dialog);
    int GetNbLoop();
    int GetNumLoop();
    int EndThread(const int &numThread);
    wxString FilterName()
    {
        return "Bm3D Filter";
    };
    
private:

    
	static void DecodeFrame(void * dataToDecode);

    CRegardsBitmap * bitmap = nullptr;
    float fSigma = 25;
    vector<thread *> listThread;
    vector<wxPoint> listPicture;
    //! Variables initialization
    const char *_tau_2D_hard = "bior";
    const char *_tau_2D_wien = "dct";
    const char *_color_space = "opp";
    const char *_patch_size = "0"; // >0: overrides default
    const char *_nb_threads = "1";
    const bool useSD_1 = false;
    const bool useSD_2 = false;
    const bool verbose = false;
    unsigned int width, height, chnls = 3;
    //int nbProcess = 1;
    int patch_size;
    int nb_threads;
    unsigned int tau_2D_wien;
    unsigned int tau_2D_hard;
    unsigned int color_space;
    bool start = false;
    int nbloop = 0;
    int numloop = 0;
    int size = 256;
};