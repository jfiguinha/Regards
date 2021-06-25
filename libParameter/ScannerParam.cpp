#include <header.h>
#include "ScannerParam.h"
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <ConvertUtility.h>
#include <fstream>
using namespace rapidxml;
using namespace Regards::Scanner;


constexpr auto POSITION_PREVIEW_FACE = 300;

CMainParam::CMainParam()
{
	sqlRequest = "";
	showFolder = true;
	isThumbnailBottom = true;
	positionPreviewThumbnail = 0;
	positionFolderCriteria = 0;
	check = false;
	//Définition des valeurs par défaut
	positionRegardsViewer.x = 0;
	positionRegardsViewer.width = 800;
	positionRegardsViewer.y = 0;
	positionRegardsViewer.height = 800;
	showThumbnail = true;
	showVideoThumbnail = true;
	position = 0;
	positionCriteriaPreview = 400;
	positionPreviewFace = POSITION_PREVIEW_FACE;
	showInfos = true;
	showFilter = true;
	delai = 3;
	fullscreen = true;
	enAvant = true;
	folder = "";
	showFolder = true;
	pertinence = 0.0;
}


wxString CMainParam::GetLastSqlRequest()
{
	return sqlRequest;
}

bool CMainParam::GetCheckThumbnailValidity()
{
	return false;
}

void CMainParam::SetLastSqlRequest(const wxString& sqlRequest)
{
	this->sqlRequest = sqlRequest;
}

void CMainParam::SetCheckIsUpdate(const bool& infos)
{
	check = infos;
}

void CMainParam::GetCheckIsUpdate(bool& infos)
{
	infos = check;
}


void CMainParam::SetShowFolder(const bool& infos)
{
	showFolder = infos;
}

void CMainParam::GetShowFolder(bool& infos)
{
	infos = showFolder;
}

void CMainParam::SetShowFace(const bool& infos)
{
	showFace = infos;
}

void CMainParam::GetShowFace(bool& infos)
{
	infos = showFace;
}

void CMainParam::SetShowFilter(const bool& infos)
{
	showFilter = infos;
}

void CMainParam::GetShowFilter(bool& infos)
{
	infos = showFilter;
}

int CMainParam::GetPositionCriteriaPreview()
{
	return positionCriteriaPreview;
}

void CMainParam::SetPositionCriteriaPreview(const int& pos)
{
	positionCriteriaPreview = pos;
}

bool CMainParam::IsThumbnailBottom()
{
	return isThumbnailBottom;
}

void CMainParam::SetThumbnailBottom(const bool& isBottom)
{
	isThumbnailBottom = isBottom;
}

void CMainParam::SetPositionPreviewThumbnail(const int& pos)
{
	positionPreviewThumbnail = pos;
}

int CMainParam::GetPositionPreviewThumbnail()
{
	return positionPreviewThumbnail;
}

void CMainParam::SetCatalogOpenTriangle(const wxString& state)
{
	criteriaTriangleList = state;
}

wxString CMainParam::GetCatalogOpenTriangle()
{
	wxString value;
	value.append(criteriaTriangleList.begin(), criteriaTriangleList.end());
	return value;
}

void CMainParam::SetCatalogCriteria(const wxString& state)
{
	criteriaList = state;
}

wxString CMainParam::GetCatalogCriteria()
{
	wxString value;
	value.append(criteriaList.begin(), criteriaList.end());
	return value;
}

CMainParam::~CMainParam()
{
	doc.clear();
}

void CMainParam::SetCriteriaParameter(xml_node<>* section)
{
	section->append_node(node("Value", criteriaList));
	section->append_node(node("TriangleValue", criteriaTriangleList));
}


wxString CMainParam::GetLastFolder()
{
	return folder;
}

void CMainParam::SetLastFolder(const wxString& folder)
{
	this->folder = folder;
}

//////////////////////////////////////////////////////////////////////////////////////////
//Saving Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void CMainParam::SaveParameter()
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

	// node containing other nodes
	xml_node<>* sectionDiaporama = node("Diaporama");
	SetDiaporamaParameter(sectionDiaporama);
	root->append_node(sectionDiaporama);
	xml_node<>* sectionWindow = node("Window");
	SetWindowParameter(sectionWindow);
	root->append_node(sectionWindow);
	xml_node<>* sectionPosition = node("Position");
	SetPositionParameter(sectionPosition);
	root->append_node(sectionPosition);
	xml_node<>* sectionCriteria = node("Criteria");
	SetCriteriaParameter(sectionCriteria);
	root->append_node(sectionCriteria);

	// save the xml data to a file (could equally well use any other ostream)
	std::ofstream file(CConvertUtility::ConvertToStdString(filename));
	if (file.is_open())
	{
		file << doc;
		file.close();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//Saving SetParameter
//////////////////////////////////////////////////////////////////////////////////////////
void CMainParam::SetDiaporamaParameter(xml_node<>* sectionDiaporama)
{
	sectionDiaporama->append_node(node("Fullscreen", to_string(fullscreen)));
	sectionDiaporama->append_node(node("Effect", to_string(numEffect)));
	sectionDiaporama->append_node(node("Delai", to_string(delai)));
	sectionDiaporama->append_node(node("Forward", to_string(enAvant)));
}

//////////////////////////////////////////////////////////////////////////////////////////
//Saving SetParameter
//////////////////////////////////////////////////////////////////////////////////////////
void CMainParam::SetWindowParameter(xml_node<>* sectionWindow)
{
	sectionWindow->append_node(node("Thumbnail", to_string(showThumbnail)));
	sectionWindow->append_node(node("VideoThumbnail", to_string(showVideoThumbnail)));
	sectionWindow->append_node(node("Infos", to_string(showInfos)));
	sectionWindow->append_node(node("Position", to_string(position)));
	sectionWindow->append_node(node("LastFolder", folder));
	sectionWindow->append_node(node("PositionCriteriaPreview", to_string(positionCriteriaPreview)));
	sectionWindow->append_node(node("PositionPreviewFace", to_string(positionPreviewFace)));
	sectionWindow->append_node(node("Filter", to_string(showFilter)));
	sectionWindow->append_node(node("Folder", to_string(showFolder)));
	sectionWindow->append_node(node("Face", to_string(showFace)));
	sectionWindow->append_node(node("LastRequest", sqlRequest));
	sectionWindow->append_node(node("PositionPreviewThumbnail", to_string(positionPreviewThumbnail)));
	sectionWindow->append_node(node("ThumbnailBottom", to_string(isThumbnailBottom)));
	sectionWindow->append_node(node("PositionCriteriaFolder", to_string(positionFolderCriteria)));
	sectionWindow->append_node(node("FacePertinence", to_string(pertinence)));
}

//////////////////////////////////////////////////////////////////////////////////////////
//Saving SetParameter
//////////////////////////////////////////////////////////////////////////////////////////
void CMainParam::SetWindowPositionParameter(xml_node<>* sectionWindowPosition, const wxRect& rc)
{
	sectionWindowPosition->append_node(node("Left", to_string(rc.x)));
	sectionWindowPosition->append_node(node("Right", to_string(rc.width)));
	sectionWindowPosition->append_node(node("Bottom", to_string(rc.height)));
	sectionWindowPosition->append_node(node("Top", to_string(rc.y)));
}

//////////////////////////////////////////////////////////////////////////////////////////
//Saving SetParameter
//////////////////////////////////////////////////////////////////////////////////////////
void CMainParam::SetPositionParameter(xml_node<>* sectionPosition)
{
	xml_node<>* sectionWindowPosition = node("Window");
	SetWindowPositionParameter(sectionWindowPosition, positionRegardsViewer);
	sectionPosition->append_node(sectionWindowPosition);
}

//////////////////////////////////////////////////////////////////////////////////////////
//Loading Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void CMainParam::LoadParameter()
{
	xml_node<>* root_node;
	//long nodeSize = 0;
	root_node = doc.first_node("Parameter");

	xml_node<>* child_node = root_node->first_node("Diaporama");
	if (child_node != nullptr)
		GetDiaporamaParameter(child_node);

	child_node = root_node->first_node("Window");
	if (child_node != nullptr)
		GetWindowParameter(child_node);

	child_node = root_node->first_node("Position");
	if (child_node != nullptr)
		GetPositionParameter(child_node);

	child_node = root_node->first_node("Criteria");
	if (child_node != nullptr)
		GetCriteriaParameter(child_node);
}


void CMainParam::GetCriteriaParameter(xml_node<>* position_node)
{
	xml_node<>* child_node = position_node->first_node("Value");
	if (child_node != nullptr)
		criteriaList = child_node->value();
	child_node = position_node->first_node("TriangleValue");
	if (child_node != nullptr)
		criteriaTriangleList = child_node->value();
}


void CMainParam::GetPositionParameter(xml_node<>* position_node)
{
	xml_node<>* child_node = position_node->first_node("Window");
	positionRegardsViewer = GetWindowPositionParameter(child_node);
}

wxRect CMainParam::GetWindowPositionParameter(xml_node<>* position_node)
{
	//wxString value;
	//wxString node_name;
	wxRect rc;
	xml_node<>* child_node = position_node->first_node("Left");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//node_name = child_node->name();
		rc.x = atoi(child_node->value());
	}

	child_node = position_node->first_node("Right");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//node_name = child_node->name();
		rc.width = atoi(child_node->value());
	}

	child_node = position_node->first_node("Bottom");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//node_name = child_node->name();
		rc.height = atoi(child_node->value());
	}

	child_node = position_node->first_node("Top");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//node_name = child_node->name();
		rc.y = atoi(child_node->value());
	}

	return rc;
}

void CMainParam::GetWindowParameter(xml_node<>* window_node)
{
	//wxString value;
	//wxString nodeName;
	xml_node<>* child_node = window_node->first_node("Thumbnail");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		showThumbnail = atoi(child_node->value());
	}

	child_node = window_node->first_node("VideoThumbnail");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		showVideoThumbnail = atoi(child_node->value());
	}

	child_node = window_node->first_node("Infos");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		showInfos = atoi(child_node->value());
	}

	child_node = window_node->first_node("Position");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		position = atoi(child_node->value());
	}

	child_node = window_node->first_node("LastFolder");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		folder = child_node->value();
	}

	child_node = window_node->first_node("PositionCriteriaPreview");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		positionCriteriaPreview = atoi(child_node->value());
	}

	child_node = window_node->first_node("PositionPreviewFace");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		positionPreviewFace = atoi(child_node->value());
	}

	child_node = window_node->first_node("PositionPreviewThumbnail");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		positionPreviewThumbnail = atoi(child_node->value());
	}

	child_node = window_node->first_node("PositionCriteriaFolder");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		positionFolderCriteria = atoi(child_node->value());
	}

	child_node = window_node->first_node("ThumbnailBottom");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		isThumbnailBottom = atoi(child_node->value());
	}


	child_node = window_node->first_node("Filter");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		showFilter = atoi(child_node->value());
	}

	child_node = window_node->first_node("Folder");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		showFolder = atoi(child_node->value());
	}

	child_node = window_node->first_node("Face");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		showFace = atoi(child_node->value());
	}

	child_node = window_node->first_node("LastRequest");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		sqlRequest = child_node->value();
	}

	child_node = window_node->first_node("FacePertinence");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		pertinence = atoi(child_node->value());
	}
}

void CMainParam::GetDiaporamaParameter(xml_node<>* diaporama_node)
{
	//wxString value;
	//wxString nodeName;
	xml_node<>* child_node = diaporama_node->first_node("Fullscreen");
	if (child_node != nullptr)
	{
		//value = child_node->value();
		//nodeName = child_node->name();
		fullscreen = atoi(child_node->value());
	}

	child_node = diaporama_node->first_node("Effect");
	if (child_node != nullptr)
	{
	//	value = child_node->value();
	//	nodeName = child_node->name();
		numEffect = atoi(child_node->value());
	}

	child_node = diaporama_node->first_node("Delai");
	if (child_node != nullptr)
	{
	//	value = child_node->value();
	//	nodeName = child_node->name();
		delai = atoi(child_node->value());
	}

	child_node = diaporama_node->first_node("Forward");
	if (child_node != nullptr)
	{
	//	value = child_node->value();
	//	nodeName = child_node->name();
		enAvant = atoi(child_node->value());
	}
}

void CMainParam::SetPertinenceValue(const double& value)
{
	pertinence = value;
}

double CMainParam::GetPertinenceValue()
{
	return pertinence;
}

//////////////////////////////////////////////////////////////////////////////////////////
//Saving Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void CMainParam::SetOptionDiaporama(const bool& fullscreen, const int& effect, const int& delai, const bool& enAvant)
{
	this->fullscreen = fullscreen;
	this->numEffect = effect;
	this->delai = delai;
	this->enAvant = enAvant;
}

void CMainParam::SetPositionWindow(const wxRect& rc)
{
	positionRegardsViewer = rc;
}

void CMainParam::SetShowThumbnail(const bool& show)
{
	showThumbnail = show;
}

void CMainParam::SetShowVideoThumbnail(const bool& show)
{
	showVideoThumbnail = show;
}

void CMainParam::SetShowInfos(const bool& infos)
{
	showInfos = infos;
}

void CMainParam::GetOptionDiaporama(bool& fullscreen, int& effect, int& delai, bool& enAvant)
{
	fullscreen = this->fullscreen;
	effect = this->numEffect;
	delai = this->delai;
	enAvant = this->enAvant;
}

int CMainParam::GetDefaultPositionPreviewFace()
{
	return POSITION_PREVIEW_FACE;
}

int CMainParam::GetPositionPreviewFace()
{
	return positionPreviewFace;
}

void CMainParam::SetPositionPreviewFace(const int& pos)
{
	positionPreviewFace = pos;
}

void CMainParam::SetPositionCriteriaFolder(const int& pos)
{
	positionFolderCriteria = pos;
}

int CMainParam::GetPositionCriteriaFolder()
{
	return positionFolderCriteria;
}

bool CMainParam::GetFullscreenDiaporamaOption()
{
	return fullscreen;
}

bool CMainParam::GetEnAvantDiaporamaOption()
{
	return enAvant;
}

int CMainParam::GetDelaiDiaporamaOption()
{
	return delai;
}

void CMainParam::GetPositionWindow(wxRect& rc)
{
	rc = positionRegardsViewer;
}

void CMainParam::GetShowThumbnail(bool& show)
{
	show = showThumbnail;
}

void CMainParam::GetShowVideoThumbnail(bool& show)
{
	show = showVideoThumbnail;
}

void CMainParam::GetShowInfos(bool& infos)
{
	infos = showInfos;
}

void CMainParam::GetPositionSplitter(int& position)
{
	position = this->position;
}

void CMainParam::SetPositionSplitter(const int& position)
{
	this->position = position;
}
