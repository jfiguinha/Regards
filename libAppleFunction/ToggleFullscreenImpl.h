//
//  HelloInterface.h
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//

#ifdef __APPLE__
#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class ToggleScreenImpl
{
public:
    ToggleScreenImpl( void );
    ~ToggleScreenImpl( void );

    void ToggleFullscreen(wxFrame * frame, int screenNumber);
    void GetFullscreenSize(int & width, int & height);
private:

      int _widthMax = 0;
      int _heightMax = 0;
};


#endif