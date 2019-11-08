#include <header.h>
#include "TypePhotoDetection.h"
#include <FileUtility.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(CTypePhotoDetectionDlg)
	//*)
#endif
//(*InternalHeaders(CTypePhotoDetectionDlg)
#include <wx/xrc/xmlres.h>
#include <wx/dir.h>
#include <RegardsConfigParam.h>
#include <turbojpeg.h>
#include <LibResource.h>
#include <ConvertUtility.h>
#include <window_id.h>
#include <SqlFindCriteria.h>
#include <SqlCriteria.h>
#include <SqlGps.h>
#include <SqlPhotoCriteria.h>
#include <SqlPhotos.h>
#include <SqlFindPhotos.h>
#include <SqlThumbnail.h>
#include <RegardsBitmap.h>
#include <libPicture.h>
#include <PictureRecognition.h>
#include <ParamInit.h>
#include <ThumbnailData.h>
#ifdef EXIV2
#include <MetadataExiv2.h>
using namespace Regards::exiv2;
#elif defined(WIN32)
#include <SetMetadataGps.h>
#include <SetMetadataDate.h>
#endif
#include <ImageLoadingFormat.h>
#include <picture_id.h>
#include <SqlPhotoCategorieUsenet.h>
#include <SqlPhotos.h>
using namespace Regards::Sqlite;


//*)

//(*IdInit(CTypePhotoDetectionDlg)
//*)

BEGIN_EVENT_TABLE(CTypePhotoDetectionDlg, wxDialog)
//(*EventTable(CTypePhotoDetectionDlg)
	EVT_IDLE(CTypePhotoDetectionDlg::OnIdle)
	EVT_SIZE(CTypePhotoDetectionDlg::OnSize)
	//*)
END_EVENT_TABLE()

class CThreadTypePhoto
{
public:

	CThreadTypePhoto()
	{
		thread = nullptr;
		mainWindow = nullptr;
		faceData = nullptr;
	};
	~CThreadTypePhoto() {

	};

	CPictureData * LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth = 0, const int &resizeHeight = 0);
	float CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &widthOutput, const int &heightOutput);

	wxString filename;
	std::thread * thread;
	wxWindow * mainWindow;
	CPictureCategorieLoadData * faceData;
};


float CThreadTypePhoto::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &widthOutput, const int &heightOutput)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float newRatio = 1;

	if (pictureWidth > pictureHeight)
		newRatio = (float)widthOutput / (float)(pictureWidth);
	else
		newRatio = (float)heightOutput / (float)(pictureHeight);

	if ((pictureHeight * newRatio) > heightOutput)
	{
		newRatio = (float)heightOutput / (float)(pictureHeight);
	}
	else
	{
		if ((pictureWidth * newRatio) > widthOutput)
		{
			newRatio = (float)widthOutput / (float)(pictureWidth);
		}
	}

	return newRatio;
}


CPictureData * CThreadTypePhoto::LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth, const int &resizeHeight)
{
	CPictureData * pictureData = nullptr;
	CLibPicture libPicture;
	CImageLoadingFormat * imageLoading = libPicture.LoadPicture(filename);
	if (imageLoading != nullptr)
	{
		pictureOK = imageLoading->IsOk();
		if (pictureOK)
		{
			imageLoading->ApplyExifOrientation(1);
			imageLoading->ConvertToRGB24(true);
			//Calcul Resize Size
			pictureData = new CPictureData();
			if (resizeWidth != 0 && resizeHeight != 0)
			{
				float ratio = CalculPictureRatio(imageLoading->GetWidth(), imageLoading->GetHeight(), resizeWidth, resizeHeight);
				pictureData->width = imageLoading->GetWidth() * ratio;
				pictureData->height = imageLoading->GetHeight() * ratio;
				imageLoading->Resize(pictureData->width, pictureData->height, 1);
			}
			else
			{
				pictureData->width = imageLoading->GetWidth();
				pictureData->height = imageLoading->GetHeight();
			}

			unsigned long outputsize = 0;
			int compressMethod = 0;
			uint8_t * data = imageLoading->GetJpegData(outputsize, compressMethod);
			pictureData->data = new uint8_t[outputsize];
			memcpy(pictureData->data, data, outputsize);
			imageLoading->DestroyJpegData(data, compressMethod);

			pictureData->size = outputsize;

		}
	}

	if (imageLoading != nullptr)
		delete imageLoading;

	imageLoading = nullptr;

	return pictureData;
}



CTypePhotoDetectionDlg::CTypePhotoDetectionDlg(wxWindow* parent)
{
	faceData = new CPictureCategorieLoadData();

	progress = 0;
	width = 0;
	height = 0;
	start = false;
	mode = 0;

	//(*Initialize(CTypePhotoDetectionDlg)
	wxXmlResource::Get()->LoadObject(this,parent,_T("CTypePhotoDetectionDlg"),_T("wxDialog"));
	Gauge1 = (wxGauge*)FindWindow(XRCID("ID_GAUGE1"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	btOK = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	progress = 0;
	Gauge1->SetRange(100);
	Gauge1->SetValue(0);
	Connect(wxEVENT_FACEPHOTOADD, wxCommandEventHandler(CTypePhotoDetectionDlg::OnFacePhotoAdd));
	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CTypePhotoDetectionDlg::OnButton1Click);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&CTypePhotoDetectionDlg::OnInit);
	//*)

	this->SetBackgroundColour(wxColour(64, 64, 64));
	pushCloseButton = false;
	nbProcessFacePhoto = 0;

#ifdef WIN32
	//Face Data Preload

	wxString model = CFileUtility::GetResourcesFolderPath() + "\\resnet34_1000_imagenet_classifier.dnn";

#else
	wxString model = CFileUtility::GetResourcesFolderPath() + "/resnet34_1000_imagenet_classifier.dnn";
#endif

	faceData->LoadData((const char*)model.mb_str(wxConvUTF8));
}

CTypePhotoDetectionDlg::~CTypePhotoDetectionDlg()
{
	//(*Destroy(CTypePhotoDetectionDlg)
	//*)
	if (faceData != nullptr)
		delete faceData;
}

void CTypePhotoDetectionDlg::Start()
{
	start = true;
}

void CTypePhotoDetectionDlg::OnSize(wxSizeEvent& evt)
{

}


//---------------------------------------------------------------------------------------
//Test FacialRecognition
//---------------------------------------------------------------------------------------
void CTypePhotoDetectionDlg::PictureCategoryRecognition(void * param)
{
	CThreadTypePhoto * path = (CThreadTypePhoto *)param;
	bool pictureOK = false;
	void * faceData = path->faceData->GetCopyData();
	CPictureCategorie * picture = new CPictureCategorie();
	CPictureData * pictureData = nullptr;
	CSqlThumbnail sqlThumbnail;
	CRegardsConfigParam * config = (CRegardsConfigParam*)CParamInit::getInstance();
	int pictureSize = config->GetFaceDetectionPictureSize();

	switch (pictureSize)
	{
	case 0: //Thumbnail Size
		if (sqlThumbnail.TestThumbnail(path->filename))
		{
			pictureData = sqlThumbnail.GetJpegThumbnail(path->filename);
			if (pictureData != nullptr)
				pictureOK = true;
		}
		else
		{
			pictureData = path->LoadPictureToJpeg(path->filename, pictureOK);
		}
		break;
	case 1: //Original Size
	{
		pictureData = path->LoadPictureToJpeg(path->filename, pictureOK);
	}
	break;

	case 2: //SD Format 
	{
		pictureData = path->LoadPictureToJpeg(path->filename, pictureOK, 640, 480);
	}
	break;


	case 3: //HD Format 
	{
		pictureData = path->LoadPictureToJpeg(path->filename, pictureOK, 1280, 720);
	}
	break;

	case 4: //Full HD Format 
	{
		pictureData = path->LoadPictureToJpeg(path->filename, pictureOK, 1920, 1080);
	}
	break;
	}

	vector<int> listCategorie;
	if (pictureOK)
	{
		if (pictureData != nullptr)
			listCategorie = picture->GetCategorieFromJpegBuffer(pictureData->data, pictureData->size, pictureData);
	}
	CSqlPhotoCategorieUsenet categoriePhoto;
	categoriePhoto.InsertPhotoProcessing(path->filename);

	for (int categorie : listCategorie)
	{
		CSqlPhotos photo;
		int numPhoto = photo.GetPhotoId(path->filename);
		categoriePhoto.InsertPhotoCategorie(numPhoto, categorie);
	}


	if (pictureData != nullptr)
		delete pictureData;

	if (picture != nullptr)
		delete picture;

	if (path->mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_FACEPHOTOADD);
		evt.SetClientData(path);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}

}

void CTypePhotoDetectionDlg::OnFacePhotoAdd(wxCommandEvent& event)
{
	CThreadTypePhoto * path = (CThreadTypePhoto *)event.GetClientData();
	if (path->thread != nullptr)
	{
		path->thread->join();
		delete(path->thread);
		path->thread = nullptr;
	}

	if (path != nullptr)
		delete path;

	int pos = (int)(((float)position / (float)nbFile) * 100.0f);
 	Gauge1->SetValue(pos);

	nbProcessFacePhoto--;
	position++;
}


void CTypePhotoDetectionDlg::OnIdle(wxIdleEvent& evt)
{
	int nbProcesseur = 1;
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
		nbProcesseur = config->GetFaceProcess();

	CSqlPhotoCategorieUsenet categoriePhoto;
	vector<wxString> listPhoto = categoriePhoto.GetPhotoListTreatment();
	if (nbFile == 0)
		nbFile = listPhoto.size();

	//Recherche Nb Fichier non traité pour le visage
	if (nbProcessFacePhoto < nbProcesseur && listPhoto.size() > 0 && !pushCloseButton)
	{
		CThreadTypePhoto * path = new CThreadTypePhoto();
		path->filename = listPhoto.at(0);
		path->mainWindow = this;
		path->faceData = faceData;
		path->thread = new thread(PictureCategoryRecognition, path);

		StaticText2->SetLabelText(path->filename);

		nbProcessFacePhoto++;

	}

	if(pushCloseButton && nbProcessFacePhoto == 0)
		this->Close();

	if (start && listPhoto.size() == 0 && nbProcessFacePhoto == 0)
	{
		start = false;
		wxMessageBox("All Pictures have been processed", "Informations", wxICON_INFORMATION);
		this->Close();
	}
}

void CTypePhotoDetectionDlg::OnInit(wxInitDialogEvent& event)
{
}

void CTypePhotoDetectionDlg::OnButton1Click(wxCommandEvent& event)
{
	pushCloseButton = true;
}
