#include "header.h"
#include "ScrollbarWnd.h"
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include "ScrollInterface.h"
using namespace Regards::Window;

#define TIMER_HIDE 1

CScrollbarWnd::CScrollbarWnd(wxWindow* parent, wxWindowID id, const bool & autohidden)
	: wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0)
{
    this->autohidden = false;
	scrollHorizontal = nullptr;
	scrollVertical = nullptr;
	scrollInterface = nullptr;
	showV = false;
	showH = false;
	width = 0;
	height = 0;
	posHauteur = 0;
	posLargeur = 0;
	controlHeight = 0;
	controlWidth = 0;
	loadingTimer = new wxTimer(this, TIMER_HIDE);
	Connect(TIMER_HIDE, wxEVT_TIMER, wxTimerEventHandler(CScrollbarWnd::OnHide), nullptr, this);
	Connect(wxEVT_SIZE, wxSizeEventHandler(CScrollbarWnd::OnSize));
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(CScrollbarWnd::OnEraseBackground));
    Connect(wxEVT_MOTION, wxMouseEventHandler(CScrollbarWnd::OnMouseMove));
    Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CScrollbarWnd::OnMouseLeave));
}

void CScrollbarWnd::OnHide(wxTimerEvent& event)
{
    if(autohidden)
    {
        showV = false;
        showH = false;
        Resize();
    }
}

void CScrollbarWnd::OnMouseLeave(wxMouseEvent& event)
{
    if(autohidden)
        loadingTimer->Start(500, true);
}

void CScrollbarWnd::OnMouseMove(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();
    
    //printf("CScrollbarWnd::OnMouseMove \n");
    
    if(autohidden)
    {
        bool resize = false;
        _showV = false;
        _showH = false;

        if (xPos >= width - (2 * scrollVertical->GetWidthSize()))
            _showV = true;

        if (yPos >= height - ( 2 * scrollHorizontal->GetHeightSize()))
            _showH = true;

        if (_showH || _showV)
        {
            if(loadingTimer->IsRunning())
                loadingTimer->Stop();

            if (_showV && !showV)
            {
                showV = _showV;
                resize = true;
            }
            if (_showH && !showH)
            {
                showH = _showH;
                resize = true;
            }
            
            
        }
        else
        {
            //Start Timer for hide
            loadingTimer->Start(500, true);
        }

        if(resize)
            Resize();
            
         
    }
    
   
    event.Skip();
}

void CScrollbarWnd::SetCentralWindow(CScrollInterface * scrollInterface, const CThemeScrollBar & theme)
{
    if(autohidden)
    {
        printf("SetCentralWindow \n");
        scrollHorizontal = new CScrollbarHorizontalWnd(scrollInterface, scrollInterface->GetWindow(), wxID_ANY, theme);
        scrollVertical = new CScrollbarVerticalWnd(scrollInterface, scrollInterface->GetWindow(), wxID_ANY, theme);
    }
    else
    {
        scrollHorizontal = new CScrollbarHorizontalWnd(scrollInterface, this, wxID_ANY, theme);
        scrollVertical = new CScrollbarVerticalWnd(scrollInterface, this, wxID_ANY, theme);
    }
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

	if (loadingTimer != nullptr)
	{
		if (loadingTimer->IsRunning())
			loadingTimer->Stop();
		delete loadingTimer;
	}
	//delete(scrollVertical);
	//delete(scrollHorizontal);
	
}

void CScrollbarWnd::UpdateScreenRatio()
{
    scrollVertical->UpdateScreenRatio();
    scrollHorizontal->UpdateScreenRatio();
    this->Resize();
}
void CScrollbarWnd::ShowVerticalScroll()
{
	showV = true;
}

void CScrollbarWnd::HideVerticalScroll()
{
	showV = false;
}
void CScrollbarWnd::HideHorizontalScroll()
{
	showH = false;
}


void CScrollbarWnd::SetControlSize(const int &width, const int &height, const bool &useScaleFactor)
{
	if (width != controlWidth || height != controlHeight)
	{
		controlWidth = width;
		controlHeight = height;
        this->useScaleFactor = useScaleFactor;
		Resize();

	}
}

int CScrollbarWnd::GetPosLargeur()
{
	int pictureWidth = width;

    if(scrollVertical != nullptr && showV)
    {
        bool valueV = scrollVertical->IsShown();
        if (valueV)
            pictureWidth -= scrollVertical->GetWidthSize();
    }

	int maxPosWidth = max(controlWidth - pictureWidth, 0);
	posLargeur = max(posLargeur, 0);
	posLargeur = min(posLargeur, maxPosWidth);
	return posLargeur;
}

int CScrollbarWnd::GetPosHauteur()
{
	int pictureHeight = height;

    if(scrollHorizontal != nullptr && showH)
    {
        bool valueH = scrollHorizontal->IsShown();
        if (valueH)
            pictureHeight -= scrollHorizontal->GetHeightSize();
    }


	int maxPosHeight = max(controlHeight - pictureHeight, 0);

	posHauteur = max(posHauteur, 0);
	posHauteur = min(posHauteur, maxPosHeight);

	return posHauteur;
}

void CScrollbarWnd::SetPosition(const int &posX, const int &posY)
{
	posHauteur = std::max(posY, 0);
	posLargeur = std::max(posX, 0);

	int pictureWidth = width;
	int pictureHeight = height;

    if(scrollHorizontal != nullptr && scrollVertical != nullptr)
    {
        bool valueH = scrollHorizontal->IsShown();
        bool valueV = scrollVertical->IsShown();
        if (valueV)
            pictureWidth -= scrollVertical->GetWidthSize();

        if (valueH)
            pictureHeight -= scrollHorizontal->GetHeightSize();
    }


	int maxPosHeight = max(controlHeight - pictureHeight, 0);
	int maxPosWidth = max(controlWidth - pictureWidth, 0);

	posHauteur = std::min(posY, maxPosHeight);
	posLargeur = std::min(posX, maxPosWidth);

	if (scrollVertical != nullptr && showV)
	{
		scrollVertical->SetPosition(posHauteur);
	}
	if (scrollHorizontal != nullptr && showH)
	{
		scrollHorizontal->SetPosition(posLargeur);
	}
}

void CScrollbarWnd::OnSize(wxSizeEvent& event)
{
#ifdef __WXGTK__
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
	int pictureWidth = event.GetSize().GetWidth();
	int pictureHeight = event.GetSize().GetHeight();
#if defined(WIN32) && defined(_DEBUG)
	wxString toShow = "CScrollbarWnd size x : " + to_string(pictureWidth) + " y : " + to_string(pictureHeight) + "\n";
	OutputDebugString(toShow.ToStdWstring().c_str());
#endif

	if(pictureWidth > 0 && pictureHeight > 0)
	{
		width = pictureWidth;
		height = pictureHeight;
		Resize();
	}
}


void CScrollbarWnd::Resize()
{
	//CDeferPos deferpos;
	int pictureWidth = width;
	int pictureHeight = height;
#ifdef __WXGTK__
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
	wxWindow * control = scrollInterface->GetWindow();
    if(!useScaleFactor)
        scale_factor = 1.0f;
    printf("CScrollbarWnd::Resize() controlHeight : %d pictureHeight : %d \n", controlHeight, pictureHeight);

    if(scrollHorizontal != nullptr && scrollVertical != nullptr)
    {   
        bool valueH = scrollHorizontal->IsShown();
        bool valueV = scrollVertical->IsShown();

		if(showV)
		{
			if(controlHeight > pictureHeight * scale_factor)
			{
				scrollVertical->Show();
				valueV = true;
			}
			else
			{
				scrollVertical->Hide();
				valueV = false;
			}
		}
		else
		{
			scrollVertical->Hide();
			valueV = false;
		}

		if(showH)
		{
			if(controlWidth > pictureWidth * scale_factor)
			{
				scrollHorizontal->Show();
				valueH = true;
			}
			else
			{
				scrollHorizontal->Hide();
				valueH = false;
			}
		}
		else
		{
			valueH = false;
			scrollHorizontal->Hide();
		}
       
        if (valueV)
            pictureWidth -= scrollVertical->GetWidthSize();

        if (valueH)
            pictureHeight -= scrollHorizontal->GetHeightSize();

        if (valueH && valueV)
            scrollVertical->ShowEmptyRectangle(true, scrollHorizontal->GetHeightSize());
        else
            scrollVertical->ShowEmptyRectangle(false, scrollHorizontal->GetHeightSize());

        if(!autohidden)
        {
            int x= control->GetSize().x;
            int y= control->GetSize().y;
            if(x != pictureWidth || y != pictureHeight)
            {
                printf("CScrollbarWnd::Resize() control->SetSize pictureWidth : %d, pictureHeight : %d \n", pictureWidth, pictureHeight);
                if(pictureWidth > 0 && pictureHeight > 0)
                    control->SetSize(pictureWidth, pictureHeight);
            }
        }

		if (valueH && showH)
            scrollHorizontal->SetSize(0, pictureHeight, pictureWidth, scrollHorizontal->GetHeightSize());
			
        if (valueV && showV)
            scrollVertical->SetSize(pictureWidth, 0, scrollVertical->GetWidthSize(), height); 

		if (scrollVertical != nullptr && showV)
            scrollVertical->UpdateScrollBar(posHauteur, pictureHeight * scale_factor, controlHeight);

		if (scrollHorizontal != nullptr&& showH)
            scrollHorizontal->UpdateScrollBar(posLargeur, pictureWidth * scale_factor, controlWidth);

        if(autohidden)
        {
            if (valueV)
                pictureWidth += scrollVertical->GetWidthSize();

            if (valueH)
                pictureHeight += scrollHorizontal->GetHeightSize();
            
            int x= control->GetSize().x;
            int y= control->GetSize().y;
            if(x != pictureWidth || y != pictureHeight)
            {
                printf("CScrollbarWnd::Resize() control->SetSize pictureWidth : %d, pictureHeight : %d \n", pictureWidth, pictureHeight);
                if(pictureWidth > 0 && pictureHeight > 0)
                    control->SetSize(pictureWidth, pictureHeight);
            }
        }
    }
}