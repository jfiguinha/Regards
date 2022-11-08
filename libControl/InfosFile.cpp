#include <header.h>
#include "InfosFile.h"
#include <libPicture.h>
#include <TreeData.h>
#ifdef EXIV2
#include <MetadataExiv2.h>
#else
#ifdef __APPLE__
#include <AppleReadExif.h>
#include <libPicture.h>
#include <MediaInfo.h>
#endif
#ifdef WIN32
#include <PictureMetadata.h>
#include <MetadataExiv2.h>
#include <libPicture.h>
#include <MediaInfo.h>
#endif
#endif
#include <TreeElementTexte.h>
#include <TreeElementTriangle.h>
#include <Gps.h>
#include <theme.h>
#include <TreeElementControlInterface.h>
using namespace Regards::Control;
using namespace Regards::Window;
using namespace Regards::Internet;
using namespace Regards::Picture;
using namespace Regards::exiv2;
#define TAILLEMAX 4096

CInfosFile::CInfosFile(CThemeTree* theme, CTreeElementControlInterface* interfaceControl): yPos(0), cxMax(0),
	xMaxPos(0),
	xMaxPosValue(0)
{
	rotation = 0;
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


const wxString CInfosFile::GetFilename()
{
	return filename;
}

void CInfosFile::MouseOver(wxDC* deviceContext, CPositionElement* element, const int& x, const int& y,
                           const int& posLargeur, const int& posHauteur, bool& update)
{
	//

	if (element != nullptr)
	{
		CTreeElement* treeElement = element->GetTreeElement();
		if (treeElement != nullptr)
			treeElement->MouseOver(deviceContext, x, y, update);
	}
}


void CInfosFile::UpdateScreenRatio()
{
	//
	this->UpdateElement();
}

void CInfosFile::AddTreeInfos(const wxString& exifKey, const wxString& exifValue, const int& index,
                              tree<CTreeData*>::iterator& top, tree<CTreeData*>::iterator& child)
{
	//
	wchar_t seps[] = L".";
	int item = 0;
	wchar_t informations[TAILLEMAX];
	wcscpy(informations, exifKey.c_str());
	wchar_t* token1 = nullptr;

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
		token = wcstok(nullptr, seps); // C4996
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


void CInfosFile::SetFile(const wxString& picture)
{
	//

	if (filename == picture)
		return;

	vector<CMetadata> listItem;
	CLibPicture libPicture;
	filename = picture;
	int width = 0;
	int height = 0;

	bool showOtherInformation = true;
	cxMax = 0;
	xMaxPos = 0;
	xMaxPosValue = 0;

#ifdef EXIV2
	CMetadataExiv2 metadata(picture);
	listItem = metadata.GetMetadata();
#else

#ifdef __APPLE__
    CAppleReadExif appleReadExif;
#elif WIN32
	WIC::CPictureMetadata metadata(picture);
#endif

    if(libPicture.TestIsVideo(picture))
    {
        CMediaInfo mediaInfo;
        listItem = mediaInfo.ReadMetadata(picture);
    }
    else if(libPicture.TestImageFormat(picture))
    {
#ifdef __APPLE__
        listItem = appleReadExif.GetPictureMetadata(picture);
#elif WIN32
		listItem = metadata.GetMetadataFromPicture();
		if(listItem.size() == 0)
		{
			EXIV2::CMetadataExiv2 meta(picture);
			listItem = meta.GetMetadata();
		}
#endif
    }
#endif

	if (listItem.size() > 0)
	{
		showOtherInformation = false;
		tree<CTreeData*>::iterator top;

		int index = 0;

		top = tr.begin();


		for (const CMetadata& meta : listItem)
		{
			auto child = top;
			AddTreeInfos(meta.key, meta.value, index, top, child);
			index++;
		}
	}

#ifdef WIN32
	wxString createDatatime = metadata.GetCreationDate();
	if (createDatatime == "")
		showOtherInformation = true;

#endif
	if (showOtherInformation)
	{
		CLibPicture lib_picture;
		lib_picture.GetPictureDimensions(picture, width, height, rotation);
		tree<CTreeData*>::iterator top;
		wxString exifinfos;
		//int item = 0;
		//int index = 0;

		top = tr.begin();

		auto child = top;

		auto treeDataPicture = new CTreeData();
		treeDataPicture->SetKey(L"Picture");
		child = tr.insert(top, treeDataPicture);

		auto treeDataFileName = new CTreeData();
		treeDataFileName->SetIsParent(false);
		treeDataFileName->SetKey(L"Filename");
		treeDataFileName->SetValue(picture);
		tr.append_child(child, treeDataFileName);

		auto treeDataWidth = new CTreeData();
		exifinfos = to_string(width);
		treeDataWidth->SetIsParent(false);
		treeDataWidth->SetKey(L"Width");
		treeDataWidth->SetValue(exifinfos);
		tr.append_child(child, treeDataWidth);

		auto treeDataHeight = new CTreeData();
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

		auto treeDataDateTime = new CTreeData();
		treeDataDateTime->SetIsParent(false);
		treeDataDateTime->SetKey(L"Date Time");
		treeDataDateTime->SetValue(dateTimeInfos);
		tr.append_child(child, treeDataDateTime);
	}

	//CreateElement();
}


void CInfosFile::CreateChildTree(tree<CTreeData*>::sibling_iterator& parent)
{
	//

	CPositionElement* posElement;
	tree<CTreeData*>::sibling_iterator it = tree<CTreeData*>::begin(parent);

	//int i =

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		bool isVisible = true;
		const int profondeur = tr.depth(it);
		CTreeData* data = *it;

		if (!data->GetValue().empty() || it.number_of_children() == 0)
		{
			int x_pos = widthPosition * (profondeur + 1);
			int widthElementColumn2 = 0;

			CTreeElementTexte* treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                                         data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(x_pos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
			                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data,
			                                   false);

			int widthElementColumn1 = x_pos + posElement->GetWidth() + themeTree.GetMargeX();

			if (!data->GetValue().empty())
			{
				x_pos = themeTree.GetMargeX();
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				                                      data->GetValue());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(x_pos, yPos, nbRow, 1, treeElementTexte->GetWidth(),
				                                   treeElementTexte->GetHeight(), ELEMENT_TEXTEVALUE, treeElementTexte,
				                                   data, false);
				widthElementColumn2 = x_pos + posElement->GetWidth() + themeTree.GetMargeX();
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
			int x_pos = widthPosition * profondeur;
			CTreeElementTriangle* treeElementTriangle = CreateTriangleElement(
				themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				true);
			treeElementTriangle->SetVisible(isVisible);
			posElement = CreatePositionElement(x_pos, yPos, nbRow, 0, treeElementTriangle->GetWidth(),
			                                   treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle,
			                                   data);

			x_pos += posElement->GetWidth() + themeTree.GetMargeX();

			CTreeElementTexte* treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                                         data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(x_pos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
			                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data,
			                                   false);

			int widthElement = x_pos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			CreateChildTree(it);
		}
		++it;
	}
}

void CInfosFile::CreateElement()
{
	//

	widthPosition = 0;
	vectorPosElement.clear();
	vectorPosElementDynamic.clear();
	tree<CTreeData*>::sibling_iterator it = tr.begin();
	const auto itend = tr.end();
	yPos = 0;
	nbRow = 0;

	while (it != itend)
	{
		CTreeData* data = *it;
		const int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			bool isVisible = true;
			int x_pos = themeTree.GetMargeX();
			int width_element = 0;

			CTreeElementTriangle* treeElementTriangle = CreateTriangleElement(
				themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				true);
			treeElementTriangle->SetVisible(isVisible);
			CPositionElement* posElement = CreatePositionElement(x_pos, yPos, nbRow, 0, treeElementTriangle->GetWidth(),
			                                                     treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE,
			                                                     treeElementTriangle,
			                                                     data);

			x_pos += posElement->GetWidth() + themeTree.GetMargeX();
			widthPosition = posElement->GetWidth() + themeTree.GetMargeX();


			CTreeElementTexte* treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                                         data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(x_pos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
			                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data,
			                                   false);

			width_element += x_pos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < width_element)
				rowWidth[0] = width_element;

			CreateChildTree(it);
		}
		++it;
	}
}


void CInfosFile::UpdateChildTree(tree<CTreeData*>::sibling_iterator& parent)
{
	tree<CTreeData*>::sibling_iterator it = tree<CTreeData*>::begin(parent);
	//int i =

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		bool is_visible = true;
		int profondeur = tr.depth(it);
		CTreeData* data = *it;

		if (!data->GetValue().empty() || it.number_of_children() == 0)
		{
			int xPos = widthPosition * (profondeur + 1);
			int widthElementColumn2 = 0;
			CTreeElementTexte* tree_element_texte;
			CPositionElement* posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				auto treeElementTexte = static_cast<CTreeElementTexte*>(posElement->GetTreeElement());
				treeElementTexte->SetVisible(is_visible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				tree_element_texte =
					CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				tree_element_texte->SetVisible(is_visible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_texte->GetWidth(),
				                                   tree_element_texte->GetHeight(), ELEMENT_TEXTE, tree_element_texte, data,
				                                   false);
			}

			int widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			if (!data->GetValue().empty())
			{
				xPos = themeTree.GetMargeX();
				posElement = GetElement(data, ELEMENT_TEXTEVALUE);
				if (posElement != nullptr)
				{
					auto element_texte = static_cast<CTreeElementTexte*>(posElement->GetTreeElement());
					element_texte->SetVisible(is_visible);
					posElement->SetX(xPos);
					posElement->SetY(yPos);
				}
				else
				{
					tree_element_texte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					                                      data->GetValue());
					tree_element_texte->SetVisible(is_visible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, tree_element_texte->GetWidth(),
					                                   tree_element_texte->GetHeight(), ELEMENT_TEXTEVALUE,
					                                   tree_element_texte, data, false);
				}
				widthElementColumn2 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			}

			if (is_visible)
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
			CTreeElementTriangle* treeElementTriangle;
			CPositionElement* posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement != nullptr)
			{
				treeElementTriangle = dynamic_cast<CTreeElementTriangle*>(posElement->GetTreeElement());
				treeElementTriangle->SetVisible(is_visible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
				treeElementTriangle->SetVisible(is_visible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(),
				                                   treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE,
				                                   treeElementTriangle, data);
			}

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				auto treeElementTexte = dynamic_cast<CTreeElementTexte*>(posElement->GetTreeElement());
				treeElementTexte->SetVisible(is_visible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				CTreeElementTexte* treeElementTexte = CreateTexteElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					data->GetKey());
				treeElementTexte->SetVisible(is_visible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
				                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data,
				                                   false);
			}
			int widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			const bool isShow = treeElementTriangle->GetOpen();
			if (isShow)
				UpdateChildTree(it);
		}
		++it;
	}
}

void CInfosFile::UpdateElement()
{
	//

	for (CPositionElement* value : vectorPosElement)
	{
		if (value != nullptr)
		{
			CTreeElement* treeElement = value->GetTreeElement();
			if (treeElement != nullptr)
				treeElement->SetVisible(false);
		}
	}

	widthPosition = 0;

	tree<CTreeData*>::sibling_iterator it = tr.begin();
	auto itend = tr.end();
	yPos = 0;
	nbRow = 0;

	while (it != itend)
	{
		CTreeData* data = *it;
		const int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			bool isVisible = true;
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			CTreeElementTriangle* treeElementTriangle;
			CPositionElement* posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement != nullptr)
			{
				treeElementTriangle = dynamic_cast<CTreeElementTriangle*>(posElement->GetTreeElement());
				treeElementTriangle->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
				treeElementTriangle->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(),
				                                   treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE,
				                                   treeElementTriangle, data);
			}
			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			widthPosition = posElement->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				auto treeElementTexte = static_cast<CTreeElementTexte*>(posElement->GetTreeElement());
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				CTreeElementTexte* treeElementTexte = CreateTexteElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
				                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data,
				                                   false);
			}
			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			const bool isOpen = treeElementTriangle->GetOpen();
			if (isOpen)
				UpdateChildTree(it);
		}
		++it;
	}
}

void CInfosFile::ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
                                const int& posLargeur, const int& posHauteur)
{
	//
	if (element != nullptr)
	{
		CTreeElement* treeElement = element->GetTreeElement();
		if (treeElement != nullptr)
		{
			if (element->GetType() == ELEMENT_TRIANGLE)
			{
				auto treeElementTriangle = static_cast<CTreeElementTriangle*>(treeElement);
				treeElementTriangle->ClickElement(window, x, y);
				UpdateElement();
				eventControl->UpdateTreeControl();
			}
		}
	}
}
