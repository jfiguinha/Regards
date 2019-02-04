#pragma once

class IMultiThreadFiltre
{
public:
    virtual int NextStep(wxDialog * dialog) = 0;
    virtual int DetermineData(const int &nbProcess) = 0;
    virtual int GetNbLoop() = 0;
    virtual int GetNumLoop() = 0;
    virtual wxString FilterName() = 0;
    virtual int EndThread(const int &numThread) = 0;
};