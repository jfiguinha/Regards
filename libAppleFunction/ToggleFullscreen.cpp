//
//  Hello.cpp
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//

#ifdef __APPLE__
#include "ToggleFullscreen.h"
#include "ToggleFullscreenImpl.h"


CToggleScreen::CToggleScreen( void )
{
    _impl = new ToggleScreenImpl();
}


CToggleScreen::~CToggleScreen( void )
{
    if ( _impl ) { delete _impl; _impl = NULL; }
}

void CToggleScreen::ToggleFullscreen(wxFrame * frame, int screenNumber)
{
     if ( _impl )
        return _impl->ToggleFullscreen(frame, screenNumber);   
}

void CToggleScreen::GetFullscreenSize(int & width, int & height)
{
     if ( _impl )
        return _impl->GetFullscreenSize(width, height);   
}



#endif