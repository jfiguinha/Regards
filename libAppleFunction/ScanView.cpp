#ifdef __APPLE__
#include "ScanView.h"
#include "MyScanInterface.h"

CScanView::CScanView( void )
{
    _impl = new MyScannerImpl();
}


CScanView::~CScanView( void )
{
    if ( _impl ) { delete _impl; _impl = NULL; }
}


void CScanView::ScanDocument()
{
    if ( _impl )
        _impl->ScanDocument();
}
#endif