#include "header.h"
#include "Thumbnail.h"
#include <LibResource.h>
#include <FileUtility.h>
#include <libPicture.h>
#include <wxmd5.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include "ScrollbarWnd.h"
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include "LoadingResource.h"
#include <SqlPhotosWithoutThumbnail.h>
#include <ImageLoadingFormat.h>
#include <ImageVideoThumbnail.h>
#include <SqlThumbnailVideo.h>
#include <wx/dcbuffer.h>
#include <wx/filename.h>
#include <ThumbnailData.h>
#include <ThumbnailDataSQL.h>
#include <ThumbnailDataStorage.h>
//#include <ThumbnailDataVideo.h>
#include <SqlFaceThumbnail.h>
#include <ThumbnailMessage.h>
using namespace Regards::Window;

//#define TBB
#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>

class CImageLoadingFormat;

#define TIMER_LOADING 4
#define TIMER_REFRESH 5
const int CThumbnail::TabSize[] = { 50,100,150,200,250,300,350,400,450,500,550,600,650,700};
const int CThumbnail::Max = 14;
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
	//int numIcone;
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

CIcone * CThumbnail::FindElement(const int &xPos, const int &yPos)
{
    TRACE();
    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * icone = iconeList->GetElement(i);
		if (icone != nullptr)
		{
			wxRect rc = icone->GetPos();
			if ((rc.x < xPos && xPos < rc.width) && (rc.y < yPos && yPos < rc.height))
			{
				return icone;
			}
		}
	}

	return nullptr;
}


void CThumbnail::EraseThumbnail(wxCommandEvent& event)
{
    TRACE();
    CSqlThumbnail sqlThumbnail;
    sqlThumbnail.EraseThumbnail();
    
    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
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
				pIcone->DestroyCache();
			}
		}
	}

	thumbnailPos = 0;
	CSqlPhotosWithoutThumbnail sqlPhoto;
	sqlPhoto.GeneratePhotoList();
    
    processIdle = true;
}

void CThumbnail::SetCheck(const bool &check)
{
    TRACE();
    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * pIcone = iconeList->GetElement(i);
		if (pIcone != nullptr)
		{
			pIcone->ShowSelectButton(check);
			pIcone->DestroyCache();
		}
	}
}

void CThumbnail::GetSelectItem(vector<CThumbnailData *> & vectorData)
{
    TRACE();
	//bool find = false;
    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * pIcone = iconeList->GetElement(i);
		if (pIcone != nullptr)
		{
			if (pIcone->IsChecked())
				vectorData.push_back(pIcone->GetData());
		}
	}

}

void CThumbnail::SetActifItem(const int &numItem, const bool &move)
{
    TRACE();

    int numElement = iconeList->GetNbElement();

	if (numItem >= numElement)
		return;

	if (numSelect != NULL)
		numSelect->SetSelected(false);

	if (numActif != nullptr)
	{
		numActif->SetActive(false);
	}

	numActif = iconeList->GetElement(numItem);

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
		}
		else{
			if (!isMoving)
			{
				wxRect rect = numActif->GetPos();
				//rect.x = posLargeur + rect.x;
				//rect.y = posHauteur + rect.y;

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
				
			}
		}


	}
    

	 
    

	numSelect = iconeList->GetElement(numItem);

	if (numSelect != nullptr)
	{
		numSelect->SetSelected(true);
	}


	this->Refresh();
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
	selectFilename = "";
	actifFilename = "";
	controlWidth = 0;
	controlHeight = 0;
	isMoving = 0;
	thumbnailPos = 0;
    oldPosLargeur = 0;
    oldPosHauteur = 0;    
	nbProcess = 0;
	numActif = nullptr;
	loadingIcone = nullptr;
	numSelect = nullptr;
	threadDataProcess = false;
	allThreadEnd = true;
    showLoadingBitmap = false;
    stepLoading = 0;
    loadingIcone = nullptr;
    m_animation = nullptr;
	render = false;
	check = false;
	
    iconeList = new CIconeList();
	this->testValidity = testValidity;
	thumbnailSizeX = 0;
	thumbnailSizeY = 0;
	posHauteur = 0;
	posLargeur = 0;
	numActif = 0;
	positionYOld = 0;
	positionXOld = 0;
	nbLigneX = 0;
	nbLigneY = 0;
	positionSize = 4;
	
	numStartElement = 0;
	nbElementToShow = 0;
	isStoragePt = false;


	//this->statusbar = statusbar;

	this->themeThumbnail = themeThumbnail;
	Connect(wxEVT_IDLE, wxIdleEventHandler(CThumbnail::OnIdle));
	Connect(wxEVT_PAINT, wxPaintEventHandler(CThumbnail::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CThumbnail::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CThumbnail::OnLButtonDown));
	Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(CThumbnail::OnLDoubleClick));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CThumbnail::OnMouseWheel));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CThumbnail::OnKeyDown));
	Connect(EVENT_ICONEUPDATE, wxCommandEventHandler(CThumbnail::UpdateRenderIcone));
	Connect(EVENT_UPDATEMESSAGE, wxCommandEventHandler(CThumbnail::UpdateMessage));
    
    Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CThumbnail::OnEnterWindow));
    Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CThumbnail::OnLeaveWindow));
    
    refreshTimer = new wxTimer(this, TIMER_REFRESH);
    Connect(TIMER_REFRESH, wxEVT_TIMER, wxTimerEventHandler(CThumbnail::OnRefreshIcone), nullptr, this);

    wxString resourcePath = CFileUtility::GetResourcesFolderPath();
    m_animation = new wxAnimation(resourcePath + "/loading.gif");

	Connect(wxEVENT_ONSTARTLOADINGPICTURE, wxCommandEventHandler(CThumbnail::StartLoadingPicture));
	Connect(wxEVENT_ONSTOPLOADINGPICTURE, wxCommandEventHandler(CThumbnail::StopLoadingPicture));
	Connect(wxEVENT_REFRESHDATA, wxCommandEventHandler(CThumbnail::EraseThumbnail));
	Connect(wxEVENT_SCROLLMOVE, wxCommandEventHandler(CThumbnail::OnScrollMove));
	Connect(wxEVENT_LEFTPOSITION, wxCommandEventHandler(CThumbnail::OnLeftPosition));
	Connect(wxEVENT_TOPPOSITION, wxCommandEventHandler(CThumbnail::OnTopPosition));
	processIdle = true;
    
}

void CThumbnail::MoveTop()
{
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVETOP);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnail::MoveLeft()
{
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVELEFT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnail::MoveBottom()
{
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVEBOTTOM);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnail::MoveRight()
{
	wxWindow * parent = this->GetParent();

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
	this->Refresh();
}

void CThumbnail::OnTopPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
	this->Refresh();
}

void CThumbnail::OnScrollMove(wxCommandEvent& event)
{
	isMoving = event.GetInt();
}

void CThumbnail::OnRefreshIcone(wxTimerEvent& event)
{
    TRACE();
	refreshTimer->Stop();
     this->Refresh();    
	 refreshTimer->Start(100);
}

CThumbnail::~CThumbnail()
{
    TRACE();
	threadDataProcess = false;
    
    refreshTimer->Stop();
    
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
	for (int i = 0; i < iconeList->GetNbElement(); i++)
	{
		CIcone * icone = iconeList->GetElement(i);
		if (icone != nullptr)
		{
			CThumbnailData * data = icone->GetData();
			if (selectFilename == data->GetFilename())
			{
				numSelect = icone;
				icone->SetSelected(true);
			}
			if (actifFilename == data->GetFilename())
			{
				numActif = icone;
				icone->SetActive(true);
			}

			if (numSelect != nullptr && numActif != nullptr)
				break;
		}
	}
}

void CThumbnail::EraseThumbnailList()
{
    TRACE();

	if (numSelect != nullptr)
	{
		CThumbnailData * data = numSelect->GetData();
		if(data != nullptr)
			selectFilename = data->GetFilename();
	}

	if (numActif != nullptr)
	{
		CThumbnailData * data = numActif->GetData();
		if (data != nullptr)
			actifFilename = data->GetFilename();
	}


    iconeList->EraseThumbnailList();

	numSelect = nullptr;
	numActif = nullptr;
	loadingIcone = nullptr;
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
	pLoadBitmap->thumbnail = this;
	pLoadBitmap->_thread = new thread(LoadPicture, pLoadBitmap);
	pThumbnailData->SetIsProcess(true);
}

void CThumbnail::ProcessIdle()
{
    TRACE();
    //printf("CThumbnail::ProcessIdle() \n");
	//printf("CThumbnail::pIconeList %d \n", pIconeList.size());
	ProcessThumbnailIdle();
    


	if (iconeList->GetNbElement() == 0 || threadDataProcess == false)
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

#ifdef TBB

	//nbProcesseur = 1;
	if (photoList.size() > 0)
	{
		wxCommandEvent * event = new wxCommandEvent(EVENT_UPDATEMESSAGE);
		event->SetExtraLong(photoList.size());
		wxQueueEvent(this, event);

		//tbb::task_scheduler_init init;  // Automatic number of threads
		tbb::task_scheduler_init init(tbb::task_scheduler_init::default_num_threads());  // Explicit number of threads

		std::vector<mytask> tasks;
		//#pragma omp parallel for

		int nbElement = iconeList->GetNbElement();

		for (int i = 0; i < nbElement; i++)
		{
			CIcone * icone = iconeList->GetElement(i);
			iconeList->Lock();

			if (icone != nullptr)
			{
				CThumbnailData * pThumbnailData = icone->GetData();

				if (pThumbnailData != nullptr)
				{
					CThreadLoadingBitmap * pLoadBitmap = new CThreadLoadingBitmap();
					pLoadBitmap->timePosition = pThumbnailData->GetTimePosition();
					pLoadBitmap->percent = pThumbnailData->GetPercent();
					pLoadBitmap->typeElement = pThumbnailData->GetTypeElement();
					pLoadBitmap->filename = pThumbnailData->GetFilename();
					pLoadBitmap->thumbnail = this;
					pLoadBitmap->numIcone = i;
					tasks.push_back(mytask(pLoadBitmap));
				}
			}
			iconeList->Unlock();
		}


		tbb::parallel_for(
			tbb::blocked_range<size_t>(0, tasks.size()),
			[&tasks](const tbb::blocked_range<size_t>& r)
		{
			for (size_t i = r.begin(); i < r.end(); ++i)
				tasks[i]();
		}
		);
	}

#else

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

			if (iconeList->GetNbElement() == 0 || threadDataProcess == false)
				return;

			wxString filename = photoList[j];
            int nbElement = iconeList->GetNbElement();
            
			for (int i = 0; i < nbElement; i++)
			{
				if (threadDataProcess == false)
					return;
				
                bool exitfor = false;
				CIcone * icone = iconeList->GetElement(i);
				iconeList->Lock();
				
				if (icone != nullptr)
				{
					CThumbnailData * pThumbnailData = icone->GetData();
					
                    if(pThumbnailData != nullptr)
                    {
                        if (pThumbnailData->GetFilename() == filename)
                        {
                            wxString filename = pThumbnailData->GetFilename();
                            bool isLoad = pThumbnailData->IsLoad();
                            bool isProcess = pThumbnailData->IsProcess();
                            if (!isProcess && !isLoad)
                            {
								ProcessThumbnail(pThumbnailData);
								nbProcess++;
								photoList.erase(photoList.begin());
                            }
                            sqlPhoto.InsertProcessStart(filename);
                            
                            exitfor = true;
                        }
                    }
				}
				iconeList->Unlock();

				if (exitfor)
					break;
			}
		}
	}

#endif

	if (photoList.size() == 0)
		processIdle = false;     

	
}

void CThumbnail::UpdateMessage(wxCommandEvent& event)
{
    TRACE();
	int nbPhoto = event.GetExtraLong();
	CThumbnailMessage * thumbnailMessage = new CThumbnailMessage();
	thumbnailMessage->nbPhoto = nbPhoto;
	thumbnailMessage->thumbnailPos = thumbnailPos;
	thumbnailMessage->nbElement = iconeList->GetNbElement();

	wxWindow * mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
	wxCommandEvent eventChange(wxEVENT_UPDATEMESSAGETHUMBNAIL);
	eventChange.SetClientData(thumbnailMessage);
	mainWnd->GetEventHandler()->AddPendingEvent(eventChange);

	//wxString message = L"Picture render missing : " + to_string(nbPhoto);

	//statusbar->SetRangeProgressBar(iconeList->GetNbElement());
	/*
	if (statusbar != nullptr)
	{
		statusbar->SetText(2, message);
		
		statusbar->SetPosProgressBar(thumbnailPos + 1);
	}
	*/
	thumbnailPos++;
}

void CThumbnail::OnIdle(wxIdleEvent& evt)
{
    //TRACE();
  // if (processIdle && !endProgram)
   //     ProcessIdle();
	StartThread();
}

bool CThumbnail::GetProcessEnd()
{
    TRACE();
	if(nbProcess > 0 || !processEnd)
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
                if (dest != nullptr && !sqlThumbnailVideo.TestThumbnail(filename, i))
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

        int xPos = event.GetX();
        int yPos = event.GetY();
        
        ::wxSetCursor(wxCursor(wxCURSOR_HAND));
        CIcone * pBitmapIcone = FindElement(xPos, yPos);
        
		if(pBitmapIcone != nullptr)
		{
        
			if (numActif != nullptr)
			{
				if (pBitmapIcone != numActif)
				{
					numActif->SetActive(false);
					needtoRedraw = true;
				}
            
			}

            if (pBitmapIcone->GetState() != ACTIFICONE)
            {
                numActif = pBitmapIcone;
                pBitmapIcone->SetActive(true);
                needtoRedraw = true;
            }
		}
    
    if(needtoRedraw)
    {
		 this->Refresh();
    }

    this->GetParent()->GetEventHandler()->ProcessEvent(event);
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

	int value = pBitmapIcone->RenderIcone(deviceContext, posLargeur, posHauteur);

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
    iconeList->DestroyCacheThumbnailList();
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

void CThumbnail::OnLButtonDown(wxMouseEvent& event)
{
    TRACE();
	wxClientDC winDC(this);
	this->SetFocus();
	int xPos = event.GetX();
	int yPos = event.GetY();

	if (numSelect != nullptr)
	{
		numSelect->SetSelected(false);
		//numSelect->RenderIcone(&winDC);
	}

	CIcone * pBitmapIcone = FindElement(xPos, yPos);
	if (pBitmapIcone != nullptr)
	{
		numSelect = pBitmapIcone;
		pBitmapIcone->OnClick(xPos, yPos, posLargeur, posHauteur);
        //
		CThumbnailData * data = pBitmapIcone->GetCopyData();
        OnPictureClick(data);
		delete data;
        pBitmapIcone->SetSelected(true);
	}
	else
	{
		FindOtherElement(&winDC, xPos, yPos);
	}

	this->Refresh();

}




void CThumbnail::StartLoadingPicture(wxCommandEvent& event)
{
    TRACE();
	int numItem = event.GetExtraLong();

    if(loadingIcone != nullptr)
       loadingIcone->StopLoadingPicture();
    
    if (numItem >= iconeList->GetNbElement())
        return;

    
    loadingIcone = iconeList->GetElement(numItem);
    
    stepLoading = 0;
    showLoadingBitmap = true;

    loadingIcone->StartLoadingPicture();
}

void CThumbnail::StopLoadingPicture(wxCommandEvent& event)
{
    TRACE();

    if(loadingIcone != nullptr)
    {
        loadingIcone->StopLoadingPicture();
        loadingIcone->DestroyCache();
    }
    
    showLoadingBitmap = false;

	this->Refresh();

}

void CThumbnail::OnPaint(wxPaintEvent& event)
{
    TRACE();
    int width = GetWindowWidth();
    int height = GetWindowHeight();
    if(width == 0 || height == 0)
        return;

	TestMaxX();
	TestMaxY();

    
	render = true;
    printf("CThumbnail::OnPaint \n");   
    printf("CThumbnail::OnPaint not buffered \n");
    
    wxBufferedPaintDC dc(this);
    wxRect rc = GetWindowRect();
    UpdateScroll();
    FillRect(&dc, rc, themeThumbnail.colorBack);
    RenderIcone(&dc);

	render = false;

    oldPosLargeur = posLargeur;
    oldPosHauteur = posHauteur;
}

void CThumbnail::Resize()
{
    TRACE();
    
	CalculControlSize();
	ResizeThumbnail();

}

void CThumbnail::CalculControlSize()
{
    TRACE();
	controlWidth = GetWidth();
	controlHeight = GetHeight();
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		ControlSize * controlSize = new ControlSize();
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
    refreshTimer->Start(100);
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

		}

}

void CThumbnail::OnMouseWheel(wxMouseEvent& event)
{
    TRACE();
    
#ifdef __APPLE__
    
    if (event.m_wheelRotation == 1)
        this->MoveTop();
    else if (event.m_wheelRotation == -1)
        this->MoveBottom();
    
#else
    
    if (event.m_wheelRotation == 120)
        this->MoveTop();
    else
        this->MoveBottom();
    
#endif

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

CIcone * CThumbnail::FindIcone(const wxString &filename)
{
    TRACE();
    
    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * icone = iconeList->GetElement(i);
		if (icone != nullptr)
		{
			CThumbnailData * pThumbnailData = icone->GetData();
			if (pThumbnailData->GetFilename() == filename)
				return icone;
		}
	}
	return nullptr;
}

void CThumbnail::UpdateRenderIcone(wxCommandEvent& event)
{
    TRACE();
    
	nbProcess--;

    CThreadLoadingBitmap * threadLoadingBitmap = (CThreadLoadingBitmap *)event.GetClientData();
    if(threadLoadingBitmap == nullptr)
    {
        return;
    }
    
    
    if(threadDataProcess != false && threadLoadingBitmap != nullptr)
    {
        if (threadLoadingBitmap->bitmapIcone != nullptr)
        {
			CThumbnailData * pThumbnailData = nullptr;
			CIcone * icone = FindIcone(threadLoadingBitmap->filename);

			if (icone != nullptr && pThumbnailData == nullptr)
				pThumbnailData = icone->GetData();
					
			if (icone != nullptr && pThumbnailData != nullptr)
            {
                if (pThumbnailData->GetTypeElement() == TYPEVIDEO)
                    pThumbnailData->SetTimePosition(threadLoadingBitmap->timePosition);
                    
				pThumbnailData->SetIsProcess(false);
                pThumbnailData->SetBitmap(threadLoadingBitmap->bitmapIcone); 
                pThumbnailData->SetIsLoading(false);
                    
                icone->DestroyCache();

            }
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

	this->Refresh();
}
