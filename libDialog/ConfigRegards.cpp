#include <header.h>
#include "ConfigRegards.h"
#include <RegardsConfigParam.h>
#include <LibResource.h>
#include <FileUtility.h>
#include <ParamInit.h>
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <ffmpeg_application.h>
#include <opencv2/core/ocl.hpp>
#ifdef USE_CUDA
#include <opencv2/cudaarithm.hpp>
using namespace cv::cuda;
#endif
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


	Connect(XRCID("ID_OK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ConfigRegards::OnbtnOkClick);
	Connect(XRCID("ID_CANCEL"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ConfigRegards::OnBtnCancelClick);
	//*)
	//Connect(wxID_ANY, wxEVT_INIT_DIALOG, (wxObjectEventFunction)&ConfigRegards::OnInit);
	Connect(XRCID("ID_VIDEOPATH"), wxEVT_COMMAND_BUTTON_CLICKED,
	        (wxObjectEventFunction)&ConfigRegards::OnbtnPathVideoClick);
	Connect(XRCID("ID_PICTUREPATH"), wxEVT_COMMAND_BUTTON_CLICKED,
	        (wxObjectEventFunction)&ConfigRegards::OnBtnPathPictureClick);
	Connect(XRCID("ID_MUSICDIAPORAMAPATH"), wxEVT_COMMAND_BUTTON_CLICKED,
	        (wxObjectEventFunction)&ConfigRegards::OnBtnMusicDiaporamaClick);


	

	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		txtVideoPath->SetValue(config->GetPathForVideoEdit());
	if (config != nullptr)
		txtPicturePath->SetValue(config->GetPathForPictureEdit());

#ifndef __APPLE__
	rbVideoEncoderHard->Clear();
	{
		wxString encoderHardware = "";
		CRegardsConfigParam* config = CParamInit::getInstance();
		if (config != nullptr)
			encoderHardware = config->GetHardwareEncoder();

		bool findEncoder = false;
		std::vector<wxString> listHard = CFFmpegApp::GetHardwareList();
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

#ifdef USE_CUDA
	int cuda_devices_number = getCudaEnabledDeviceCount();
	if(cuda_devices_number > 0)
		cbHardwareAccelerator->AppendString("Cuda");
#endif

	init();

	SetAutoLayout(TRUE);
}

void ConfigRegards::OnbtnPathVideoClick(wxCommandEvent& event)
{
	wxString label = CLibResource::LoadStringFromResource(L"LBLSELECTVIDEOEDITOR", 1);
	wxString allfiles = CLibResource::LoadStringFromResource(L"LBLALLFILES", 1);

	wxFileDialog openFileDialog(nullptr, label, "", "",
	                            allfiles, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
                                
    wxString documentPath = CFileUtility::GetDocumentFolderPath();
    openFileDialog.SetDirectory(documentPath);
                                
	if (openFileDialog.ShowModal() == wxID_OK)
		txtVideoPath->SetValue(openFileDialog.GetPath());
}

void ConfigRegards::OnBtnPathPictureClick(wxCommandEvent& event)
{
	wxString label = CLibResource::LoadStringFromResource(L"LBLSELECTPICTUREEDITOR", 1);
	wxString allfiles = CLibResource::LoadStringFromResource(L"LBLALLFILES", 1);

	wxFileDialog openFileDialog(nullptr, label, "", "",
	                            allfiles, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
                                
                                
    wxString documentPath = CFileUtility::GetDocumentFolderPath();
    openFileDialog.SetDirectory(documentPath);
                                
	if (openFileDialog.ShowModal() == wxID_OK)
		txtPicturePath->SetValue(openFileDialog.GetPath());
}

void ConfigRegards::OnBtnMusicDiaporamaClick(wxCommandEvent& event)
{
	wxString label = CLibResource::LoadStringFromResource(L"LBLSELECTPICTUREEDITOR", 1);
	wxString allfiles = CLibResource::LoadStringFromResource(L"LBLALLFILES", 1);
	wxString filename = CLibResource::LoadStringFromResource(L"LBLFILESNAME", 1);

	wxFileDialog openFileDialog(nullptr, label, "", filename,
	                            "mp3 " + filename + " (*.mp3)|*.mp3|aac " + filename + " (*.aac)|*.aac|wav " + filename
	                            + " (*.wav)|*.wav", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    wxString documentPath = CFileUtility::GetDocumentFolderPath();
    openFileDialog.SetDirectory(documentPath);

	if (openFileDialog.ShowModal() == wxID_OK)
		txtMusicDiaporamaPath->SetValue(openFileDialog.GetPath());
}

ConfigRegards::~ConfigRegards()
{
	//(*Destroy(ConfigRegards)
	//*)
}

void ConfigRegards::init()
{
	this->SetTitle("Configuration");
	CRegardsConfigParam* regardsParam = CParamInit::getInstance();

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

	int autoRotate = regardsParam->GetDetectOrientation();
	if (autoRotate == 0)
		rbAutoRotate->SetSelection(1);
	else
		rbAutoRotate->SetSelection(0);

	int autoContrast = regardsParam->GetAutoConstrast();
	if (autoContrast == 0)
		rbContrastCorrection->SetSelection(1);
	else
		rbContrastCorrection->SetSelection(0);

	int videoFaceDetection = regardsParam->GetFaceVideoDetection();
	if (videoFaceDetection == 0)
		rbVideoFaceDetection->SetSelection(1);
	else
		rbVideoFaceDetection->SetSelection(0);

	int useSuperResolution = regardsParam->GetUseSuperResolution();
	if (useSuperResolution == 0)
		rbUSESUPERDNN->SetSelection(1);
	else
		rbUSESUPERDNN->SetSelection(0);

	int faceDetection = regardsParam->GetFaceDetection();
	if (faceDetection == 0)
		rbFaceDetection->SetSelection(1);
	else
		rbFaceDetection->SetSelection(0);

	txtMusicDiaporamaPath->SetValue(regardsParam->GetMusicDiaporama());

	int timeDiaporama = regardsParam->GetDiaporamaTime();
	scTime->SetValue(timeDiaporama);

	int thumbnailProcess = regardsParam->GetThumbnailProcess();
	scProcessThumbnail->SetValue(thumbnailProcess);

	int exifProcess = regardsParam->GetExifProcess();
	scProcessExif->SetValue(exifProcess);

	int faceProcess = regardsParam->GetFaceProcess();
	scProcessFace->SetValue(faceProcess);

	int dataInMemory = regardsParam->GetDatabaseInMemory();
	if (dataInMemory == 0)
		rbDatabaseInMemory->SetSelection(1);
	else
		rbDatabaseInMemory->SetSelection(0);

	int interpolation = regardsParam->GetInterpolationType();
	rbInterpolation->SetSelection(interpolation);

	int superDnn = regardsParam->GetSuperResolutionType();
	cbUSESUPERDNNFILTER->SetSelection(superDnn);


	int openclOpenGLInterop = regardsParam->GetIsOpenCLOpenGLInteropSupport();
	if (openclOpenGLInterop == 0)
		rbOpenCLOpenGLInterop->SetSelection(1);
	else
		rbOpenCLOpenGLInterop->SetSelection(0);


	int numItem = 0;
	wxString encoder = regardsParam->GetHardwareEncoder();
	if (encoder != "")
	{
		numItem = rbVideoEncoderHard->FindString(encoder);
		rbVideoEncoderHard->SetSelection(numItem);
	}
	else
		rbVideoEncoderHard->SetSelection(0);

	wxString decoder = regardsParam->GetHardwareDecoder();
	if (decoder != "")
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

	int skinMode = regardsParam->GetSkinWindowMode();
	rbSkin->SetSelection(skinMode);
}

void ConfigRegards::OnbtnOkClick(wxCommandEvent& event)
{
	isOk = true;
	bool showInfosRestart = false;
	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	CMainParam* mainparam = CMainParamInit::getInstance();
	int _faceDetection = regardsParam->GetFaceDetection();
	int nbProcesseur = thread::hardware_concurrency();

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

	int autoRotate = rbAutoRotate->GetSelection();
	if (autoRotate == 0)
		regardsParam->SetDectectOrientation(1);
	else
		regardsParam->SetDectectOrientation(0);

	int videoFaceDetection = rbVideoFaceDetection->GetSelection();
	if (videoFaceDetection == 0)
		regardsParam->SetFaceVideoDetection(1);
	else
		regardsParam->SetFaceVideoDetection(0);

	int faceDetection = rbFaceDetection->GetSelection();
	if (faceDetection == 0)
		regardsParam->SetFaceDetection(1);
	else
		regardsParam->SetFaceDetection(0);

	if (_faceDetection == faceDetection)
		showInfosRestart = true;

	int autoContrast = rbContrastCorrection->GetSelection();
	if (autoContrast == 0)
		regardsParam->SetAutoConstrast(1);
	else
		regardsParam->SetAutoConstrast(0);

	int useDnn = rbUSESUPERDNN->GetSelection();
	if (useDnn == 0)
		regardsParam->SetUseSuperResolution(1);
	else
		regardsParam->SetUseSuperResolution(0);

	int interpolation = rbInterpolation->GetSelection();
	regardsParam->SetInterpolationType(interpolation);

	int superDnn = cbUSESUPERDNNFILTER->GetSelection();
	regardsParam->SetSuperResolutionType(superDnn);

	int timeDiaporama = scTime->GetValue();
	regardsParam->SetDiaporamaTime(timeDiaporama);

	int skinMode = rbSkin->GetSelection();
	regardsParam->SetSkinWindowMode(skinMode);

	int thumbnailProcess = scProcessThumbnail->GetValue();
	int faceProcess = scProcessFace->GetValue();
	int exifProcess = scProcessExif->GetValue();

	int openclOpenGLInterop = rbOpenCLOpenGLInterop->GetSelection();
	if (openclOpenGLInterop == 0)
		regardsParam->SetIsOpenCLOpenGLInteropSupport(1);
	else
		regardsParam->SetIsOpenCLOpenGLInteropSupport(0);

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

		int dataInMemory = rbDatabaseInMemory->GetSelection();
		if (dataInMemory == 0)
			regardsParam->SetDatabaseInMemory(1);
		else
			regardsParam->SetDatabaseInMemory(0);

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
