#include "RegardsConfigParam.h"
#include <fstream>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
using namespace rapidxml;

CRegardsConfigParam::CRegardsConfigParam()
{
	numLibPreview = 2;
	numEffect = 0;
	openCLNumIndex = 0;
	numLibEffect = 0;
	numLibVideo = 0;
	diaporamaTime = 3;
	diaporamaEffect = 0;
	diaporamaFullscreen = 1;
	openCLPlatformName = "INTEL";
	geolocUrl = "http://www.geoplugin.net";
	thumbnailQuality = 1;
	thumbnailIconeCache = 1;
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

void CRegardsConfigParam::SetEffectLibrary(const int &numLib)
{
	numLibEffect = numLib;
}

int CRegardsConfigParam::GetPreviewLibrary()
{
    return numLibPreview;
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
}

void CRegardsConfigParam::SetVideoLibrary(xml_node<>* sectionPosition)
{
	sectionPosition->append_node(node("NumLibrary", to_string(numLibVideo)));
}

void CRegardsConfigParam::SetImageLibrary(xml_node<>* sectionPosition)
{
	sectionPosition->append_node(node("NumLibrary", to_string(numLibPreview)));
	sectionPosition->append_node(node("OpenCLNumIndex", to_string(openCLNumIndex)));
	sectionPosition->append_node(node("OpenCLPlatformName", openCLPlatformName));
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
	std::ofstream file(filename.c_str());
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

int CRegardsConfigParam::GetDiaporamaTransitionEffect()
{
	return diaporamaEffect;
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
}