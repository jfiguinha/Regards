#ifdef __APPLE__
#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


class MyScannerImpl;

class CScanView
{
public:
    CScanView ( void );
    ~CScanView( void );
    
    void ScanDocument();
    
private:
    MyScannerImpl * _impl;
};

#endif