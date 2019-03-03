#include <header.h>
#include "CategoryWnd.h"
#include <SqlFindFacePhoto.h>
#include <Criteria.h>
#include <SqlPhotoCategorie.h>
#include <SqlFindCriteria.h>
#include <SqlFindCatalog.h>
#include <SqlFindPhotos.h>
#include <SqlCatalog.h>
#include "TreeDataCategory.h"
#include <SQLRemoveData.h>
#include <TreeElementTexte.h>
#include <TreeElementTriangle.h>
#include <TreeElementCheckBox.h>
#include <SqlFindFolderCatalog.h>
#include <SqlFolderCatalog.h>
#include <TreeElementDelete.h>
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <LibResource.h>
#include <ConvertUtility.h>
#include <ThumbnailData.h>
#include <window_id.h>
#include "MainWindow.h"
#include <FileGeolocation.h>
#include "ViewerParamInit.h"
#include "ViewerParam.h"
using namespace Regards::Sqlite;
using namespace Regards::Viewer;

CCategoryWnd::CCategoryWnd(CWindowMain * windowMain, CThemeTree * theme, CTreeElementControlInterface * interfaceControl)
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
	MonthName = CConvertUtility::split(CLibResource::LoadStringFromResource(L"LBLMONTHNAME",1), ',');
	config = (CViewerParam *)CViewerParamInit::getInstance();
	this->windowMain = windowMain;
}

void CCategoryWnd::RefreshListPhoto()
{
	UpdateSQLSearchCriteria();
}

CCategoryWnd::~CCategoryWnd()
{

}

tree<CTreeData *>::iterator CCategoryWnd::FindExifKey(const wxString & exifkey)
{
	tree<CTreeData *>::iterator it = tr.begin();
	tree<CTreeData *>::iterator itend = tr.end();

	//int i = 

	while (it != itend) {
		CTreeData * data = *it;
		if (data->GetExifKey() == exifkey)
			return it;
		it++;
	}

	return itend;
}


tree<CTreeData *>::iterator CCategoryWnd::FindFolderKey(const wxString & path)
{
	tree<CTreeData *>::iterator it = tr.begin();
	tree<CTreeData *>::iterator itend = tr.end();

	//int i = 

	while (it != itend) {
		CTreeData * data = *it;
		if (data->GetKey() == path)
			return it;
		it++;
	}

	return itend;
}

void CCategoryWnd::Init()
{

	CSqlFindCatalog sqlCatalog;
	CatalogVector catalogVector;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	LoadState();

	top = tr.begin();

	idElement = 0;

	sqlCatalog.GetCatalog(&catalogVector);

	for (CCatalog catalog : catalogVector)
	{
		CTreeDataCategory * treeData = new CTreeDataCategory();
		numCatalog = catalog.GetNumCatalog();
		treeData->SetNumElement(idElement);
		treeData->SetIdElement(-1);
		treeData->SetNumParent(0);
		treeData->SetIsParent(true);
		treeData->SetNumLevel(0);
		treeData->SetNumCatalog(numCatalog);
		treeData->SetKey(catalog.GetLibelle());
		treeData->SetExifKey(catalog.GetLibelle());
		treeData->SetTypeElement(TYPECATALOG);
		child = tr.insert(top, treeData);
		treeData->child = child;
		InitCatalogCategorie(child, idElement);
	}

	CreateElement();

	UpdateElement();
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
wxString CCategoryWnd::GetCatalogLibelle(const int &numCatalog)
{
	CSqlCatalog sqlCatalog;
	return sqlCatalog.GetCatalogLibelle(numCatalog);
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CCategoryWnd::InitCatalogCategorie(tree<CTreeData *>::iterator parent, int numParent)
{

	//Récupération des catégories principales
	CSqlPhotoCategorie sqlCategorie;
	PhotoCategorieVector categorieVector;
	tree<CTreeData *>::iterator child;
	sqlCategorie.LoadPhotoCategorie(&categorieVector, 1);

	for (PhotoCategorieVector::iterator fit = categorieVector.begin(); fit != categorieVector.end(); fit++)
	{
		//if (fit->GetId() != 2)
		//{
			idElement++;
			CTreeDataCategory * treeData = new CTreeDataCategory();
			treeData->SetNumElement(idElement);
			treeData->SetNumCatalog(numCatalog);
			treeData->SetIdElement(-1);
			treeData->SetNumCategorie(fit->GetId());
			treeData->SetNumParent(numParent);
			treeData->SetIsParent(false);
			treeData->SetNumLevel(1);
			treeData->SetKey(fit->GetLibelle());
			treeData->SetTypeElement(TYPECATEGORIE);
			child = tr.append_child(parent, treeData);
			treeData->child = child;
			switch(fit->GetId())
			{
			case 2:
				LoadFolder(numCatalog, child, idElement);
				break;
			case 4:
				LoadFace(numCatalog, child, idElement);
				break;
			default:
				LoadCategorie(treeData->GetNumCategorie(), child, idElement);
			}
	}
}

void CCategoryWnd::LoadFace(const int &numCatalog, tree<CTreeData *>::iterator parent, int numParent)
{
	CSqlFindFacePhoto sqlFindFace;
	std::vector<CFaceName> listFace = sqlFindFace.GetListFaceNameSelectable();
	tree<CTreeData *>::iterator child;
	for (std::vector<CFaceName>::iterator fit = listFace.begin(); fit != listFace.end(); fit++)
	{
		idElement++;
		CTreeDataCategory * treeData = new CTreeDataCategory();
		treeData->SetNumElement(idElement);
		treeData->SetIdElement(fit->numFace);
		treeData->SetNumCatalog(numCatalog);
		treeData->SetNumCategorie(4);
		treeData->SetNumLevel(2);
		treeData->SetNumParent(numParent);
		treeData->SetIsParent(false);
		treeData->SetKey(fit->faceName);
		treeData->SetTypeElement(TYPEFACE);
		child = tr.append_child(parent, treeData);
		treeData->child = child;
	}
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CCategoryWnd::LoadFolder(const int &numCatalog, tree<CTreeData *>::iterator parent, int numParent)
{

	CSqlFindFolderCatalog sqlFolderCatalog;
	FolderCatalogVector folderCatalogVector;
	sqlFolderCatalog.GetFolderCatalog(&folderCatalogVector, numCatalog);
	tree<CTreeData *>::iterator child;

	for (FolderCatalogVector::iterator fit = folderCatalogVector.begin(); fit != folderCatalogVector.end(); fit++)
	{
		idElement++;
		CTreeDataCategory * treeData = new CTreeDataCategory();
		treeData->SetNumElement(idElement);
		treeData->SetIdElement(fit->GetNumFolder());
		treeData->SetNumCatalog(numCatalog);
		treeData->SetNumCategorie(2);
		treeData->SetNumLevel(2);
		treeData->SetNumParent(numParent);
		treeData->SetIsParent(false);
		treeData->SetKey(fit->GetFolderPath());
		treeData->SetTypeElement(TYPEFOLDER);
		child = tr.append_child(parent, treeData);
		treeData->child = child;
	}
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
tree<CTreeData *>::iterator CCategoryWnd::FindChild(tree<CTreeData *>::iterator parent, const wxString &catlibelle)
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
void CCategoryWnd::AddCategorie(const int &numCatalog, const int &numCategorie, tree<CTreeData *>::iterator parent, const int &numParent, const wxString &catlibelle, const int &id)
{
	//int item = 0;
	//int index = 0;
	wchar_t seps[] = L".";

	int level = 2;
	wxString exifKey = L"";
	wchar_t libelle[1024];
	wchar_t * next_token1 = nullptr;
	tree<CTreeData *>::iterator child = parent;
	wcscpy(libelle, catlibelle.c_str());
	int numParentElement = numParent;
#if defined(WIN32) && _MSC_VER < 1900
	wchar_t * token = wcstok(libelle, seps); // C4996
#else
	// Establish string and get the first token:
	wchar_t * token = wcstok(libelle, seps, &next_token1); // C4996
#endif

	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		idElement++;

		if (exifKey != L"")
			exifKey.append(L".");
		exifKey.append(token);

		CTreeDataCategory * treeData = new CTreeDataCategory();
		treeData->SetIsParent(false);
		if (numCategorie == 3 && level == 3)
		{
            wchar_t * pEnd;
			long numMonth = wcstol(token,&pEnd,10);
			treeData->SetKey(MonthName.at(numMonth - 1));
		}
		else
			treeData->SetKey(token);
		treeData->SetNumElement(idElement);
		treeData->SetTypeElement(TYPECRITERIA);
		treeData->SetNumCatalog(numCatalog);
		treeData->SetNumCategorie(numCategorie);
		treeData->SetNumParent(numParentElement);
		treeData->SetNumLevel(level++);

		if (catlibelle == exifKey)
			treeData->SetIdElement(id);
		else
			treeData->SetIdElement(-1);

		//Recherche de la clé
		tree<CTreeData *>::iterator it = FindKey(treeData->GetKey(), child);
		if (it != nullptr)
		{
			child = it;
			CTreeDataCategory * data = (CTreeDataCategory *)*it;
			treeData->SetNumParent(data->GetNumParent());
			numParentElement = data->GetNumElement();
			delete(treeData);
		}
		else
		{
			numParentElement = idElement;
			treeData->SetExifKey(exifKey);
			tree<CTreeData *>::iterator it = FindChild(child, treeData->GetExifKey());
			if (it == nullptr)
				child = tr.append_child(child, treeData);
			else
				child = tr.insert(it, treeData);
			treeData->child = child;
		}
#if defined(WIN32) && _MSC_VER < 1900
		token = wcstok(nullptr, seps); // C4996
#else
		token = wcstok(nullptr, seps, &next_token1); // C4996
#endif

	}
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CCategoryWnd::LoadCategorie(const int &numCategorie, tree<CTreeData *>::iterator parent, int numParent)
{
	CSqlFindCriteria findCriteria;
	CriteriaVector criteriaVector;
	findCriteria.SearchCriteria(&criteriaVector, numCategorie, numCatalog);
	//int item = 0;
	//int index = 0;
	wchar_t seps[] = L".";

	for (CCriteria criteria : criteriaVector)
	{
		int level = 2;
		wxString exifKey = L"";
		wxString libelle = L"";
		wchar_t * next_token1 = nullptr;
		tree<CTreeData *>::iterator child = parent;
		if (criteria.GetLibelle() != L"")
		{
			libelle = criteria.GetLibelle();

			int numParentElement = numParent;
			//int index = 0;
			// Establish string and get the first token:
#if defined(WIN32) && _MSC_VER < 1900
			wchar_t * token = wcstok((wchar_t *)libelle.wc_str(), seps); // C4996
#else
			wchar_t * token = wcstok((wchar_t *)libelle.wc_str(), seps, &next_token1); // C4996
#endif

			// Note: strtok is deprecated; consider using strtok_s instead
			do
			{
				idElement++;

				if (exifKey != L"")
					exifKey.append(L".");
				exifKey.append(token);

				CTreeDataCategory * treeData = new CTreeDataCategory();
				treeData->SetIsParent(false);

				if (numCategorie == 3 && level == 3)
				{
                    wchar_t * pEnd;
                    long numMonth = wcstol(token,&pEnd,10);
					treeData->SetKey(MonthName.at(numMonth - 1));
				}
				else
					treeData->SetKey(token);
				treeData->SetNumElement(idElement);
				treeData->SetTypeElement(TYPECRITERIA);
				treeData->SetNumCatalog(numCatalog);
				treeData->SetNumCategorie(numCategorie);
				treeData->SetNumParent(numParentElement);
				treeData->SetNumLevel(level++);

				if (criteria.GetLibelle().c_str() == exifKey)
					treeData->SetIdElement(criteria.GetId());
				else
					treeData->SetIdElement(-1);

				//Recherche de la clé
				tree<CTreeData *>::iterator it = FindKey(treeData->GetKey(), child);
				if (it != nullptr)
				{
					child = it;
					CTreeDataCategory * data = (CTreeDataCategory *)*it;
					treeData->SetNumParent(data->GetNumParent());
					numParentElement = data->GetNumElement();
				}
				else
				{
					numParentElement = idElement;
					treeData->SetExifKey(exifKey);

					//Ordonnancement par Alphabet
					//child = tr.append_child(child, treeData);
					tree<CTreeData *>::iterator it = FindChild(child, treeData->GetExifKey());
					if (it == nullptr)
						child = tr.append_child(child, treeData);
					else
						child = tr.insert(it, treeData);

					treeData->child = child;
				}

#if defined(WIN32) &&  _MSC_VER < 1900
	token = wcstok(nullptr, seps); // C4996
#else
	token = wcstok(nullptr, seps, &next_token1); // C4996
#endif
				//index++;

			} while (token != nullptr);
		}
	}
}


wxString CCategoryWnd::GetSqlRequest()
{
	CViewerParam * viewerParam = (CViewerParam *)CViewerParamInit::getInstance();
    vector<int> listFolder;
	vector<int> listFace;
	vector<int> listFolderNotSelected;
	vector<int> listFaceNotSelected;
	vector<int> listFaceSelected;
    vector<int> listCriteriaNotIn;
	double pertinence = 0.0;
    
	for (CPositionElement * value : vectorPosElementDynamic)
	{
        if (value->GetType() == ELEMENT_CHECKBOX)
        {
            CTreeElementCheckBox * checkBox = (CTreeElementCheckBox *)value->GetTreeElement();
            CTreeDataCategory * treeData = (CTreeDataCategory *)value->GetTreeData();
            
            if (!checkBox->GetCheckState())
            {
                if (treeData->GetNumCategorie() != 2 && treeData->GetNumCategorie() != 4 && treeData->GetIdElement() != -1)
                    listCriteriaNotIn.push_back(treeData->GetIdElement());
				else if(treeData->GetNumCategorie() == 2)
					listFolderNotSelected.push_back(treeData->GetIdElement());
				else if(treeData->GetNumCategorie() == 4)
					listFaceNotSelected.push_back(treeData->GetIdElement());
            }
			else
			{
				if(treeData->GetNumCategorie() == 4)
					listFaceSelected.push_back(treeData->GetIdElement());
			}

        }
    }
    
	FolderCatalogVector folderList;
	CSqlFindFolderCatalog folderCatalog;
	folderCatalog.GetFolderCatalog(&folderList, NUMCATALOGID);

	for (CFolderCatalog folderCatalog : folderList)
	{
		std::vector<int>::iterator it;
		it = find(listFolderNotSelected.begin(), listFolderNotSelected.end(), folderCatalog.GetNumFolder());
		if (it == listFolderNotSelected.end())
			listFolder.push_back(folderCatalog.GetNumFolder());
	}
  
	if(viewerParam != nullptr)
		pertinence = viewerParam->GetPertinenceValue();

    CSqlFindPhotos sqlFindPhotos;
    sqlRequest = sqlFindPhotos.GenerateSqlRequest(NUMCATALOGID, listFolder, listCriteriaNotIn, listFaceNotSelected, listFaceSelected, pertinence);

    viewerParam->SetLastSqlRequest(sqlRequest);
    
    return sqlRequest;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CCategoryWnd::UpdateSQLSearchCriteria()
{
    CSqlFindPhotos sqlFindPhotos;
    sqlRequest = GetSqlRequest();
    sqlFindPhotos.SearchPhotos(sqlRequest);
    
    CViewerParam * viewerParam = (CViewerParam *)CViewerParamInit::getInstance();
    viewerParam->SetLastSqlRequest(sqlRequest);

	if (windowMain != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_CRITERIASHOWUPDATE);
		this->windowMain->GetEventHandler()->AddPendingEvent(evt);
	}

}


void CCategoryWnd::SaveStateTriangle()
{
	int nbElement = 0;
	stateTriangleValue = L"";

	for (CPositionElement * value : vectorPosElementDynamic)
	{
		if (value != nullptr)
		{
			if (value->GetType() == ELEMENT_TRIANGLE)
			{
				//bool isOpen = true;
				CTreeData * treeData = value->GetTreeData();
				CTreeElementTriangle * triangle = (CTreeElementTriangle *)value->GetTreeElement();
				if (triangle != nullptr && treeData != nullptr)
				{
					bool isOpen = triangle->GetOpen();
					wxString exifKey = treeData->GetExifKey();
					wxString key = treeData->GetKey();
					stateTriangleValue.append(exifKey);
					stateTriangleValue.append(L":");
					stateTriangleValue.append(key);
					if (isOpen)
						stateTriangleValue.append(L":1");
					else
						stateTriangleValue.append(L":0");

					stateTriangleValue.append(L";"); nbElement++;
				}
			}
		}
	}

	
	if (config != nullptr)
		config->SetCatalogOpenTriangle(stateTriangleValue);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CCategoryWnd::SaveState()
{
	int nbElement = 0;
	stateValue = L"";

	for (CPositionElement * value : vectorPosElementDynamic)
	{
		if (value != nullptr)
		{
			if (value->GetType() == ELEMENT_CHECKBOX)
			{
				//bool checkState = false;
				CTreeData * treeData = value->GetTreeData();
				CTreeElementCheckBox * checkBox = (CTreeElementCheckBox *)value->GetTreeElement();
				if (checkBox != nullptr && treeData != nullptr)
				{
					bool checkState = checkBox->GetCheckState();
					wxString exifKey = treeData->GetExifKey();
					wxString key = treeData->GetKey();
					stateValue.append(exifKey);
					stateValue.append(L":");
					stateValue.append(key);
					if (checkState)
						stateValue.append(L":1");
					else
						stateValue.append(L":0");

					stateValue.append(L";"); nbElement++;
				}
			}
		}
	}

	if (config != nullptr)
		config->SetCatalogCriteria(stateValue);
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CCategoryWnd::LoadState()
{

	if (config != nullptr)
	{
		stateValue = config->GetCatalogCriteria();
		stateTriangleValue = config->GetCatalogOpenTriangle();
		//UpdateElement(true);
	}

}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
CPositionElement * CCategoryWnd::VerifChildParentCheckBox(const int &numParent, bool & isAllUncheck, const bool &checkOn)
{
	CPositionElement * parent = nullptr;
	isAllUncheck = true;

	for (CPositionElement * value : vectorPosElementDynamic)
	{
		if (value != nullptr)
		{
			if (value->GetType() == ELEMENT_CHECKBOX)
			{
				CTreeDataCategory * treeData = (CTreeDataCategory *)value->GetTreeData();
				if (treeData->GetNumElement() == numParent)
				{
					parent = value;
				}
				else if (treeData->GetNumParent() == numParent)
				{
					CTreeElementCheckBox * checkBox = (CTreeElementCheckBox *)value->GetTreeElement();
					if (checkBox->GetCheckState() == !checkOn)
					{
						isAllUncheck = false;
						return parent;
					}
				}
			}
		}
	}
	return parent;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CCategoryWnd::VerifParentCheckBox(CTreeDataCategory * treeData, const bool &checkOn)
{
	bool isAllUnchecked = false;
	int numParent = treeData->GetNumParent();
    CPositionElement * parent = nullptr;
	do
	{
		parent = VerifChildParentCheckBox(numParent, isAllUnchecked, checkOn);
		if (isAllUnchecked && parent != nullptr)
		{
			CTreeDataCategory * treeData = (CTreeDataCategory *)parent->GetTreeData();
			if (treeData != nullptr)
			{
				if (treeData->GetIdElement() != -1)
				{

					if (treeData->GetNumElement() == numParent)
					{
						CTreeElementCheckBox * checkBox = (CTreeElementCheckBox *)parent->GetTreeElement();
						checkBox->SetCheckState(checkOn);
						numParent = treeData->GetNumParent();
					}
				}
				else
				{
                    for (CPositionElement * value : vectorPosElementDynamic)
					{
						if (value != nullptr)
						{
							if (value->GetType() == ELEMENT_CHECKBOX)
							{
								CTreeDataCategory * treeData = (CTreeDataCategory *)value->GetTreeData();
								if (treeData->GetNumElement() == numParent)
								{
									CTreeElementCheckBox * checkBox = (CTreeElementCheckBox *)value->GetTreeElement();
									if (checkBox != nullptr)
									{
										checkBox->SetCheckState(checkOn);
										numParent = treeData->GetNumParent();
									}
									break;
								}
							}
						}
					}
				}
			}
		}

	} while (isAllUnchecked && numParent > 0);
    
    if(checkOn)
    {
        if(parent != nullptr)
        {
            CTreeElementCheckBox * checkBox = (CTreeElementCheckBox *)parent->GetTreeElement();
            if(!checkBox->GetCheckState())
            {
                checkBox->SetCheckState(checkOn);
            }
        }
    }
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CCategoryWnd::GestionCheckBoxParent(const int &numParent, const bool &check)
{
	for (CPositionElement * value : vectorPosElementDynamic)
	{
		if (value != nullptr)
		{
			if (value->GetType() == ELEMENT_CHECKBOX)
			{
				CTreeDataCategory * treeData = (CTreeDataCategory *)value->GetTreeData();
				if (treeData->GetNumParent() == numParent)
				{
					CTreeElementCheckBox * checkBox = (CTreeElementCheckBox *)value->GetTreeElement();
					checkBox->SetCheckState(check);
					GestionCheckBoxParent(treeData->GetNumElement(), check);
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CCategoryWnd::GestionCheckBox(CTreeDataCategory * treeDataSelect, CTreeElementCheckBox * checkBoxSelect)
{
	bool state = checkBoxSelect->GetCheckState();
	wxString exifKey = treeDataSelect->GetExifKey();
	//int numCategorie = treeDataSelect->GetNumCategorie();
	//int numParent = treeDataSelect->GetNumParent();
	bool isParent = treeDataSelect->IsParent();

	if (isParent)
	{
        for (CPositionElement * value : vectorPosElementDynamic)
		{
			if (value != nullptr)
			{
				if (value->GetType() == ELEMENT_CHECKBOX)
				{
					CTreeElementCheckBox * checkBox = (CTreeElementCheckBox *)value->GetTreeElement();
					checkBox->SetCheckState(state);
				}
			}
		}
	}
	else
	{
		GestionCheckBoxParent(treeDataSelect->GetNumElement(), state);
	}
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void  CCategoryWnd::ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	//CTreeElement * treeElement = element->GetTreeElement();

	if (element->GetType() == ELEMENT_CHECKBOX)
	{
		CTreeDataCategory * treeData = (CTreeDataCategory *)element->GetTreeData();
		CTreeElementCheckBox * checkBox = (CTreeElementCheckBox *)element->GetTreeElement();

		checkBox->SetCheckState(!checkBox->GetCheckState());

		GestionCheckBox(treeData, checkBox);
		if (!checkBox->GetCheckState())
			VerifParentCheckBox(treeData,false);
		else
			VerifParentCheckBox(treeData,true);
		
		SaveState();

		UpdateSQLSearchCriteria();

		//UpdateElement();
		eventControl->UpdateTreeControl();
	}
	else if (element->GetType() == ELEMENT_TRIANGLE)
	{
		CTreeElementTriangle * triangle = (CTreeElementTriangle *)element->GetTreeElement();
		triangle->ClickElement(window, x, y);

		SaveStateTriangle();

		UpdateElement();
		eventControl->UpdateTreeControl();
	}
}



void CCategoryWnd::RefreshCriteriaSearch()
{
    SaveState();
    
    UpdateSQLSearchCriteria();
    
    //UpdateElement();
    eventControl->UpdateTreeControl();
}

void CCategoryWnd::UpdateScreenRatio()
{
    printf("CCategoryWnd::UpdateScreenRatio() \n");
    UpdateElement();
    eventControl->UpdateTreeControl();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool CCategoryWnd::GetTriangleState(const wxString &exifKey, const wxString &key)
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
bool CCategoryWnd::GetCheckState(const wxString &exifKey, const wxString &key)
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
	return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

void CCategoryWnd::CreateChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	CPositionElement *  posElement = nullptr;
	CTreeElementTexte * treeElementTexte = nullptr;
	//CTreeElementDelete * treeElementDelete = nullptr;
	CTreeElementCheckBox * treeElementCheck = nullptr;
	CTreeElementTriangle * treeElementTriangle = nullptr;

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		int widthElement = 0;
		int profondeur = tr.depth(it);
		CTreeDataCategory * data = (CTreeDataCategory*)*it;
		if (profondeur == 1 && (data->GetValue().size() > 0 || it.number_of_children() == 0))
		{
			int xPos = widthPosition * (profondeur + 1);
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;
		}
		else if (data->GetValue().size() > 0 || it.number_of_children() == 0)
		{

			int xPos = widthPosition * (profondeur + 1);
			bool check = GetCheckState(data->GetExifKey(), data->GetKey());
			treeElementCheck = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementCheck->GetWidth(), treeElementCheck->GetHeight(), ELEMENT_CHECKBOX, treeElementCheck, data);
			xPos += treeElementCheck->GetWidth() + themeTree.GetMargeX();
			/*
			if (data->GetTypeElement() == TYPEFOLDER)
			{
				treeElementDelete = CreateDeleteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight());
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementDelete->GetWidth(), treeElementDelete->GetHeight(), ELEMENT_DELETE, treeElementDelete, data);
				xPos += treeElementDelete->GetWidth() + themeTree.GetMargeX();
			}
			*/
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;
		}
		else
		{

			int xPos = widthPosition *profondeur;
			bool isOpen = GetTriangleState(data->GetExifKey(), data->GetKey());
			treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), isOpen);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
			xPos += treeElementTriangle->GetWidth() + themeTree.GetMargeX();
			bool check = GetCheckState(data->GetExifKey(), data->GetKey());
			treeElementCheck = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementCheck->GetWidth(), treeElementCheck->GetHeight(), ELEMENT_CHECKBOX, treeElementCheck, data);

			xPos += treeElementCheck->GetWidth() + themeTree.GetMargeX();



			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			CreateChildTree(it);
		}
		it++;

	}
}


void CCategoryWnd::CreateElement()
{
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
			CTreeElementCheckBox * treeElementCheck = nullptr;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CPositionElement * posElement = nullptr;
			bool isOpen = GetTriangleState(data->GetExifKey(), data->GetKey());
			treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), isOpen);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);

			widthPosition = xPos + posElement->GetWidth();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			bool check = GetCheckState(data->GetExifKey(), data->GetKey());
			treeElementCheck = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementCheck->GetWidth(), treeElementCheck->GetHeight(), ELEMENT_CHECKBOX, treeElementCheck, data);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

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



void CCategoryWnd::UpdateElement(const bool &init)
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
			CTreeElementCheckBox * treeElementCheck = nullptr;
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

			posElement = GetElement(data, ELEMENT_CHECKBOX);

			if (posElement == nullptr)
			{
				bool check = GetCheckState(data->GetExifKey(), data->GetKey());
				treeElementCheck = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
				treeElementCheck->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementCheck->GetWidth(), treeElementCheck->GetHeight(), ELEMENT_CHECKBOX, treeElementCheck, data);
			}
			else
			{
				treeElementCheck = (CTreeElementCheckBox *)posElement->GetTreeElement();
				treeElementCheck->SetVisible(isVisible);
                treeElementCheck->SetElementPos(xPos,yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}


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

void CCategoryWnd::UpdateChildTree(tree<CTreeData *>::sibling_iterator &parent, const bool &init)
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
			int xPos = widthPosition * (profondeur + 1);
			CTreeElementTexte * treeElementTexte = nullptr;
			CPositionElement *  posElement = GetElement(data, ELEMENT_TEXTE);

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
			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;
		}
		else if (data->GetValue().size() > 0 || it.number_of_children() == 0)
		{
			//CTreeElementDelete * treeElementDelete = nullptr;
			CTreeElementCheckBox * treeElementCheck = nullptr;
			int xPos = widthPosition * (profondeur + 1);
			CTreeElementTexte * treeElementTexte = nullptr;

			CPositionElement * posElement = GetElement(data, ELEMENT_CHECKBOX);
			if (posElement == nullptr)
			{
				bool check = GetCheckState(data->GetExifKey(), data->GetKey());
				treeElementCheck = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
				treeElementCheck->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementCheck->GetWidth(), treeElementCheck->GetHeight(), ELEMENT_CHECKBOX, treeElementCheck, data);
			}
			else
			{
				treeElementCheck = (CTreeElementCheckBox *)posElement->GetTreeElement();
				treeElementCheck->SetVisible(isVisible);
                treeElementCheck->SetElementPos(xPos,yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			xPos += treeElementCheck->GetWidth() + themeTree.GetMargeX();

			/*
			if (data->GetTypeElement() == TYPEFOLDER)
			{
				posElement = GetElement(data, ELEMENT_DELETE);
				if (posElement == nullptr)
				{
					treeElementDelete = CreateDeleteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight());
					treeElementDelete->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementDelete->GetWidth(), treeElementDelete->GetHeight(), ELEMENT_DELETE, treeElementDelete, data);
				}
				else
				{
					treeElementDelete = (CTreeElementDelete *)posElement->GetTreeElement();
					treeElementDelete->SetVisible(isVisible);
					posElement->SetX(xPos);
					posElement->SetY(yPos);
				}

				xPos += treeElementDelete->GetWidth() + themeTree.GetMargeX();
			}
			*/
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

			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;
		}
		else
		{
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementCheckBox * treeElementCheck = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;

			int xPos = widthPosition *profondeur;

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

			xPos += treeElementTriangle->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_CHECKBOX);
			if (posElement == nullptr)
			{
				bool check = GetCheckState(data->GetExifKey(), data->GetKey());
				treeElementCheck = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
				treeElementCheck->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementCheck->GetWidth(), treeElementCheck->GetHeight(), ELEMENT_CHECKBOX, treeElementCheck, data);
			}
			else
			{
				treeElementCheck = (CTreeElementCheckBox *)posElement->GetTreeElement();
				treeElementCheck->SetVisible(isVisible);
                treeElementCheck->SetElementPos(xPos,yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			xPos += treeElementCheck->GetWidth() + themeTree.GetMargeX();

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

			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			bool isShow = treeElementTriangle->GetOpen();
			if (isShow)
				UpdateChildTree(it, init);
		}
		it++;

	}
}