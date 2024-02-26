#include <header.h>
#include "CriteriaTree.h"
#include <TreeElementTexte.h>
#include <TreeElementTriangle.h>
#include <TreeElementStar.h>
#include "TreeElementTexteClick.h"
#include <SqlPhotos.h>
#include "TreeDataLink.h"
#include "TreeDataStars.h"
#include <PhotoCriteria.h>
#include <wx/filename.h>
#include <libPicture.h>
#include <SqlFindFacePhoto.h>
#include <LibResource.h>
#include <SqlPhotoCategorie.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Control;
using namespace Regards::Window;
using namespace Regards::Sqlite;
using namespace Regards::Picture;
#define TAILLEMAX 4096

PhotoCategorieVector CCriteriaTree::photoCategorieVector;

CCriteriaTree::CCriteriaTree(CThemeTree* theme, CTreeElementControlInterface* interfaceControl): yPos(0)
{
	rotation = 0;
	numPhotoId = 0;
	filename = L"";
	widthPosition = 0;
	rowWidth.push_back(0);
	rowWidth.push_back(0);
	themeTree = *theme;
	themeTree.themeTriangle.SetHeight(themeTree.GetRowHeight());
	themeTree.themeCheckbox.SetHeight(themeTree.GetRowHeight());
	themeTree.themeDelete.SetHeight(themeTree.GetRowHeight());
	themeTree.themeSlide.SetHeight(themeTree.GetRowHeight());
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	eventControl = interfaceControl;
}
;


wxString CCriteriaTree::GetFilename()
{
	return filename;
}

void CCriteriaTree::MouseOver(wxDC* deviceContext, CPositionElement* element, const int& x, const int& y,
                              const int& posLargeur, const int& posHauteur, bool& update)
{
	if (element != nullptr)
	{
		CTreeElement* treeElement = element->GetTreeElement();
		if (treeElement != nullptr)
			treeElement->MouseOver(deviceContext, x, y, update);
	}
}


void CCriteriaTree::UpdateScreenRatio()
{
	//this->UpdateElement();
}

void CCriteriaTree::AddTreeInfos(const wxString& exifKey, const wxString& exifValue, const int& index,
                                 tree<CTreeData*>::iterator& top, tree<CTreeData*>::iterator& child)
{
	wchar_t seps[] = L".";
	int item = 0;
	wchar_t informations[TAILLEMAX];
	wcscpy(informations, exifKey.c_str());
	wchar_t* token1;

	// Establish string and get the first token:
#if defined(WIN32) && _MSC_VER < 1900
	wchar_t * token = wcstok(informations, seps); // C4996
#else
	wchar_t* token = wcstok(informations, seps, &token1); // C4996
#endif

	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		auto treeData = new CTreeData();
		treeData->SetKey(token);
#if defined(WIN32) && _MSC_VER < 1900
	wchar_t * token = wcstok(informations, seps); // C4996
#else
		wchar_t* token2 = wcstok(informations, seps, &token1); // C4996
#endif

		if (token2 != nullptr)
		{
			treeData->SetIsParent(true);

			if (index > 0)
			{
				if (item == 0)
				{
					tree<CTreeData*>::iterator it;
					//Recherche de la clé
					it = FindKey(treeData->GetKey());
					if (it != nullptr)
					{
						child = it;
						item++;
						delete(treeData);
						continue;
					}
				}
				else
				{
					tree<CTreeData*>::iterator it;
					//Recherche de la clé
					it = FindKey(treeData->GetKey(), child);
					if (it != nullptr)
					{
						child = it;
						item++;
						delete(treeData);
						continue;
					}
				}
			}

			if (item > 0)
			{
				child = tr.append_child(child, treeData);
			}
			else
			{
				child = tr.insert(top, treeData);
			}
		}
		else
		{
			treeData->SetIsParent(false);
			treeData->SetValue(exifValue);
			treeData->SetExifKey(exifKey);
			tr.append_child(child, treeData);
		}
		item++;
	}
}

void CCriteriaTree::SetFile(const wxString& picture, const int& numPhotoId)
{
	if (filename == picture)
		return;

	filename = picture;
	this->numPhotoId = numPhotoId;

	top = tr.begin();
	tree<CTreeData*>::iterator top;
	//	int item = 0;
	//	int index = 0;

	top = tr.begin();

	auto child = top;

	if (photoCategorieVector.size() == 0)
	{
		CSqlPhotoCategorie sqlPhotoCategorie;
		sqlPhotoCategorie.LoadPhotoCategorie(&photoCategorieVector, 1);
	}

	auto treeDataPicture = new CTreeData();
	treeDataPicture->SetKey("Criteria");
	child = tr.insert(top, treeDataPicture);

	for (CPhotoCategorie photoCategorie : photoCategorieVector)
	{
		if (photoCategorie.GetId() == 4)
		{
			CSqlFindFacePhoto sqlFindFacePhoto;
			std::vector<CFaceName> listFaceName = sqlFindFacePhoto.GetListFaceName(picture);
			wxString libelleCategorie = photoCategorie.GetLibelle();
			for (int i = 0; i < listFaceName.size(); i++)
			{
				wxString value = listFaceName[i].faceName;
				auto treeDataFileName = new CTreeDataLink();
				treeDataFileName->SetIsParent(false);
				treeDataFileName->SetKey(libelleCategorie);
				treeDataFileName->SetValue(value);
				treeDataFileName->SetType(2);
				treeDataFileName->SetLinkType(0);
				treeDataFileName->SetId(listFaceName[i].numFace);
				treeDataFileName->SetLinkPath("");
				tr.append_child(child, treeDataFileName);
			}
		}
		else if (photoCategorie.GetId() == 2)
		{
			wxString libelleCategorie = photoCategorie.GetLibelle();
			wxFileName dirname = wxFileName::DirName(picture);

			auto treeDataFileName = new CTreeData();
			treeDataFileName->SetIsParent(false);
			treeDataFileName->SetKey(libelleCategorie);
			treeDataFileName->SetValue(dirname.GetPath());
			tr.append_child(child, treeDataFileName);
		}
		else if (photoCategorie.GetId() == 5)
		{
			wxString libelleCategorie = photoCategorie.GetLibelle();
			auto treeDataFileName = new CTreeData();
			treeDataFileName->SetIsParent(false);
			treeDataFileName->SetKey("usenet.category");
			treeDataFileName->SetValue(libelleCategorie);
			tr.append_child(child, treeDataFileName);
		}
		else if (photoCategorie.GetId() == 6)
		{
			//Find Value
			CriteriaVector criteriaVector;
			CSqlPhotos sqlPhotos;
			sqlPhotos.GetPhotoCriteriaByCategorie(&criteriaVector, picture, photoCategorie.GetId());

			wxString libelleCategorie = photoCategorie.GetLibelle();
			auto treeDataFileName = new CTreeDataStars();
			treeDataFileName->SetNumPhotoId(numPhotoId);
			treeDataFileName->SetIsParent(false);
			treeDataFileName->SetKey(libelleCategorie);
			if (criteriaVector.size() == 0)
				treeDataFileName->SetValue("0");
			else
			{
				CCriteria criteria = criteriaVector[0];
				treeDataFileName->SetValue(criteria.GetLibelle());
			}
			treeDataFileName->SetType(6);
			tr.append_child(child, treeDataFileName);
		}
		else
		{
			CriteriaVector criteriaVector;
			CSqlPhotos sqlPhotos;
			sqlPhotos.GetPhotoCriteriaByCategorie(&criteriaVector, picture, photoCategorie.GetId());
			if (criteriaVector.size() == 0)
			{
				wxString libelleCategorie = photoCategorie.GetLibelle();
				wxString value = "";
				if (photoCategorie.GetId() == 7)
				{
					/*
					CTreeDataLink * treeDataFileName = new CTreeDataLink();
					treeDataFileName->SetIsParent(false);
					treeDataFileName->SetKey(libelleCategorie);
					treeDataFileName->SetValue("No Keyword");
					treeDataFileName->SetType(2);
					treeDataFileName->SetLinkType(7);
					treeDataFileName->SetId(photoCategorie.GetId());
					treeDataFileName->SetLinkPath("");
					tr.append_child(child, treeDataFileName);
					*/
				}
				else
				{
					auto treeDataFileName = new CTreeDataLink();
					treeDataFileName->SetIsParent(false);
					treeDataFileName->SetKey(libelleCategorie);
					treeDataFileName->SetValue(value);
					treeDataFileName->SetType(2);
					treeDataFileName->SetLinkType(0);
					treeDataFileName->SetLinkPath("");
					tr.append_child(child, treeDataFileName);
				}
			}
			else
			{
				if (photoCategorie.GetId() == 3)
				{
					CCriteria criteria = criteriaVector.at(0);
					wxString libelleCategorie = photoCategorie.GetLibelle();
					wxString value = criteria.GetLibelle();

					auto treeDataFileName = new CTreeDataLink();
					treeDataFileName->SetIsParent(false);
					treeDataFileName->SetKey(libelleCategorie);
					treeDataFileName->SetValue(value);
					treeDataFileName->SetType(2);
					treeDataFileName->SetLinkType(3);
					treeDataFileName->SetId(criteria.GetId());
					treeDataFileName->SetLinkPath("");
					tr.append_child(child, treeDataFileName);
				}
				else if (photoCategorie.GetId() == 1)
				{
					CCriteria criteria = criteriaVector.at(0);
					wxString libelleCategorie = photoCategorie.GetLibelle();
					wxString value = criteria.GetLibelle();

					auto treeDataFileName = new CTreeDataLink();
					treeDataFileName->SetIsParent(false);
					treeDataFileName->SetKey(libelleCategorie);
					treeDataFileName->SetValue(value);
					treeDataFileName->SetType(2);
					treeDataFileName->SetLinkType(1);
					treeDataFileName->SetLinkPath("");
					treeDataFileName->SetId(criteria.GetId());
					tr.append_child(child, treeDataFileName);
				}
				/*
				else if (photoCategorie.GetId() == 6)
				{
					CCriteria criteria = criteriaVector.at(0);
					wxString libelleCategorie = photoCategorie.GetLibelle();
					wxString value = criteria.GetLibelle();

					CTreeDataStars * treeDataFileName = new CTreeDataStars();
					treeDataFileName->SetNumPhotoId(numPhotoId);
					treeDataFileName->SetIsParent(false);
					treeDataFileName->SetKey(libelleCategorie);
					treeDataFileName->SetValue(value);
					treeDataFileName->SetType(6);
					tr.append_child(child, treeDataFileName);
				}*/
				else if (photoCategorie.GetId() == 7)
				{
					/*
					CCriteria criteria = criteriaVector.at(0);
					wxString libelleCategorie = photoCategorie.GetLibelle();
					wxString value = criteria.GetLibelle();

					CTreeDataLink * treeDataFileName = new CTreeDataLink();
					treeDataFileName->SetIsParent(false);
					treeDataFileName->SetKey(libelleCategorie);
					treeDataFileName->SetValue(value);
					treeDataFileName->SetType(2);
					treeDataFileName->SetLinkType(7);
					treeDataFileName->SetId(criteria.GetId());
					treeDataFileName->SetLinkPath("");
					tr.append_child(child, treeDataFileName);
					*/
				}
				else
				{
					for (CCriteria criteria : criteriaVector)
					{
						wxString libelleCategorie = photoCategorie.GetLibelle();
						wxString value = criteria.GetLibelle();

						auto treeDataFileName = new CTreeDataLink();
						treeDataFileName->SetIsParent(false);
						treeDataFileName->SetKey(libelleCategorie);
						treeDataFileName->SetValue(value);
						treeDataFileName->SetType(2);
						treeDataFileName->SetLinkType(0);
						treeDataFileName->SetId(criteria.GetId());
						treeDataFileName->SetLinkPath("");
						tr.append_child(child, treeDataFileName);
					}
				}
			}
		}
	}

	CreateElement();
}


void CCriteriaTree::CreateElement()
{
	bool isVisible = true;
	widthPosition = 0;
	vectorPosElement.clear();
	vectorPosElementDynamic.clear();
	tree<CTreeData*>::sibling_iterator it = tr.begin();
	auto itend = tr.end();
	yPos = 0;
	nbRow = 0;

	while (it != itend)
	{
		CTreeData* data = *it;
		int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			CTreeElementTexte* treeElementTexte = nullptr;
			CTreeElementStar* treeElementStar = nullptr;

			auto treeElementTriangle = CreateTriangleElement(
				themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
			treeElementTriangle->SetVisible(isVisible);
			auto posElement = CreatePositionElement(xPos, yPos, nbRow, 0,
			                                        treeElementTriangle->GetWidth(),
			                                        treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE,
			                                        treeElementTriangle, data);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			widthPosition = posElement->GetWidth() + themeTree.GetMargeX();


			if (data->GetType() == 2)
			{
				auto dataLink = static_cast<CTreeDataLink*>(data);
				treeElementTexte = CreateTexteLinkElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(),
					dataLink->GetLinkPath(), dataLink->GetLinkType());
			}
			else if (data->GetType() == 6)
			{
				auto datastar = static_cast<CTreeDataStars*>(data);
				treeElementStar = CreateStarElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(),
					data->GetValue(), datastar->GetNumPhotoId());
			}
			/*
			else if (data->GetType() == 7)
			{
				CTreeDataKeyWord * dataLink = (CTreeDataKeyWord *)data;
				treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(), dataLink->GetValue());
			}
			*/
			else
				treeElementTexte = CreateTexteElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());

			if (data->GetType() != 6)
			{
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(
					xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
					treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data,
					false);
			}
			else
			{
				treeElementStar->SetVisible(isVisible);
				posElement = CreatePositionElement(
					xPos, yPos, nbRow, 0, treeElementStar->GetWidth(),
					treeElementStar->GetHeight(), ELEMENT_STAR, treeElementStar, data, false);
			}


			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			if (treeElementTriangle->GetOpen())
				CreateChildTree(it);
		}
		++it;
	}
}


void CCriteriaTree::ClickOnElement(CPositionElement* element, wxWindow* window,
                                   const int& x, const int& y,
                                   const int& posLargeur, const int& posHauteur)
{
	if (window == nullptr)
		return;

	wxWindow* criteriaTreeWindow = window->FindWindowById(CRITERIATREEWINDOWID);
	CTreeElement* treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_STAR)
	{
		auto treeElementStar = static_cast<CTreeElementStar*>(treeElement);
		treeElementStar->ClickElement(window, x, y);
		window->Refresh();

		wxWindow* criteriawindow = window->FindWindowById(CRITERIAFOLDERWINDOWID);

		if (criteriawindow)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_UPDATECRITERIA);
			evt.SetExtraLong(2);
			criteriawindow->GetEventHandler()->AddPendingEvent(evt);
		}
	}
	else if (element->GetType() == ELEMENT_TEXTEVALUE)
	{
		auto treeElementTexte = static_cast<CTreeElementTexteClick*>(treeElement);
		//treeElementTexte->ClickElement(window, x, y);
		if (treeElementTexte->GetTypeLink() == 3)
		{
			CLibPicture libpicture;
			if (libpicture.TestIsExifCompatible(filename))
			{
				auto treedata = static_cast<CTreeDataLink*>(element->GetTreeData());
				auto photoCriteria = new CPhotoCriteria();
				photoCriteria->SetCriteriaId(treedata->GetId());
				photoCriteria->SetPhotoId(numPhotoId);
				auto event = new wxCommandEvent(wxEVT_SHOWCALENDAR);
				event->SetClientData(photoCriteria);
				wxQueueEvent(criteriaTreeWindow, event);
			}
			else
			{
				wxString labelInformations = CLibResource::LoadStringFromResource(
					L"labelInformations", 1);
				wxString notCompatibleFormat = CLibResource::LoadStringFromResource(
					L"NotCompatibleFormat", 1);
				wxMessageBox(notCompatibleFormat, labelInformations,
				             wxICON_INFORMATION);
			}
		}
		else if (treeElementTexte->GetTypeLink() == 1)
		{
			CLibPicture libpicture;
			if (libpicture.TestIsExifCompatible(filename))
			{
				auto treedata = static_cast<CTreeDataLink*>(element->GetTreeData());
				auto photoCriteria = new CPhotoCriteria();
				photoCriteria->SetCriteriaId(treedata->GetId());
				photoCriteria->SetPhotoId(numPhotoId);
				auto event = new wxCommandEvent(wxEVT_SHOWMAP);
				event->SetClientData(photoCriteria);
				wxQueueEvent(criteriaTreeWindow, event);
			}
			else
			{
				wxString labelInformations = CLibResource::LoadStringFromResource(
					L"labelInformations", 1);
				wxString notCompatibleFormat = CLibResource::LoadStringFromResource(
					L"NotCompatibleFormat", 1);
				wxMessageBox(notCompatibleFormat, labelInformations,
				             wxICON_INFORMATION);
			}
		}
		else if (treeElementTexte->GetTypeLink() == 7)
		{
			auto treedata = static_cast<CTreeDataLink*>(element->GetTreeData());
			auto photoCriteria = new CPhotoCriteria();
			photoCriteria->SetCriteriaId(treedata->GetId());
			photoCriteria->SetPhotoId(numPhotoId);
			auto event = new wxCommandEvent(wxEVT_SHOWKEYWORD);
			event->SetClientData(photoCriteria);
			wxQueueEvent(criteriaTreeWindow, event);
		}
	}
}

void CCriteriaTree::CreateChildTree(tree<CTreeData*>::sibling_iterator& parent)
{
	CPositionElement* pos_element;
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;
	//int i =

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		CTreeData* data = *it;

		if (data->GetValue().size() > 0 || it.number_of_children() == 0)
		{
			int xPos = widthPosition * (profondeur + 1);
			int widthElementColumn2 = 0;
			CTreeElementTexte* tree_element_texte;

			if (data->GetType() == 2)
			{
				auto dataLink = static_cast<CTreeDataLink*>(data);
				tree_element_texte = CreateTexteLinkElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					data->GetKey(), dataLink->GetLinkPath(),
					dataLink->GetLinkType());
			}
			else
				tree_element_texte = CreateTexteElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					data->GetKey());

			tree_element_texte->SetVisible(isVisible);
			pos_element = CreatePositionElement(
				xPos, yPos, nbRow, 0, tree_element_texte->GetWidth(),
				themeTree.GetRowHeight(), ELEMENT_TEXTE, tree_element_texte, data,
				false);

			const int width_element_column1 = xPos + pos_element->GetWidth() + themeTree.
				GetMargeX();

			if (data->GetValue() != "")
			{
				CTreeElementStar* treeElementStar = nullptr;
				xPos = themeTree.GetMargeX();

				if (data->GetType() == 2)
				{
					auto dataLink = static_cast<CTreeDataLink*>(data);
					tree_element_texte = CreateTexteLinkElement(
						themeTree.GetRowWidth(), themeTree.GetRowHeight(),
						data->GetValue(), dataLink->GetLinkPath(),
						dataLink->GetLinkType());
				}
				else if (data->GetType() == 6)
				{
					auto treedataStar = static_cast<CTreeDataStars*>(data);
					treeElementStar = CreateStarElement(
						themeTree.GetRowWidth(), themeTree.GetRowHeight(),
						data->GetKey(), data->GetValue(),
						treedataStar->GetNumPhotoId());
				}
				else
					tree_element_texte = CreateTexteElement(
						themeTree.GetRowWidth(), themeTree.GetRowHeight(),
						data->GetValue());

				if (data->GetType() == 6)
				{
					treeElementStar->SetVisible(isVisible);
					pos_element = CreatePositionElement(
						xPos, yPos, nbRow, 1, treeElementStar->GetWidth(),
						themeTree.GetRowHeight(), ELEMENT_STAR, treeElementStar,
						data, true);
				}
				else
				{
					tree_element_texte->SetVisible(isVisible);
					pos_element = CreatePositionElement(
						xPos, yPos, nbRow, 1, tree_element_texte->GetWidth(),
						themeTree.GetRowHeight(), ELEMENT_TEXTEVALUE,
						tree_element_texte, data, (data->GetType() == 2));
				}
				widthElementColumn2 = xPos + pos_element->GetWidth() + themeTree.
					GetMargeX();
			}

			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < width_element_column1)
				rowWidth[0] = width_element_column1;

			if (rowWidth[1] < widthElementColumn2)
				rowWidth[1] = widthElementColumn2;
		}
		else
		{
			int xPos = widthPosition * profondeur;
			CTreeElementTexte* tree_element_texte;
			//CTreeElementStar * treeElementStar = nullptr;
			CTreeElementTriangle* treeElementTriangle = CreateTriangleElement(
				themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
			treeElementTriangle->SetVisible(isVisible);
			pos_element = CreatePositionElement(
				xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(),
				treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE,
				treeElementTriangle, data);

			xPos += pos_element->GetWidth() + themeTree.GetMargeX();

			if (data->GetType() == 2)
			{
				auto dataLink = static_cast<CTreeDataLink*>(data);
				tree_element_texte = CreateTexteLinkElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					data->GetKey(), dataLink->GetLinkPath(),
					dataLink->GetLinkType());
			}
			else
				tree_element_texte = CreateTexteElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					data->GetKey());

			tree_element_texte->SetVisible(isVisible);
			pos_element = CreatePositionElement(
				xPos, yPos, nbRow, 0, tree_element_texte->GetWidth(),
				tree_element_texte->GetHeight(), ELEMENT_TEXTE, tree_element_texte,
				data, false);

			const int width_element = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < width_element)
				rowWidth[0] = width_element;

			CreateChildTree(it);
		}
		++it;
	}
}
