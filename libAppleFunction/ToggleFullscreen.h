//
//  Hello.h
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


class ToggleScreenImpl;

class CToggleScreen
{
public:
    CToggleScreen ( void );
    ~CToggleScreen( void );
    void ToggleFullscreen(wxFrame * frame, int screenNumber = 0);
    void GetFullscreenSize(int & width, int & height);
private:
    ToggleScreenImpl * _impl;
};




#endif