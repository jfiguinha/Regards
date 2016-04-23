//
//  glPane.h
//  libMinimal
//
//  Created by figuinha jacques on 28/05/2015.
//  Copyright (c) 2015 figuinha jacques. All rights reserved.
//
#pragma once
#include <GLcontext.h>
#include "wx/wx.h"
#include "wx/glcanvas.h"


class CVideoWindowContext : public wxGLContext
{
    
public:
    CVideoWindowContext(wxGLCanvas *canvas);

};


