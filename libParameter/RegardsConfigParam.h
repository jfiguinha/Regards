#pragma once


#include "ConfigParam.h"
#include <iostream>
#include <fstream>
#include <vector>


using namespace rapidxml;
using namespace std;


class CRegardsConfigParam : public CConfigParam
{
public:
	CRegardsConfigParam();
	~CRegardsConfigParam();

	int GetPreviewLibrary();
	void SetPreviewLibrary(const int &numLib);
    
    float GetIconSizeRatio();
    void SetIconSizeRatio(const float &ratio);
    
	int GetVideoLibrary();
	void SetVideoLibrary(const int &numLib);

	int GetEffectLibrary();
	void SetEffectLibrary(const int &numLib);

	int GetOpenCLPlatformIndex();
	void SetOpenCLPlatformIndex(const int &numIndex);

	wxString GetOpenCLPlatformName();
	void SetOpenCLPlatformName(const wxString &platformName);

	int GetEffect();
	void SetEffect(const int &numEffect);

	wxString GetUrlServer();

	int GetDiaporamaTime();
	void SetDiaporamaTime(const int &diaporamaTime);
	int GetDiaporamaTransitionEffect();
	void SetDiaporamaTransitionEffect(const int &diaporamaEffect);
	int GetDiaporamaFullscreen();
	void SetDiaporamaFullscreen(const int &diaporamaFullscreen);

	int GetThumbnailQuality();
	void SetThumbnailQuality(const int &quality);

	int GetThumbnailIconeCache();
	void SetThumbnailIconeCache(const int &iconeCache);

protected:

	void InitVideoToolbar();
	void InitBitmapToolbar();
	void InitClickToolbar();
	void InitToolbar();

	void LoadParameter();
	void SaveParameter();
    
    void SetIconParameter(xml_node<>* sectionPosition);
    void GetIconParameter(xml_node<>* position_node);

	void SetImageLibrary(xml_node<>* sectionPosition);
	void GetImageLibrary(xml_node<> * position_node);

	void SetEffectLibrary(xml_node<>* sectionPosition);
	void GetEffectLibrary(xml_node<> * position_node);

	void SetVideoLibrary(xml_node<>* sectionPosition);
	void GetVideoLibrary(xml_node<> * position_node);

	void SetDiaporamaParameter(xml_node<>* sectionPosition);
	void GetDiaporamaParameter(xml_node<> * position_node);

	void SetGeolocalisationServer(xml_node<>* sectionPosition);
	void GetGeolocalisationServer(xml_node<> * position_node);

	void SetThumbnail(xml_node<>* sectionPosition);
	void GetThumbnail(xml_node<> * position_node);
	
	int numLibPreview;
	int numLibEffect;
	int numLibVideo;
	int numEffect;
	int openCLNumIndex;

	//Diaporama
	int diaporamaTime;
	int diaporamaEffect;
	int diaporamaFullscreen;

	int thumbnailQuality;
	int thumbnailIconeCache;

    float iconSizeRatio = 1.0;
	wxString openCLPlatformName;
	wxString geolocUrl;
};

