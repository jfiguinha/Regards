#include "Pane.h"
using namespace Regards::Window;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPane::CPane(wxWindow* parent, wxWindowID id, CPaneInterface * paneInterface, const int &idPane, const CThemePane & theme) :
CWindowMain(parent, id)
{
	this->paneInterface = paneInterface;
	titleBar = new CTitleBar(this, wxID_ANY, this);
	this->idPane = idPane;
	this->themePane = themePane;
	//titleBar->Show(false)
	Connect(wxEVT_MOTION, wxMouseEventHandler(CPane::OnMouseMove));
	Connect(wxEVT_PAINT, wxPaintEventHandler(CPane::OnPaint));
}

void CPane::OnPaint(wxPaintEvent& event)
{
	titleBar->Redraw();
}

CPane::~CPane()
{
	delete(titleBar);
}

void CPane::UpdateScreenRatio()
{
    titleBar->UpdateScreenRatio();
    this->Resize();
}

void CPane::SetClosable(const bool &value)
{
	titleBar->SetClosable(value);
}

int CPane::GetTitleHeight()
{
	return themePane.GetHeight();
}

void CPane::Resize()
{
	if (titleBarVisible)
	{
		//CDeferPos deferpos;
		titleBar->SetSize(0, 0, width, titleBar->GetHeight());
		if (hWndOther != nullptr)
			hWndOther->SetSize(0, titleBar->GetHeight(), width, height - titleBar->GetHeight());
	}
	else if (hWndOther != nullptr)
	{
		hWndOther->SetSize(0, 0, width, height);
	}
}

void CPane::OnMouseMove(wxMouseEvent& event)
{
	//int xPos = event.GetX();
	//int yPos = event.GetY();
}


void CPane::SetTitleBarVisibility(const bool &visible)
{
	titleBarVisible = visible;
	titleBar->Show(visible);
	Resize();
}

void CPane::ClosePane()
{
	paneInterface->ClosePane(idPane);
}

void CPane::SetTitle(const wxString & title)
{
	titleBar->SetTitle(title);
}

int CPane::SetTooltipText(const wxString & tooltip)
{
	return titleBar->SetTooltipText(tooltip);
}
