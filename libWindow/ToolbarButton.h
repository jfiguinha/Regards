#pragma once
#include <theme.h>
#include "ToolbarElement.h"
using namespace Regards::Window;

class CRegardsBitmap;

class CToolbarButton : public CToolbarElement
{
public:
	CToolbarButton(const CThemeToolbarButton & theme);
	virtual ~CToolbarButton();

	void SetLibelle(const wxString &libelle);
	void SetButtonResourceId(const wxString &resourceId);
	void DrawButton(wxDC * dc, const int &x, const int &y);
	void Resize(const int &tailleX, const int &tailleY);
    void ReplaceColor(const wxColor & colorToReplace, const wxColor & colorActifReplacement, const wxColor & colorInactifReplacement);
	int GetWidth();
	int GetHeight();
    void SetBorder(const int &size);
	int GetBitmapWidth();
	int GetBitmapHeight();

protected:

	void CalculNewSize(int &imageWidth, int &imageHeight);
	virtual void DrawElement(wxDC * dc, const int &x, const int &y, const bool &inactif = false);

private:
    wxImage CreateFromSVG(const int & buttonWidth, const int & buttonHeight);
    CRegardsBitmap *   BilinearFiltering(CRegardsBitmap *  pBitmap);
	void CreateInactifButton(wxDC * dc, const int &x, const int &y);
	void CreateActifButton(wxDC * dc, const int &x, const int &y);
	void CreatePushButton(wxDC * dc, const int &x, const int &y);
	void DrawShapeElement(wxDC * dc, const wxRect &rc);
	bool LoadPictureFromResource();
			
    
	int width;
	int height;
	wxString resourceId;
			
	wxString resourceFolder;
	string resourceExtension;

	wxString libelle;
	int buttonWidth;
	int buttonHeight;

    wxString vector;
    wxImage button;
	CThemeToolbarButton themeButton;
    wxImage wxBackground;
    
    wxColor colorToReplace;
    wxColor colorActifReplacement;
    wxColor colorInactifReplacement;
};
