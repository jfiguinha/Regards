#include "ScrollbarWnd.h"
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include "ScrollInterface.h"
#include <algorithm>    // std::max

using namespace Regards::Window;

CScrollbarWnd::CScrollbarWnd(wxWindow* parent, wxWindowID id)
	: wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0)
{
	Connect(wxEVT_SIZE, wxSizeEventHandler(CScrollbarWnd::OnSize));
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(CScrollbarWnd::OnEraseBackground));
}

void CScrollbarWnd::SetCentralWindow(CScrollInterface * scrollInterface, const CThemeScrollBar & theme)
{
	scrollHorizontal = new CScrollbarHorizontalWnd(scrollInterface, this, wxID_ANY, theme);
	scrollVertical = new CScrollbarVerticalWnd(scrollInterface, this, wxID_ANY, theme);
	this->scrollInterface = scrollInterface;
	showV = true;
	showH = true;
	scrollInterface->SetScrollbar(this);	
}

void CScrollbarWnd::SetPageSize(const int &pageSize)
{
	scrollHorizontal->SetPageSize(pageSize);
	scrollVertical->SetPageSize(pageSize);
}

void CScrollbarWnd::SetLineSize(const int &lineSize)
{
	scrollHorizontal->SetLineSize(lineSize);
	scrollVertical->SetLineSize(lineSize);
}

int CScrollbarWnd::GetBarHeight()
{
	return scrollHorizontal->GetHeightSize();
}

int CScrollbarWnd::GetBarWidth()
{
	return scrollVertical->GetWidthSize();
}

CScrollbarHorizontalWnd * CScrollbarWnd::GetHScrollbar()
{
	return scrollHorizontal;
}

CScrollbarVerticalWnd * CScrollbarWnd::GetVScrollbar()
{
	return scrollVertical;
}

CScrollbarWnd::~CScrollbarWnd()
{
	delete(scrollHorizontal);
	delete(scrollVertical);
}

void CScrollbarWnd::UpdateScreenRatio()
{
    scrollVertical->UpdateScreenRatio();
    scrollHorizontal->UpdateScreenRatio();
    this->Resize();
}


void CScrollbarWnd::HideVerticalScroll()
{
	showV = false;
}
void CScrollbarWnd::HideHorizontalScroll()
{
	showH = false;
}


void CScrollbarWnd::SetControlSize(const int &width, const int &height)
{
	if (width != controlWidth || height != controlHeight)
	{
		controlWidth = width;
		controlHeight = height;
		if (scrollVertical != nullptr)
		{
			scrollVertical->UpdateScrollBar(posHauteur, this->height, controlHeight);
		}
		if (scrollHorizontal != nullptr)
		{
			scrollHorizontal->UpdateScrollBar(posLargeur, this->width, controlWidth);
		}
		Resize();
	}
}

int CScrollbarWnd::GetPosLargeur()
{
	return posLargeur;
}

int CScrollbarWnd::GetPosHauteur()
{
	return posHauteur;
}

void CScrollbarWnd::SetPosition(const int &posX, const int &posY)
{
	posHauteur = std::max(posY, 0);
	posLargeur = std::max(posX, 0);

	int maxPosHeight = max(controlHeight - this->height, 0);
	int maxPosWidth = max(controlWidth - this->width, 0);

	posHauteur = std::min(posY, maxPosHeight);
	posLargeur = std::min(posX, maxPosWidth);

	if (scrollVertical != nullptr)
	{
		scrollVertical->SetPosition(posHauteur);
		scrollVertical->Refresh();
	}
	if (scrollHorizontal != nullptr)
	{
		scrollHorizontal->SetPosition(posLargeur);
		scrollHorizontal->Refresh();
	}
}

void CScrollbarWnd::OnSize(wxSizeEvent& event)
{
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();

	if (scrollVertical != nullptr)
	{
		scrollVertical->UpdateScrollBar(posHauteur, height, controlHeight);
	}
	if (scrollHorizontal != nullptr)
	{
		scrollHorizontal->UpdateScrollBar(posLargeur, width, controlWidth);
	}
	Resize();
}


void CScrollbarWnd::Resize()
{
	//CDeferPos deferpos;
	int pictureWidth = width;
	int pictureHeight = height;

    if(scrollHorizontal != nullptr && scrollVertical != nullptr)
    {
    
        bool valueH = scrollHorizontal->IsShown();
        bool valueV = scrollVertical->IsShown();
        wxWindow * control = scrollInterface->GetWindow();
        if (valueV)
            pictureWidth -= scrollVertical->GetWidthSize();

        if (valueH)
            pictureHeight -= scrollHorizontal->GetHeightSize();

        if (valueH && valueV)
            scrollVertical->ShowEmptyRectangle(true, scrollHorizontal->GetHeightSize());
        else
            scrollVertical->ShowEmptyRectangle(false, scrollHorizontal->GetHeightSize());

        control->SetSize(0, 0, pictureWidth, pictureHeight);
        if (valueH)
            scrollHorizontal->SetSize(0, pictureHeight, pictureWidth, scrollHorizontal->GetHeightSize());

        if (valueV)
            scrollVertical->SetSize(pictureWidth, 0, scrollVertical->GetWidthSize(), height);
    }
}