//
//  MapSelect.hpp
//  Regards.libExplorer
//
//  Created by figuinha jacques on 01/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
using namespace std;

namespace Regards
{
    namespace Window
    {
        class CMapSelect
        {
        public:
            CMapSelect();
            ~CMapSelect();
            
            wxString SelectNewMapLocalisation(wxWindow * window,const wxString & url);
            wxString GetLatitude();
            wxString GetLongitude();
            double GetLatitudeNumber();
            double GetLongitudeNumber();
        private:
            wxString latitude = "";
            wxString longitude = "";
            
        };
    }
}