#include <header.h>
#include "ConfigRegards.h"
#include <RegardsConfigParam.h>
#include <LibResource.h>
#include <FileUtility.h>
#include <ParamInit.h>
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <opencv2/core/ocl.hpp>
#include <Gps.h>
#include <MediaExtractor.h>
#include <appcontext.h>
extern AppContext application_context;
using namespace Regards::Viewer;
#ifndef WX_PRECOMP
//(*InternalHeadersPCH(ConfigRegards)
//*)
#endif
//(*InternalHeaders(ConfigRegards)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(ConfigRegards)
//*)

BEGIN_EVENT_TABLE(ConfigRegards, wxDialog)
		//(*EventTable(ConfigRegards)
		//*)
END_EVENT_TABLE()

ConfigRegards::ConfigRegards(wxWindow* parent)
{
	isOk = false;
	//(*Initialize(ConfigRegards)
	SetTitle("Regards Configuration");
	wxXmlResource::Get()->LoadObject(this, parent,_T("ConfigRegards"),_T("wxDialog"));
	rbTransitionEffect = static_cast<wxComboBox*>(FindWindow(XRCID("ID_RBTRANSITIONEFFECT")));
	rbTransitionDiaporamaEffect = static_cast<wxComboBox*>(FindWindow(XRCID("ID_RBDIAPORAMATRANSITIONEFFECT")));
	sbDiaporama = static_cast<wxStaticBox*>(FindWindow(XRCID("ID_SBDIAPORAMA")));
	btOk = static_cast<wxButton*>(FindWindow(XRCID("ID_OK")));
	scTime = static_cast<wxSpinCtrl*>(FindWindow(XRCID("ID_SCTIME")));
	stTime = static_cast<wxStaticText*>(FindWindow(XRCID("ID_STTIME")));
	scProcessExif = static_cast<wxSpinCtrl*>(FindWindow(XRCID("ID_SCEXIF")));
	scProcessFace = static_cast<wxSpinCtrl*>(FindWindow(XRCID("ID_SCFACE")));
	scProcessThumbnail = static_cast<wxSpinCtrl*>(FindWindow(XRCID("ID_SCTHUMBNAIL")));
	btCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_CANCEL")));
	sbThumbnail = static_cast<wxStaticBox*>(FindWindow(XRCID("ID_STATICBOX2")));
	//ID_RBVIDEOFACEDETECTION
	rbVideoFaceDetection = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBVIDEOFACEDETECTION")));
	rbFaceDetection = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBFACEDETECTION")));
	txtPicturePath = static_cast<wxTextCtrl*>(FindWindow(XRCID("ID_TXTPICTUREPATH")));
	btPicturePath = static_cast<wxButton*>(FindWindow(XRCID("ID_PICTUREPATH")));
	txtVideoPath = static_cast<wxTextCtrl*>(FindWindow(XRCID("ID_TXTVIDEOPATH")));
	btVideoPath = static_cast<wxButton*>(FindWindow(XRCID("ID_VIDEOPATH")));

	rbDatabaseInMemory = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBDATAINMEMORY")));
	rbAutoRotate = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBROTATEAUTO")));
	rbInterpolation = static_cast<wxComboBox*>(FindWindow(XRCID("ID_CBINTERPOLATIONFILTER")));
	cbHardwareAccelerator = static_cast<wxComboBox*>(FindWindow(XRCID("ID_CBHARDWAREACCELERATE")));
	rbOpenCLOpenGLInterop = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBOPENGLOPENCLINTEROP")));

	rbContrastCorrection = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBAUTOCONTRAST")));

	txtMusicDiaporamaPath = static_cast<wxTextCtrl*>(FindWindow(XRCID("ID_TXTMUSICDIAPORAMAPATH")));
	btMusicDiaporamaPath = static_cast<wxButton*>(FindWindow(XRCID("ID_MUSICDIAPORAMAPATH")));


	rbUSESUPERDNN = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBUSESUPERDNN")));
	cbUSESUPERDNNFILTER = static_cast<wxComboBox*>(FindWindow(XRCID("ID_CBUSESUPERDNNFILTER")));


	rbVideoEncoderHard = static_cast<wxComboBox*>(FindWindow(XRCID("ID_CBVIDEOENCODERHARD")));
	rbVideoDecoderHard = static_cast<wxComboBox*>(FindWindow(XRCID("ID_CBVIDEODECODERHARD")));

	rbSkin = static_cast<wxComboBox*>(FindWindow(XRCID("ID_RBSKINCOLOR")));

	txtAPIKey = static_cast<wxTextCtrl*>(FindWindow(XRCID("ID_TXTAPIKEY")));

	Bind(wxEVT_BUTTON, &ConfigRegards::OnbtnOkClick, this, XRCID("ID_OK"));
	Bind(wxEVT_BUTTON, &ConfigRegards::OnBtnCancelClick, this, XRCID("ID_CANCEL"));
	//*)
	Bind(wxEVT_BUTTON, &ConfigRegards::OnbtnPathVideoClick, this, XRCID("ID_VIDEOPATH"));
	Bind(wxEVT_BUTTON, &ConfigRegards::OnBtnPathPictureClick, this, XRCID("ID_PICTUREPATH"));
	Bind(wxEVT_BUTTON, &ConfigRegards::OnBtnMusicDiaporamaClick, this, XRCID("ID_MUSICDIAPORAMAPATH"));

	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
		txtVideoPath->SetValue(config->GetPathForVideoEdit());
		txtPicturePath->SetValue(config->GetPathForPictureEdit());
	}

#ifndef __APPLE__
	rbVideoEncoderHard->Clear();
	{
		wxString encoderHardware = "";
		CRegardsConfigParam* configParam = CParamInit::getInstance();
		if (configParam != nullptr)
			encoderHardware = configParam->GetHardwareEncoder();

		bool findEncoder = false;
		std::vector<wxString> listHard = Regards::Media::GetHardwareList();
		if (listHard.size() > 0)
		{
			for (wxString hardware : listHard)
			{
				rbVideoEncoderHard->AppendString(hardware);
				if (hardware == encoderHardware)
					findEncoder = true;
			}

			if (encoderHardware == "none" || !findEncoder)
				rbVideoEncoderHard->SetStringSelection(listHard[0]);
			else
				rbVideoEncoderHard->SetStringSelection(encoderHardware);
		}
		else
			rbVideoEncoderHard->SetStringSelection("none");

	}
#else

	rbVideoEncoderHard->AppendString("videotoolbox");
	rbVideoEncoderHard->SetStringSelection("videotoolbox");

#endif

	
	cbHardwareAccelerator->AppendString("CPU");
	if (cv::ocl::haveOpenCL())
		cbHardwareAccelerator->AppendString("OpenCL");

	init();

	SetAutoLayout(TRUE);
}



void ConfigRegards::SelectFile(const wxString& label, const wxString& wildcard, wxTextCtrl* target,
                                const wxString& defaultFilename)
{
	wxFileDialog openFileDialog(nullptr, label, "", defaultFilename,
	                             wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	wxString documentPath = CFileUtility::GetDocumentFolderPath();
	openFileDialog.SetDirectory(documentPath);

	if (openFileDialog.ShowModal() == wxID_OK)
		target->SetValue(openFileDialog.GetPath());
}

void ConfigRegards::OnbtnPathVideoClick(wxCommandEvent& event)
{
	wxString label = CLibResource::LoadStringFromResource(L"LBLSELECTVIDEOEDITOR", 1);
	wxString allfiles = CLibResource::LoadStringFromResource(L"LBLALLFILES", 1);
	SelectFile(label, allfiles, txtVideoPath);
}

void ConfigRegards::OnBtnPathPictureClick(wxCommandEvent& event)
{
	wxString label = CLibResource::LoadStringFromResource(L"LBLSELECTPICTUREEDITOR", 1);
	wxString allfiles = CLibResource::LoadStringFromResource(L"LBLALLFILES", 1);
	SelectFile(label, allfiles, txtPicturePath);
}

void ConfigRegards::OnBtnMusicDiaporamaClick(wxCommandEvent& event)
{
	// NOTE: label volontairement laisse a l'identique (LBLSELECTPICTUREEDITOR) - point non modifie a la demande.
	wxString label = CLibResource::LoadStringFromResource(L"LBLSELECTPICTUREEDITOR", 1);
	wxString filename = CLibResource::LoadStringFromResource(L"LBLFILESNAME", 1);
	wxString wildcard = "mp3 " + filename + " (*.mp3)|*.mp3|aac " + filename + " (*.aac)|*.aac|wav " + filename
	                     + " (*.wav)|*.wav";
	SelectFile(label, wildcard, txtMusicDiaporamaPath, filename);
}

ConfigRegards::~ConfigRegards()
{
	//(*Destroy(ConfigRegards)
	//*)
}

int ConfigRegards::InvertBinary(int value)
{
	return (value == 0) ? 1 : 0;
}

void ConfigRegards::SetInvertedRadioSelection(wxRadioBox* radio, int paramValue)
{
	radio->SetSelection(InvertBinary(paramValue));
}

int ConfigRegards::GetInvertedRadioValue(wxRadioBox* radio)
{
	return InvertBinary(radio->GetSelection());
}

void ConfigRegards::init()
{
	this->SetTitle("Configuration");
	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	if (regardsParam == nullptr)
		return;

	int transition = max((regardsParam->GetEffect() - 300), 0);
	if (transition == 0)
		rbTransitionEffect->SetSelection(0);
	else
		rbTransitionEffect->SetSelection(transition);

	int transitionDiaporama = max((regardsParam->GetDiaporamaTransitionEffect() - 400), 0);
	if (transitionDiaporama == 0)
		rbTransitionDiaporamaEffect->SetSelection(0);
	else
		rbTransitionDiaporamaEffect->SetSelection(transitionDiaporama);

	SetInvertedRadioSelection(rbAutoRotate, regardsParam->GetDetectOrientation());
	SetInvertedRadioSelection(rbContrastCorrection, regardsParam->GetAutoConstrast());
	SetInvertedRadioSelection(rbVideoFaceDetection, regardsParam->GetFaceVideoDetection());
	SetInvertedRadioSelection(rbUSESUPERDNN, regardsParam->GetUseSuperResolution());

	SetInvertedRadioSelection(rbFaceDetection, regardsParam->GetFaceDetection());

	txtMusicDiaporamaPath->SetValue(regardsParam->GetMusicDiaporama());

	int timeDiaporama = regardsParam->GetDiaporamaTime();
	scTime->SetValue(timeDiaporama);

	int thumbnailProcess = regardsParam->GetThumbnailProcess();
	scProcessThumbnail->SetValue(thumbnailProcess);

	int exifProcess = regardsParam->GetExifProcess();
	scProcessExif->SetValue(exifProcess);

	int faceProcess = regardsParam->GetFaceProcess();
	scProcessFace->SetValue(faceProcess);

	SetInvertedRadioSelection(rbDatabaseInMemory, regardsParam->GetDatabaseInMemory());

	int interpolation = regardsParam->GetInterpolationType();
	rbInterpolation->SetSelection(interpolation);

	int superDnn = regardsParam->GetSuperResolutionType();
	cbUSESUPERDNNFILTER->SetSelection(superDnn);


	SetInvertedRadioSelection(rbOpenCLOpenGLInterop, regardsParam->GetIsOpenCLOpenGLInteropSupport());


	int numItem = 0;
	wxString encoder = regardsParam->GetHardwareEncoder();
	if (encoder != "" && encoder != "none")
	{
		numItem = rbVideoEncoderHard->FindString(encoder);
		rbVideoEncoderHard->SetSelection(numItem);
	}
	else
		rbVideoEncoderHard->SetSelection(0);

	wxString decoder = regardsParam->GetHardwareDecoder();
	if (decoder != "" && decoder != "none")
	{
		numItem = rbVideoDecoderHard->FindString(decoder);
		rbVideoDecoderHard->SetSelection(numItem);
	}
	else
		rbVideoDecoderHard->SetSelection(0);


	if (regardsParam->GetIsUseCuda())
	{
		cbHardwareAccelerator->SetStringSelection("Cuda");
		oldAccelerator = "Cuda";
	}
	else if (regardsParam->GetIsOpenCLSupport())
	{
		cbHardwareAccelerator->SetStringSelection("OpenCL");
		oldAccelerator = "OpenCL";
	}
	else
	{
		cbHardwareAccelerator->SetStringSelection("CPU");
		oldAccelerator = "CPU";
	}

	txtAPIKey->SetValue(regardsParam->GetApiKey());

	int skinMode = regardsParam->GetSkinWindowMode();
	rbSkin->SetSelection(skinMode);
}

void ConfigRegards::OnbtnOkClick(wxCommandEvent& event)
{
	isOk = true;
	bool showInfosRestart = false;
	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	if (regardsParam == nullptr)
		return;

	CMainParam* mainparam = CMainParamInit::getInstance();
	int oldFaceDetection = regardsParam->GetFaceDetection();
	wxString oldApiKey = regardsParam->GetApiKey();
	int nbProcesseur = std::thread::hardware_concurrency();

	if (mainparam != nullptr)
	{
		mainparam->SetPathForVideoEdit(txtVideoPath->GetValue());
		mainparam->SetPathForPictureEdit(txtPicturePath->GetValue());
	}

	regardsParam->SetMusicDiaporama(txtMusicDiaporamaPath->GetValue());

	int transition = rbTransitionEffect->GetSelection();
	if (transition == 0)
		regardsParam->SetEffect(0);
	else
		regardsParam->SetEffect(transition + 300);

	int transitionDiaporama = rbTransitionDiaporamaEffect->GetSelection();
	regardsParam->SetDiaporamaTransitionEffect(transitionDiaporama + 400);

	regardsParam->SetDectectOrientation(GetInvertedRadioValue(rbAutoRotate));
	regardsParam->SetFaceVideoDetection(GetInvertedRadioValue(rbVideoFaceDetection));

	int newFaceDetection = GetInvertedRadioValue(rbFaceDetection);
	regardsParam->SetFaceDetection(newFaceDetection);
	if (oldFaceDetection != newFaceDetection)
		showInfosRestart = true;

	regardsParam->SetAutoConstrast(GetInvertedRadioValue(rbContrastCorrection));
	regardsParam->SetUseSuperResolution(GetInvertedRadioValue(rbUSESUPERDNN));

	int interpolation = rbInterpolation->GetSelection();
	regardsParam->SetInterpolationType(interpolation);
	application_context.SetInterpolationMethod(interpolation);

	int superDnn = cbUSESUPERDNNFILTER->GetSelection();
	regardsParam->SetSuperResolutionType(superDnn);

	int timeDiaporama = scTime->GetValue();
	regardsParam->SetDiaporamaTime(timeDiaporama);

	int skinMode = rbSkin->GetSelection();
	regardsParam->SetSkinWindowMode(skinMode);

	int thumbnailProcess = scProcessThumbnail->GetValue();
	int faceProcess = scProcessFace->GetValue();
	int exifProcess = scProcessExif->GetValue();

	regardsParam->SetIsOpenCLOpenGLInteropSupport(GetInvertedRadioValue(rbOpenCLOpenGLInterop));

	wxString oldencoder = regardsParam->GetHardwareEncoder();
	wxString olddecoder = regardsParam->GetHardwareDecoder();

	wxString encoder = rbVideoEncoderHard->GetStringSelection();
	wxString decoder = rbVideoDecoderHard->GetStringSelection();

	if (encoder == "auto")
		regardsParam->SetHardwareEncoder("");
	else
		regardsParam->SetHardwareEncoder(encoder);

	if (decoder == "auto")
		regardsParam->SetHardwareDecoder("");
	else
		regardsParam->SetHardwareDecoder(decoder);

	wxString hardwareAccelerator = cbHardwareAccelerator->GetStringSelection();
	if (hardwareAccelerator == "Cuda")
	{
		regardsParam->SetIsUseCuda(1);
        regardsParam->SetIsCudaSupport(1);
	}
	else if (hardwareAccelerator == "OpenCL")
	{
		regardsParam->SetIsOpenCLSupport(1);
        regardsParam->SetIsUseCuda(0);
        regardsParam->SetIsCudaSupport(0);
	}
	else
	{
		regardsParam->SetIsUseCuda(0);
		regardsParam->SetIsOpenCLSupport(0);
        regardsParam->SetIsCudaSupport(0);
	}

	if(oldAccelerator != hardwareAccelerator)
		showInfosRestart = true;

	if (oldencoder != encoder)
		showInfosRestart = true;
	if (olddecoder != decoder)
		showInfosRestart = true;

	

	//Géolocalisation
	wxString apiKey = txtAPIKey->GetValue();
	if (apiKey != oldApiKey)
	{
		wxString urlServer = regardsParam->GetGeoLocUrlServer();
		bool result = Regards::Internet::CGps::IsLocalisationAvailable(urlServer, apiKey);
		if (!result)
		{
			wxMessageBox(_("Geolocalisation service is not available. Please check your geoplugin.net API key."));
		}
		else
		{
			showInfosRestart = true;
			regardsParam->SetApiKey(apiKey);
		}
	}

	if (thumbnailProcess == 0 || faceProcess == 0 || exifProcess == 0)
	{
		wxString errorProcessNumberMin = CLibResource::LoadStringFromResource(L"ErrorProcessNumberMin", 1);
		wxString errorInfo = CLibResource::LoadStringFromResource(L"informationserror", 1);
		wxMessageBox(errorProcessNumberMin, errorInfo);
	}
	else if ((thumbnailProcess + exifProcess) > nbProcesseur && faceProcess > nbProcesseur && (thumbnailProcess > 1 ||
		faceProcess > 1 || exifProcess > 1))
	{
		wxString errorProcessNumberMax = CLibResource::LoadStringFromResource(L"ErrorProcessNumberMax", 1);
		wxString errorInfo = CLibResource::LoadStringFromResource(L"informationserror", 1);
		wxMessageBox(errorProcessNumberMax, errorInfo);
	}
	else
	{
		regardsParam->SetFaceProcess(faceProcess);
		regardsParam->SetExifProcess(exifProcess);
		regardsParam->SetThumbnailProcess(thumbnailProcess);

		regardsParam->SetDatabaseInMemory(GetInvertedRadioValue(rbDatabaseInMemory));

		if (showInfosRestart)
		{
			wxString labelRestart = CLibResource::LoadStringFromResource(L"labelRestart", 1);
			wxString lblInfos = CLibResource::LoadStringFromResource(L"labelInformations", 1);
			wxMessageBox(labelRestart, lblInfos);
		}


		this->Close();
	}
}

void ConfigRegards::OnBtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}

bool ConfigRegards::IsOk()
{
	return isOk;
}
