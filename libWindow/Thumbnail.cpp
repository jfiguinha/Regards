#include "header.h"
#include "Thumbnail.h"
#include <FileUtility.h>
#include <wxmd5.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarWnd.h"
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <SqlPhotosWithoutThumbnail.h>
#include <ImageLoadingFormat.h>
#include <ImageVideoThumbnail.h>
#include <SqlThumbnailVideo.h>
#include <wx/dcbuffer.h>
#include <wx/filename.h>
#include "LibResource.h"
#include <ThumbnailData.h>
#include "ThumbnailMessage.h"
#include <libPicture.h>

using namespace Regards::Picture;
using namespace Regards::Window;


class CImageLoadingFormat;

#define TIMER_LOADING 4
#define TIMER_REFRESH 5
#define TIMER_ANIMATION 6

wxDEFINE_EVENT(EVENT_ICONEUPDATE, wxCommandEvent);
wxDEFINE_EVENT(EVENT_UPDATEMESSAGE, wxCommandEvent);





class CThreadLoadingBitmap
{
public:

	CThreadLoadingBitmap()
	{
		bitmapIcone = nullptr;
		_thread = nullptr;
		thumbnail = nullptr;
	};
	~CThreadLoadingBitmap(){};

	int percent;
	wxString filename;
	int typeElement;
	int photoId;
	int timePosition;
    CImageLoadingFormat * bitmapIcone;
	thread * _thread;
	CThumbnail * thumbnail;
};

#define WM_NEWFOLDER 501
#define WM_DISPLAYPICTURE 502
#define WM_STOPPICTURELOAD 503
#define WM_UPDATESTATUSBAR 504
#define WM_LOADTHUMBNAIL 505

bool CThumbnail::ItemCompFonct(int xPos, int yPos, CIcone * icone, CWindowMain * parent)   /* Définit une fonction. */
{
	wxRect rc = icone->GetPos();
	if ((rc.x < xPos && xPos < rc.width) && (rc.y < yPos && yPos < rc.height))
	{
		return true;
	}
	return false;
}


CIcone * CThumbnail::FindElement(const int &xPos, const int &yPos)
{
    TRACE();
	pItemCompFonct _pf = &ItemCompFonct;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
}


void CThumbnail::EraseThumbnail(wxCommandEvent& event)
{
    TRACE();

	wxString title = CLibResource::LoadStringFromResource(L"LBLSTOPALLPROCESS", 1);
	wxString message = CLibResource::LoadStringFromResource(L"LBLSTOPPROCESS", 1);
	StopAllProcess(title, message, this);
	
    CSqlThumbnail sqlThumbnail;
    sqlThumbnail.EraseThumbnail();
    
	for (int i = 0;i < nbElementInIconeList;i++)
	{
        CIcone * pIcone = iconeList->GetElement(i);
		if(pIcone != nullptr)
		{
			CThumbnailData * pThumbnailData = pIcone->GetData();
			if (pThumbnailData != nullptr)
			{
				pThumbnailData->InitLoadState();
				wxString filename = pThumbnailData->GetFilename();
				pThumbnailData->SetIsProcess(false);
                pThumbnailData->SetIsLoading(false);
			}
		}
	}

	thumbnailPos = 0;
	CSqlPhotosWithoutThumbnail sqlPhoto;
	sqlPhoto.GeneratePhotoList();

	SetStopProcess(false);
	
    processIdle = true;
}

void CThumbnail::SetCheck(const bool &check)
{
    TRACE();
	for (int i = 0;i < nbElementInIconeList;i++)
	{
        CIcone * pIcone = iconeList->GetElement(i);
		if (pIcone != nullptr)
		{
			pIcone->ShowSelectButton(check);
		}
	}
}

void CThumbnail::GetSelectItem(vector<CThumbnailData *> & vectorData)
{
    TRACE();
	for (int i = 0;i < nbElementInIconeList;i++)
	{
        CIcone * pIcone = iconeList->GetElement(i);
		if (pIcone != nullptr)
		{
			if (pIcone->IsChecked())
				vectorData.push_back(pIcone->GetData());
		}
	}
}

bool CThumbnail::ItemCompFonctPhotoId(int xPos, int yPos, CIcone* icone, CWindowMain* parent)   /* Définit une fonction. */
{
	CThumbnailData* pThumbnailData = icone->GetData();
	if (pThumbnailData->GetNumPhotoId() == xPos)
		return true;
	return false;
}

CIcone* CThumbnail::GetIconeById(const int& idPhoto)
{
	pItemCompFonct _pf = &ItemCompFonctPhotoId;
	return iconeList->FindElement(idPhoto, 0, &_pf, this);
}

int CThumbnail::GetNumItemById(const int& idPhoto)
{
	TRACE();
	pItemCompFonct _pf = &ItemCompFonctPhotoId;
	CIcone* icone = iconeList->FindElement(idPhoto, 0, &_pf, this);
	if(icone != nullptr)
		return icone->GetNumElement();
	return 0;
}

void CThumbnail::SetActifItem(const int &idPhoto, const bool &move)
{
    TRACE();

	int numItem = GetNumItemById(idPhoto);

	isMovingScroll = move;

	/*
	if (numOldItem == numItem)
	{
		//printf("toto is back !!!!");
		return;
	}
	*/
	if (numItem >= nbElementInIconeList)
		return;

	if (numSelectPhotoId != -1)
	{
		CIcone* numSelect = GetIconeById(numSelectPhotoId);
		if(numSelect != nullptr)
			numSelect->SetSelected(false);
	}
		

	if (numActifPhotoId != -1)
	{
		CIcone* numActif = GetIconeById(numActifPhotoId);
		if (numActif != nullptr)
			numActif->SetSelected(false);
	}

	numActifPhotoId = iconeList->GetPhotoId(numItem);
	isMovingScroll = false;
   
	if (move)
	{
		if (numItem == 0)
		{
			if (this->GetParent() != nullptr)
			{
				wxSize * size = new wxSize();
				wxCommandEvent evt(wxEVENT_SETPOSITION);
				size->x = 0;
				size->y = 0;
				evt.SetClientData(size);
				this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
			}
            posLargeur = 0;
            posHauteur = 0;   
		}
		else{
			if (!isMoving)
			{
				wxRect rect;

				if (numActifPhotoId != -1)
				{
					CIcone* numActif = GetIconeById(numActifPhotoId);
					if (numActif != nullptr)
						rect = numActif->GetPos();
				}

				//Positionnement au milieu
				
				int yPos = max((rect.y - this->GetWindowHeight() / 2),0);
				int xPos = max((rect.x - this->GetWindowWidth() / 2), 0);

				if (this->GetParent() != nullptr)
				{
					wxSize * size = new wxSize();
					wxCommandEvent evt(wxEVENT_SETPOSITION);
					size->x = xPos;
					size->y = yPos;
					evt.SetClientData(size);
                    this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
				}

 				posLargeur = xPos;
				posHauteur = yPos;
				//isMovingScroll = true;
			}
		}
	}
 
	numSelectPhotoId = iconeList->GetPhotoId(numItem);

	if (numSelectPhotoId != -1)
	{
		CIcone* numSelect = GetIconeById(numSelectPhotoId);
		if (numSelect != nullptr)
			numSelect->SetSelected(true);
	}

	numOldItem = numItem;
    moveOnPaint = true;
	this->ForceRefresh();
}

int CThumbnail::ImageSuivante()
{
	numOldItem++;
	if (numOldItem == nbElementInIconeList)
		numOldItem = 0;
	return numOldItem;
}

int CThumbnail::ImagePrecedente()
{
	numOldItem--;
	if (numOldItem < 0)
		numOldItem = nbElementInIconeList - 1;
	return numOldItem;
}

int CThumbnail::ImageFin()
{
	return nbElementInIconeList - 1;
}

int CThumbnail::ImageDebut()
{
	return 0;
}

int CThumbnail::GetNbElement()
{
	return nbElementInIconeList;
}

wxString CThumbnail::GetFilename(const int &numItem)
{
	wxString filename = "";
	if (iconeList != nullptr)
	{
		filename = iconeList->GetFilename(numItem);
		
	}
	return filename;
}

void CThumbnail::SetTheme(CThemeThumbnail * theme)
{
    TRACE();
	themeThumbnail = *theme;
}

void CThumbnail::ZoomOn()
{
    TRACE();
	positionSize++;
	if (positionSize > Max)
		positionSize = Max;

	int sizeIcone = TabSize[positionSize - 1];
	SetIconeSize(sizeIcone, sizeIcone);

	this->Refresh();
}

void CThumbnail::ZoomOff()
{
    TRACE();
	positionSize--;
	if (positionSize < 1)
		positionSize = 1;

	int sizeIcone = TabSize[positionSize - 1];
	SetIconeSize(sizeIcone, sizeIcone);

	this->Refresh();
}

void CThumbnail::ZoomPosition(const int &position)
{
    TRACE();
	positionSize = position+1;
	if (positionSize < 1)
		positionSize = 1;
	if (positionSize > Max)
		positionSize = Max;

	int sizeIcone = TabSize[positionSize - 1];
	SetIconeSize(sizeIcone, sizeIcone);

	this->Refresh();
}


CThumbnail::CThumbnail(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CWindowMain("CThumbnail",parent, id)
{
	controlWidth = 0;
	controlHeight = 0;
	isMoving = 0;
	thumbnailPos = 0;
    oldPosLargeur = 0;
    oldPosHauteur = 0;    
	nbProcess = 0;
	allThreadEnd = true;
    showLoadingBitmap = false;
    stepLoading = 0;
    m_animation = nullptr;
	render = false;
	check = false;
	threadDataProcess = true;
    iconeList = new CIconeList();
	this->testValidity = testValidity;
	thumbnailSizeX = 0;
	thumbnailSizeY = 0;
	posHauteur = 0;
	posLargeur = 0;
	nbLigneX = 0;
	nbLigneY = 0;
	
	
	numStartElement = 0;
	nbElementToShow = 0;
	isStoragePt = false;


	//this->statusbar = statusbar;

	this->themeThumbnail = themeThumbnail;
	Connect(wxEVT_IDLE, wxIdleEventHandler(CThumbnail::OnIdle));
	Connect(wxEVT_PAINT, wxPaintEventHandler(CThumbnail::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CThumbnail::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CThumbnail::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CThumbnail::OnLButtonUp));
	Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(CThumbnail::OnLDoubleClick));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CThumbnail::OnMouseWheel));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CThumbnail::OnKeyDown));
    Connect(wxEVT_KEY_UP, wxKeyEventHandler(CThumbnail::OnKeyUp));
	Connect(EVENT_ICONEUPDATE, wxCommandEventHandler(CThumbnail::UpdateRenderIcone));
	Connect(EVENT_UPDATEMESSAGE, wxCommandEventHandler(CThumbnail::UpdateMessage));
    
    Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CThumbnail::OnEnterWindow));
    Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CThumbnail::OnLeaveWindow));
    
    refreshTimer = new wxTimer(this, TIMER_REFRESH);
    Connect(TIMER_REFRESH, wxEVT_TIMER, wxTimerEventHandler(CThumbnail::OnRefreshIcone), nullptr, this);

	timerAnimation = new wxTimer(this, TIMER_ANIMATION);
	Connect(TIMER_ANIMATION, wxEVT_TIMER, wxTimerEventHandler(CThumbnail::OnAnimation), nullptr, this);

    wxString resourcePath = CFileUtility::GetResourcesFolderPath();
    m_animation = new wxAnimation(resourcePath + "/loading.gif");

	Connect(wxEVENT_ONSTARTLOADINGPICTURE, wxCommandEventHandler(CThumbnail::StartLoadingPicture));
	Connect(wxEVENT_ONSTOPLOADINGPICTURE, wxCommandEventHandler(CThumbnail::StopLoadingPicture));
	Connect(wxEVENT_REFRESHDATA, wxCommandEventHandler(CThumbnail::EraseThumbnail));
	Connect(wxEVENT_SCROLLMOVE, wxCommandEventHandler(CThumbnail::OnScrollMove));
	Connect(wxEVENT_LEFTPOSITION, wxCommandEventHandler(CThumbnail::OnLeftPosition));
	Connect(wxEVENT_TOPPOSITION, wxCommandEventHandler(CThumbnail::OnTopPosition));
	Connect(wxEVENT_REFRESHTHUMBNAIL, wxCommandEventHandler(CThumbnail::OnRefreshThumbnail));
	processIdle = true;

	listProcessWindow.push_back(this);
    
	TabSize = { 100,150,200,250,300,350,400,450,500,550,600,650,700 };
	Max = TabSize.size();
	positionSize = 3;

	m_waitingAnimation = new wxActivityIndicator(this, wxID_ANY);
	m_waitingAnimation->Stop();
	m_waitingAnimation->Hide();
	//m_animation->SetSize(wxSize(this->GetHeight(), this->GetHeight()));
	//m_animation->SetBackgroundColour(themeSlider.colorBack);
}

int CThumbnail::GetTabValue()
{
	return positionSize;
}

void CThumbnail::OnAnimation(wxTimerEvent& event)
{
	this->Refresh();
}

void CThumbnail::ChangeTabValue(const vector<int>& TabNewSize, const int & positionSize)
{
	TabSize = TabNewSize;
	Max = TabSize.size();
	this->positionSize = positionSize;
	int sizeIcone = TabSize[positionSize - 1];
	SetIconeSize(sizeIcone, sizeIcone);

	this->Refresh();
}

void CThumbnail::RefreshIcone(const int& idPhoto)
{
	wxClientDC dc(this);
	CIcone* icone = GetIconeById(idPhoto);
	if (icone != nullptr)
		icone->RenderIcone(&dc, posLargeur, posHauteur, false, false);
}

void CThumbnail::OnRefreshThumbnail(wxCommandEvent& event)
{
	int idPhoto = event.GetId();
	if(idPhoto != -1)
	{
		RefreshIcone(idPhoto);
		
	}
	//this->Refresh();
}

void CThumbnail::MoveTop()
{
	wxWindow * parent = this->GetParent();
	moveOnPaint = false;
	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVETOP);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnail::MoveLeft()
{
	wxWindow * parent = this->GetParent();
	moveOnPaint = false;
	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVELEFT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnail::MoveBottom()
{
	wxWindow * parent = this->GetParent();
	moveOnPaint = false;
	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVEBOTTOM);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnail::MoveRight()
{
	wxWindow * parent = this->GetParent();
	moveOnPaint = false;
	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVERIGHT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnail::OnLeftPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posLargeur = pos;
	moveOnPaint = false;
	this->Refresh();
}

void CThumbnail::OnTopPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
 	posHauteur = pos;
	moveOnPaint = false;
	this->Refresh();
}

void CThumbnail::OnScrollMove(wxCommandEvent& event)
{
	isMovingScroll = isMoving = event.GetInt();
	moveOnPaint = false;

}

void CThumbnail::OnRefreshIcone(wxTimerEvent& event)
{
    TRACE();
     this->Refresh();    
}

CThumbnail::~CThumbnail()
{
    TRACE();
	threadDataProcess = false;
    
    refreshTimer->Stop();

	timerAnimation->Stop();
    
    while(refreshTimer->IsRunning())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    delete refreshTimer;
    
	if (m_animation != nullptr)
		delete m_animation;
        
	if (iconeList != nullptr)
	{
		delete iconeList;
	}

	if (m_waitingAnimation != nullptr)
		delete m_waitingAnimation;
}

int CThumbnail::GetWidth()
{
    TRACE();
	return thumbnailSizeX;
}

int CThumbnail::GetHeight()
{
    TRACE();
	return thumbnailSizeY;
}

int CThumbnail::GetIconeHeight()
{
    TRACE();
	return themeThumbnail.themeIcone.GetHeight();
}

int CThumbnail::GetIconeWidth()
{
    TRACE();
	return themeThumbnail.themeIcone.GetWidth();
}

void CThumbnail::AfterSetList()
{
	for (int i = 0; i < nbElementInIconeList; i++)
	{
		CIcone * icone = iconeList->GetElement(i);
		if (icone != nullptr)
		{
			CThumbnailData * data = icone->GetData();
			if (numSelectPhotoId == data->GetNumPhotoId())
			{
				icone->SetSelected(true);
			}
			if (numActifPhotoId == data->GetNumPhotoId())
			{
				icone->SetActive(true);
			}

			if (numSelectPhotoId != -1 && numActifPhotoId != -1)
				break;
		}
	}
}

void CThumbnail::EraseThumbnailList(CIconeList* iconeListLocal)
{
	TRACE();

	iconeListLocal->EraseThumbnailList();

    delete iconeListLocal;
    iconeListLocal = nullptr;
}

void CThumbnail::SetIconeSize(const int &width, const int &height)
{
    TRACE();
	themeThumbnail.themeIcone.SetWidth(width);
	themeThumbnail.themeIcone.SetHeight(height);
    
    ResizeThumbnail();
}

#ifdef TBB
struct mytask {
	mytask(CThreadLoadingBitmap * pLoadBitmap)
	{
		this->pLoadBitmap = pLoadBitmap;
	}

	void operator()()
	{
		CThumbnail::LoadPicture(pLoadBitmap);
	}


	CThreadLoadingBitmap * pLoadBitmap;
};
#endif

void CThumbnail::ProcessThumbnail(CThumbnailData * pThumbnailData)
{
	CThreadLoadingBitmap * pLoadBitmap = new CThreadLoadingBitmap();
	pLoadBitmap->timePosition = pThumbnailData->GetTimePosition();
	pLoadBitmap->percent = pThumbnailData->GetPercent();
	pLoadBitmap->typeElement = pThumbnailData->GetTypeElement();
	pLoadBitmap->filename = pThumbnailData->GetFilename();
	pLoadBitmap->photoId = pThumbnailData->GetNumPhotoId();
	pLoadBitmap->thumbnail = this;
	pLoadBitmap->_thread = new thread(LoadPicture, pLoadBitmap);
	pThumbnailData->SetIsProcess(true);
}

void CThumbnail::ProcessIdle()
{
    TRACE();
	if (nbElementInIconeList == 0 || threadDataProcess == false)
    {
        processIdle = false;
		return;
    }
    
	int nbProcesseur = 1;
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
		nbProcesseur = config->GetThumbnailProcess();
	//int nbProcesseur = thread::hardware_concurrency();
	vector<wxString> photoList;
	CSqlPhotosWithoutThumbnail sqlPhoto;
	sqlPhoto.UpdatePhotoList();
	sqlPhoto.GetPhotoList(&photoList);

	//nbProcesseur = 1;
	if (photoList.size() > 0)
	{
		wxCommandEvent * event = new wxCommandEvent(EVENT_UPDATEMESSAGE);
		event->SetExtraLong(photoList.size());
		wxQueueEvent(this, event);


		for (int j = 0; nbProcess < nbProcesseur; j++)
		{
			if (j >= photoList.size())
				break;

			if (nbElementInIconeList == 0 || threadDataProcess == false)
				return;

			wxString filename = photoList[j];
            
			for (int i = 0; i < nbElementInIconeList; i++)
			{
				bool exitfor = false;

				if (threadDataProcess == false)
					return;

				wxString filelocalName = iconeList->GetFilename(i);
				if (filename == filelocalName)
				{
					
					CIcone* icone = iconeList->GetElement(i);
					if (icone != nullptr)
					{
						CThumbnailData* pThumbnailData = icone->GetData();
						if (pThumbnailData != nullptr)
						{
							bool isLoad = pThumbnailData->IsLoad();
							bool isProcess = pThumbnailData->IsProcess();
							if (!isProcess && !isLoad)
							{
								ProcessThumbnail(pThumbnailData);
								nbProcess++;
							}
							sqlPhoto.InsertProcessStart(filelocalName);

							exitfor = true;
						}

					}

				}

				if (exitfor)
					break;
			}
		}
	}

	if (photoList.size() == 0)
	{
		processIdle = false;
	}

	
}

void CThumbnail::UpdateMessage(wxCommandEvent& event)
{
    TRACE();
	int nbPhoto = event.GetExtraLong();
	CThumbnailMessage * thumbnailMessage = new CThumbnailMessage();
	thumbnailMessage->nbPhoto = nbPhoto;
	thumbnailMessage->thumbnailPos = thumbnailPos;
	thumbnailMessage->nbElement = nbElementInIconeList;

	wxWindow * mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
	wxCommandEvent eventChange(wxEVENT_UPDATEMESSAGETHUMBNAIL);
	eventChange.SetClientData(thumbnailMessage);
	mainWnd->GetEventHandler()->AddPendingEvent(eventChange);

	thumbnailPos++;
}

void CThumbnail::OnIdle(wxIdleEvent& evt)
{
	StartThread();
}

bool CThumbnail::GetProcessEnd()
{
    TRACE();
	if(nbProcess > 0)
		return false;
	return true;
}

wxString CThumbnail::GetWaitingMessage()
{
    TRACE();
	return "Window Thumbnail waiting : " + to_string(this->GetId()) + " - NbProcess Waiting : " + to_string(nbProcess);
}


void CThumbnail::LoadPicture(void * param)
{
    TRACE();
	CLibPicture libPicture;
	CThreadLoadingBitmap * threadLoadingBitmap = (CThreadLoadingBitmap *)param;
    if(threadLoadingBitmap == nullptr)
        return;
        
	if(libPicture.TestIsVideo(threadLoadingBitmap->filename) || libPicture.TestIsPDF(threadLoadingBitmap->filename) || libPicture.TestIsAnimation(threadLoadingBitmap->filename))
	{
        vector<CImageVideoThumbnail *> listVideo;
        libPicture.LoadAllVideoThumbnail(threadLoadingBitmap->filename, &listVideo, true, true);
        
        if(listVideo.size() > 0)
        {
            CSqlThumbnailVideo sqlThumbnailVideo;
            
            int selectPicture = listVideo.size() / 2;
            for(int i = 0;i < listVideo.size();i++)
            {
                CImageVideoThumbnail * bitmap = listVideo[i];
				wxString filename = threadLoadingBitmap->filename;// bitmap->image->GetFilename();
                int compressMethod = 0;
                unsigned long outputsize = 0;
                bitmap->image->ConvertToRGB24(true);
                uint8_t * dest = bitmap->image->GetJpegData(outputsize, compressMethod);
                if (dest != nullptr)
                    sqlThumbnailVideo.InsertThumbnail(filename, dest, outputsize, bitmap->image->GetWidth(), bitmap->image->GetHeight(), i, bitmap->rotation, bitmap->percent, bitmap->timePosition);

                bitmap->image->DestroyJpegData(dest, compressMethod);

                if(i == selectPicture)
                    threadLoadingBitmap->bitmapIcone = bitmap->image;

                dest = nullptr;
                
                if(i != selectPicture)
                    delete bitmap;
            }
        }
	}
	else
	{
		threadLoadingBitmap->bitmapIcone = libPicture.LoadThumbnail(threadLoadingBitmap->filename);


	}


	wxCommandEvent * event = new wxCommandEvent(EVENT_ICONEUPDATE);
	event->SetClientData(threadLoadingBitmap);
	wxQueueEvent(threadLoadingBitmap->thumbnail, event);
}

void CThumbnail::OnMouseMove(wxMouseEvent& event)
{
    TRACE();

    bool needtoRedraw = false;
	isMovingScroll = true;

	if (mouseClickBlock && enableDragAndDrop)
	{
		int xPos = event.GetX();
		int yPos = event.GetY();
		if (numActifPhotoId != -1)
		{
			if (!mouseClickMove)
				nbElementChecked = GetNbIconSelected();
			mouseClickMove = true;
			xPosDrag = xPos;
			yPosDrag = yPos;

			if (numActifPhotoId != -1)
			{
				CIcone* numActif = GetIconeById(numActifPhotoId);
				numActif->SetChecked(true);
			}			
		}

		if (yPos < 100)
			MoveTop();
		else if (yPos > this->GetWindowHeight() - 100)
			MoveBottom();
		else
			this->Refresh();
	}
	else
	{

		int xPos = event.GetX();
		int yPos = event.GetY();
		int iconePhotoId = -1;
		::wxSetCursor(wxCursor(wxCURSOR_HAND));
		CIcone * pBitmapIcone = FindElement(xPos, yPos);

		if (pBitmapIcone != nullptr)
		{
			if (pBitmapIcone->GetData() != nullptr)
				iconePhotoId = pBitmapIcone->GetData()->GetNumPhotoId();

			if (numActifPhotoId != -1)
			{
				if (iconePhotoId != numActifPhotoId)
				{
					if (numActifPhotoId != -1)
					{
						CIcone* numActif = GetIconeById(numActifPhotoId);
						numActif->SetActive(false);
					}
					needtoRedraw = true;
					refreshTimer->Stop();
				}

				if (pBitmapIcone != nullptr)
				{
					if (!refreshTimer->IsRunning())
					{
						CThumbnailData * data = pBitmapIcone->GetData();
						if (data != nullptr)
						{
							wxString filename = data->GetFilename();
							CLibPicture libPicture;
							if (libPicture.TestIsVideo(filename) || libPicture.TestIsPDF(filename) || libPicture.TestIsAnimation(filename))
								refreshTimer->Start(100);
						}
					}
				}

			}

			if (pBitmapIcone->GetState() != ACTIFICONE)
			{
				numActifPhotoId = iconePhotoId;
				pBitmapIcone->SetActive(true);
				if (pBitmapIcone != nullptr)
				{
					CThumbnailData * data = pBitmapIcone->GetData();
					if (data != nullptr)
					{
						wxString filename = data->GetFilename();
						CLibPicture libPicture;
						if (libPicture.TestIsVideo(filename) || libPicture.TestIsPDF(filename) || libPicture.TestIsAnimation(filename))
							refreshTimer->Start(100);
					}
				}

				needtoRedraw = true;
			}
		}

		if (needtoRedraw)
		{
			this->Refresh();
		}

		//this->GetParent()->GetEventHandler()->ProcessEvent(event);
	}

  
}

void CThumbnail::RenderBitmap(wxDC * deviceContext, CIcone * pBitmapIcone, const int &posLargeur, const int &posHauteur)
{
    TRACE();
    
	if (pBitmapIcone == nullptr)
		return;

	int nbProcesseur = 1;
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
		nbProcesseur = config->GetThumbnailProcess();

	int value = pBitmapIcone->RenderIcone(deviceContext, posLargeur, posHauteur, flipHorizontal, flipVertical);

	if (preprocess_thumbnail)
	{
		if (value == 1 && nbProcess < nbProcesseur)
		{
			if (pBitmapIcone != nullptr)
			{
				CThumbnailData * pThumbnailData = pBitmapIcone->GetData();

				if (pThumbnailData != nullptr)
				{
					bool isLoad = pThumbnailData->IsLoad();
					bool isProcess = pThumbnailData->IsProcess();
					if (!isProcess && !isLoad)
					{
						ProcessThumbnail(pThumbnailData);
						nbProcess++;
					}
				}
			}
		}
	}

    //this->UpdateScroll();
}

void CThumbnail::UpdateScreenRatio()
{
    TRACE();
    this->Resize();
}

void CThumbnail::OpenFileViewer(const wxString &filename)
{
    TRACE();
	//ShellExecute(m_hWnd, L"open", filename.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}

void CThumbnail::OnLDoubleClick(wxMouseEvent& event)
{
    TRACE();

		CThumbnailData * pThumbnailData = nullptr;


		int xPos = event.GetX();
		int yPos = event.GetY();

		CIcone * pBitmapIcone = FindElement(xPos, yPos);
		if (pBitmapIcone != nullptr)
		{
			pThumbnailData = pBitmapIcone->GetData();
			if (pThumbnailData != nullptr)
			{
				switch (pThumbnailData->GetTypeElement())
				{
					case TYPEPHOTO:
					{
						OpenFileViewer(pThumbnailData->GetFilename());
					}
					break;
				}
			}
		}
}

void CThumbnail::OnLButtonUp(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();
	
	mouseClickBlock = false;
	if (mouseClickMove && enableDragAndDrop)
	{
		OnMouseRelease(xPos, yPos);
		mouseClickMove = false;
		this->Refresh();
	}
}



void CThumbnail::OnLButtonDown(wxMouseEvent& event)
{
    TRACE();
	wxClientDC winDC(this);
	this->SetFocus();
	int xPos = event.GetX();
	int yPos = event.GetY();

	mouseClickBlock = true;

	if (numSelectPhotoId != -1)
	{
		CIcone* numSelect = GetIconeById(numSelectPhotoId);
		if(numSelect != nullptr)
			numSelect->SetSelected(false);
	}

	int iconePhotoId = -1;
	CIcone * pBitmapIcone = FindElement(xPos, yPos);
	if (pBitmapIcone != nullptr)
		if (pBitmapIcone->GetData() != nullptr)
			iconePhotoId = pBitmapIcone->GetData()->GetNumPhotoId();


	if (pBitmapIcone != nullptr)
	{
		numSelectPhotoId = iconePhotoId;
		bool value = pBitmapIcone->OnClick(xPos, yPos, posLargeur, posHauteur);
        //
		if (!value)
		{
			CThumbnailData * data = pBitmapIcone->GetCopyData();
			OnPictureClick(data);
			delete data;
		}

        pBitmapIcone->SetSelected(true);
	}
	else
	{
		FindOtherElement(&winDC, xPos, yPos);
	}

	if (numActifPhotoId != -1 && enableDragAndDrop)
	{
		if (numActifPhotoId != -1)
		{
			int returnValue = 0;
			CIcone* numActif = GetIconeById(numActifPhotoId);
			bitmapIconDrag = numActif->GetBitmapIcone(returnValue);
		}
		wxImage image = bitmapIconDrag.ConvertToImage();
		unsigned char* alphaData = new unsigned char[image.GetWidth() * image.GetHeight()];
		memset(alphaData, 128, image.GetWidth() * image.GetHeight());
		image.SetAlpha(alphaData);
		bitmapIconDrag = image;

	}
	moveOnPaint = true;
	this->Refresh();

}

int CThumbnail::GetNbIconSelected()
{
	int nbCheck = 0;
	int nbElement = nbElementInIconeList;
	for (int i = 0; i < nbElement; i++)
	{
		CIcone * icone = iconeList->GetElement(i);
		if (icone->IsChecked())
			nbCheck++;
	}
	return nbCheck;
}


void CThumbnail::StartLoadingPicture(wxCommandEvent& event)
{
    TRACE();
	int numItem = event.GetExtraLong();

	if (numloadingIconePhotoId != -1)
	{
		CIcone* loadingIcone = GetIconeById(numloadingIconePhotoId);
		if (loadingIcone != nullptr)
			loadingIcone->StopLoadingPicture();
	}     
    
    if (numItem >= nbElementInIconeList)
        return;

    
	numloadingIconePhotoId = iconeList->GetPhotoId(numItem);
    
    stepLoading = 0;
    showLoadingBitmap = true;

	if (numloadingIconePhotoId != -1)
	{
		CIcone* loadingIcone = GetIconeById(numloadingIconePhotoId);
		if(loadingIcone != nullptr)
			loadingIcone->StartLoadingPicture();
	}
}

void CThumbnail::StopLoadingPicture(wxCommandEvent& event)
{
    TRACE();

	if (numloadingIconePhotoId != -1)
	{
		CIcone* loadingIcone = GetIconeById(numloadingIconePhotoId);
		if (loadingIcone != nullptr)
		{
			loadingIcone->StopLoadingPicture();
		}

    }
    
    showLoadingBitmap = false;

	this->Refresh();

}

void CThumbnail::OnPaint(wxPaintEvent& event)
{
    TRACE();
    int width = GetWindowWidth();
    int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;

	if (threadDataProcess == false)
	{
		wxBufferedPaintDC dc(this);
		wxRect rc = GetWindowRect();
		//UpdateScroll();
		FillRect(&dc, rc, themeThumbnail.colorBack);
		if (!animationStart)
		{
			m_waitingAnimation->Show();
			m_waitingAnimation->Start();
			animationStart = true;
			timerAnimation->Start(100);
		}

		m_waitingAnimation->SetSize(wxSize(width, height));
		m_waitingAnimation->SetBackgroundColour(themeThumbnail.colorBack);
		return;
		
	}
	else if(animationStart)
	{
		timerAnimation->Stop();
		m_waitingAnimation->Stop();
		m_waitingAnimation->Hide();
	}
      

	if (numSelectPhotoId != -1 && !isMovingScroll && moveOnPaint)
	{
		CIcone* numSelect = GetIconeById(numSelectPhotoId);
		if (numSelect != nullptr)
		{
			wxRect rect = numSelect->GetPos();
			int yPos = max((rect.y - this->GetWindowHeight() / 2), 0);
			int xPos = max((rect.x - this->GetWindowWidth() / 2), 0);
			posLargeur = xPos;
			posHauteur = yPos;
		}
	}
	
	TestMaxX();
	TestMaxY();
    
	render = true;
    //printf("CThumbnail::OnPaint \n");   
    //printf("CThumbnail::OnPaint not buffered \n");
    wxBufferedPaintDC dc(this);
    wxRect rc = GetWindowRect();
    //UpdateScroll();
    FillRect(&dc, rc, themeThumbnail.colorBack);
    
    lockIconeList.lock();
	RenderIcone(&dc);
    lockIconeList.unlock();
    
	render = false;
    oldPosLargeur = posLargeur;
    oldPosHauteur = posHauteur;

	if (this->GetParent() != nullptr && moveOnPaint)
	{
		wxSize * size = new wxSize();
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		size->x = posLargeur;
		size->y = posHauteur;
		evt.SetClientData(size);
		this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
	}
	//delete dc;

	if(mouseClickBlock && mouseClickMove && enableDragAndDrop)
	{
		dc.DrawBitmap(bitmapIconDrag, xPosDrag - (bitmapIconDrag.GetWidth() / 2), yPosDrag - (bitmapIconDrag.GetHeight() / 2));

		if (nbElementChecked > 1)
		{
			wxString libelle = L"";

			libelle = to_string(nbElementChecked);

			if (libelle != L"")
			{
				CThemeIcone themeIcone;
				CThemeFont themeFont = themeIcone.font;
				themeFont.SetFontSize(18);
				wxSize size = GetSizeTexte(&dc, libelle, themeFont);
				int localx = xPosDrag - (bitmapIconDrag.GetWidth() / 2);
				int localy = yPosDrag - (bitmapIconDrag.GetHeight() / 2);

				int xPos = xPosDrag - size.x / 2;
				int yPos = yPosDrag - size.y / 2;


				
				dc.SetBrush(wxBrush(themeIcone.colorSelectTop));
				dc.DrawRoundedRectangle(localx + bitmapIconDrag.GetWidth() / 4, localy + bitmapIconDrag.GetHeight() / 4, bitmapIconDrag.GetWidth() / 2, bitmapIconDrag.GetHeight() / 2, -0.25);
				dc.SetBrush(wxNullBrush);

				dc.SetBrush(wxBrush(*wxWHITE));
				CWindowMain::DrawTexte(&dc, libelle, xPos, yPos, themeFont);
				dc.SetBrush(wxNullBrush);
			}
		}
		

		
	}
	
}

void CThumbnail::Resize()
{
    TRACE();
	CalculControlSize();
	ResizeThumbnail();
	Refresh();
}

void CThumbnail::CalculControlSize()
{
    TRACE();
	controlWidth = GetWidth();
	controlHeight = GetHeight();
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		CControlSize * controlSize = new CControlSize();
		wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
		controlSize->controlWidth = controlWidth;
		controlSize->controlHeight = controlHeight;
		evt.SetClientData(controlSize);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CThumbnail::TestMaxX()
{
    TRACE();
    
	long xValue = GetWidth() - GetWindowWidth();

	if (posLargeur >= xValue)
		posLargeur = xValue;

	if (posLargeur < 0)
		posLargeur = 0;
}

void CThumbnail::OnEnterWindow(wxMouseEvent& event)
{
    TRACE();
   // refreshTimer->Start(100);
}
void CThumbnail::OnLeaveWindow(wxMouseEvent& event)
{
    TRACE();
    refreshTimer->Stop();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CThumbnail::TestMaxY()
{
    TRACE();
    
	long yValue = GetHeight() - GetWindowHeight();

	if (posHauteur >= yValue)
		posHauteur = yValue;

	if (posHauteur < 0)
		posHauteur = 0;
}

void CThumbnail::OnKeyUp(wxKeyEvent& event)
{
     switch (event.m_keyCode)
    {
    case WXK_CONTROL:
        controlKeyPush = false;
        break;
    }   
}

void CThumbnail::OnKeyDown(wxKeyEvent& event)
{
    TRACE();


    switch (event.m_keyCode)
    {
    case WXK_UP:
        this->MoveTop();
        break;
    case WXK_LEFT:
        this->MoveLeft();
        break;
    case WXK_DOWN:
        this->MoveBottom();
        break;
    case WXK_RIGHT:
        this->MoveRight();
        break;
    case WXK_CONTROL:
        controlKeyPush = true;
        break;
    }

}

void CThumbnail::OnMouseWheel(wxMouseEvent& event)
{
    TRACE();
    int move = 0;
#ifdef __APPLE__
    
    if (event.m_wheelRotation == 1)
        move = 0;
    else if (event.m_wheelRotation == -1)
       move = 1;
    
#else
    
    if (event.m_wheelRotation == 120)
        move = 0;
    else
        move = 1;
    
#endif

	if (controlKeyPush)
		move += 10;



	switch (move)
	{
	case 0:
		this->MoveTop();
		break;
	case 1:
		this->MoveBottom();
		break;
	case 10:
        {
            wxWindow * mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
            if (mainWindow != nullptr)
            {
                wxCommandEvent evt(wxEVENT_PICTUREPREVIOUS);
                mainWindow->GetEventHandler()->AddPendingEvent(evt);
            }
        }
		break;
	case 11:
        {
            wxWindow * mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
            if (mainWindow != nullptr)
            {
                wxCommandEvent evt(wxEVENT_PICTURENEXT);
                mainWindow->GetEventHandler()->AddPendingEvent(evt);
            }
        }
		break;
	}

}



void CThumbnail::InitScrollingPos()
{
    TRACE();

	posHauteur = 0;
	posLargeur = 0;


	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		wxSize * size = new wxSize();
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		size->x = posLargeur;
		size->y = posHauteur;
		evt.SetClientData(size);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}

}


void CThumbnail::UpdateRenderIcone(wxCommandEvent& event)
{
    TRACE();

	
	wxString * filename = new wxString();
    CThreadLoadingBitmap * threadLoadingBitmap = (CThreadLoadingBitmap *)event.GetClientData();
    if(threadLoadingBitmap == nullptr)
    {
        return;
    }
	if (threadDataProcess != false)
	{
		nbProcess--;

		if (threadLoadingBitmap != nullptr)
		{
			if (threadLoadingBitmap->bitmapIcone != nullptr)
			{
				CThumbnailData* pThumbnailData = nullptr;
				CIcone* icone = GetIconeById(threadLoadingBitmap->photoId);
				*filename = threadLoadingBitmap->filename;
				if (icone != nullptr && pThumbnailData == nullptr)
					pThumbnailData = icone->GetData();

				if (icone != nullptr && pThumbnailData != nullptr)
				{
					if (pThumbnailData->GetTypeElement() == TYPEVIDEO)
					{
						pThumbnailData->SetTimePosition(threadLoadingBitmap->timePosition);
					}
					pThumbnailData->SetIsProcess(false);
					pThumbnailData->SetBitmap(threadLoadingBitmap->bitmapIcone);
					pThumbnailData->SetIsLoading(false);
				}

				//RefreshIcone(threadLoadingBitmap->photoId);
			}
		}

		if (threadLoadingBitmap->bitmapIcone != nullptr)
		{
			delete(threadLoadingBitmap->bitmapIcone);
			threadLoadingBitmap->bitmapIcone = nullptr;
		}

		if (threadLoadingBitmap->_thread != nullptr)
		{
			if (threadLoadingBitmap->_thread->joinable())
				threadLoadingBitmap->_thread->join();

			delete threadLoadingBitmap->_thread;

			threadLoadingBitmap->_thread = nullptr;
		}


		if (threadLoadingBitmap != nullptr)
		{
			delete threadLoadingBitmap;
			threadLoadingBitmap = nullptr;
		}

		CLibPicture libPicture;
		if (libPicture.TestIsVideo(*filename) || libPicture.TestIsPDF(*filename) || libPicture.TestIsAnimation(*filename))
		{
			wxWindow* mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
			if (mainWnd != nullptr)
			{
				wxCommandEvent eventChange(wxEVENT_ENDVIDEOTHUMBNAIL);
				eventChange.SetClientData(filename);
				mainWnd->GetEventHandler()->AddPendingEvent(eventChange);
			}
		}
		else
			delete filename;
	}
	else
	{
		wxCommandEvent* event = new wxCommandEvent(EVENT_ICONEUPDATE);
		event->SetClientData(threadLoadingBitmap);
		wxQueueEvent(this, event);
	}
	if (!timerAnimation->IsRunning())
		timerAnimation->Start(50, true);
}
