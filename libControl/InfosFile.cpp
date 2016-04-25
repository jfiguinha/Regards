#include "InfosFile.h"
#include <ConvertUtility.h>
#include <libPicture.h>
#ifdef EXIV2
#include <PictureMetadata.h>
#endif
#ifdef __APPLE__
#include <AppleReadExif.h>
#include <libPicture.h>
#include <MediaInfo.h>
#endif
#ifdef WIN32
#include <PictureMetadata.h>
#include <libPicture.h>
#include <MediaInfo.h>
#endif
#include <TreeElementTexte.h>
#include <TreeElementTriangle.h>
#include <Gps.h>
#include <libResource.h>
using namespace Regards::Control;
using namespace Regards::Window;
using namespace Regards::Internet;
#define TAILLEMAX 4096

CInfosFile::CInfosFile(CThemeTree * theme, CTreeElementControlInterface * interfaceControl)
{
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



wxString CInfosFile::GetFilename()
{
	return filename;
}

void CInfosFile::MouseOver(wxDC * deviceContext, CPositionElement * element, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	if (element != nullptr)
	{
		CTreeElement * treeElement = element->GetTreeElement();
		if (treeElement != nullptr)
			treeElement->MouseOver(deviceContext, x, y);
	}
}


void CInfosFile::UpdateScreenRatio()
{
    this->UpdateElement();
}

void CInfosFile::AddTreeInfos(const wxString &exifKey, const wxString &exifValue, const int &index, tree<CTreeData *>::iterator &top, tree<CTreeData *>::iterator &child)
{
	wchar_t seps[] = L".";
	int item = 0;
	wchar_t informations[TAILLEMAX];
	wcscpy(informations, exifKey.c_str());
    wchar_t * token1;
    
	// Establish string and get the first token:
#ifdef WIN32 
#if _MSC_VER < 1900
	wchar_t * token = wcstok(informations, seps); // C4996
#else
	wchar_t * token = wcstok(informations, seps, &token1); // C4996
#endif
#else
    wchar_t * token = wcstok(informations, seps, &token1); // C4996
#endif
	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		CTreeData * treeData = new CTreeData();
		treeData->SetKey(token);
#ifdef WIN32
#if _MSC_VER < 1900
		token = wcstok(nullptr, seps); // C4996
#else
		token = wcstok(nullptr, seps, &token1); // C4996
#endif
#else
        token = wcstok(nullptr, seps, &token1); // C4996
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



void CInfosFile::SetFile(const wxString & picture)
{
	if (filename == picture)
		return;


	filename = picture;
	int width = 0;
	int height = 0;
	
	bool showOtherInformation = true;
    cxMax = 0;
    xMaxPos = 0;
    xMaxPosValue = 0;
    
   
#ifdef __APPLE__
    CAppleReadExif appleReadExif;
#else
	CPictureMetadata metadata(picture);
#endif
    vector<CMetadata> listItem;
    CLibPicture libPicture;
    

#if defined(__APPLE__) || defined(WIN32)

    if(libPicture.TestIsVideo(picture))
    {
        CMediaInfo mediaInfo;
        listItem = mediaInfo.ReadMetadata(picture);
    }
    else if(libPicture.TestImageFormat(picture))
    {
#ifdef __APPLE__        
        listItem = appleReadExif.GetPictureMetadata(picture);
#else

		
		listItem = metadata.GetMetadataFromPicture();
#endif
    }

#else

	listItem = metadata.GetMetadata();
	   
#endif

    if(listItem.size() > 0)
    {
        showOtherInformation = false;
        tree<CTreeData *>::iterator top;
        tree<CTreeData *>::iterator child;
        int index = 0;
        
        top = tr.begin();
        
        for(CMetadata meta : listItem)
        {
            tree<CTreeData *>::iterator child = top;
            AddTreeInfos(meta.key, meta.value, index, top, child);
            index++;
        }
    }

#ifdef WIN32
	wxString createDatatime = metadata.GetCreationDate();
	if (createDatatime == "")
		showOtherInformation = true;

#endif
	if(showOtherInformation)
	{
		CLibPicture libPicture;
		libPicture.GetDimensions(picture, width, height, rotation);
		tree<CTreeData *>::iterator top;
		wxString exifinfos;
		//int item = 0;
		//int index = 0;

		top = tr.begin();

		tree<CTreeData *>::iterator child = top;

		CTreeData * treeDataPicture = new CTreeData();
		treeDataPicture->SetKey(L"Picture");
		child = tr.insert(top, treeDataPicture);

		CTreeData * treeDataFileName = new CTreeData();
		treeDataFileName->SetIsParent(false);
		treeDataFileName->SetKey(L"Filename");
		treeDataFileName->SetValue(picture);
		tr.append_child(child, treeDataFileName);

		CTreeData * treeDataWidth = new CTreeData();
        exifinfos = to_string(width);
		treeDataWidth->SetIsParent(false);
		treeDataWidth->SetKey(L"Width");
		treeDataWidth->SetValue(exifinfos);
		tr.append_child(child, treeDataWidth);

		CTreeData * treeDataHeight = new CTreeData();
        exifinfos = to_string(height);
		treeDataHeight->SetIsParent(false);
		treeDataHeight->SetKey(L"Height");
		treeDataHeight->SetValue(exifinfos);
		tr.append_child(child, treeDataHeight);

		wxStructStat strucStat;
		wxStat(picture, &strucStat);
		//wxFileOffset filelen=strucStat.st_size;
		wxDateTime last_modified_time(strucStat.st_mtime);
		wxString dateTimeInfos = last_modified_time.FormatISOCombined(' ');

		CTreeData * treeDataDateTime = new CTreeData();
		treeDataDateTime->SetIsParent(false);
		treeDataDateTime->SetKey(L"Date Time");
		treeDataDateTime->SetValue(dateTimeInfos);
		tr.append_child(child, treeDataDateTime);

	}

	//CreateElement();
	
}



void CInfosFile::CreateChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
	CPositionElement * posElement = nullptr;
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;
	//int i = 

	for (int i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		CTreeData * data = *it;

		if (data->GetValue().size() > 0 || it.number_of_children() == 0)
		{
			int xPos = widthPosition * (profondeur + 1);
			int widthElementColumn1 = 0;
			int widthElementColumn2 = 0;
			CTreeElementTexte * treeElementTexte = nullptr;
			
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

			widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			if (data->GetValue() != L"")
			{
				xPos = themeTree.GetMargeX();
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetValue());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 1, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTEVALUE, treeElementTexte, data, false);
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
			treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
			treeElementTriangle->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

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

void CInfosFile::CreateElement()
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
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CPositionElement * posElement = nullptr;
	
			treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
			treeElementTriangle->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
		
			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			widthPosition = posElement->GetWidth() + themeTree.GetMargeX();

	
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
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


void CInfosFile::UpdateChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;
	//int i = 

	for (int i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		CTreeData * data = *it;

		if (data->GetValue().size() > 0 || it.number_of_children() == 0)
		{
			int xPos = widthPosition * (profondeur + 1);
			int widthElementColumn1 = 0;
			int widthElementColumn2 = 0;
			CTreeElementTexte * treeElementTexte = nullptr;
			CPositionElement * posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
			}

			widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			if (data->GetValue() != L"")
			{
				xPos = themeTree.GetMargeX();
				posElement = GetElement(data, ELEMENT_TEXTEVALUE);
				if (posElement != nullptr)
				{
					CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
					treeElementTexte->SetVisible(isVisible);
					posElement->SetX(xPos);
					posElement->SetY(yPos);
				}
				else
				{
					treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetValue());
					treeElementTexte->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTEVALUE, treeElementTexte, data, false);
				}
				widthElementColumn2 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			}

			if (isVisible)
			{
				yPos += themeTree.GetRowHeight();

				nbRow++;
				if (rowWidth[0] < widthElementColumn1)
					rowWidth[0] = widthElementColumn1;

				if (rowWidth[1] < widthElementColumn2)
					rowWidth[1] = widthElementColumn2;
			}
		}
		else
		{
			int xPos = widthPosition * profondeur;
			int widthElement = 0;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CPositionElement * posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement != nullptr)
			{
				treeElementTriangle = (CTreeElementTriangle *)posElement->GetTreeElement();
				treeElementTriangle->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
				treeElementTriangle->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
			}

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
			}
			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			bool isShow = treeElementTriangle->GetOpen();
			if (isShow)
				UpdateChildTree(it);
		}
		it++;

	}
}

void CInfosFile::UpdateElement()
{
	for (CPositionElement * value : vectorPosElement)
	{
		if (value != nullptr)
		{
			CTreeElement * treeElement = value->GetTreeElement();
			if (treeElement != nullptr)
				treeElement->SetVisible(false);
		}
	}

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
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CPositionElement * posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement != nullptr)
			{
				treeElementTriangle = (CTreeElementTriangle *)posElement->GetTreeElement();
				treeElementTriangle->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
				treeElementTriangle->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
			}
			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			widthPosition = posElement->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
			}
			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			bool isOpen = treeElementTriangle->GetOpen();
			if (isOpen)
				UpdateChildTree(it);
		}
		it++;

	}
}

void CInfosFile::ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	CTreeElement * treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_TRIANGLE)
	{
		CTreeElementTriangle * treeElementTriangle = (CTreeElementTriangle *)treeElement;
		treeElementTriangle->ClickElement(window, x, y);
		UpdateElement();
		eventControl->Update();
	}

}