#include <header.h>
#include "InfosSeparationBarExplorer.h"
#include <WindowMain.h>
#include <LibResource.h>
using namespace Regards::Window;

CInfosSeparationBarExplorer::~CInfosSeparationBarExplorer(void)
{
}

bool CInfosSeparationBarExplorer::GetSelected()
{
	return isSelected;
}

bool CInfosSeparationBarExplorer::GetShow()
{
	return open;
}

void CInfosSeparationBarExplorer::ShowExpandIcon(const bool& showExpandIcon)
{
	this->showExpandIcon = showExpandIcon;
}


CInfosSeparationBarExplorer::CInfosSeparationBarExplorer(const CThemeInfosSeparationBar& theme)
	: CInfosSeparationBar(theme)
{
	libelleSelectAll = CLibResource::LoadStringFromResource(L"LBLSelectAll", 1);
	isSelected = false;
	open = true;
}

void CInfosSeparationBarExplorer::OnClick(const int& x, const int& y)
{
	if ((rcSelect.x < x && x < (rcSelect.x + rcSelect.width)) && (rcSelect.y < y && y < (rcSelect.y + rcSelect.height)))
	{
		isSelected = !isSelected;
	}
	if (showExpandIcon)
	{
		if ((rcShowSelect.x < x && x < (rcShowSelect.x + rcShowSelect.width)) && (rcShowSelect.y < y && y < (rcShowSelect.y + rcShowSelect.height)))
		{
			open = !open;
		}
	}

}

void CInfosSeparationBarExplorer::CreateIcone(wxImage & bitmap, const wxString & name)
{
	if (!bitmap.IsOk() || (bitmap.GetHeight() != theme.GetCheckboxHeight() || bitmap.GetWidth() !=
		theme.GetCheckboxWidth()))
	{
		bitmap = CLibResource::CreatePictureFromSVG(name, theme.GetCheckboxWidth(),
			theme.GetCheckboxHeight());
		bitmap = bitmap.ConvertToDisabled();
	}
}

void CInfosSeparationBarExplorer::RenderIcone(wxDC* deviceContext, const int& posLargeur, const int& posHauteur)
{
	RenderTitle(deviceContext);
	int x = 0;
	int y = 0;

	CreateIcone(bitmapCheckOn, "IDB_CHECKBOX_ON");
	CreateIcone(bitmapCheckOff, "IDB_CHECKBOX_OFF");
	CreateIcone(bitmapFolderExpand, "IDB_EXPANDFOLDER");
	CreateIcone(bitmapFolderContract, "IDB_RETRACTFOLDER");

	int xPos = x;
	int yPos = y + (theme.GetHeight() - bitmapCheckOn.GetHeight());

	
	if (showExpandIcon)
	{

		rcSelect.x = _xPos + xPos + bitmapCheckOn.GetWidth() + 10 + posLargeur;
		rcSelect.y = _yPos + yPos + posHauteur;
		rcSelect.width = bitmapCheckOn.GetWidth();
		rcSelect.height = bitmapCheckOn.GetHeight();

		rcShowSelect.x = _xPos + xPos + posLargeur;
		rcShowSelect.y = _yPos + yPos + posHauteur;
		rcShowSelect.width = bitmapCheckOn.GetWidth();
		rcShowSelect.height = bitmapCheckOn.GetHeight();

		if (isSelected)
			deviceContext->DrawBitmap(bitmapCheckOn, xPos + bitmapCheckOn.GetWidth() + 10, yPos);
		else
			deviceContext->DrawBitmap(bitmapCheckOff, xPos + bitmapCheckOn.GetWidth() + 10, yPos);

		if (open)
			deviceContext->DrawBitmap(bitmapFolderContract, xPos, yPos);
		else
			deviceContext->DrawBitmap(bitmapFolderExpand, xPos, yPos);

		wxSize size = CWindowMain::GetSizeTexte(deviceContext, libelleSelectAll, theme.themeFont);

		xPos = xPos + bitmapCheckOn.GetWidth() + 30 + bitmapCheckOn.GetWidth();
		yPos = y + (theme.GetHeight() - size.y) - (bitmapCheckOn.GetHeight() - size.y) / 2;
	}
	else
	{
		rcSelect.x = _xPos + xPos + posLargeur;
		rcSelect.y = _yPos + yPos + posHauteur;
		rcSelect.width = bitmapCheckOn.GetWidth();
		rcSelect.height = bitmapCheckOn.GetHeight();

		if (isSelected)
			deviceContext->DrawBitmap(bitmapCheckOn, xPos, yPos);
		else
			deviceContext->DrawBitmap(bitmapCheckOff, xPos, yPos);

		wxSize size = CWindowMain::GetSizeTexte(deviceContext, libelleSelectAll, theme.themeFont);

		xPos = xPos + 10 + bitmapCheckOn.GetWidth();
		yPos = y + (theme.GetHeight() - size.y) - (bitmapCheckOn.GetHeight() - size.y) / 2;

		
	}


	CWindowMain::DrawTexte(deviceContext, libelleSelectAll, xPos, yPos, theme.themeFont);

}
