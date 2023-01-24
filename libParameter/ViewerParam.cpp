#include <header.h>
#include "ViewerParam.h"
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <ConvertUtility.h>
#include <window_id.h>
#include <fstream>
using namespace rapidxml;
using namespace Regards::Viewer;


constexpr auto POSITION_PREVIEW_FACE = 300;

CMainParam::CMainParam(): showFace(false), numEffect(0)
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
	pertinence = 70;
	showInfos = true;
	showFilter = true;
	delai = 3;
	fullscreen = true;
	enAvant = true;
	folder = "";
	windowMode = 1;
	leftPanelPos = {0, 0, 0, 0};
	rightPanelPos = {0, 0, 0, 0};

	filepath = "";
	windowVisible = WM_INFOS;

	pathProgramVideo = "";
	pathProgramPicture = "";

	posFace = 3;
	posFolder = 3;
}


void CMainParam::SetSlideFacePos(const int& pos)
{
	posFace = pos;
}

void CMainParam::GetSlideFacePos(int& pos)
{
	pos = posFace;
}

void CMainParam::SetSlideFolderPos(const int& pos)
{
	posFolder = pos;
}

void CMainParam::GetSlideFolderPos(int& pos)
{
	pos = posFolder;
}

wxString CMainParam::GetPathForVideoEdit()
{
	return pathProgramVideo;
}

void CMainParam::SetPathForVideoEdit(const wxString& pathProgram)
{
	pathProgramVideo = pathProgram;
}

wxString CMainParam::GetPathForPictureEdit()
{
	return pathProgramPicture;
}

void CMainParam::SetPathForPictureEdit(const wxString& pathProgram)
{
	pathProgramPicture = pathProgram;
}

void CMainParam::SetPositionLeftPanel(const wxRect& position)
{
	leftPanelPos = position;
}

void CMainParam::SetPositionRightPanel(const wxRect& position)
{
	rightPanelPos = position;
}

wxRect CMainParam::GetPositionLeftPanel()
{
	return leftPanelPos;
}

wxRect CMainParam::GetPositionRightPanel()
{
	return rightPanelPos;
}

void CMainParam::SetViewerMode(const int& mode)
{
	windowMode = mode;
}

int CMainParam::GetViewerMode()
{
	return windowMode;
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
	SetPositionParameter(sectionPosition, positionRegardsViewer, "window");
	root->append_node(sectionPosition);
	xml_node<>* sectionCriteria = node("Criteria");
	SetCriteriaParameter(sectionCriteria);
	root->append_node(sectionCriteria);

	// save the xml data to a file (could equally well use any other ostream)
	ofstream file(CConvertUtility::ConvertToStdString(filename));
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
	sectionWindow->append_node(node("windowMode", to_string(windowMode)));
	sectionWindow->append_node(node("windowVisible", to_string(windowVisible)));
	sectionWindow->append_node(node("LastFilepath", CConvertUtility::ConvertToUTF8(filepath)));
	sectionWindow->append_node(node("VideoEditorPath", pathProgramVideo));
	sectionWindow->append_node(node("ImageEditorPath", pathProgramPicture));
	sectionWindow->append_node(node("SlideFacePos", to_string(posFace)));
	sectionWindow->append_node(node("SlideFolderPos", to_string(posFolder)));

	xml_node<>* sectionPosition = node("leftPosPanel");
	SetPositionParameter(sectionPosition, leftPanelPos, "window");
	sectionWindow->append_node(sectionPosition);

	sectionPosition = node("rightPosPanel");
	SetPositionParameter(sectionPosition, rightPanelPos, "window");
	sectionWindow->append_node(sectionPosition);
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
void CMainParam::SetPositionParameter(xml_node<>* sectionPosition, const wxRect& pos, const wxString& nodeName)
{
	xml_node<>* sectionWindowPosition = node(nodeName);
	SetWindowPositionParameter(sectionWindowPosition, pos);
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
		GetPositionParameter(child_node, positionRegardsViewer);

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


void CMainParam::GetPositionParameter(xml_node<>* position_node, wxRect& position)
{
	xml_node<>* child_node = position_node->first_node("window");
	if (child_node != nullptr)
		position = GetWindowPositionParameter(child_node);
}

wxRect CMainParam::GetWindowPositionParameter(xml_node<>* position_node)
{
	wxRect rc;
	xml_node<>* child_node = position_node->first_node("Left");
	if (child_node != nullptr)
	{
		rc.x = atoi(child_node->value());
	}

	child_node = position_node->first_node("Right");
	if (child_node != nullptr)
	{
		rc.width = atoi(child_node->value());
	}

	child_node = position_node->first_node("Bottom");
	if (child_node != nullptr)
	{
		rc.height = atoi(child_node->value());
	}

	child_node = position_node->first_node("Top");
	if (child_node != nullptr)
	{
		rc.y = atoi(child_node->value());
	}

	return rc;
}

void CMainParam::GetWindowParameter(xml_node<>* window_node)
{
	xml_node<>* child_node = window_node->first_node("Thumbnail");
	if (child_node != nullptr)
	{
		showThumbnail = atoi(child_node->value());
	}

	child_node = window_node->first_node("VideoThumbnail");
	if (child_node != nullptr)
	{
		showVideoThumbnail = atoi(child_node->value());
	}

	child_node = window_node->first_node("Infos");
	if (child_node != nullptr)
	{
		showInfos = atoi(child_node->value());
	}

	child_node = window_node->first_node("Position");
	if (child_node != nullptr)
	{
		position = atoi(child_node->value());
	}

	child_node = window_node->first_node("LastFolder");
	if (child_node != nullptr)
	{
		folder = child_node->value();
	}

	child_node = window_node->first_node("PositionCriteriaPreview");
	if (child_node != nullptr)
	{
		positionCriteriaPreview = atoi(child_node->value());
	}

	child_node = window_node->first_node("PositionPreviewFace");
	if (child_node != nullptr)
	{
		positionPreviewFace = atoi(child_node->value());
	}

	child_node = window_node->first_node("PositionPreviewThumbnail");
	if (child_node != nullptr)
	{
		positionPreviewThumbnail = atoi(child_node->value());
	}

	child_node = window_node->first_node("PositionCriteriaFolder");
	if (child_node != nullptr)
	{
		positionFolderCriteria = atoi(child_node->value());
	}

	child_node = window_node->first_node("ThumbnailBottom");
	if (child_node != nullptr)
	{
		isThumbnailBottom = atoi(child_node->value());
	}


	child_node = window_node->first_node("Filter");
	if (child_node != nullptr)
	{
		showFilter = atoi(child_node->value());
	}

	child_node = window_node->first_node("Folder");
	if (child_node != nullptr)
	{
		showFolder = atoi(child_node->value());
	}

	child_node = window_node->first_node("Face");
	if (child_node != nullptr)
	{
		showFace = atoi(child_node->value());
	}

	child_node = window_node->first_node("LastRequest");
	if (child_node != nullptr)
	{
		sqlRequest = child_node->value();
	}

	child_node = window_node->first_node("FacePertinence");
	if (child_node != nullptr)
	{
		pertinence = atoi(child_node->value());
	}

	child_node = window_node->first_node("windowMode");
	if (child_node != nullptr)
	{
		windowMode = atoi(child_node->value());
	}

	child_node = window_node->first_node("windowVisible");
	if (child_node != nullptr)
	{
		windowVisible = atoi(child_node->value());
	}

	child_node = window_node->first_node("LastFilepath");
	if (child_node != nullptr)
	{
		filepath = child_node->value();
	}

	child_node = window_node->first_node("VideoEditorPath");
	if (child_node != nullptr)
	{
		pathProgramVideo = child_node->value();
	}

	child_node = window_node->first_node("ImageEditorPath");
	if (child_node != nullptr)
	{
		pathProgramPicture = child_node->value();
	}

	child_node = window_node->first_node("SlideFacePos");
	if (child_node != nullptr)
	{
		posFace = atoi(child_node->value());
	}

	child_node = window_node->first_node("SlideFolderPos");
	if (child_node != nullptr)
	{
		posFolder = atoi(child_node->value());
	}


	child_node = window_node->first_node("leftPosPanel");
	if (child_node != nullptr)
	{
		GetPositionParameter(child_node, leftPanelPos);
	}

	child_node = window_node->first_node("rightPosPanel");
	if (child_node != nullptr)
	{
		GetPositionParameter(child_node, rightPanelPos);
	}
}

void CMainParam::GetDiaporamaParameter(xml_node<>* diaporama_node)
{
	xml_node<>* child_node = diaporama_node->first_node("Fullscreen");
	if (child_node != nullptr)
	{
		fullscreen = atoi(child_node->value());
	}

	child_node = diaporama_node->first_node("Effect");
	if (child_node != nullptr)
	{
		numEffect = atoi(child_node->value());
	}

	child_node = diaporama_node->first_node("Delai");
	if (child_node != nullptr)
	{
		delai = atoi(child_node->value());
	}

	child_node = diaporama_node->first_node("Forward");
	if (child_node != nullptr)
	{
		enAvant = atoi(child_node->value());
	}
}

void CMainParam::SetPertinenceValue(const int& value)
{
	pertinence = value;
}

int CMainParam::GetPertinenceValue()
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


int CMainParam::GetVisibleWindowPanelInfos()
{
	return windowVisible;
}

void CMainParam::SetVisibleWindowPanelInfos(const int& windowVisible)
{
	this->windowVisible = windowVisible;
}

wxString CMainParam::GetLastShowPicture()
{
	return filepath;
}

void CMainParam::SetLastShowPicture(const wxString& filepath)
{
	this->filepath = filepath;
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
