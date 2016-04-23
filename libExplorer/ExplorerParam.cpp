#include "ExplorerParam.h"

using namespace rapidxml;
using namespace Regards::Explorer;

CExplorerParam::CExplorerParam()
{
	//Définition des valeurs par défaut
	positionRegardsViewer.x = 0;
	positionRegardsViewer.width = 800;
	positionRegardsViewer.y = 0;
	positionRegardsViewer.height = 800;

	showFolder = 1;
	showInfos = 1;
	showPreview = 1;
	positionFolderCriteria = 200;
	positionPreviewThumbnail = 200;
	positionPreviewInfos = 200;
	positionFolderCriteriaPreviewThumbnail = 400;

	viewerProgram = "RegardsViewer.exe";
	shopProgram = "RegardsShop64.exe";

}

CExplorerParam::~CExplorerParam()
{
	doc.clear();
}

void CExplorerParam::SetCatalogOpenTriangle(const wxString &state)
{
    criteriaTriangleList = state;
}

wxString CExplorerParam::GetCatalogOpenTriangle()
{
	wxString value;
	value.append(criteriaTriangleList.begin(), criteriaTriangleList.end());
	return value;
}

void CExplorerParam::SetCatalogCriteria(const wxString &state)
{
    criteriaList=state;
}

wxString CExplorerParam::GetCatalogCriteria()
{
	wxString value;
	value.append(criteriaList.begin(), criteriaList.end());
	return value;
}

//////////////////////////////////////////////////////////////////////////////////////////
//Saving Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void CExplorerParam::SaveParameter()
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
	xml_node<>* sectionWindowSplitter = node("WindowSplitter");
	SetWindowSplitterParameter(sectionWindowSplitter);
	root->append_node(sectionWindowSplitter);
	xml_node<>* sectionPosition = node("Position");
	SetPositionParameter(sectionPosition);
	root->append_node(sectionPosition);
	xml_node<>* sectionProgramExtern = node("ProgramExtern");
	SetProgramExtern(sectionProgramExtern);
	root->append_node(sectionProgramExtern);
	xml_node<>* sectionPreviewExtern = node("Preview");
	SetPreviewParameter(sectionPreviewExtern);
	root->append_node(sectionPreviewExtern);
	xml_node<>* sectionCriteria = node("Criteria");
	SetCriteriaParameter(sectionCriteria);
	root->append_node(sectionCriteria);

	// save the xml data to a file (could equally well use any other ostream)
	std::ofstream file(filename.ToStdString());
	if (file.is_open())
	{
		file << doc;
		file.close();
	}

}

void CExplorerParam::SetCriteriaParameter(xml_node<>* section)
{
	section->append_node(node("Value", criteriaList));
	section->append_node(node("TriangleValue", criteriaTriangleList));
}

void CExplorerParam::SetWindowSplitterParameter(xml_node<>* sectionPosition)
{
	xml_node<>* sectionWindowPosition = node("FolderCriteria");
	SetFolderCriteriaParameter(sectionWindowPosition);
	sectionPosition->append_node(sectionWindowPosition);

	sectionWindowPosition = node("PreviewThumbnail");
	SetPreviewThumbnailParameter(sectionWindowPosition);
	sectionPosition->append_node(sectionWindowPosition);

	sectionWindowPosition = node("PreviewInfos");
	SetPreviewInfosParameter(sectionWindowPosition);
	sectionPosition->append_node(sectionWindowPosition);

	sectionWindowPosition = node("FolderCriteriaPreviewThumbnail");
	SetCategoryFolderImageViewParameter(sectionWindowPosition);
	sectionPosition->append_node(sectionWindowPosition);
}

void CExplorerParam::SetProgramExtern(xml_node<>* sectionPosition)
{
	sectionPosition->append_node(node("Viewer", viewerProgram));
	sectionPosition->append_node(node("Shop", shopProgram));
}

void CExplorerParam::SetFolderCriteriaParameter(xml_node<>* sectionWindow)
{
	sectionWindow->append_node(node("Folder", to_string(showFolder)));
	sectionWindow->append_node(node("Position", to_string(positionFolderCriteria)));
}

void CExplorerParam::SetPreviewThumbnailParameter(xml_node<>* sectionWindow)
{
	sectionWindow->append_node(node("ShowPreview", to_string(showPreview)));
	sectionWindow->append_node(node("Position", to_string(positionPreviewThumbnail)));
}

void CExplorerParam::SetPreviewInfosParameter(xml_node<>* sectionWindow)
{
	sectionWindow->append_node(node("ShowInfos", to_string(showInfos)));
	sectionWindow->append_node(node("Position", to_string(positionPreviewInfos)));
}

void CExplorerParam::SetCategoryFolderImageViewParameter(xml_node<>* sectionWindow)
{
	sectionWindow->append_node(node("Position", to_string(positionFolderCriteriaPreviewThumbnail)));
}

//////////////////////////////////////////////////////////////////////////////////////////
//Saving SetParameter
//////////////////////////////////////////////////////////////////////////////////////////
void CExplorerParam::SetWindowPositionParameter(xml_node<>* sectionWindowPosition, const wxRect &rc)
{
	sectionWindowPosition->append_node(node("Left", to_string(rc.x)));
	sectionWindowPosition->append_node(node("Right", to_string(rc.width)));
	sectionWindowPosition->append_node(node("Bottom", to_string(rc.y)));
	sectionWindowPosition->append_node(node("Top", to_string(rc.height)));
}

//////////////////////////////////////////////////////////////////////////////////////////
//Saving SetParameter
//////////////////////////////////////////////////////////////////////////////////////////
void CExplorerParam::SetPositionParameter(xml_node<>* sectionPosition)
{
	xml_node<>* sectionWindowPosition = node("Window");
	SetWindowPositionParameter(sectionWindowPosition, positionRegardsViewer);
	sectionPosition->append_node(sectionWindowPosition);
}

//////////////////////////////////////////////////////////////////////////////////////////
//Loading Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void CExplorerParam::LoadParameter()
{
	xml_node<> * root_node;
	//long nodeSize = 0;
	root_node = doc.first_node("Parameter");

	xml_node<> * child_node = root_node->first_node("WindowSplitter");
	if (child_node != 0)
		GetWindowSplitterParameter(child_node);

	child_node = root_node->first_node("Position");
	if (child_node != 0)
		GetPositionParameter(child_node);

	child_node = root_node->first_node("ProgramExtern");
	if (child_node != 0)
		GetProgramParameter(child_node);

	child_node = root_node->first_node("Preview");
	if (child_node != 0)
		GetPreviewParameter(child_node);

	child_node = root_node->first_node("Criteria");
	if (child_node != 0)
		GetCriteriaParameter(child_node);
}

//////////////////////////////////////////////////////////////////////////////////////////
//Saving SetParameter
//////////////////////////////////////////////////////////////////////////////////////////
void CExplorerParam::SetPreviewParameter(xml_node<>* sectionWindow)
{
	sectionWindow->append_node(node("Effect", to_string(numEffect)));
}

void CExplorerParam::GetPreviewParameter(xml_node<> * position_node)
{
	xml_node<> * child_node = position_node->first_node("Effect");
	if (child_node != 0)
		numEffect = atoi(child_node->value());
}

void CExplorerParam::GetWindowSplitterParameter(xml_node<> * position_node)
{
	xml_node<> * child_node = position_node->first_node("FolderCriteria");
	if (child_node != 0)
		GetFolderCriteriaParameter(child_node);

	child_node = position_node->first_node("PreviewThumbnail");
	if (child_node != 0)
		GetPreviewThumbnailParameter(child_node);

	child_node = position_node->first_node("PreviewInfos");
	if (child_node != 0)
		GetPreviewInfosParameter(child_node);

	child_node = position_node->first_node("FolderCriteriaPreviewThumbnail");
	if (child_node != 0)
		GetCategoryFolderImageViewParameter(child_node);
}

void CExplorerParam::GetFolderCriteriaParameter(xml_node<> * position_node)
{
	wxString value = "";
	wxString nodeName = "";
	xml_node<> * child_node = position_node->first_node("Folder");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		showFolder = atoi(child_node->value());
	}

	child_node = position_node->first_node("Position");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		positionFolderCriteria = atoi(child_node->value());
	}
}

void CExplorerParam::GetPreviewThumbnailParameter(xml_node<> * position_node)
{
	wxString value = "";
	wxString nodeName = "";
	xml_node<> * child_node = position_node->first_node("ShowPreview");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		showPreview = atoi(child_node->value());
	}

	child_node = position_node->first_node("Position");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		positionPreviewThumbnail = atoi(child_node->value());
	}
}

void CExplorerParam::GetPreviewInfosParameter(xml_node<> * position_node)
{
	wxString value = "";
	wxString nodeName = "";
	xml_node<> * child_node = position_node->first_node("ShowInfos");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		showInfos = atoi(child_node->value());
	}

	child_node = position_node->first_node("Position");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		positionPreviewInfos = atoi(child_node->value());
	}
}

void CExplorerParam::GetCriteriaParameter(xml_node<> * position_node)
{
	xml_node<> * child_node = position_node->first_node("Value");
	if (child_node != 0)
		criteriaList = child_node->value();
	child_node = position_node->first_node("TriangleValue");
	if (child_node != 0)
		criteriaTriangleList = child_node->value();
}

void CExplorerParam::GetCategoryFolderImageViewParameter(xml_node<> * position_node)
{
	wxString value = "";
	wxString nodeName = "";
	xml_node<> * child_node = position_node->first_node("Position");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		positionFolderCriteriaPreviewThumbnail = atoi(child_node->value());
	}
}

void CExplorerParam::GetPositionParameter(xml_node<> * position_node)
{
	xml_node<> * child_node = position_node->first_node("Window");
	if (child_node != 0)
		positionRegardsViewer = GetWindowPositionParameter(child_node);
}

void CExplorerParam::GetProgramParameter(xml_node<> * position_node)
{
	xml_node<> * child_node = position_node->first_node("Viewer");
	if (child_node != 0)
		viewerProgram = child_node->value();

	child_node = position_node->first_node("Shop");
	if (child_node != 0)
		shopProgram = child_node->value();
}

wxRect CExplorerParam::GetWindowPositionParameter(xml_node<> * position_node)
{
	wxString value = "";
	wxString nodeName = "";
	wxRect rc = { 0, 0, 0, 0 };
	xml_node<> * child_node = position_node->first_node("Left");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		rc.x = atoi(child_node->value());
	}

	child_node = position_node->first_node("Right");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		rc.width = atoi(child_node->value());
	}

	child_node = position_node->first_node("Bottom");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		rc.y = atoi(child_node->value());
	}

	child_node = position_node->first_node("Top");
	if (child_node != 0)
	{
		value = child_node->value();
		nodeName = child_node->name();
		rc.height = atoi(child_node->value());
	}

	return rc;
}

void CExplorerParam::SetPositionWindow(const wxRect &rc)
{
	positionRegardsViewer = rc;
}


void CExplorerParam::GetPositionWindow(wxRect &rc)
{
	rc = positionRegardsViewer;
}


void CExplorerParam::SetPositionFolderCriteria(const int &position)
{
	positionFolderCriteria = position;
}

void CExplorerParam::SetPositionPreviewThumbnail(const int &position)
{
	positionPreviewThumbnail = position;
}

void CExplorerParam::SetPositionPreviewInfos(const int &position)
{
	positionPreviewInfos = position;
}

void CExplorerParam::SetPositionFolderCriteriaPreviewThumbnail(const int &position)
{
	positionFolderCriteriaPreviewThumbnail = position;
}

void CExplorerParam::SetShowFolder(const int &show)
{
	showFolder = show;
}

void CExplorerParam::SetShowPreview(const int &show)
{
	showPreview = show;
}

void CExplorerParam::SetShowInfos(const int &show)
{
	showInfos = show;
}

int CExplorerParam::GetPositionFolderCriteria()
{
	return positionFolderCriteria;
}

int CExplorerParam::GetPositionPreviewThumbnail()
{
	return positionPreviewThumbnail;
}

int CExplorerParam::GetPositionPreviewInfos()
{
	return positionPreviewInfos;
}

int CExplorerParam::GetShowFolder()
{
	return showFolder;
}

int CExplorerParam::GetShowPreview()
{
	return showPreview;
}

int CExplorerParam::GetShowInfos()
{
	return showInfos;
}

int CExplorerParam::GetPositionFolderCriteriaPreviewThumbnail()
{
	return positionFolderCriteriaPreviewThumbnail;
}


//Program Extern
wxString CExplorerParam::GetViewerProgram()
{
	return viewerProgram;
}

wxString CExplorerParam::GetShopProgram()
{
	return shopProgram;
}

void CExplorerParam::SetViewerProgram(const wxString &name)
{
	viewerProgram = name;
}

void CExplorerParam::SetShopProgram(const wxString &name)
{
	shopProgram = name;
}

