#include <header.h>
#include "CriteriaTree.h"
#include <TreeElementTexte.h>
#include <TreeElementTriangle.h>
#include <TreeElementStar.h>
#include "TreeElementTexteClick.h"
#include <SqlPhotos.h>
#include "TreeDataLink.h"
#include "TreeDataStars.h"
#include "TreeDataKeyWord.h"
#include <PhotoCriteria.h>
#include <wx/filename.h>
#include <libPicture.h>
#include <SqlFindFacePhoto.h>
#include <LibResource.h>
#include <SqlPhotoCriteria.h>
#include <SqlPhotoCategorie.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Control;
using namespace Regards::Window;
using namespace Regards::Sqlite;
#define TAILLEMAX 4096

PhotoCategorieVector CCriteriaTree::photoCategorieVector;

CCriteriaTree::CCriteriaTree(CThemeTree * theme, CTreeElementControlInterface * interfaceControl)
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
    
};



wxString CCriteriaTree::GetFilename()
{
	return filename;
}

void CCriteriaTree::MouseOver(wxDC * deviceContext, CPositionElement * element, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	if (element != nullptr)
	{
		CTreeElement * treeElement = element->GetTreeElement();
		if (treeElement != nullptr)
			treeElement->MouseOver(deviceContext, x, y);
	}
}


void CCriteriaTree::UpdateScreenRatio()
{
    //this->UpdateElement();
}

void CCriteriaTree::AddTreeInfos(const wxString &exifKey, const wxString &exifValue, const int &index, tree<CTreeData *>::iterator &top, tree<CTreeData *>::iterator &child)
{
	wchar_t seps[] = L".";
	int item = 0;
	wchar_t informations[TAILLEMAX];
	wcscpy(informations, exifKey.c_str());
    wchar_t * token1;
    
	// Establish string and get the first token:
#if defined(WIN32) && _MSC_VER < 1900
	wchar_t * token = wcstok(informations, seps); // C4996
#else
	wchar_t * token = wcstok(informations, seps, &token1); // C4996
#endif

	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		CTreeData * treeData = new CTreeData();
		treeData->SetKey(token);
#if defined(WIN32) && _MSC_VER < 1900
	wchar_t * token = wcstok(informations, seps); // C4996
#else
	wchar_t * token = wcstok(informations, seps, &token1); // C4996
#endif

		if (token != nullptr)
		{
			treeData->SetIsParent(true);

			if (index > 0)
			{
				if (item == 0)
				{
					tree<CTreeData *>::iterator it;
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
					tree<CTreeData *>::iterator it;
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

void CCriteriaTree::SetFile(const wxString & picture, const int &numPhotoId)
{
	if (filename == picture)
		return;
    
    filename = picture;
    this->numPhotoId = numPhotoId;
    
    top = tr.begin();
    tree<CTreeData *>::iterator top;
    //	int item = 0;
    //	int index = 0;
    
    top = tr.begin();
    
    tree<CTreeData *>::iterator child = top;
    
    if(photoCategorieVector.size() == 0)
    {
        CSqlPhotoCategorie sqlPhotoCategorie;
        sqlPhotoCategorie.LoadPhotoCategorie(&photoCategorieVector, 1);
    }
    
    CTreeData * treeDataPicture = new CTreeData();
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
				CTreeDataLink * treeDataFileName = new CTreeDataLink();
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

			CTreeData * treeDataFileName = new CTreeData();
			treeDataFileName->SetIsParent(false);
			treeDataFileName->SetKey(libelleCategorie);
			treeDataFileName->SetValue(dirname.GetPath());
			tr.append_child(child, treeDataFileName);
		}
		else if (photoCategorie.GetId() == 5)
		{
			wxString libelleCategorie = photoCategorie.GetLibelle();
			CTreeData * treeDataFileName = new CTreeData();
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
			CTreeDataStars * treeDataFileName = new CTreeDataStars();
			treeDataFileName->SetNumPhotoId(numPhotoId);
			treeDataFileName->SetIsParent(false);
			treeDataFileName->SetKey(libelleCategorie);
			if(criteriaVector.size() == 0)
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
			if(criteriaVector.size() == 0)
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
					CTreeDataLink * treeDataFileName = new CTreeDataLink();
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

					CTreeDataLink * treeDataFileName = new CTreeDataLink();
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

					CTreeDataLink * treeDataFileName = new CTreeDataLink();
					treeDataFileName->SetIsParent(false);
					treeDataFileName->SetKey(libelleCategorie);
					treeDataFileName->SetValue(value);
					treeDataFileName->SetType(2);
					treeDataFileName->SetLinkType(1);
					treeDataFileName->SetLinkPath("");
					treeDataFileName->SetId(criteria.GetId());
					tr.append_child(child, treeDataFileName);
				}
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
				}
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

						CTreeDataLink * treeDataFileName = new CTreeDataLink();
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
    
    tree<CTreeData *>::sibling_iterator it = tr.begin();
    tree<CTreeData *>::iterator itend = tr.end();
    yPos = 0;
    nbRow = 0;
    
    while (it != itend) {
        CTreeData * data = *it;
        int profondeur = tr.depth(it);
        if (profondeur == 0)
        {
            
            int xPos = themeTree.GetMargeX();
            int widthElement = 0;
            CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementStar * treeElementStar = nullptr;
            CTreeElementTriangle * treeElementTriangle = nullptr;
            CPositionElement * posElement = nullptr;
            
            treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
            treeElementTriangle->SetVisible(isVisible);
            posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
            
            xPos += posElement->GetWidth() + themeTree.GetMargeX();
            widthPosition = posElement->GetWidth() + themeTree.GetMargeX();
            
            
            if(data->GetType() == 2)
            {
                CTreeDataLink * dataLink = (CTreeDataLink *)data;
				treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(), dataLink->GetLinkPath(), dataLink->GetLinkType());
            }
			else if (data->GetType() == 6)
			{
				CTreeDataStars * datastar = (CTreeDataStars *)data;
				treeElementStar = CreateStarElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(), data->GetValue(), datastar->GetNumPhotoId());
			}
			/*
			else if (data->GetType() == 7)
			{
				CTreeDataKeyWord * dataLink = (CTreeDataKeyWord *)data;
				treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(), dataLink->GetValue());
			}
			*/
            else
                treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
            
			if (data->GetType() != 6)
			{
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
			}
			else
			{
				treeElementStar->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementStar->GetWidth(), treeElementStar->GetHeight(), ELEMENT_STAR, treeElementStar, data, false);

			}


            widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
            yPos += themeTree.GetRowHeight();
            nbRow++;
            if (rowWidth[0] < widthElement)
                rowWidth[0] = widthElement;
            
            if (treeElementTriangle->GetOpen())
                CreateChildTree(it);
            
        }
        it++;
        
    }
}



void CCriteriaTree::ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
    CTreeElement * treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_STAR)
	{
		CTreeElementStar * treeElementStar = (CTreeElementStar *)treeElement;
		treeElementStar->ClickElement(window, x, y);
		window->Refresh();
		wxWindow* window = window->FindWindowById(CRITERIAFOLDERWINDOWID);
		if (window)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_UPDATECRITERIA);
			evt.SetExtraLong(2);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
	}
    else if (element->GetType() == ELEMENT_TEXTEVALUE)
    {
        CTreeElementTexteClick * treeElementTexte= (CTreeElementTexteClick *)treeElement;
        //treeElementTexte->ClickElement(window, x, y);
        if(treeElementTexte->GetTypeLink() == 3)
        {
			CLibPicture libpicture;
			if(libpicture.TestIsExifCompatible(filename))
			{
				CTreeDataLink * treedata = (CTreeDataLink *)element->GetTreeData();
				CPhotoCriteria * photoCriteria = new CPhotoCriteria();
				photoCriteria->SetCriteriaId(treedata->GetId());
				photoCriteria->SetPhotoId(numPhotoId);
				wxCommandEvent * event = new wxCommandEvent(wxEVT_SHOWCALENDAR);
				event->SetClientData(photoCriteria);
				wxQueueEvent(window->GetParent()->GetParent(), event);
			}
			else
			{
                wxString labelInformations = CLibResource::LoadStringFromResource(L"labelInformations",1);
                wxString notCompatibleFormat = CLibResource::LoadStringFromResource(L"NotCompatibleFormat",1);
				wxMessageBox(notCompatibleFormat, labelInformations, wxICON_INFORMATION);
			}

        }
        else if(treeElementTexte->GetTypeLink() == 1)
        {
			CLibPicture libpicture;
			if(libpicture.TestIsExifCompatible(filename))
			{
				CTreeDataLink * treedata = (CTreeDataLink *)element->GetTreeData();
				CPhotoCriteria * photoCriteria = new CPhotoCriteria();
				photoCriteria->SetCriteriaId(treedata->GetId());
				photoCriteria->SetPhotoId(numPhotoId);
				wxCommandEvent * event = new wxCommandEvent(wxEVT_SHOWMAP);
				event->SetClientData(photoCriteria);
				wxQueueEvent(window->GetParent()->GetParent(), event);
			}
			else
			{
                wxString labelInformations = CLibResource::LoadStringFromResource(L"labelInformations",1);
                wxString notCompatibleFormat = CLibResource::LoadStringFromResource(L"NotCompatibleFormat",1);
				wxMessageBox(notCompatibleFormat, labelInformations, wxICON_INFORMATION);
			}
        }
		else if (treeElementTexte->GetTypeLink() == 7)
		{
			CTreeDataLink * treedata = (CTreeDataLink *)element->GetTreeData();
			CPhotoCriteria * photoCriteria = new CPhotoCriteria();
			photoCriteria->SetCriteriaId(treedata->GetId());
			photoCriteria->SetPhotoId(numPhotoId);
			wxCommandEvent * event = new wxCommandEvent(wxEVT_SHOWKEYWORD);
			event->SetClientData(photoCriteria);
			wxQueueEvent(window->GetParent()->GetParent(), event);
		}
    }
    
}

void CCriteriaTree::CreateChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
    CPositionElement * posElement = nullptr;
    tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
    //tree<CTreeData *>::iterator itend = tr.end(parent);
    bool isVisible = true;
    //int i =
    
    for (auto i = 0; i < parent.number_of_children(); i++)
    {
        int profondeur = tr.depth(it);
        CTreeData * data = *it;
        
        if (data->GetValue().size() > 0 || it.number_of_children() == 0)
        {
            int xPos = widthPosition * (profondeur + 1);
            int widthElementColumn1 = 0;
            int widthElementColumn2 = 0;
            CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementStar * treeElementStar = nullptr;
            
            if(data->GetType() == 2)
            {
                CTreeDataLink * dataLink = (CTreeDataLink *)data;
                treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(), dataLink->GetLinkPath(), dataLink->GetLinkType());
            }
            else
                treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
            
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

            widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
            
            if (data->GetValue() != "")
            {
                xPos = themeTree.GetMargeX();
                
                if(data->GetType() == 2)
                {
                    CTreeDataLink * dataLink = (CTreeDataLink *)data;
                    treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetValue(), dataLink->GetLinkPath(), dataLink->GetLinkType());
                }
				else if (data->GetType() == 6)
				{
					CTreeDataStars * treedataStar = (CTreeDataStars *)data;
					treeElementStar = CreateStarElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(), data->GetValue(), treedataStar->GetNumPhotoId());
				}
                else
                    treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetValue());
                
				if (data->GetType() == 6)
				{
					treeElementStar->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, treeElementStar->GetWidth(), themeTree.GetRowHeight(), ELEMENT_STAR, treeElementStar, data, true);
				}
				else
				{
					treeElementTexte->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, treeElementTexte->GetWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTEVALUE, treeElementTexte, data, (data->GetType() == 2));
				}
                widthElementColumn2 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
            }
            
            yPos += themeTree.GetRowHeight();
            
            nbRow++;
            if (rowWidth[0] < widthElementColumn1)
                rowWidth[0] = widthElementColumn1;
            
            if (rowWidth[1] < widthElementColumn2)
                rowWidth[1] = widthElementColumn2;
        }
        else
        {
            int xPos = widthPosition * profondeur;
            int widthElement = 0;
            CTreeElementTexte * treeElementTexte = nullptr;
            CTreeElementTriangle * treeElementTriangle = nullptr;
			CTreeElementStar * treeElementStar = nullptr;
            treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
            treeElementTriangle->SetVisible(isVisible);
            posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
            
            xPos += posElement->GetWidth() + themeTree.GetMargeX();
            
            if(data->GetType() == 2)
            {
                CTreeDataLink * dataLink = (CTreeDataLink *)data;
                treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(), dataLink->GetLinkPath(), dataLink->GetLinkType());
            }
            else
                treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
            
			treeElementTexte->SetVisible(isVisible);
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