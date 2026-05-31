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
#include "CategoryRepository.h"
#include <RegardsConfigParam.h>
#include "CategoryQueryService.h"
#include "CategoryTreeStatePersistence.h"
#include "CategoryHierarchyIndex.h"
#include "CategorySelectionController.h"
using namespace Regards::Sqlite;
using namespace Regards::Viewer;
using namespace Regards::Window;

CCategoryWnd::CCategoryWnd(CWindowMain* windowMain, CThemeTree* theme, CTreeElementControlInterface* interfaceControl):
	numCatalog(0)
{
	sqlRequest_ = "";
	oldSqlRequest_ = "";

	themeTree = *theme;
	themeTree.themeTriangle.SetHeight(themeTree.GetRowHeight());
	themeTree.themeCheckbox.SetHeight(themeTree.GetRowHeight());
	themeTree.themeDelete.SetHeight(themeTree.GetRowHeight());
	themeTree.themeSlide.SetHeight(themeTree.GetRowHeight());
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	eventControl = interfaceControl;
	rowWidth.push_back(0);
	rowWidth.push_back(0);

	// Noms des mois (ressource localisée)
	monthNames_ = CConvertUtility::split(
		CLibResource::LoadStringFromResource(L"LBLMONTHNAME", 1), ',');

	config = CMainParamInit::getInstance();

	repo_ = std::make_unique<CategoryRepository>();
	queryService_ = std::make_unique<CategoryQueryService>(*repo_, windowMain);
	selectionCtrl_ = std::make_unique<CategorySelectionController>();
	persistence_ = std::make_unique<CategoryTreeStatePersistence>(CMainParamInit::getInstance());
	index_ = std::make_unique<CategoryHierarchyIndex>(&tr, *repo_, monthNames_);

	this->windowMain = windowMain;
}

void CCategoryWnd::RefreshListPhoto()
{
	queryService_->UpdateAndNotify(vectorPosElementDynamic, oldSqlRequest_);
}

CCategoryWnd::~CCategoryWnd()
{
}


void CCategoryWnd::Init()
{
	persistence_->Load();

	int faceDetection = 0;
	if (auto* cfg = CParamInit::getInstance())
		faceDetection = cfg->GetFaceDetection();

	index_->Build(faceDetection);

	CreateElement();

	UpdateElement();
}

wxString CCategoryWnd::GetSqlRequest()
{
	sqlRequest_ = queryService_->BuildSqlRequest(vectorPosElementDynamic);
	return sqlRequest_;
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
		selectionCtrl_->OnCheckBoxClicked(element, vectorPosElementDynamic);

		persistence_->SaveCheckState(vectorPosElementDynamic);

		queryService_->UpdateAndNotify(vectorPosElementDynamic, oldSqlRequest_);

		//UpdateElement();
		eventControl->UpdateTreeControl();
	}
	else if (element->GetType() == ELEMENT_TRIANGLE)
	{
		auto triangle = dynamic_cast<CTreeElementTriangle*>(element->GetTreeElement());
		triangle->ClickElement(window, x, y);

		persistence_->SaveTriangleState(vectorPosElementDynamic);

		UpdateElement();
		eventControl->UpdateTreeControl();
	}
}


void CCategoryWnd::RefreshCriteriaSearch()
{
	persistence_->SaveCheckState(vectorPosElementDynamic);

	queryService_->UpdateAndNotify(vectorPosElementDynamic, oldSqlRequest_);

	//UpdateElement();
	eventControl->UpdateTreeControl();
}

void CCategoryWnd::UpdateScreenRatio()
{
	//printf("CCategoryWnd::UpdateScreenRatio() \n");
	UpdateElement();
	eventControl->UpdateTreeControl();
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

			bool check = persistence_->GetCheckState(data->GetExifKey(), data->GetKey(), data->GetNumCategorie());
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
			bool isOpen = persistence_->GetTriangleState(data->GetExifKey(), data->GetKey());
			CTreeElementTriangle* treeElementTriangle = CreateTriangleElement(
				themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				isOpen);
			CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(),
			                      treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle,
			                      data);
			xPos += treeElementTriangle->GetWidth() + themeTree.GetMargeX();
			bool check = persistence_->GetCheckState(data->GetExifKey(), data->GetKey(), data->GetNumCategorie());
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
			bool isOpen = persistence_->GetTriangleState(data->GetExifKey(), data->GetKey());
			CTreeElementTriangle* treeElementTriangle = CreateTriangleElement(
				themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				isOpen);
			CPositionElement* posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(),
			                                                     treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE,
			                                                     treeElementTriangle,
			                                                     data);

			widthPosition = xPos + posElement->GetWidth();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			bool check = persistence_->GetCheckState(data->GetExifKey(), data->GetKey(), data->GetNumCategorie());
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
				bool isOpen = persistence_->GetTriangleState(data->GetExifKey(), data->GetKey());
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
				bool check = persistence_->GetCheckState(data->GetExifKey(), data->GetKey(), data->GetNumCategorie());
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
				bool check = persistence_->GetCheckState(data->GetExifKey(), data->GetKey(), data->GetNumCategorie());
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
				bool isOpen = persistence_->GetTriangleState(data->GetExifKey(), data->GetKey());
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
				bool check = persistence_->GetCheckState(data->GetExifKey(), data->GetKey(), data->GetNumCategorie());
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
