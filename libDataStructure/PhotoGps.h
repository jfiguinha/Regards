//
//  PhotoGps.hpp
//  Regards.libDataStructure
//
//  Created by figuinha jacques on 29/09/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//
#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <vector>
using namespace std;

class CPhotoGps
{
public:
    CPhotoGps(){};
    ~CPhotoGps(){};
    
    void SetId(const int &numId);
    int GetId();
    
    void SetPath(const wxString &path);
    wxString GetPath();
    
    void SetLatitude(const wxString &latitude);
    wxString GetLatitude();
    double GetLatitudeNumber();
    
    void SetLongitude(const wxString &longitude);
    wxString GetLongitude();
    double GetLongitudeNumber();
    
private:
    int numId;
    wxString path;
    wxString latitude;
    wxString longitude;
};


typedef std::vector<CPhotoGps> PhotoGpsVector;