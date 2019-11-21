#include <header.h>
#include "CategoryDetection.h"
#include <FileUtility.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(CCategoryDetectionDlg)
	//*)
#endif
//(*InternalHeaders(CCategoryDetectionDlg)
#include <wx/xrc/xmlres.h>
#include <wx/dir.h>
#include <RegardsConfigParam.h>
#include <turbojpeg.h>
#include <LibResource.h>
#include <ConvertUtility.h>
#include <window_id.h>
#include <SqlPhotos.h>
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

using namespace Regards::Sqlite;

#define wxEVENT_CATEGORYADD 1
//*)

//(*IdInit(CCategoryDetectionDlg)
//*)

BEGIN_EVENT_TABLE(CCategoryDetectionDlg, wxDialog)
//(*EventTable(CCategoryDetectionDlg)
	EVT_IDLE(CCategoryDetectionDlg::OnIdle)
	EVT_SIZE(CCategoryDetectionDlg::OnSize)
	//*)
END_EVENT_TABLE()

class CThreadCategory
{
public:

	CThreadCategory()
	{
		thread = nullptr;
		mainWindow = nullptr;
		//faceData = nullptr;
	};
	~CThreadCategory() {

	};

	CPictureData * LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth = 0, const int &resizeHeight = 0);
	float CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &widthOutput, const int &heightOutput);

	wxString filename;
	std::thread * thread;
	wxWindow * mainWindow;
	CPictureCategorieLoadData * categoryData;
};


float CThreadCategory::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &widthOutput, const int &heightOutput)
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


CPictureData * CThreadCategory::LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth, const int &resizeHeight)
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



CCategoryDetectionDlg::CCategoryDetectionDlg(wxWindow* parent)
{
	progress = 0;
	width = 0;
	height = 0;
	start = false;
	mode = 0;

	//(*Initialize(CCategoryDetectionDlg)
	wxXmlResource::Get()->LoadObject(this,parent,_T("CategoryDetectionDlg"),_T("wxDialog"));
	Gauge1 = (wxGauge*)FindWindow(XRCID("ID_GAUGE1"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	btOK = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	progress = 0;
	Gauge1->SetRange(100);
	Gauge1->SetValue(0);
	Connect(wxEVENT_CATEGORYADD, wxCommandEventHandler(CCategoryDetectionDlg::OnCategoryAdd));
	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CCategoryDetectionDlg::OnButton1Click);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&CCategoryDetectionDlg::OnInit);
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

	categorieData = new CPictureCategorieLoadData();
	//pictureCategorie = new CPictureCategorie();
	categorieData->LoadData((const char*)model.mb_str(wxConvUTF8));

	SetTitle("Category USENET recognition");
}

CCategoryDetectionDlg::~CCategoryDetectionDlg()
{
	//(*Destroy(CCategoryDetectionDlg)
	//*)
	if (categorieData != nullptr)
		delete categorieData;
}

void CCategoryDetectionDlg::OnSize(wxSizeEvent& evt)
{

}


//---------------------------------------------------------------------------------------
//Test FacialRecognition
//---------------------------------------------------------------------------------------
void CCategoryDetectionDlg::CategoryRecognition(void * param)
{
	CSqlPhotoCategorieUsenet facePhoto;
	CThreadCategory * path = (CThreadCategory *)param;
	bool pictureOK = false;
	CRegardsConfigParam * config = (CRegardsConfigParam*)CParamInit::getInstance();
	int pictureSize = config->GetFaceDetectionPictureSize();
	CSqlThumbnail sqlThumbnail;
	CPictureData * pictureData = nullptr;
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

	if (pictureOK)
	{
		CSqlPhotos sqlPhotos;
		int numPhotoId = sqlPhotos.GetPhotoId(path->filename);

		CPictureCategorie * pictureCategorie = new CPictureCategorie();
		std::vector<string> labels;
		void * copyData = path->categoryData->GetCopyData();
		std::vector<int> listCategorie = pictureCategorie->GetCategorieFromJpegBuffer(pictureData->data, pictureData->size, copyData);
		for (int i : listCategorie)
		{
			char label[255];
			pictureCategorie->GetLabel(i, label, 255, copyData);
			wxString _libelle = wxString::FromUTF8(label);
			facePhoto.InsertPhotoCategorie(numPhotoId, i, _libelle);
			

			labels.push_back(label);
		}
		path->categoryData->DeleteCopy(copyData);
		delete pictureCategorie;
	}




	if (path->mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_CATEGORYADD);
		evt.SetClientData(path);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}

}



void CCategoryDetectionDlg::OnCategoryAdd(wxCommandEvent& event)
{
	CThreadCategory * path = (CThreadCategory *)event.GetClientData();
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

void CCategoryDetectionDlg::SetStart()
{
	start = true;
}

void CCategoryDetectionDlg::OnIdle(wxIdleEvent& evt)
{
	int nbProcesseur = 1;
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
		nbProcesseur = config->GetFaceProcess();

	CSqlPhotoCategorieUsenet facePhoto;
	vector<wxString> listPhoto = facePhoto.GetPhotoListTreatment();
	if (nbFile == 0)
		nbFile = listPhoto.size();

	//Recherche Nb Fichier non traité pour le visage
	if (nbProcessFacePhoto < nbProcesseur && listPhoto.size() > 0 && !pushCloseButton)
	{
		facePhoto.InsertPhotoProcessing(listPhoto.at(0));

		CThreadCategory * path = new CThreadCategory();
		path->filename = listPhoto.at(0);
		path->mainWindow = this;
		path->categoryData = categorieData;
		path->thread = new thread(CategoryRecognition, path);

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

void CCategoryDetectionDlg::OnInit(wxInitDialogEvent& event)
{
}

void CCategoryDetectionDlg::OnButton1Click(wxCommandEvent& event)
{
	pushCloseButton = true;
}
