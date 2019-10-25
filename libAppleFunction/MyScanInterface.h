#ifdef __APPLE__
#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MyScannerImpl
{
public:
    MyScannerImpl ( void );
    ~MyScannerImpl( void );

    void ScanDocument();


};
#endif