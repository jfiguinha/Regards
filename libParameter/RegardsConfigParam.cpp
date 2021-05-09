#include "header.h"
#include "RegardsConfigParam.h"
#include <fstream>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <config_id.h>
#include <ConvertUtility.h>
using namespace rapidxml;

CRegardsConfigParam::CRegardsConfigParam()
{
	iconSizeRatio = 1.0;
	numLibPreview = 2;
	numEffect = 0;
	openCLNumIndex = 0;
	numLibEffect = 0;
	numLibVideo = 0;
	diaporamaTime = 3;
	diaporamaEffect = 0;
	diaporamaFullscreen = 1;
	openCLPlatformName = "";
	dataInMemory = 0;
	geolocUrl = "http://www.geoplugin.net";
	thumbnailQuality = 1;
	thumbnailIconeCache = 1;
	pictureSize = 0;
    nbProcessThumbnail = 1;
    nbProcessExif = 1;
    nbProcessFace = 1;
    nbGpsFileByMinute = 60;
    numLanguage = 1;
	numInterpolation = 0;
	openCLSupport = 0;
	useDxva2 = 0;
	detectOrientation = 0;
	autoContrast = 0;
	videoDecoderHardware = "";
	openCLFaceSupport = 0;
	videoFaceDetection = 0;
	faceDetection = 1;
	fastFaceDetection = 1;
}


void CRegardsConfigParam::SetFastDetectionFace(const int& fastDetection)
{
	this->fastFaceDetection = fastDetection;
}
int CRegardsConfigParam::GetFastDetectionFace()
{
	return fastFaceDetection;
}

int CRegardsConfigParam::GetFaceVideoDetection()
{
	return videoFaceDetection;
}

void CRegardsConfigParam::SetFaceVideoDetection(const int &videoFaceDetection)
{
	this->videoFaceDetection = videoFaceDetection;
}

wxString CRegardsConfigParam::GetVideoDecoderHardware()
{
	return videoDecoderHardware;
}

void CRegardsConfigParam::SetVideoDecoderHardware(const wxString &numLib)
{
	videoDecoderHardware = numLib;
}

int CRegardsConfigParam::GetAutoConstrast()
{
	return autoContrast;
}

void CRegardsConfigParam::SetAutoConstrast(const int &autoContrast)
{
	this->autoContrast = autoContrast;
}

void CRegardsConfigParam::SetDxva2Actif(const int &useDxva2)
{
	this->useDxva2 = useDxva2;
}

bool CRegardsConfigParam::GetDxva2Actif()
{
	return useDxva2;
}

bool CRegardsConfigParam::GetIsOpenCLSupport()
{
	return openCLSupport;
}

void CRegardsConfigParam::SetIsOpenCLSupport(const int& openCLSupport)
{
	this->openCLSupport = openCLSupport;
}

int CRegardsConfigParam::GetInterpolationType()
{
	return numInterpolation;
}

void CRegardsConfigParam::SetInterpolationType(const int& numInterpolation)
{
	this->numInterpolation = numInterpolation;
}

int CRegardsConfigParam::GetOpenCLLoadFromBinaries(){
    return loadFromBinaries;
    
}
void CRegardsConfigParam::SetOpenCLLoadFromBinaries(const int &loadFromBinaries)
{
    this->loadFromBinaries = loadFromBinaries;
}

int CRegardsConfigParam::GetThumbnailProcess()
{
    return nbProcessThumbnail;
}

void CRegardsConfigParam::SetThumbnailProcess(const int &nbProcess)
{
    nbProcessThumbnail = nbProcess;
}

int CRegardsConfigParam::GetNumLanguage()
{
    return numLanguage;
}

void CRegardsConfigParam::SetNumLanguage(const int &numLanguage)
{
    this->numLanguage = numLanguage;
}

int CRegardsConfigParam::GetFaceDetection()
{
	return faceDetection;
}

void CRegardsConfigParam::SetFaceDetection(const int &faceDetection)
{
	this->faceDetection = faceDetection;
}

int CRegardsConfigParam::GetFaceOpenCLProcess()
{
	return openCLFaceSupport;
}

void CRegardsConfigParam::SetFaceOpenCLProcess(const int &openclProcess)
{
	this->openCLFaceSupport = openclProcess;
}

int CRegardsConfigParam::GetFaceProcess()
{
    return nbProcessFace;
}

void CRegardsConfigParam::SetFaceProcess(const int &nbProcess)
{
    nbProcessFace = nbProcess;
}

int CRegardsConfigParam::GetExifProcess()
{
    return nbProcessExif;
}

void CRegardsConfigParam::SetExifProcess(const int &nbProcess)
{
    nbProcessExif = nbProcess;
}

int CRegardsConfigParam::GetFaceDetectionPictureSize()
{
	return pictureSize;
}

void CRegardsConfigParam::SetFaceDetectionPictureSize(const int &numIndex)
{
	pictureSize = numIndex;
}

bool CRegardsConfigParam::GetDatabaseInMemory()
{
	return dataInMemory;
}

void CRegardsConfigParam::SetDatabaseInMemory(const int &value)
{
	dataInMemory = value;
}

float CRegardsConfigParam::GetIconSizeRatio()
{
    return iconSizeRatio;
}

void CRegardsConfigParam::SetIconSizeRatio(const float &ratio)
{
    this->iconSizeRatio = ratio;
}

int CRegardsConfigParam::GetThumbnailQuality()
{
	return thumbnailQuality;
}

void CRegardsConfigParam::SetThumbnailQuality(const int &quality)
{
	thumbnailQuality = quality;
}

int CRegardsConfigParam::GetThumbnailIconeCache()
{
	return thumbnailIconeCache;
}

void CRegardsConfigParam::SetThumbnailIconeCache(const int &iconeCache)
{
	thumbnailIconeCache = iconeCache;
}

wxString CRegardsConfigParam::GetUrlServer()
{
	wxString value = geolocUrl;
	return value;
}

int CRegardsConfigParam::GetNbGpsIterationByMinute()
{
	return nbGpsFileByMinute;
}

CRegardsConfigParam::~CRegardsConfigParam()
{
	doc.clear();
}

int CRegardsConfigParam::GetVideoLibrary()
{
	return numLibVideo;
}

void CRegardsConfigParam::SetVideoLibrary(const int &numLib)
{
	numLibVideo = numLib;
}

int CRegardsConfigParam::GetEffectLibrary()
{
	return numLibEffect;
}

int CRegardsConfigParam::GetSoundVolume()
{
	return soundVolume;
}

void CRegardsConfigParam::SetSoundVolume(const int &soundVolume)
{
	this->soundVolume = soundVolume;
}

void CRegardsConfigParam::SetEffectLibrary(const int &numLib)
{
	numLibEffect = numLib;
}

int CRegardsConfigParam::GetPreviewLibrary()
{
    return LIBOPENCL;
}
void CRegardsConfigParam::SetPreviewLibrary(const int &numLib)
{
	this->numLibPreview = numLib;
}

int CRegardsConfigParam::GetOpenCLPlatformIndex()
{
	return openCLNumIndex;
}

void CRegardsConfigParam::SetOpenCLPlatformIndex(const int &numIndex)
{
	openCLNumIndex = numIndex;
}

wxString CRegardsConfigParam::GetOpenCLPlatformName()
{
	return openCLPlatformName;
}

void CRegardsConfigParam::SetOpenCLPlatformName(const wxString &platformName)
{
	openCLPlatformName = platformName;
}

int CRegardsConfigParam::GetEffect()
{
	if (numEffect < 300)
		return 0;
	return numEffect;
}

void CRegardsConfigParam::SetEffect(const int &numEffect)
{
	this->numEffect = numEffect;
}

void CRegardsConfigParam::SetEffectLibrary(xml_node<>* sectionPosition)
{
	sectionPosition->append_node(node("NumLibrary", to_string(numLibEffect)));
	sectionPosition->append_node(node("NumEffect", to_string(numEffect)));
    sectionPosition->append_node(node("NbProcessThumbnail", to_string(nbProcessThumbnail)));
    sectionPosition->append_node(node("NbProcessExif", to_string(nbProcessExif)));
    sectionPosition->append_node(node("NbProcessFace", to_string(nbProcessFace)));
    sectionPosition->append_node(node("GpsFileByMinute", to_string(nbGpsFileByMinute)));
	sectionPosition->append_node(node("videoFaceDetection", to_string(videoFaceDetection)));
	sectionPosition->append_node(node("FaceDetection", to_string(faceDetection)));
	sectionPosition->append_node(node("FastFaceDetection", to_string(fastFaceDetection)));
}

void CRegardsConfigParam::SetVideoLibrary(xml_node<>* sectionPosition)
{
	sectionPosition->append_node(node("NumLibrary", to_string(numLibVideo)));
	sectionPosition->append_node(node("UseDXVA2", to_string(useDxva2)));
	sectionPosition->append_node(node("HardwareDecoder", videoDecoderHardware));
	sectionPosition->append_node(node("HardwareEncoder", videoEncoderHardware));
	sectionPosition->append_node(node("SoundVolume", to_string(soundVolume)));
}

void CRegardsConfigParam::SetImageLibrary(xml_node<>* sectionPosition)
{
	sectionPosition->append_node(node("NumLibrary", to_string(numLibPreview)));
	sectionPosition->append_node(node("OpenCLNumIndex", to_string(openCLNumIndex)));
	sectionPosition->append_node(node("OpenCLPlatformName", openCLPlatformName));
    sectionPosition->append_node(node("LoadFromBinaries", to_string(loadFromBinaries)));
	sectionPosition->append_node(node("NumInterpolation", to_string(numInterpolation)));
	sectionPosition->append_node(node("OpenCLSupport", to_string(openCLSupport)));
	sectionPosition->append_node(node("detectOrientation", to_string(detectOrientation)));
	sectionPosition->append_node(node("autoContrast", to_string(autoContrast)));
	sectionPosition->append_node(node("openCLFaceSupport", to_string(openCLFaceSupport)));
}


int CRegardsConfigParam::GetDiaporamaTime()
{
	return diaporamaTime;
}

void CRegardsConfigParam::SetDiaporamaTime(const int &diaporamaTime)
{
	this->diaporamaTime = diaporamaTime;
}

void CRegardsConfigParam::GetVideoLibrary(xml_node<> * position_node)
{
	wxString value = "";
	wxString nodeName = "";
	xml_node<> * child_node = position_node->first_node("NumLibrary");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		numLibVideo = atoi(child_node->value());
	}

	child_node = position_node->first_node("HardwareDecoder");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		videoDecoderHardware = child_node->value();
	}

	child_node = position_node->first_node("HardwareEncoder");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		videoEncoderHardware = child_node->value();
	}

	child_node = position_node->first_node("UseDXVA2");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		useDxva2 = atoi(child_node->value());
	}

	child_node = position_node->first_node("SoundVolume");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		soundVolume = atoi(child_node->value());
	}
}

void CRegardsConfigParam::GetEffectLibrary(xml_node<> * position_node)
{
	wxString value = "";
	wxString nodeName = "";
	xml_node<> * child_node = position_node->first_node("NumLibrary");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		numLibEffect = atoi(child_node->value());
	}

	child_node = position_node->first_node("NumEffect");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		numEffect = atoi(child_node->value());
	}
    
	child_node = position_node->first_node("NbProcessThumbnail");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		nbProcessThumbnail = atoi(child_node->value());
	}
    
	child_node = position_node->first_node("NbProcessExif");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		nbProcessExif = atoi(child_node->value());
	}
    
	child_node = position_node->first_node("NbProcessFace");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		nbProcessFace = atoi(child_node->value());
	}
    
	child_node = position_node->first_node("GpsFileByMinute");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		nbGpsFileByMinute = atoi(child_node->value());
	}

	child_node = position_node->first_node("videoFaceDetection");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		videoFaceDetection = atoi(child_node->value());
	}

	child_node = position_node->first_node("FaceDetection");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		faceDetection = atoi(child_node->value());
	}

	child_node = position_node->first_node("FastFaceDetection");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		fastFaceDetection = atoi(child_node->value());
	}
}

void CRegardsConfigParam::GetImageLibrary(xml_node<> * position_node)
{
	wxString value = "";
	wxString nodeName = "";
	xml_node<> * child_node = position_node->first_node("NumLibrary");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		numLibPreview = atoi(child_node->value());
	}

	child_node = position_node->first_node("OpenCLNumIndex");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		openCLNumIndex = atoi(child_node->value());
	}

	child_node = position_node->first_node("OpenCLPlatformName");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		openCLPlatformName = child_node->value();
	}

	child_node = position_node->first_node("LoadFromBinaries");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		loadFromBinaries = atoi(child_node->value());
	}

	child_node = position_node->first_node("NumInterpolation");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		numInterpolation = atoi(child_node->value());
	}	

	child_node = position_node->first_node("OpenCLSupport");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		openCLSupport = atoi(child_node->value());
	}

	child_node = position_node->first_node("detectOrientation");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		detectOrientation = atoi(child_node->value());
	}

	child_node = position_node->first_node("autoContrast");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		autoContrast = atoi(child_node->value());
	}

	child_node = position_node->first_node("openCLFaceSupport");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		openCLFaceSupport = atoi(child_node->value());
	}
}



//////////////////////////////////////////////////////////////////////////////////////////
//Saving Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void CRegardsConfigParam::SaveParameter()
{
	doc.clear();
	// xml declaration
	xml_node<>* decl = doc.allocate_node(node_declaration);
	xml_attribute<>* ver = doc.allocate_attribute(stralloc("version"), stralloc("1.0"));
	xml_attribute<>* encoding = doc.allocate_attribute(stralloc("encoding"), stralloc("ISO-8859-1"));
	doc.append_attribute(ver);
	doc.append_attribute(encoding);
	doc.append_node(decl);

	xml_node<>* root = doc.allocate_node(node_element, stralloc("Parameter"));
	doc.append_node(root);

	xml_node<>* sectionImageLib = node("ImageLibrary");
	SetImageLibrary(sectionImageLib);
	root->append_node(sectionImageLib);
	xml_node<>* sectionVideoLib = node("VideoLibrary");
	SetVideoLibrary(sectionVideoLib);
	root->append_node(sectionVideoLib);
	xml_node<>* sectionEffectLib = node("EffectLibrary");
	SetEffectLibrary(sectionEffectLib);
	root->append_node(sectionEffectLib);
	xml_node<>* sectionDatabase = node("Database");
	SetDatabaseParameter(sectionDatabase);
	root->append_node(sectionDatabase);
	xml_node<>* sectionDiaporama = node("Diaporama");
	SetDiaporamaParameter(sectionDiaporama);
	root->append_node(sectionDiaporama);
	xml_node<>* sectionGeolocalisation = node("Geolocalisation");
	SetGeolocalisationServer(sectionGeolocalisation);
	root->append_node(sectionGeolocalisation);
	xml_node<>* sectionThumbnail = node("Thumbnail");
	SetThumbnail(sectionThumbnail);
	root->append_node(sectionThumbnail);
    xml_node<>* sectionIcon = node("Icon");
    SetIconParameter(sectionIcon);
    root->append_node(sectionIcon);
	// save the xml data to a file (could equally well use any other ostream)
	std::ofstream file(CConvertUtility::ConvertToStdString(filename));
	if (file.is_open())
	{
		file << doc;
		file.close();
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
//Loading Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void CRegardsConfigParam::LoadParameter()
{
	xml_node<> * root_node;
	//long nodeSize = 0;
	root_node = doc.first_node("Parameter");

	xml_node<> * child_node = root_node->first_node("ImageLibrary");
	if (child_node != 0)
		GetImageLibrary(child_node);

	child_node = root_node->first_node("VideoLibrary");
	if (child_node != 0)
		GetVideoLibrary(child_node);

	child_node = root_node->first_node("EffectLibrary");
	if (child_node != 0)
		GetEffectLibrary(child_node);

	child_node = root_node->first_node("Database");
	if (child_node != 0)
		GetDatabaseParameter(child_node);

	child_node = root_node->first_node("Diaporama");
	if (child_node != 0)
		GetDiaporamaParameter(child_node);

	child_node = root_node->first_node("Geolocalisation");
	if (child_node != 0)
		GetGeolocalisationServer(child_node);

	child_node = root_node->first_node("Thumbnail");
	if (child_node != 0)
		GetThumbnail(child_node);
    
    child_node = root_node->first_node("Icon");
    if (child_node != 0)
        GetIconParameter(child_node);
    
}

void CRegardsConfigParam::SetIconParameter(xml_node<>* sectionPosition)
{
    sectionPosition->append_node(node("Ratio", to_string(iconSizeRatio)));
}

void CRegardsConfigParam::GetIconParameter(xml_node<>* position_node)
{
    wxString value = "";
    wxString nodeName = "";
    xml_node<> * child_node = position_node->first_node("Ratio");
    if (child_node != 0)
    {
        value = child_node->value();
        nodeName = child_node->name();
        iconSizeRatio = atof(child_node->value());
    }
}


void CRegardsConfigParam::SetDatabaseParameter(xml_node<>* sectionPosition)
{
	sectionPosition->append_node(node("InMemory", to_string(dataInMemory)));
    sectionPosition->append_node(node("Language", to_string(numLanguage)));
}

void CRegardsConfigParam::GetDatabaseParameter(xml_node<> * position_node)
{
	wxString value = "";
	wxString nodeName = "";
	xml_node<> * child_node = position_node->first_node("InMemory");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		dataInMemory = atoi(child_node->value());
	}

    child_node = position_node->first_node("Language");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		numLanguage = atoi(child_node->value());
	}
}

void CRegardsConfigParam::SetDiaporamaParameter(xml_node<>* sectionPosition)
{
	sectionPosition->append_node(node("Time", to_string(diaporamaTime)));
	sectionPosition->append_node(node("Effect", to_string(diaporamaEffect)));
	sectionPosition->append_node(node("Fullscreen", to_string(diaporamaFullscreen)));
}

void CRegardsConfigParam::GetDiaporamaParameter(xml_node<> * position_node)
{
	wxString value = "";
	wxString nodeName = "";
	xml_node<> * child_node = position_node->first_node("Time");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		diaporamaTime = atoi(child_node->value());
	}

	child_node = position_node->first_node("Effect");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		diaporamaEffect = atoi(child_node->value());
	}

	child_node = position_node->first_node("Fullscreen");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		diaporamaFullscreen = atoi(child_node->value());
	}
}

int CRegardsConfigParam::GetDetectOrientation()
{
	return detectOrientation;// diaporamaEffect;
}

void CRegardsConfigParam::SetDectectOrientation(const int& detectOrientation)
{
	this->detectOrientation = detectOrientation;
}

int CRegardsConfigParam::GetDiaporamaTransitionEffect()
{
	return 100;// diaporamaEffect;
}

void CRegardsConfigParam::SetDiaporamaTransitionEffect(const int &diaporamaEffect)
{
	this->diaporamaEffect = diaporamaEffect;
}

int CRegardsConfigParam::GetDiaporamaFullscreen()
{
	return diaporamaFullscreen;
}

void CRegardsConfigParam::SetDiaporamaFullscreen(const int &diaporamaFullscreen)
{
	this->diaporamaFullscreen = diaporamaFullscreen;
}

void CRegardsConfigParam::SetGeolocalisationServer(xml_node<>* sectionPosition)
{
	sectionPosition->append_node(node("Url", geolocUrl));
}

void CRegardsConfigParam::GetGeolocalisationServer(xml_node<> * position_node)
{
	wxString value = "";
	wxString nodeName = "";
	xml_node<> * child_node = position_node->first_node("Url");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		geolocUrl = child_node->value();
	}
}

void CRegardsConfigParam::SetThumbnail(xml_node<>* sectionPosition)
{
	sectionPosition->append_node(node("Quality", to_string(thumbnailQuality)));
	sectionPosition->append_node(node("IconeCache", to_string(thumbnailIconeCache)));
	sectionPosition->append_node(node("FacePictureSize", to_string(pictureSize)));
}

void CRegardsConfigParam::GetThumbnail(xml_node<> * position_node)
{
	wxString value = "";
	wxString nodeName = "";
	xml_node<> * child_node = position_node->first_node("Quality");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		thumbnailQuality = atoi(child_node->value());
	}

	child_node = position_node->first_node("IconeCache");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		thumbnailIconeCache = atoi(child_node->value());
	}

	child_node = position_node->first_node("FacePictureSize");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		pictureSize = atoi(child_node->value());
	}
}