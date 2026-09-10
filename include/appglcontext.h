#pragma once
#include <wx/glcanvas.h>

class AppGLContext
{
public:

	AppGLContext()
	{
        // 2. Création du contexte associé avec les attributs Core Profile
        
        ctxAttrs.CoreProfile().OGLVersion(3, 3).EndList(); 
	}

    // 1. Définition des attributs visuels de la fenêtre de rendu
    static wxGLAttributes& GetCanvasAttributes()
    {
        static wxGLAttributes attrs;
        attrs.PlatformDefaults()
             .RGBA()
             .DoubleBuffer()
             .Depth(24)
             .Stencil(8)
             .EndList();
        return attrs;
    }


	wxGLContextAttrs ctxAttrs;

};