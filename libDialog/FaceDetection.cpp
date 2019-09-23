#include <header.h>
#include "FaceDetection.h"
#include <FileUtility.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(CFaceDetectionDlg)
	//*)
#endif
//(*InternalHeaders(CFaceDetectionDlg)
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
#include <SqlFacePhoto.h>
#include <SqlFaceDescriptor.h>
#include <SqlFindFacePhoto.h>
#include <SqlFaceRecognition.h>
#include <SqlFaceLabel.h>
using namespace Regards::Sqlite;


//*)

//(*IdInit(CFaceDetectionDlg)
//*)

BEGIN_EVENT_TABLE(CFaceDetectionDlg, wxDialog)
//(*EventTable(CFaceDetectionDlg)
	EVT_IDLE(CFaceDetectionDlg::OnIdle)
	EVT_SIZE(CFaceDetectionDlg::OnSize)
	//*)
END_EVENT_TABLE()

class CThreadFace
{
public:

	CThreadFace()
	{
		thread = nullptr;
		mainWindow = nullptr;
		faceData = nullptr;
	};
	~CThreadFace() {

	};

	CPictureData * LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth = 0, const int &resizeHeight = 0);
	float CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &widthOutput, const int &heightOutput);

	wxString filename;
	std::thread * thread;
	wxWindow * mainWindow;
	CFaceLoadData * faceData;
};


float CThreadFace::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &widthOutput, const int &heightOutput)
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


CPictureData * CThreadFace::LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth, const int &resizeHeight)
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



CFaceDetectionDlg::CFaceDetectionDlg(wxWindow* parent)
{
	faceData = new CFaceLoadData();

	progress = 0;
	width = 0;
	height = 0;
	start = false;
	mode = 0;

	//(*Initialize(CFaceDetectionDlg)
	wxXmlResource::Get()->LoadObject(this,parent,_T("CFaceDetectionDlg"),_T("wxDialog"));
	Gauge1 = (wxGauge*)FindWindow(XRCID("ID_GAUGE1"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	btOK = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	progress = 0;
	Gauge1->SetRange(100);
	Gauge1->SetValue(0);
	Connect(wxEVENT_FACEPHOTOADD, wxCommandEventHandler(CFaceDetectionDlg::OnFacePhotoAdd));
	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CFaceDetectionDlg::OnButton1Click);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&CFaceDetectionDlg::OnInit);
	//*)

	this->SetBackgroundColour(wxColour(64, 64, 64));
	pushCloseButton = false;
	nbProcessFacePhoto = 0;

#ifdef WIN32
	//Face Data Preload

	wxString model = CFileUtility::GetResourcesFolderPath() + "\\dlib_face_recognition_resnet_model_v1.dat";
	wxString facelandmark = CFileUtility::GetResourcesFolderPath() + "\\shape_predictor_5_face_landmarks.dat";

#else
	wxString model = CFileUtility::GetResourcesFolderPath() + "/dlib_face_recognition_resnet_model_v1.dat";
	//wxString facelandmark = CFileUtility::GetResourcesFolderPath() + "/shape_predictor_68_face_landmarks.dat";
	wxString facelandmark = CFileUtility::GetResourcesFolderPath() + "/shape_predictor_5_face_landmarks.dat";
#endif

	faceData->LoadData((const char*)model.mb_str(wxConvUTF8), (const char*)facelandmark.mb_str(wxConvUTF8));
}

CFaceDetectionDlg::~CFaceDetectionDlg()
{
	//(*Destroy(CFaceDetectionDlg)
	//*)
	if (faceData != nullptr)
		delete faceData;
}

void CFaceDetectionDlg::Start()
{
	start = true;
}

void CFaceDetectionDlg::OnSize(wxSizeEvent& evt)
{

}


//---------------------------------------------------------------------------------------
//Test FacialRecognition
//---------------------------------------------------------------------------------------
void CFaceDetectionDlg::FacialRecognition(void * param)
{
	CThreadFace * path = (CThreadFace *)param;
	bool pictureOK = false;
	CSqlFaceLabel sqlfaceLabel;
	void * faceData = path->faceData->GetCopyFaceData();
	CSqlFacePhoto sqlfacePhoto;
	CSqlFaceDescriptor sqlfaceDescritor;
	CFace * face = new CFace();
	CPictureData * pictureData = nullptr;
	int nbFaceFound = 0;
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




	if (pictureOK)
	{
		if (pictureData != nullptr)
			face->LoadPictureFromJpegBuffer(pictureData->data, pictureData->size, 0, faceData);

		int nbFace = face->GetNbFaceFound();

		if (nbFace > 0)
		{
			for (int i = 0; i < nbFace; i++)
			{
				//bool findFaceCompatible = false;
				long width = 0;
				long height = 0;
				double pertinence = 0.0;
				bool result = false;
				uint8_t * data = nullptr;
				uint8_t * dataBitmap = nullptr;
				unsigned long size = 0;
				long bitmapSize = 0;

				face->GetFaceData(i, width, height, pertinence, dataBitmap, bitmapSize);
				if (bitmapSize > 0)
				{
					dataBitmap = new uint8_t[bitmapSize];
					face->GetFaceData(i, width, height, pertinence, dataBitmap, bitmapSize);

					size = bitmapSize;

					uint8_t * data = nullptr;

					const int JPEG_QUALITY = 75;

					tjhandle _jpegCompressor = tjInitCompress();

					tjCompress2(_jpegCompressor, dataBitmap, width, 0, height, TJPF_RGBX,
						&data, &size, TJSAMP_444, JPEG_QUALITY,
						TJFLAG_FASTDCT);

					tjDestroy(_jpegCompressor);


					if (size > 0 && data != nullptr)
					{
						int numFace = 0;
						if (data != nullptr)
						{
							numFace = sqlfacePhoto.InsertFace(path->filename, i, width, height, pertinence, data, size);
							//numFace = sqlfacePhoto.GetNumFace(path->filename, i);
						}

						//Récupération des données du visage

						CFaceDescriptor * faceDescriptor = new CFaceDescriptor();
						faceDescriptor->numFace = numFace;
						faceDescriptor->size = 0;
						face->GetFaceDescriptor(i, faceDescriptor->descriptor, faceDescriptor->size);
						if (faceDescriptor->size > 0)
						{
							faceDescriptor->descriptor = new char[faceDescriptor->size + 1];
							face->GetFaceDescriptor(i, faceDescriptor->descriptor, faceDescriptor->size);
						}

						sqlfaceDescritor.InsertFaceDescriptor(numFace, faceDescriptor->descriptor, faceDescriptor->size);

						FindCompatibility(faceDescriptor);

						nbFaceFound++;
					}

				}

				if (data != nullptr)
				{
					tjFree(data);
					data = nullptr;
				}
				//delete[] data;

				if (dataBitmap != nullptr)
					delete[] dataBitmap;
			}
		}
	}


	if (nbFaceFound == 0)
	{
		sqlfacePhoto.InsertFace(path->filename, 0, 0, 0, 0, nullptr, 0);
	}

	if (pictureData != nullptr)
		delete pictureData;

	if (face != nullptr)
		delete face;

	path->faceData->DeleteCopyFaceData(faceData);

	if (path->mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_FACEPHOTOADD);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}

}


//---------------------------------------------------------------------------------------
//Test si compatible avec une autre Face à déplacer ailleurs
//---------------------------------------------------------------------------------------	
void CFaceDetectionDlg::FindCompatibility(CFaceDescriptor * faceDescriptor)
{
	if (faceDescriptor != nullptr)
	{
		CSqlFaceLabel sqlfaceLabel;
		CFace * face = new CFace();
		bool findFaceCompatible = false;
		CSqlFaceRecognition sqlfaceRecognition;
		CSqlFindFacePhoto sqlfindFacePhoto;

		std::vector<CFaceDescriptor*> listFace = sqlfindFacePhoto.GetUniqueFaceDescriptor(faceDescriptor->numFace);
		for (int i = 0; i < listFace.size(); i++)
		{

			CFaceDescriptor * facede = listFace[i];
			float isCompatible = face->IsCompatibleFace(faceDescriptor->descriptor, facede->descriptor);
			if (isCompatible < 0.6)
			{
				sqlfaceRecognition.InsertFaceRecognition(faceDescriptor->numFace, facede->numFace);
				findFaceCompatible = true;
				break;
			}
		}

		for (int i = 0; i < listFace.size(); i++)
		{
			CFaceDescriptor * facede = listFace[i];
			delete facede;
		}

		if (!findFaceCompatible)
		{
			wxString label = "Face number " + to_string(faceDescriptor->numFace);
			sqlfaceRecognition.InsertFaceRecognition(faceDescriptor->numFace, faceDescriptor->numFace);
			sqlfaceLabel.InsertFaceLabel(faceDescriptor->numFace, label, true);
		}

		if (faceDescriptor != nullptr)
			delete faceDescriptor;

		delete face;
	}
}

void CFaceDetectionDlg::OnFacePhotoAdd(wxCommandEvent& event)
{
	CThreadFace * path = (CThreadFace *)event.GetClientData();
	if (path->thread != nullptr)
	{
		path->thread->join();
		delete(path->thread);
		path->thread = nullptr;
	}

	if (path != nullptr)
		delete path;

	nbProcessFacePhoto--;
}


void CFaceDetectionDlg::OnIdle(wxIdleEvent& evt)
{
	int nbProcesseur = 1;
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
		nbProcesseur = config->GetFaceProcess();

	CSqlFacePhoto facePhoto;
	vector<wxString> listPhoto = facePhoto.GetPhotoListTreatment();

	//Recherche Nb Fichier non traité pour le visage
	if (nbProcessFacePhoto < nbProcesseur && listPhoto.size() > 0 && !pushCloseButton)
	{
		PhotosVector photoList;
		CSqlFindPhotos findphotos;
		findphotos.GetAllPhotos(&photoList);

		CSqlFacePhoto sqlFacePhoto;
		sqlFacePhoto.InsertFaceTreatment(listPhoto.at(0));

		CThreadFace * path = new CThreadFace();
		path->filename = listPhoto.at(0);
		path->mainWindow = this;
		path->faceData = faceData;
		path->thread = new thread(FacialRecognition, path);

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

void CFaceDetectionDlg::OnInit(wxInitDialogEvent& event)
{
}

void CFaceDetectionDlg::OnButton1Click(wxCommandEvent& event)
{
	pushCloseButton = true;
}
