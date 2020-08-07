#include <header.h>
#include "InfosSeparationBarFace.h"
#include <WindowMain.h>
#include <ChangeLabel.h>
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
#include <LibResource.h>
#include <SqlFaceLabel.h>
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Sqlite;

CInfosSeparationBarFace::~CInfosSeparationBarFace(void)
{
	
}

bool CInfosSeparationBarFace::GetSelected()
{
	return isSelectIcone;
}

CInfosSeparationBarFace::CInfosSeparationBarFace(const CThemeInfosSeparationBar &theme)
	: CInfosSeparationBar(theme)
{
	parentWindow = nullptr;
	isSelected = false;
	isSelectIcone = false;
   // checkOnVector = CLibResource::GetVector(L"IDB_CHECKBOX_ON");
   // checkOffVector = CLibResource::GetVector(L"IDB_CHECKBOX_OFF");
	//editVector = CLibResource::GetVector(L"IDB_EDIT_LABEL");
	libelleSelectAll = CLibResource::LoadStringFromResource(L"LBLSELECTABLE", 1);
	libelleSelectIcone = CLibResource::LoadStringFromResource(L"LBLSelectAll", 1);
}

int CInfosSeparationBarFace::GetNumFace()
{
	return this->numFace;
}

void CInfosSeparationBarFace::SetParentWindow(wxWindow * parentWindow)
{
	this->parentWindow = parentWindow;
}

void CInfosSeparationBarFace::SetNumFace(const CFaceName &faceName)
{
	this->numFace = faceName.numFace;
	this->isSelected = faceName.isSelectable;
}

void CInfosSeparationBarFace::OnClick(const int &x, const int &y)
{
	bool updateInfos = false;

	if ((rcSelect.x < x && x < (rcSelect.x + rcSelect.width)) && ((rcSelect.y) < y && y < (rcSelect.y + rcSelect.height)))
	{
		isSelected = !isSelected;

		//Mise à jour de la base de données
		CSqlFaceLabel sqlFaceLabel;
		sqlFaceLabel.UpdateFaceLabel(numFace, isSelected);
		
		updateInfos = true;
	}
	else if ((rcSelectIcone.x < x && x < (rcSelectIcone.x + rcSelectIcone.width)) && ((rcSelectIcone.y) < y && y < (rcSelectIcone.y + rcSelectIcone.height)))
	{
		isSelectIcone = !isSelectIcone;
	}
	else if ((xPosEdit < x && x < (xPosEdit + bitmapEdit.GetWidth())) && (yPosEdit < y && y < (yPosEdit + bitmapEdit.GetHeight())))
	{
		//Modification du titre
		ChangeLabel changeLabel(parentWindow);
		changeLabel.SetLabel(title);
		changeLabel.ShowModal();
		if (changeLabel.IsOk())
		{
			wxString newLabel = changeLabel.GetNewLabel();
			CSqlFaceLabel sqlFaceLabel;
			sqlFaceLabel.UpdateFaceLabel(numFace, newLabel);
			title = newLabel;
			updateInfos = true;

		}
	}


	if(updateInfos)
	{
		CWindowMain * windowMain = (CWindowMain *)parentWindow->FindWindowById(MAINVIEWERWINDOWID);
		if(windowMain != nullptr)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_FACEINFOSUPDATE);
			windowMain->GetEventHandler()->AddPendingEvent(evt);
		}
	}
}


void CInfosSeparationBarFace::RenderIcone(wxDC * deviceContext, const int &posLargeur, const int &posHauteur)
{
	RenderTitle(deviceContext);
    int x = 0;
    int y = 0;	   
	
	if (!bitmapCheckOn.IsOk() || (bitmapCheckOn.GetHeight() != theme.GetCheckboxHeight() || bitmapCheckOn.GetWidth() != theme.GetCheckboxWidth()))
    {
        bitmapCheckOn = CLibResource::CreatePictureFromSVG("IDB_CHECKBOX_ON", theme.GetCheckboxWidth(), theme.GetCheckboxHeight());
        bitmapCheckOn = bitmapCheckOn.ConvertToDisabled();
    }
        
	if (!bitmapCheckOff.IsOk() ||  (bitmapCheckOff.GetHeight() != theme.GetCheckboxHeight() || bitmapCheckOff.GetWidth() != theme.GetCheckboxWidth()))
    {
        bitmapCheckOff = CLibResource::CreatePictureFromSVG("IDB_CHECKBOX_OFF", theme.GetCheckboxWidth(), theme.GetCheckboxHeight());
        bitmapCheckOff = bitmapCheckOff.ConvertToDisabled();
    }

	if(!bitmapEdit.IsOk())
	{
		bitmapEdit = CLibResource::CreatePictureFromSVG("IDB_EDIT_LABEL", theme.GetCheckboxWidth(), theme.GetCheckboxHeight());
		bitmapEdit = bitmapEdit.ConvertToDisabled();
	}

	if (bitmapEdit.IsOk())
		deviceContext->DrawBitmap(bitmapEdit, titleRectPos.x + titleRectPos.width + 5, titleRectPos.y);
  
	xPosEdit = _xPos + posLargeur + titleRectPos.x + titleRectPos.width + 5;
	yPosEdit = _yPos + posHauteur + titleRectPos.y;

	int xPos = x;
	int yPos = y + (theme.GetHeight() - bitmapCheckOn.GetHeight());
    
	rcSelect.x = _xPos + xPos + posLargeur;
	rcSelect.y = _yPos + yPos + posHauteur;
	rcSelect.width = bitmapCheckOn.GetWidth();
	rcSelect.height = bitmapCheckOn.GetHeight();

    if (isSelected && bitmapCheckOn.IsOk())
        deviceContext->DrawBitmap(bitmapCheckOn, xPos, yPos);
    else if (bitmapCheckOff.IsOk())
        deviceContext->DrawBitmap(bitmapCheckOff, xPos, yPos);
    
    wxSize size = CWindowMain::GetSizeTexte(deviceContext, libelleSelectAll, theme.themeFont);
    
    xPos = xPos + 5 + bitmapCheckOn.GetWidth();
    yPos = y + (theme.GetHeight() - size.y) - (bitmapCheckOn.GetHeight() - size.y) / 2;
    
    CWindowMain::DrawTexte(deviceContext, libelleSelectAll, xPos, yPos, theme.themeFont);

	xPos += size.x + 5;
	yPos = y + (theme.GetHeight() - bitmapCheckOn.GetHeight());

    if (isSelectIcone && bitmapCheckOn.IsOk())
        deviceContext->DrawBitmap(bitmapCheckOn, xPos, yPos);
    else if (bitmapCheckOff.IsOk())
        deviceContext->DrawBitmap(bitmapCheckOff, xPos, yPos);

	rcSelectIcone.x = _xPos + xPos + posLargeur;
	rcSelectIcone.y = _yPos + yPos + posHauteur;
	rcSelectIcone.width = bitmapCheckOn.GetWidth();
	rcSelectIcone.height = bitmapCheckOn.GetHeight();
    
    size = CWindowMain::GetSizeTexte(deviceContext, libelleSelectAll, theme.themeFont);
    
    xPos = xPos + 5 + bitmapCheckOn.GetWidth();
    yPos = y + (theme.GetHeight() - size.y) - (bitmapCheckOn.GetHeight() - size.y) / 2;
    
    CWindowMain::DrawTexte(deviceContext, libelleSelectIcone, xPos, yPos, theme.themeFont);

	
}
