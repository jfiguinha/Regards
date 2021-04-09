#include <header.h>
#include "KeywordWnd.h"
#include <SqlFindFacePhoto.h>
#include <Criteria.h>
#include <SqlPhotoCategorie.h>
#include <SqlFindCriteria.h>
#include <SqlCriteria.h>
#include <SqlFindPhotos.h>
#include <SqlPhotos.h>
#include <SqlCatalog.h>
#include "TreeDataCategory.h"
#include <TreeElementTexte.h>
#include <TreeElementTriangle.h>
#include <TreeElementDelete.h>
#include <TreeElementCheckBox.h>
#include <SqlFindFolderCatalog.h>
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <LibResource.h>
#include <ConvertUtility.h>
#include <ThumbnailData.h>
#include <SqlFindCatalog.h>
#include <theme.h>
#include <SqlPhotoCriteria.h>
#include <TreeWindow.h>
using namespace Regards::Sqlite;
using namespace Regards::Viewer;
using namespace Regards::Window;

CKeywordWnd::CKeywordWnd(CWindowMain * windowMain, CThemeTree * theme, CTreeElementControlInterface * interfaceControl)
{
    sqlRequest = "";
	treeDataModify = nullptr;
	widthPosition = 0;

	themeTree = *theme;
	themeTree.themeTriangle.SetHeight(themeTree.GetRowHeight());
	themeTree.themeCheckbox.SetHeight(themeTree.GetRowHeight());
	themeTree.themeDelete.SetHeight(themeTree.GetRowHeight());
	themeTree.themeSlide.SetHeight(themeTree.GetRowHeight());
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	eventControl = interfaceControl;
	rowWidth.push_back(0);
	rowWidth.push_back(0);
	yPos = 0;

	this->windowMain = windowMain;
}

CKeywordWnd::~CKeywordWnd()
{

}

void CKeywordWnd::Init(const wxString & filename)
{
	CSqlFindCatalog sqlCatalog;
	CatalogVector catalogVector;
	CriteriaVector criteriaVector;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;
	CSqlPhotos photos;
	top = tr.begin();

	numPhotoId = photos.GetPhotoId(filename);

	idElement = 0;

	sqlCatalog.GetCatalog(&catalogVector);

	for (CPhotoCatalog catalog : catalogVector)
	{
		CTreeDataCategory * treeData = new CTreeDataCategory();
		numCatalog = catalog.GetNumCatalog();
		treeData->SetNumElement(idElement);
		treeData->SetIdElement(-1);
		treeData->SetNumParent(0);
		treeData->SetIsParent(true);
		treeData->SetNumLevel(0);
		treeData->SetNumCatalog(numCatalog);
		treeData->SetKey("Keyword");
		treeData->SetExifKey("Keyword");
		treeData->SetTypeElement(TYPECATALOG);
		child = tr.insert(top, treeData);
		treeData->child = child;
		InitKeyWordCategorie(child, idElement);
	}

	photos.GetPhotoCriteriaByCategorie(&criteriaVector, filename, 7);

	for (CCriteria criteria : criteriaVector)
	{
		wxString localkey = to_string(criteria.GetId()) + L":" + criteria.GetLibelle() + L":1:";
		stateValue += localkey;
	}

	CreateElement();

	UpdateElement();
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CKeywordWnd::InitKeyWordCategorie(tree<CTreeData *>::iterator parent, int numParent)
{

	//Récupération des catégories principales
	CSqlFindCriteria sqlCriteria;
	CriteriaVector criteriaVector;
	tree<CTreeData *>::iterator child;
	sqlCriteria.SearchCriteria(&criteriaVector, 7, 1);

	for (CriteriaVector::iterator fit = criteriaVector.begin(); fit != criteriaVector.end(); fit++)
	{
		idElement++;
		CTreeDataCategory * treeData = new CTreeDataCategory();
		treeData->SetNumElement(idElement);
		treeData->SetNumCatalog(numCatalog);
		treeData->SetIdElement(-1);
		treeData->SetNumCategorie(fit->GetId());
		treeData->SetNumParent(numParent);
		treeData->SetIsParent(false);
		treeData->SetNumLevel(0);
		treeData->SetExifKey(to_string(fit->GetId()));
		treeData->SetKey(fit->GetLibelle());
		treeData->SetTypeElement(TYPECATEGORIE);
		child = tr.append_child(parent, treeData);
		treeData->child = child;
	}
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
tree<CTreeData *>::iterator CKeywordWnd::FindChild(tree<CTreeData *>::iterator parent, const wxString &catlibelle)
{
	tree<CTreeData *>::sibling_iterator itOld;
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	itOld = it;

	//int i = 

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		CTreeData * data = *it;
		if (data->GetExifKey() > catlibelle)
		{
			return itOld;
		}
		itOld = it;
		it++;
	}

	return nullptr;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void  CKeywordWnd::ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	//CTreeElement * treeElement = element->GetTreeElement();

	if (element->GetType() == ELEMENT_CHECKBOX)
	{
		CSqlPhotoCriteria photoCriteria;
		CTreeDataCategory * treeData = (CTreeDataCategory *)element->GetTreeData();
		CTreeElementCheckBox * checkBox = (CTreeElementCheckBox *)element->GetTreeElement();

		checkBox->SetCheckState(!checkBox->GetCheckState());
		
		int numCategorie = treeData->GetNumCategorie();
		if (checkBox->GetCheckState())
		{
			photoCriteria.InsertPhotoCriteria(numPhotoId, numCategorie);
		}
		else
		{
			photoCriteria.DeletePhotoCriteria(numPhotoId, numCategorie);
		}
		//UpdateElement();
		eventControl->UpdateTreeControl();
	}
	else if (element->GetType() == ELEMENT_TRIANGLE)
	{
		CTreeElementTriangle * triangle = (CTreeElementTriangle *)element->GetTreeElement();
		triangle->ClickElement(window, x, y);

		UpdateElement();
		eventControl->UpdateTreeControl();
	}
	else if (element->GetType() == ELEMENT_DELETE)
	{
		//CTreeElementDelete * deleteElement = (CTreeElementDelete *)element->GetTreeElement();
		CTreeDataCategory * treeData = (CTreeDataCategory *)element->GetTreeData();

		wxMessageDialog dialog(nullptr, _("Do you want to delete this criteria ?"), _("Confirmation"), wxOK | wxCANCEL);
		int returnValue = dialog.ShowModal();

		if (wxID_OK == returnValue)
		{
			CSqlCriteria sqlCriteria;
			CSqlPhotoCriteria sqlPhotoCriteria;

			int numCriteria = atoi(treeData->GetExifKey());
			//int numCriteria = criteriaVector[i].GetId();
			sqlCriteria.DeletePhotoCriteria(1, numCriteria);
			sqlPhotoCriteria.DeleteCriteria(numCriteria);
			sqlCriteria.DeleteCriteria(numCriteria, 7);

			wxWindow * mainWnd = window->FindWindowById(MAINVIEWERWINDOWID);
			wxCommandEvent * eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
			wxQueueEvent(mainWnd, eventChange);

			wxWindow * keyword = window->FindWindowById(KEYWORDCRITERIAWINDOWID);
			wxCommandEvent * eventRefresh = new wxCommandEvent(wxEVENT_REFRESHDATA);
			wxQueueEvent(keyword, eventRefresh);

			UpdateElement();
			eventControl->UpdateTreeControl();
		}

	}
	else if (element->GetType() == ELEMENT_TEXTE)
	{
		//CTreeElementTexte * texte = (CTreeElementTexte *)element->GetTreeElement();
		CTreeDataCategory * treeData = (CTreeDataCategory *)element->GetTreeData();

		wxTextEntryDialog dlg(nullptr, wxT("Update Keyword : \n"),
			wxT("Update keyword"),
			treeData->GetKey(), wxOK | wxCANCEL);
		if (dlg.ShowModal() == wxID_OK)
		{
			//bool isNew = false;
			// We can be certain that this string contains letters only.
			wxString value = dlg.GetValue();
			CSqlCriteria sqlCriteria;
			sqlCriteria.UpdateCriteria(1, treeData->GetNumCategorie(), value);

			wxWindow * mainWnd = window->FindWindowById(MAINVIEWERWINDOWID);
			wxCommandEvent * eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
			wxQueueEvent(mainWnd, eventChange);

			wxWindow * keyword = window->FindWindowById(KEYWORDCRITERIAWINDOWID);
			wxCommandEvent * eventRefresh = new wxCommandEvent(wxEVENT_REFRESHDATA);
			wxQueueEvent(keyword, eventRefresh);
		}

		UpdateElement();
		eventControl->UpdateTreeControl();

	}
}



void CKeywordWnd::UpdateScreenRatio()
{
    printf("CKeywordWnd::UpdateScreenRatio() \n");
    UpdateElement();
    eventControl->UpdateTreeControl();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool CKeywordWnd::GetTriangleState(const wxString &exifKey, const wxString &key)
{
	wxString localkey = exifKey + L":" + key + L":";
	size_t pos = stateTriangleValue.find(localkey);
	if (pos < stateTriangleValue.size())
	{
		pos += localkey.size();
		wchar_t check = stateTriangleValue[pos];
		if (check == '1')
			return true;
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool CKeywordWnd::GetCheckState(const wxString &exifKey, const wxString &key)
{
	wxString localkey = exifKey + L":" + key + L":";
	size_t pos = stateValue.find(localkey);
	if (pos < stateValue.size())
	{
		pos += localkey.size();
		wchar_t check = stateValue[pos];
		if (check == '1')
			return true;
		return false;
	}
	return false;
}

void CKeywordWnd::Resize()
{
	UpdateElement(true);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

void CKeywordWnd::CreateChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	CPositionElement *  posElement = nullptr;
	CTreeElementTexte * treeElementTexte = nullptr;
	CTreeElementDelete * treeElementDelete = nullptr;
	CTreeElementCheckBox * treeElementCheck = nullptr;
	//CTreeElementTriangle * treeElementTriangle = nullptr;

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		int widthElement = 0;
		int profondeur = tr.depth(it);
		CTreeDataCategory * data = (CTreeDataCategory*)*it;
		if (profondeur == 1 && (data->GetValue().size() > 0 || it.number_of_children() == 0))
		{
			int xPosInit = widthPosition * (profondeur + 1);
			int xPos = xPosInit;

			//xPos += posElement->GetWidth() + themeTree.GetMargeX();
			//xPos = windowMain->GetWidth() / 2;
			

			bool check = GetCheckState(data->GetExifKey(), data->GetKey());
			treeElementCheck = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
			xPos = windowMain->GetWidth() - treeElementCheck->GetWidth() - themeTree.GetMargeX();
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementCheck->GetWidth(), treeElementCheck->GetHeight(), ELEMENT_CHECKBOX, treeElementCheck, data);
			//xPos += posElement->GetWidth() + themeTree.GetMargeX();

			xPos = xPosInit + treeElementCheck->GetWidth() + themeTree.GetMargeX();
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data);

			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			treeElementDelete = CreateDeleteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight());
			treeElementDelete->SetVisible(true);
			posElement = CreatePositionElement(windowMain->GetWidth() - treeElementCheck->GetWidth() - themeTree.GetMargeX() - treeElementDelete->GetWidth() - themeTree.GetMargeX(), yPos, nbRow, 0, treeElementDelete->GetWidth(), treeElementDelete->GetHeight(), ELEMENT_DELETE, treeElementDelete, data);

			//xPos += treeElementDelete->GetWidth() + themeTree.GetMargeX();

			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;
		}
		it++;

	}
}


void CKeywordWnd::CreateElement()
{
	vectorPosElement.clear();
	vectorPosElementDynamic.clear();
	tree<CTreeData *>::sibling_iterator it = tr.begin();
	tree<CTreeData *>::iterator itend = tr.end();
	yPos = 0;
	nbRow = 0;
	widthPosition = 0;

	while (it != itend) {
		CTreeDataCategory * data = (CTreeDataCategory*)*it;
		int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			//CTreeElementCheckBox * treeElementCheck = nullptr;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CPositionElement * posElement = nullptr;
			bool isOpen = GetTriangleState(data->GetExifKey(), data->GetKey());
			treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), isOpen);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);

			widthPosition = xPos + posElement->GetWidth();

			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			CreateChildTree(it);
		}
		it++;

	}
}



void CKeywordWnd::UpdateElement(const bool &init)
{
	tree<CTreeData *>::sibling_iterator it = tr.begin();
	tree<CTreeData *>::iterator itend = tr.end();
	yPos = 0;
	nbRow = 0;
	widthPosition = 0;

	for (CPositionElement * value : vectorPosElement)
	{
		if (value != nullptr)
		{
			CTreeElement * treeElement = value->GetTreeElement();
			treeElement->SetVisible(false);
		}
	}

	bool isVisible = true;

	while (it != itend) {
		CTreeDataCategory * data = (CTreeDataCategory*)*it;
		int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			//CTreeElementCheckBox * treeElementCheck = nullptr;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CPositionElement * posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement == nullptr)
			{
				bool isOpen = GetTriangleState(data->GetExifKey(), data->GetKey());
				treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), isOpen);
				treeElementTriangle->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
			}
			else
			{
				treeElementTriangle = (CTreeElementTriangle *)posElement->GetTreeElement();
				treeElementTriangle->SetVisible(isVisible);
               treeElementTriangle->SetElementPos(xPos,yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);

			}

			widthPosition = xPos + posElement->GetWidth();

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement == nullptr)
			{
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
			}
			else
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
                treeElementTexte->SetElementPos(xPos,yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			bool isOpen = treeElementTriangle->GetOpen();
			if (isOpen)
				UpdateChildTree(it, init);
		}
		it++;

	}

	
}

void CKeywordWnd::UpdateChildTree(tree<CTreeData *>::sibling_iterator &parent, const bool &init)
{
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		int widthElement = 0;
		int profondeur = tr.depth(it);
		CTreeDataCategory * data = (CTreeDataCategory*)*it;
		if (profondeur == 1 && (data->GetValue().size() > 0 || it.number_of_children() == 0))
		{
			int xPosInit = widthPosition * (profondeur + 1);
			int xPos = xPosInit;
			CTreeElementTexte * treeElementTexte = nullptr;
			

			
			//xPos += posElement->GetWidth() + themeTree.GetMargeX();
			
			CTreeElementCheckBox * treeElementCheck = nullptr;
			CPositionElement *  posElement = GetElement(data, ELEMENT_CHECKBOX);

			if (posElement == nullptr)
			{
				bool check = GetCheckState(data->GetExifKey(), data->GetKey());
				treeElementCheck = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
				treeElementCheck->SetVisible(isVisible);
				xPos = xPosInit;
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementCheck->GetWidth(), treeElementCheck->GetHeight(), ELEMENT_CHECKBOX, treeElementCheck, data);
			}
			else
			{
				treeElementCheck = (CTreeElementCheckBox *)posElement->GetTreeElement();
				treeElementCheck->SetVisible(isVisible);
				treeElementCheck->SetElementPos(xPos,yPos);
				xPos = xPosInit;
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			posElement = GetElement(data, ELEMENT_TEXTE);

			if (posElement == nullptr)
			{
				xPos = xPosInit + treeElementCheck->GetWidth() + themeTree.GetMargeX();
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data);
			}
			else
			{
				xPos = xPosInit + treeElementCheck->GetWidth() + themeTree.GetMargeX();
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				treeElementTexte->SetElementPos(xPos, yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			CTreeElementDelete * treeElementDelete = nullptr;
			posElement = GetElement(data, ELEMENT_DELETE);
			if (posElement == nullptr)
			{
				xPos = windowMain->GetWidth() - treeElementCheck->GetWidth() - themeTree.GetMargeX() - treeElementDelete->GetWidth() - themeTree.GetMargeX();
				treeElementDelete = CreateDeleteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight());
				treeElementDelete->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementDelete->GetWidth(), treeElementDelete->GetHeight(), ELEMENT_DELETE, treeElementDelete, data);
			}
			else
			{
				treeElementDelete = (CTreeElementDelete *)posElement->GetTreeElement();
				treeElementDelete->SetVisible(isVisible);
				xPos = windowMain->GetWidth() - treeElementCheck->GetWidth() - themeTree.GetMargeX() - treeElementDelete->GetWidth() - themeTree.GetMargeX();
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			//xPos = GetWidth() - treeElementDelete->GetWidth();


			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

		
		}
		it++;

	}
}