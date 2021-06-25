#include <header.h>
#include "CategoryWnd.h"
#include <SqlFindFacePhoto.h>
#include <Criteria.h>
#include <SqlPhotoCategorie.h>
#include <SqlFindCriteria.h>
#include <SqlFindPhotos.h>
#include <SqlCatalog.h>
#include "TreeDataCategory.h"
#include <TreeElementTexte.h>
#include <TreeElementTriangle.h>
#include <TreeElementCheckBox.h>
#include <SqlFindFolderCatalog.h>
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <LibResource.h>
#include <ConvertUtility.h>
#include <ThumbnailData.h>
#include <SqlFindCatalog.h>
#include <theme.h>
#include <TreeWindow.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
using namespace Regards::Sqlite;
using namespace Regards::Viewer;
using namespace Regards::Window;

CCategoryWnd::CCategoryWnd(CWindowMain* windowMain, CThemeTree* theme, CTreeElementControlInterface* interfaceControl):
	numCatalog(0),
	idElement(0)
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
	MonthName = CConvertUtility::split(CLibResource::LoadStringFromResource(L"LBLMONTHNAME", 1), ',');
	config = CMainParamInit::getInstance();
	this->windowMain = windowMain;
}

void CCategoryWnd::RefreshListPhoto()
{
	UpdateSQLSearchCriteria();
}

CCategoryWnd::~CCategoryWnd()
{
}

tree<CTreeData*>::iterator CCategoryWnd::FindExifKey(const wxString& exifkey)
{
	auto it = tr.begin();
	auto itend = tr.end();

	//int i = 

	while (it != itend)
	{
		CTreeData* data = *it;
		if (data->GetExifKey() == exifkey)
			return it;
		++it;
	}

	return itend;
}


tree<CTreeData*>::iterator CCategoryWnd::FindFolderKey(const wxString& path)
{
	auto it = tr.begin();
	auto itend = tr.end();

	//int i = 

	while (it != itend)
	{
		CTreeData* data = *it;
		if (data->GetKey() == path)
			return it;
		++it;
	}

	return itend;
}

void CCategoryWnd::Init()
{
	CSqlFindCatalog sqlCatalog;
	CatalogVector catalogVector;
	tree<CTreeData*>::iterator top;
	tree<CTreeData*>::iterator child;

	LoadState();

	top = tr.begin();

	idElement = 0;

	sqlCatalog.GetCatalog(&catalogVector);

	for (CPhotoCatalog catalog : catalogVector)
	{
		auto treeData = new CTreeDataCategory();
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
wxString CCategoryWnd::GetCatalogLibelle(const int& numCatalog)
{
	CSqlCatalog sqlCatalog;
	return sqlCatalog.GetCatalogLibelle(numCatalog);
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CCategoryWnd::InitCatalogCategorie(tree<CTreeData*>::iterator parent, int numParent)
{
	int faceDetection = 0;
	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		faceDetection = regardsParam->GetFaceDetection();
	}

	//Récupération des catégories principales
	CSqlPhotoCategorie sqlCategorie;
	PhotoCategorieVector categorieVector;
	tree<CTreeData*>::iterator child;
	sqlCategorie.LoadPhotoCategorie(&categorieVector, 1);

	for (auto fit = categorieVector.begin(); fit != categorieVector.end(); ++fit)
	{
		//if (fit->GetId() != 2)
		//{

		if (fit->GetId() == 4 && !faceDetection)
			continue;

		idElement++;
		auto treeData = new CTreeDataCategory();
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
		switch (fit->GetId())
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

void CCategoryWnd::LoadFace(const int& numCatalog, tree<CTreeData*>::iterator parent, int numParent)
{
	CSqlFindFacePhoto sqlFindFace;
	std::vector<CFaceName> listFace = sqlFindFace.GetListFaceNameSelectable();
	tree<CTreeData*>::iterator child;
	for (auto fit = listFace.begin(); fit != listFace.end(); ++fit)
	{
		idElement++;
		auto treeData = new CTreeDataCategory();
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
void CCategoryWnd::LoadFolder(const int& numCatalog, tree<CTreeData*>::iterator parent, int numParent)
{
	CSqlFindFolderCatalog sqlFolderCatalog;
	FolderCatalogVector folderCatalogVector;
	sqlFolderCatalog.GetFolderCatalog(&folderCatalogVector, numCatalog);
	tree<CTreeData*>::iterator child;

	for (auto fit = folderCatalogVector.begin(); fit != folderCatalogVector.end(); ++fit)
	{
		idElement++;
		auto treeData = new CTreeDataCategory();
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
tree<CTreeData*>::iterator CCategoryWnd::FindChild(tree<CTreeData*>::iterator parent, const wxString& catlibelle)
{
	tree<CTreeData*>::sibling_iterator itOld;
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	itOld = it;

	//int i = 

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		CTreeData* data = *it;
		if (data->GetExifKey() > catlibelle)
		{
			return itOld;
		}
		itOld = it;
		++it;
	}

	return nullptr;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CCategoryWnd::AddCategorie(const int& numCatalog, const int& numCategorie, tree<CTreeData*>::iterator parent,
                                const int& numParent, const wxString& catlibelle, const int& id)
{
	//int item = 0;
	//int index = 0;
	wchar_t seps[] = L".";

	int level = 2;
	wxString exifKey = L"";
	wchar_t libelle[1024];
	wchar_t* next_token1 = nullptr;
	auto child = parent;
	wcscpy(libelle, catlibelle.c_str());
	int numParentElement = numParent;
#if defined(WIN32) && _MSC_VER < 1900
	wchar_t* token = wcstok(libelle, seps); // C4996
#else
	// Establish string and get the first token:
	wchar_t* token = wcstok(libelle, seps, &next_token1); // C4996
#endif

	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		idElement++;

		if (exifKey != L"")
			exifKey.append(L".");
		exifKey.append(token);

		auto treeData = new CTreeDataCategory();
		treeData->SetIsParent(false);
		if (numCategorie == 3 && level == 3)
		{
			wchar_t* pEnd;
			const long numMonth = wcstol(token, &pEnd, 10);
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
		auto it = FindKey(treeData->GetKey(), child);
		if (it != nullptr)
		{
			child = it;
			auto data = static_cast<CTreeDataCategory*>(*it);
			treeData->SetNumParent(data->GetNumParent());
			numParentElement = data->GetNumElement();
			delete(treeData);
		}
		else
		{
			numParentElement = idElement;
			treeData->SetExifKey(exifKey);
			auto tree_datas = FindChild(child, treeData->GetExifKey());
			if (tree_datas == nullptr)
				child = tr.append_child(child, treeData);
			else
				child = tr.insert(tree_datas, treeData);
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
void CCategoryWnd::LoadCategorie(const int& numCategorie, tree<CTreeData*>::iterator parent, int numParent)
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
		wchar_t* next_token1 = nullptr;
		auto child = parent;
		if (criteria.GetLibelle() != L"")
		{
			libelle = criteria.GetLibelle();

			int numParentElement = numParent;
			//int index = 0;
			// Establish string and get the first token:
#if defined(WIN32) && _MSC_VER < 1900
			wchar_t* token = wcstok((wchar_t*)libelle.wc_str(), seps); // C4996
#else
			wchar_t* token = wcstok((wchar_t*)libelle.wc_str(), seps, &next_token1); // C4996
#endif

			// Note: strtok is deprecated; consider using strtok_s instead
			do
			{
				idElement++;

				if (exifKey != L"")
					exifKey.append(L".");
				exifKey.append(token);

				auto treeData = new CTreeDataCategory();
				treeData->SetIsParent(false);

				if (numCategorie == 3 && level == 3)
				{
					wchar_t* pEnd;
					long numMonth = wcstol(token, &pEnd, 10);
					if (numMonth > 0 && numMonth <= 12)
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
				auto it = FindKey(treeData->GetKey(), child);
				if (it != nullptr)
				{
					child = it;
					auto data = static_cast<CTreeDataCategory*>(*it);
					treeData->SetNumParent(data->GetNumParent());
					numParentElement = data->GetNumElement();
				}
				else
				{
					numParentElement = idElement;
					treeData->SetExifKey(exifKey);

					//Ordonnancement par Alphabet
					//child = tr.append_child(child, treeData);
					auto tree_datas = FindChild(child, treeData->GetExifKey());
					if (tree_datas == nullptr)
						child = tr.append_child(child, treeData);
					else
						child = tr.insert(tree_datas, treeData);

					treeData->child = child;
				}

#if defined(WIN32) &&  _MSC_VER < 1900
				token = wcstok(nullptr, seps); // C4996
#else
				token = wcstok(nullptr, seps, &next_token1); // C4996
#endif
				//index++;
			}
			while (token != nullptr);
		}
	}
}


wxString CCategoryWnd::GetSqlRequest()
{
	auto viewerParam = CMainParamInit::getInstance();
	vector<int> listFolder;
	vector<int> listFace;
	vector<int> listFolderNotSelected;
	vector<int> listFaceNotSelected;
	vector<int> listFaceSelected;
	vector<int> listCriteriaNotIn;
	vector<int> listStarSelected;
	vector<int> listStarNotSelected;
	vector<int> listKeywordSelected;
	vector<int> listKeywordNotSelected;
	double pertinence = 0.0;

	for (CPositionElement* value : vectorPosElementDynamic)
	{
		if (value->GetType() == ELEMENT_CHECKBOX)
		{
			auto checkBox = static_cast<CTreeElementCheckBox*>(value->GetTreeElement());
			auto treeData = static_cast<CTreeDataCategory*>(value->GetTreeData());

			if (!checkBox->GetCheckState())
			{
				if ((treeData->GetNumCategorie() != 2 && treeData->GetNumCategorie() != 4 && treeData->GetNumCategorie()
					!= 6 && treeData->GetNumCategorie() != 7) && treeData->GetIdElement() != -1)
					listCriteriaNotIn.push_back(treeData->GetIdElement());
				else if (treeData->GetNumCategorie() == 2)
					listFolderNotSelected.push_back(treeData->GetIdElement());
				else if (treeData->GetNumCategorie() == 4)
					listFaceNotSelected.push_back(treeData->GetIdElement());
				else if (treeData->GetNumCategorie() == 6)
					listStarNotSelected.push_back(treeData->GetIdElement());
				else if (treeData->GetNumCategorie() == 7)
					listKeywordNotSelected.push_back(treeData->GetIdElement());
			}
			else if (treeData->GetIdElement() != -1)
			{
				if (treeData->GetNumCategorie() == 4)
					listFaceSelected.push_back(treeData->GetIdElement());
				else if (treeData->GetNumCategorie() == 6)
					listStarSelected.push_back(treeData->GetIdElement());
				else if (treeData->GetNumCategorie() == 7)
					listKeywordSelected.push_back(treeData->GetIdElement());
			}
		}
	}

	FolderCatalogVector folderList;
	CSqlFindFolderCatalog folderCatalog;
	folderCatalog.GetFolderCatalog(&folderList, NUMCATALOGID);

	for (CFolderCatalog folder_catalog : folderList)
	{
		std::vector<int>::iterator it;
		it = find(listFolderNotSelected.begin(), listFolderNotSelected.end(), folder_catalog.GetNumFolder());
		if (it == listFolderNotSelected.end())
			listFolder.push_back(folder_catalog.GetNumFolder());
	}

	if (viewerParam != nullptr)
		pertinence = viewerParam->GetPertinenceValue();

	wxString libelleNotGeo = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
	CSqlFindPhotos sqlFindPhotos;
	sqlRequest = sqlFindPhotos.GenerateSqlRequest(NUMCATALOGID, listFolder, listCriteriaNotIn, listFaceNotSelected,
	                                              listFaceSelected, listStarSelected, listStarNotSelected,
	                                              listKeywordSelected, listKeywordNotSelected, libelleNotGeo,
	                                              pertinence);

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

	auto viewerParam = CMainParamInit::getInstance();
	viewerParam->SetLastSqlRequest(sqlRequest);

	if (windowMain != nullptr)
	{
		const wxCommandEvent evt(wxEVENT_REFRESHFOLDERLIST);
		windowMain->GetEventHandler()->AddPendingEvent(evt);
	}
}


void CCategoryWnd::SaveStateTriangle()
{
	int nbElement = 0;
	stateTriangleValue = L"";

	for (CPositionElement* value : vectorPosElementDynamic)
	{
		if (value != nullptr)
		{
			if (value->GetType() == ELEMENT_TRIANGLE)
			{
				//bool isOpen = true;
				CTreeData* treeData = value->GetTreeData();
				auto triangle = static_cast<CTreeElementTriangle*>(value->GetTreeElement());
				if (triangle != nullptr && treeData != nullptr)
				{
					const bool isOpen = triangle->GetOpen();
					wxString exifKey = treeData->GetExifKey();
					wxString key = treeData->GetKey();
					stateTriangleValue.append(exifKey);
					stateTriangleValue.append(L":");
					stateTriangleValue.append(key);
					if (isOpen)
						stateTriangleValue.append(L":1");
					else
						stateTriangleValue.append(L":0");

					stateTriangleValue.append(L";");
					nbElement++;
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

	for (CPositionElement* value : vectorPosElementDynamic)
	{
		if (value != nullptr)
		{
			if (value->GetType() == ELEMENT_CHECKBOX)
			{
				//bool checkState = false;
				CTreeData* treeData = value->GetTreeData();
				auto checkBox = dynamic_cast<CTreeElementCheckBox*>(value->GetTreeElement());
				if (checkBox != nullptr && treeData != nullptr)
				{
					const bool checkState = checkBox->GetCheckState();
					wxString exifKey = treeData->GetExifKey();
					wxString key = treeData->GetKey();
					stateValue.append(exifKey);
					stateValue.append(L":");
					stateValue.append(key);
					if (checkState)
						stateValue.append(L":1");
					else
						stateValue.append(L":0");

					stateValue.append(L";");
					nbElement++;
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
CPositionElement* CCategoryWnd::VerifChildParentCheckBox(const int& numParent, bool& isAllUncheck, const bool& checkOn)
{
	CPositionElement* parent = nullptr;
	isAllUncheck = true;

	for (CPositionElement* value : vectorPosElementDynamic)
	{
		if (value != nullptr)
		{
			if (value->GetType() == ELEMENT_CHECKBOX)
			{
				auto treeData = static_cast<CTreeDataCategory*>(value->GetTreeData());
				if (treeData->GetNumElement() == numParent)
				{
					parent = value;
				}
				else if (treeData->GetNumParent() == numParent)
				{
					auto checkBox = dynamic_cast<CTreeElementCheckBox*>(value->GetTreeElement());
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
void CCategoryWnd::VerifParentCheckBox(CTreeDataCategory* treeData, const bool& checkOn)
{
	bool isAllUnchecked = false;
	int numParent = treeData->GetNumParent();
	CPositionElement* parent;
	do
	{
		parent = VerifChildParentCheckBox(numParent, isAllUnchecked, checkOn);
		if (isAllUnchecked && parent != nullptr)
		{
			auto tree_data_category = static_cast<CTreeDataCategory*>(parent->GetTreeData());
			if (tree_data_category != nullptr)
			{
				if (tree_data_category->GetIdElement() != -1)
				{
					if (tree_data_category->GetNumElement() == numParent)
					{
						auto checkBox = static_cast<CTreeElementCheckBox*>(parent->GetTreeElement());
						checkBox->SetCheckState(checkOn);
						numParent = tree_data_category->GetNumParent();
					}
				}
				else
				{
					for (CPositionElement* value : vectorPosElementDynamic)
					{
						if (value != nullptr)
						{
							if (value->GetType() == ELEMENT_CHECKBOX)
							{
								auto data_category = static_cast<CTreeDataCategory*>(value->GetTreeData());
								if (data_category->GetNumElement() == numParent)
								{
									auto checkBox = dynamic_cast<CTreeElementCheckBox*>(value->GetTreeElement());
									if (checkBox != nullptr)
									{
										checkBox->SetCheckState(checkOn);
										numParent = data_category->GetNumParent();
									}
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	while (isAllUnchecked && numParent > 0);

	if (checkOn)
	{
		if (parent != nullptr)
		{
			auto checkBox = dynamic_cast<CTreeElementCheckBox*>(parent->GetTreeElement());
			if (!checkBox->GetCheckState())
			{
				checkBox->SetCheckState(checkOn);
			}
		}
	}
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CCategoryWnd::GestionCheckBoxParent(const int& numParent, const bool& check)
{
	for (CPositionElement* value : vectorPosElementDynamic)
	{
		if (value != nullptr)
		{
			if (value->GetType() == ELEMENT_CHECKBOX)
			{
				auto treeData = static_cast<CTreeDataCategory*>(value->GetTreeData());
				if (treeData->GetNumParent() == numParent)
				{
					auto checkBox = dynamic_cast<CTreeElementCheckBox*>(value->GetTreeElement());
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
void CCategoryWnd::GestionCheckBox(CTreeDataCategory* treeDataSelect, CTreeElementCheckBox* checkBoxSelect)
{
	const bool state = checkBoxSelect->GetCheckState();
	wxString exif_key = treeDataSelect->GetExifKey();
	const bool isParent = treeDataSelect->IsParent();

	if (isParent)
	{
		for (CPositionElement* value : vectorPosElementDynamic)
		{
			if (value != nullptr)
			{
				if (value->GetType() == ELEMENT_CHECKBOX)
				{
					auto checkBox = static_cast<CTreeElementCheckBox*>(value->GetTreeElement());
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
void CCategoryWnd::ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
                                  const int& posLargeur, const int& posHauteur)
{
	//CTreeElement * treeElement = element->GetTreeElement();

	if (element->GetType() == ELEMENT_CHECKBOX)
	{
		auto treeData = static_cast<CTreeDataCategory*>(element->GetTreeData());
		auto checkBox = dynamic_cast<CTreeElementCheckBox*>(element->GetTreeElement());

		checkBox->SetCheckState(!checkBox->GetCheckState());

		GestionCheckBox(treeData, checkBox);
		if (!checkBox->GetCheckState())
			VerifParentCheckBox(treeData, false);
		else
			VerifParentCheckBox(treeData, true);

		SaveState();

		UpdateSQLSearchCriteria();

		//UpdateElement();
		eventControl->UpdateTreeControl();
	}
	else if (element->GetType() == ELEMENT_TRIANGLE)
	{
		auto triangle = dynamic_cast<CTreeElementTriangle*>(element->GetTreeElement());
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
bool CCategoryWnd::GetTriangleState(const wxString& exifKey, const wxString& key)
{
	const wxString localkey = exifKey + L":" + key + L":";
	size_t pos = stateTriangleValue.find(localkey);
	if (pos < stateTriangleValue.size())
	{
		pos += localkey.size();
		const wchar_t check = stateTriangleValue[pos];
		if (check == '1')
			return true;
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool CCategoryWnd::GetCheckState(const wxString& exifKey, const wxString& key)
{
	const wxString localkey = exifKey + L":" + key + L":";
	size_t pos = stateValue.find(localkey);
	if (pos < stateValue.size())
	{
		pos += localkey.size();
		const wchar_t check = stateValue[pos];
		if (check == '1')
			return true;
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

void CCategoryWnd::CreateChildTree(tree<CTreeData*>::sibling_iterator& parent)
{
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);
	CPositionElement* pos_element;
	CTreeElementTexte* tree_element_texte;
	CTreeElementCheckBox* tree_element_check;

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		int width_element;
		const int profondeur = tr.depth(it);
		auto data = static_cast<CTreeDataCategory*>(*it);
		if (profondeur == 1 && (!data->GetValue().empty() || it.number_of_children() == 0))
		{
			int xPos = widthPosition * (profondeur + 1);
			tree_element_texte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			pos_element = CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_texte->GetWidth(),
			                                    tree_element_texte->GetHeight(), ELEMENT_TEXTE, tree_element_texte,
			                                    data,
			                                    false);

			width_element = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < width_element)
				rowWidth[0] = width_element;
		}
		else if (!data->GetValue().empty() || it.number_of_children() == 0)
		{
			int xPos = widthPosition * (profondeur + 1);
			bool check = GetCheckState(data->GetExifKey(), data->GetKey());
			tree_element_check = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
			CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_check->GetWidth(),
			                      tree_element_check->GetHeight(), ELEMENT_CHECKBOX, tree_element_check, data);
			xPos += tree_element_check->GetWidth() + themeTree.GetMargeX();
			tree_element_texte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			pos_element = CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_texte->GetWidth(),
			                                    tree_element_texte->GetHeight(), ELEMENT_TEXTE, tree_element_texte,
			                                    data,
			                                    false);

			width_element = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < width_element)
				rowWidth[0] = width_element;
		}
		else
		{
			int xPos = widthPosition * profondeur;
			bool isOpen = GetTriangleState(data->GetExifKey(), data->GetKey());
			CTreeElementTriangle* treeElementTriangle = CreateTriangleElement(
				themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				isOpen);
			CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(),
			                      treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle,
			                      data);
			xPos += treeElementTriangle->GetWidth() + themeTree.GetMargeX();
			bool check = GetCheckState(data->GetExifKey(), data->GetKey());
			tree_element_check = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
			CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_check->GetWidth(),
			                      tree_element_check->GetHeight(), ELEMENT_CHECKBOX, tree_element_check, data);

			xPos += tree_element_check->GetWidth() + themeTree.GetMargeX();


			tree_element_texte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			pos_element = CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_texte->GetWidth(),
			                                    tree_element_texte->GetHeight(), ELEMENT_TEXTE, tree_element_texte,
			                                    data,
			                                    false);

			width_element = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < width_element)
				rowWidth[0] = width_element;

			CreateChildTree(it);
		}
		++it;
	}
}


void CCategoryWnd::CreateElement()
{
	vectorPosElement.clear();
	vectorPosElementDynamic.clear();
	tree<CTreeData*>::sibling_iterator it = tr.begin();
	const auto itend = tr.end();
	yPos = 0;
	nbRow = 0;
	widthPosition = 0;

	while (it != itend)
	{
		auto data = static_cast<CTreeDataCategory*>(*it);
		const int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			bool isOpen = GetTriangleState(data->GetExifKey(), data->GetKey());
			CTreeElementTriangle* treeElementTriangle = CreateTriangleElement(
				themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				isOpen);
			CPositionElement* posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(),
			                                                     treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE,
			                                                     treeElementTriangle,
			                                                     data);

			widthPosition = xPos + posElement->GetWidth();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			bool check = GetCheckState(data->GetExifKey(), data->GetKey());
			CTreeElementCheckBox* treeElementCheck = CreateCheckBoxElement(
				themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				check);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementCheck->GetWidth(),
			                                   treeElementCheck->GetHeight(), ELEMENT_CHECKBOX, treeElementCheck, data);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			CTreeElementTexte* treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                                         data->GetKey());
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
			                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data,
			                                   false);

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			CreateChildTree(it);
		}
		++it;
	}
}


void CCategoryWnd::UpdateElement(const bool& init)
{
	tree<CTreeData*>::sibling_iterator it = tr.begin();
	const auto itend = tr.end();
	yPos = 0;
	nbRow = 0;
	widthPosition = 0;

	for (CPositionElement* value : vectorPosElement)
	{
		if (value != nullptr)
		{
			CTreeElement* treeElement = value->GetTreeElement();
			treeElement->SetVisible(false);
		}
	}

	while (it != itend)
	{
		auto data = static_cast<CTreeDataCategory*>(*it);
		const int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			bool isVisible = true;
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			CTreeElementCheckBox* tree_element_check;
			CTreeElementTriangle* tree_element_triangle;
			CPositionElement* posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement == nullptr)
			{
				bool isOpen = GetTriangleState(data->GetExifKey(), data->GetKey());
				tree_element_triangle =
					CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), isOpen);
				tree_element_triangle->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_triangle->GetWidth(),
				                                   tree_element_triangle->GetHeight(), ELEMENT_TRIANGLE,
				                                   tree_element_triangle, data);
			}
			else
			{
				tree_element_triangle = static_cast<CTreeElementTriangle*>(posElement->GetTreeElement());
				tree_element_triangle->SetVisible(isVisible);
				tree_element_triangle->SetElementPos(xPos, yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			widthPosition = xPos + posElement->GetWidth();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_CHECKBOX);

			if (posElement == nullptr)
			{
				bool check = GetCheckState(data->GetExifKey(), data->GetKey());
				tree_element_check = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
				tree_element_check->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_check->GetWidth(),
				                                   tree_element_check->GetHeight(), ELEMENT_CHECKBOX,
				                                   tree_element_check,
				                                   data);
			}
			else
			{
				tree_element_check = dynamic_cast<CTreeElementCheckBox*>(posElement->GetTreeElement());
				tree_element_check->SetVisible(isVisible);
				tree_element_check->SetElementPos(xPos, yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}


			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement == nullptr)
			{
				auto treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				                                           data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
				                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data,
				                                   false);
			}
			else
			{
				auto treeElementTexte = dynamic_cast<CTreeElementTexte*>(posElement->GetTreeElement());
				treeElementTexte->SetVisible(isVisible);
				treeElementTexte->SetElementPos(xPos, yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			const bool isOpen = tree_element_triangle->GetOpen();
			if (isOpen)
				UpdateChildTree(it, init);
		}
		++it;
	}
}

void CCategoryWnd::UpdateChildTree(tree<CTreeData*>::sibling_iterator& parent, const bool& init)
{
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		bool isVisible = true;
		int width_element;
		const int profondeur = tr.depth(it);
		auto data = static_cast<CTreeDataCategory*>(*it);
		if (profondeur == 1 && (!data->GetValue().empty() || it.number_of_children() == 0))
		{
			int xPos = widthPosition * (profondeur + 1);
			CPositionElement* posElement = GetElement(data, ELEMENT_TEXTE);

			if (posElement == nullptr)
			{
				CTreeElementTexte* treeElementTexte = CreateTexteElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
				                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data,
				                                   false);
			}
			else
			{
				auto treeElementTexte = dynamic_cast<CTreeElementTexte*>(posElement->GetTreeElement());
				treeElementTexte->SetVisible(isVisible);
				treeElementTexte->SetElementPos(xPos, yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			width_element = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < width_element)
				rowWidth[0] = width_element;
		}
		else if (!data->GetValue().empty() || it.number_of_children() == 0)
		{
			//CTreeElementDelete * treeElementDelete = nullptr;
			CTreeElementCheckBox* tree_element_check;
			int xPos = widthPosition * (profondeur + 1);

			CPositionElement* posElement = GetElement(data, ELEMENT_CHECKBOX);
			if (posElement == nullptr)
			{
				bool check = GetCheckState(data->GetExifKey(), data->GetKey());
				tree_element_check = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
				tree_element_check->SetVisible(isVisible);
				CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_check->GetWidth(),
				                      tree_element_check->GetHeight(), ELEMENT_CHECKBOX, tree_element_check,
				                      data);
			}
			else
			{
				tree_element_check = dynamic_cast<CTreeElementCheckBox*>(posElement->GetTreeElement());
				tree_element_check->SetVisible(isVisible);
				tree_element_check->SetElementPos(xPos, yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			xPos += tree_element_check->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement == nullptr)
			{
				CTreeElementTexte* treeElementTexte = CreateTexteElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
				                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data,
				                                   false);
			}
			else
			{
				auto treeElementTexte = dynamic_cast<CTreeElementTexte*>(posElement->GetTreeElement());
				treeElementTexte->SetVisible(isVisible);
				treeElementTexte->SetElementPos(xPos, yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			width_element = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < width_element)
				rowWidth[0] = width_element;
		}
		else
		{
			CTreeElementCheckBox* tree_element_check;
			CTreeElementTriangle* tree_element_triangle;

			int xPos = widthPosition * profondeur;

			CPositionElement* posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement == nullptr)
			{
				bool isOpen = GetTriangleState(data->GetExifKey(), data->GetKey());
				tree_element_triangle =
					CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), isOpen);
				tree_element_triangle->SetVisible(isVisible);
				CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_triangle->GetWidth(),
				                      tree_element_triangle->GetHeight(), ELEMENT_TRIANGLE,
				                      tree_element_triangle, data);
			}
			else
			{
				tree_element_triangle = static_cast<CTreeElementTriangle*>(posElement->GetTreeElement());
				tree_element_triangle->SetVisible(isVisible);
				tree_element_triangle->SetElementPos(xPos, yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			xPos += tree_element_triangle->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_CHECKBOX);
			if (posElement == nullptr)
			{
				bool check = GetCheckState(data->GetExifKey(), data->GetKey());
				tree_element_check = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
				tree_element_check->SetVisible(isVisible);
				CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_check->GetWidth(),
				                      tree_element_check->GetHeight(), ELEMENT_CHECKBOX, tree_element_check,
				                      data);
			}
			else
			{
				tree_element_check = dynamic_cast<CTreeElementCheckBox*>(posElement->GetTreeElement());
				tree_element_check->SetVisible(isVisible);
				tree_element_check->SetElementPos(xPos, yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			xPos += tree_element_check->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement == nullptr)
			{
				CTreeElementTexte* treeElementTexte = CreateTexteElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
				                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data,
				                                   false);
			}
			else
			{
				auto treeElementTexte = dynamic_cast<CTreeElementTexte*>(posElement->GetTreeElement());
				treeElementTexte->SetVisible(isVisible);
				treeElementTexte->SetElementPos(xPos, yPos);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			width_element = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < width_element)
				rowWidth[0] = width_element;

			const bool isShow = tree_element_triangle->GetOpen();
			if (isShow)
				UpdateChildTree(it, init);
		}
		++it;
	}
}
