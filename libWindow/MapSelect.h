//
//  MapSelect.hpp
//  Regards.libExplorer
//
//  Created by figuinha jacques on 01/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#pragma once

namespace Regards
{
    namespace Window
    {
        class CMapDialogBox;
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

            CMapDialogBox * mapDialog = nullptr;
            wxString latitude;
            wxString longitude;

        };
    }
}