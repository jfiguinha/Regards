#pragma once
#include "ConfigParam.h"
using namespace rapidxml;


class CRegardsConfigParam : public CConfigParam
{
public:
	CRegardsConfigParam();
	~CRegardsConfigParam() override;

	int GetInterpolationType();
	void SetInterpolationType(const int& numInterpolation);

	int GetPreviewLibrary();
	void SetPreviewLibrary(const int& numLib);

	bool GetDatabaseInMemory();
	void SetDatabaseInMemory(const int& value);

	float GetIconSizeRatio();
	void SetIconSizeRatio(const float& ratio);

	int GetAutoConstrast();
	void SetAutoConstrast(const int& autoContrast);

	int GetFaceDetection();
	void SetFaceDetection(const int& faceDetection);

	int GetVideoLibrary();
	void SetVideoLibrary(const int& numLib);

	wxString GetVideoDecoderHardware();
	void SetVideoDecoderHardware(const wxString& numLib);

	wxString GetMusicDiaporama();
	void SetMusicDiaporama(const wxString& musicDiaporama);

	int GetEffectLibrary();
	void SetEffectLibrary(const int& numLib);

	int GetSoundVolume();
	void SetSoundVolume(const int& soundVolume);

	int GetOpenCLPlatformIndex();
	void SetOpenCLPlatformIndex(const int& numIndex);

	void SetFastDetectionFace(const int& fastDetection);
	int GetFastDetectionFace();

	int GetFaceVideoDetection();
	void SetFaceVideoDetection(const int& videoFaceDetection);

	int GetFaceDetectionPictureSize();
	void SetFaceDetectionPictureSize(const int& numIndex);

	wxString GetOpenCLPlatformName();
	void SetOpenCLPlatformName(const wxString& platformName);

	int GetOpenCLLoadFromBinaries();
	void SetOpenCLLoadFromBinaries(const int& loadFromBinaries);

	int GetEffect();
	void SetEffect(const int& numEffect);

	wxString GetUrlServer();
	int GetNbGpsIterationByMinute();

	int GetDiaporamaTime();
	void SetDiaporamaTime(const int& diaporamaTime);

	int GetDiaporamaTransitionEffect();
	void SetDiaporamaTransitionEffect(const int& diaporamaEffect);

	int GetDiaporamaFullscreen();
	void SetDiaporamaFullscreen(const int& diaporamaFullscreen);

	int GetThumbnailQuality();
	void SetThumbnailQuality(const int& quality);

	int GetThumbnailIconeCache();
	void SetThumbnailIconeCache(const int& iconeCache);

	int GetThumbnailProcess();
	void SetThumbnailProcess(const int& nbProcess);

	int GetFaceProcess();
	void SetFaceProcess(const int& nbProcess);

	int GetFaceOpenCLProcess();
	void SetFaceOpenCLProcess(const int& openclProcess);

	int GetExifProcess();
	void SetExifProcess(const int& nbProcess);

	int GetNumLanguage();
	void SetNumLanguage(const int& numLanguage);

	bool GetIsOpenCLSupport();
	void SetIsOpenCLSupport(const int& openCLSupport);

	void SetDxva2Actif(const int& useDxva2);
	bool GetDxva2Actif();

	int GetDetectOrientation();
	void SetDectectOrientation(const int& detectOrientation);

protected:
	void LoadParameter() override;
	void SaveParameter() override;

	void SetIconParameter(xml_node<>* sectionPosition);
	void GetIconParameter(xml_node<>* position_node);

	void SetImageLibrary(xml_node<>* sectionPosition);
	void GetImageLibrary(xml_node<>* position_node);

	void SetDatabaseParameter(xml_node<>* sectionPosition);
	void GetDatabaseParameter(xml_node<>* position_node);

	void SetEffectLibrary(xml_node<>* sectionPosition);
	void GetEffectLibrary(xml_node<>* position_node);

	void SetVideoLibrary(xml_node<>* sectionPosition);
	void GetVideoLibrary(xml_node<>* position_node);

	void SetDiaporamaParameter(xml_node<>* sectionPosition);
	void GetDiaporamaParameter(xml_node<>* position_node);

	void SetGeolocalisationServer(xml_node<>* sectionPosition);
	void GetGeolocalisationServer(xml_node<>* position_node);

	void SetThumbnail(xml_node<>* sectionPosition);
	void GetThumbnail(xml_node<>* position_node);

	int pictureSize;
	int numLibPreview;
	int numLibEffect;
	int numLibVideo;
	int numEffect;
	int openCLNumIndex;
	int numLanguage;
	int numInterpolation;
	int detectOrientation;

	//Diaporama
	int diaporamaTime;
	int diaporamaEffect;
	int diaporamaFullscreen;

	int thumbnailQuality;
	int thumbnailIconeCache;

	float iconSizeRatio;
	wxString openCLPlatformName;
	wxString geolocUrl;

	int dataInMemory;

	int nbProcessThumbnail;
	int nbProcessExif;
	int nbProcessFace;
	int nbGpsFileByMinute;

	int loadFromBinaries = 0;
	int openCLSupport = 0;
	int useDxva2 = 0;

	int openCLFaceSupport = 0;

	int autoContrast = 0;
	int soundVolume = 100;

	int videoFaceDetection = 0;
	int faceDetection = 1;
	int fastFaceDetection = 1;
	wxString videoDecoderHardware;
	wxString videoEncoderHardware;

	wxString musicDiaporama;
};
