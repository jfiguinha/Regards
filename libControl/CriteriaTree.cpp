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
#include <appcontext.h>
#include <SqlPhotoCategorie.h>
#include <wx/tokenzr.h>
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
extern AppContext application_context;
PhotoCategorieVector CCriteriaTree::photoCategorieVector;

// Category ids (as returned by CPhotoCategorie::GetId()):
//   1 = GPS, 2 = Folder, 3 = Date, 4 = Faces, 5 = Usenet category label,
//   6 = Stars, 7 = Keywords (currently disabled), other = generic criteria list

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


wxString CCriteriaTree::GetFilename()
{
	return filename;
}

void CCriteriaTree::UpdateScreenRatio()
{
	//this->UpdateElement();
}

// ============================================================================
// SetFile / category loading & dispatch
// ============================================================================

void CCriteriaTree::LoadCategories()
{
	if (photoCategorieVector.size() == 0)
	{
		CSqlPhotoCategorie sqlPhotoCategorie;
		sqlPhotoCategorie.LoadPhotoCategorie(&photoCategorieVector, 1);
	}
}

void CCriteriaTree::SetFile(const wxString& picture, const int& numPhotoId)
{
	if (filename == picture)
		return;

	filename = picture;
	this->numPhotoId = numPhotoId;

	LoadCategories();

	top = tr.begin();
	child = top;

	auto treeDataPicture = new CTreeData();
	treeDataPicture->SetKey("Criteria");
	child = tr.insert(top, treeDataPicture);

	for (CPhotoCategorie & photoCategorie : photoCategorieVector)
	{
		ProcessCategory(picture, photoCategorie, child);
	}

	RenderTree();
}

void CCriteriaTree::ProcessCategory(const wxString& picture, CPhotoCategorie& photoCategorie,
                                    tree<CTreeData*>::iterator& child)
{
	switch (photoCategorie.GetId())
	{
	case 4:
		AddFaces(picture, photoCategorie, child);
		break;
	case 2:
		AddFolder(picture, photoCategorie, child);
		break;
	case 5:
		// Small enough to keep inline: just a label/value swap, no dedicated Add* needed.
		CreateTextNode(child, "usenet.category", photoCategorie.GetLibelle());
		break;
	case 1:
		AddGPS(picture, photoCategorie, child);
		break;
	case 3:
		AddDate(picture, photoCategorie, child);
		break;
	case 6:
		AddStars(picture, photoCategorie, child);
		break;
	default:
		AddKeywords(picture, photoCategorie, child);
		break;
	}
}

// ============================================================================
// Category handlers
// ============================================================================

void CCriteriaTree::AddFaces(const wxString& picture, CPhotoCategorie& photoCategorie,
                             tree<CTreeData*>::iterator& child)
{
	CSqlFindFacePhoto sqlFindFacePhoto;
	std::vector<CFaceName> listFaceName = sqlFindFacePhoto.GetListFaceName(picture);
	wxString libelleCategorie = photoCategorie.GetLibelle();

	for (size_t i = 0; i < listFaceName.size(); i++)
	{
		CreateLinkNode(child, libelleCategorie, listFaceName[i].faceName, 0, listFaceName[i].numFace);
	}
}

void CCriteriaTree::AddFolder(const wxString& picture, CPhotoCategorie& photoCategorie,
                              tree<CTreeData*>::iterator& child)
{
	wxFileName dirname = wxFileName::DirName(picture);
	CreateTextNode(child, photoCategorie.GetLibelle(), dirname.GetPath());
}

void CCriteriaTree::AddGPS(const wxString& picture, CPhotoCategorie& photoCategorie,
                           tree<CTreeData*>::iterator& child)
{
	CriteriaVector criteriaVector;
	CSqlPhotos sqlPhotos;
	sqlPhotos.GetPhotoCriteriaByCategorie(&criteriaVector, picture, photoCategorie.GetId());

	wxString libelleCategorie = photoCategorie.GetLibelle();

	if (criteriaVector.size() == 0)
	{
		wxString value = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
		CreateLinkNode(child, libelleCategorie, value, 1, 0);
	}
	else
	{
		CCriteria criteria = criteriaVector.at(0);
		wxString value = criteria.GetLibelle();

		if (value.find(application_context.special_key) == 0)
			value = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);

		CreateLinkNode(child, libelleCategorie, value, 1, criteria.GetId());
	}
}

void CCriteriaTree::AddDate(const wxString& picture, CPhotoCategorie& photoCategorie,
                            tree<CTreeData*>::iterator& child)
{
	CriteriaVector criteriaVector;
	CSqlPhotos sqlPhotos;
	sqlPhotos.GetPhotoCriteriaByCategorie(&criteriaVector, picture, photoCategorie.GetId());

	wxString libelleCategorie = photoCategorie.GetLibelle();

	if (criteriaVector.size() == 0)
	{
		CreateLinkNode(child, libelleCategorie, "1970.01.01", 3, 0);
	}
	else
	{
		CCriteria& criteria = criteriaVector.at(0);
		CreateLinkNode(child, libelleCategorie, criteria.GetLibelle(), 3, criteria.GetId());
	}
}

void CCriteriaTree::AddStars(const wxString& picture, CPhotoCategorie& photoCategorie,
                             tree<CTreeData*>::iterator& child)
{
	CriteriaVector criteriaVector;
	CSqlPhotos sqlPhotos;
	sqlPhotos.GetPhotoCriteriaByCategorie(&criteriaVector, picture, photoCategorie.GetId());

	wxString value = (criteriaVector.size() == 0) ? wxString("0") : criteriaVector[0].GetLibelle();
	CreateStarNode(child, photoCategorie.GetLibelle(), value);
}

void CCriteriaTree::AddKeywords(const wxString& picture, CPhotoCategorie& photoCategorie,
                                tree<CTreeData*>::iterator& child)
{
	if (photoCategorie.GetId() == 7)
	{
		// Keyword linking is currently disabled (feature not yet implemented upstream).
		return;
	}

	CriteriaVector criteriaVector;
	CSqlPhotos sqlPhotos;
	sqlPhotos.GetPhotoCriteriaByCategorie(&criteriaVector, picture, photoCategorie.GetId());

	wxString libelleCategorie = photoCategorie.GetLibelle();

	if (criteriaVector.size() == 0)
	{
		CreateLinkNode(child, libelleCategorie, "", 0, 0);
	}
	else
	{
		for (CCriteria & criteria : criteriaVector)
		{
			CreateLinkNode(child, libelleCategorie, criteria.GetLibelle(), 0, criteria.GetId());
		}
	}
}

// ============================================================================
// Tree node factories
// ============================================================================

void CCriteriaTree::CreateLinkNode(tree<CTreeData*>::iterator& parent, const wxString& key, const wxString& value,
                                   const int& linkType, const int& id)
{
	auto treeDataFileName = new CTreeDataLink();
	treeDataFileName->SetIsParent(false);
	treeDataFileName->SetKey(key);
	treeDataFileName->SetValue(value);
	treeDataFileName->SetType(2);
	treeDataFileName->SetLinkType(linkType);
	treeDataFileName->SetId(id);
	treeDataFileName->SetLinkPath("");
	tr.append_child(parent, treeDataFileName);
}

void CCriteriaTree::CreateStarNode(tree<CTreeData*>::iterator& parent, const wxString& key, const wxString& value)
{
	auto treeDataFileName = new CTreeDataStars();
	treeDataFileName->SetNumPhotoId(numPhotoId);
	treeDataFileName->SetIsParent(false);
	treeDataFileName->SetKey(key);
	treeDataFileName->SetValue(value);
	treeDataFileName->SetType(6);
	tr.append_child(parent, treeDataFileName);
}

void CCriteriaTree::CreateTextNode(tree<CTreeData*>::iterator& parent, const wxString& key, const wxString& value)
{
	auto treeDataFileName = new CTreeData();
	treeDataFileName->SetIsParent(false);
	treeDataFileName->SetKey(key);
	treeDataFileName->SetValue(value);
	tr.append_child(parent, treeDataFileName);
}

// ============================================================================
// Rendering
// ============================================================================

void CCriteriaTree::CreateElement()
{
	// Kept for API compatibility with any existing external callers.
	RenderTree();
}

void CCriteriaTree::RenderTree()
{
	CTreeElementTriangle* treeElementTriangle = nullptr;
	CPositionElement* posElement = nullptr;
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

			posElement = RenderTriangle(data,
				xPos,
				yPos,
				isVisible,
				RenderMode::Create);

			treeElementTriangle = dynamic_cast<CTreeElementTriangle*>(posElement->GetTreeElement());

			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			widthPosition = posElement->GetWidth() + themeTree.GetMargeX();


			if (data->GetType() != 6)
			{
				if (data->GetType() == 2)
				{
					posElement = RenderTextLink(data,
						xPos,
						yPos,
						isVisible,
						RenderMode::Create);
				}
				else
				{
					posElement = RenderText(data,
						xPos,
						yPos,
						isVisible,
						RenderMode::Create);
				}
			}
			else
			{
				posElement = RenderStar(data,
					xPos,
					yPos,
					isVisible,
					RenderMode::Create);
			}


			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			if (treeElementTriangle->GetOpen())
				RenderNode(it);
		}
		++it;
	}
}

CPositionElement* CCriteriaTree::RenderTextLinkValue(
	CTreeData* data,
	int& xPos,
	int& yPos,
	bool visible)
{
	CPositionElement* posElement = nullptr;
	CTreeElementTexte* treeElementTexte = nullptr;

	auto dataLink = static_cast<CTreeDataLink*>(data);
	treeElementTexte = CreateTexteLinkElement(
		themeTree.GetRowWidth(), themeTree.GetRowHeight(),
		data->GetValue(), dataLink->GetLinkPath(),
		dataLink->GetLinkType());
	treeElementTexte->SetVisible(visible);
	posElement = CreatePositionElement(
		xPos, yPos, nbRow, 1, treeElementTexte->GetWidth(),
		themeTree.GetRowHeight(), ELEMENT_TEXTEVALUE,
		treeElementTexte, data, (data->GetType() == 2));

	return posElement;
}


void CCriteriaTree::RenderNode(tree<CTreeData*>::sibling_iterator& parent)
{
	CPositionElement* pos_element;
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);
	bool isVisible = true;

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		CTreeData* data = *it;

		if (data->GetValue().size() > 0 || it.number_of_children() == 0)
		{
			int xPos = widthPosition * (profondeur + 1);
			int widthElementColumn2 = 0;

			if (data->GetType() == 2)
			{

				pos_element = RenderTextLink(data,
					xPos,
					yPos,
					isVisible,
					RenderMode::Create);
			}
			else
			{
				pos_element = RenderText(data,
					xPos,
					yPos,
					isVisible,
					RenderMode::Create);
			}

			const int width_element_column1 = xPos + pos_element->GetWidth() + themeTree.
				GetMargeX();

			if (data->GetValue() != "")
			{
				CTreeElementStar* treeElementStar = nullptr;
				xPos = themeTree.GetMargeX();


				if (data->GetType() == 6)
				{
					pos_element = RenderStar(data,
						xPos,
						yPos,
						isVisible,
						RenderMode::Create);
				}
				else
				{

					if (data->GetType() == 2)
					{
						RenderTextLinkValue(data,
							xPos,
							yPos,
							isVisible);
					}
					else
					{
						pos_element = RenderTextValue(data,
							xPos,
							yPos,
							isVisible,
							RenderMode::Create);
					}

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
			CPositionElement* posElement = nullptr;

			posElement = RenderTriangle(data,
				xPos,
				yPos,
				isVisible,
				RenderMode::Create);

			xPos += pos_element->GetWidth() + themeTree.GetMargeX();

			if (data->GetType() == 2)
			{

				pos_element = RenderTextLink(data,
					xPos,
					yPos,
					isVisible,
					RenderMode::Create);
			}
			else
			{
				pos_element = RenderText(data,
					xPos,
					yPos,
					isVisible,
					RenderMode::Create);
			}

			const int width_element = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < width_element)
				rowWidth[0] = width_element;

			RenderNode(it);
		}
		++it;
	}
}

// ============================================================================
// Input handling
// ============================================================================

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
			wxCommandEvent evt(wxEVENT_UPDATECRITERIA);
			evt.SetExtraLong(2);
			criteriawindow->GetEventHandler()->AddPendingEvent(evt);
		}
	}
	else if (element->GetType() == ELEMENT_TEXTEVALUE)
	{
		auto treeElementTexte = static_cast<CTreeElementTexteClick*>(treeElement);
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

// ============================================================================
// Legacy / currently unused
// ============================================================================

void CCriteriaTree::AddTreeInfos(const wxString& exifKey,
	const wxString& exifValue,
	const int& index,
	tree<CTreeData*>::iterator& top,
	tree<CTreeData*>::iterator& child)
{

	AddTreeInfosImpl<CTreeData>(
		exifKey,
		index,
		top,
		child,
		[&](CTreeData* treeData)
		{
			treeData->SetIsParent(false);
			treeData->SetValue(exifValue);
			treeData->SetExifKey(exifKey);
		});
}
