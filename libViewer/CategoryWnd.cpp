#include <header.h>
#include "CategoryWnd.h"
#include "TreeDataCategory.h"
#include <TreeElementTriangle.h>
#include "ViewerParamInit.h"
#include <LibResource.h>
#include <ConvertUtility.h>
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

	RenderElement(RenderMode::Create);
	RenderElement(RenderMode::Update);
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

	switch(element->GetType())
	{
		case ELEMENT_CHECKBOX:
		{
			selectionCtrl_->OnCheckBoxClicked(element, vectorPosElementDynamic);

			persistence_->SaveCheckState(vectorPosElementDynamic);

			queryService_->UpdateAndNotify(vectorPosElementDynamic, oldSqlRequest_);

			//UpdateElement();
			eventControl->UpdateTreeControl();
			break;
		}
		case ELEMENT_TRIANGLE:
		{
			auto triangle = dynamic_cast<CTreeElementTriangle*>(element->GetTreeElement());
			triangle->ClickElement(window, x, y);
			persistence_->SaveTriangleState(vectorPosElementDynamic);
			RenderElement(RenderMode::Update);
			eventControl->UpdateTreeControl();
			break;
		}
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
	RenderElement(RenderMode::Update);
	eventControl->UpdateTreeControl();
}


bool CCategoryWnd::GetCheckState(CTreeData* data)
{
	CTreeDataCategory* dataCategory =static_cast<CTreeDataCategory*>(data);
	return persistence_->GetCheckState(data->GetExifKey(), data->GetKey(), dataCategory->GetNumCategorie());
}
bool CCategoryWnd::GetTriangleState(CTreeData* data)
{
	return persistence_->GetTriangleState(data->GetExifKey(), data->GetKey());
}


void CCategoryWnd::AdvanceRow(int xPos, CPositionElement* posElement)
{
	const int width =
		xPos + posElement->GetWidth() + themeTree.GetMargeX();

	yPos += themeTree.GetRowHeight();
	++nbRow;

	rowWidth[0] = std::max(rowWidth[0], width);
}


void CCategoryWnd::RenderElement(RenderMode mode)
{
	tree<CTreeData*>::sibling_iterator it = tr.begin();
	const auto itend = tr.end();
	yPos = 0;
	nbRow = 0;
	widthPosition = 0;

	HideAll();

	while (it != itend)
	{
		auto data = static_cast<CTreeDataCategory*>(*it);
		const int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			bool isVisible = true;
			if (mode == RenderMode::Create)
				isVisible = false;

			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			CPositionElement* posElement = nullptr;
			CTreeElementTriangle* tree_element_triangle = nullptr;

			posElement = RenderTriangle(data,
				xPos,
				yPos,
				isVisible,
				mode);

			tree_element_triangle = dynamic_cast<CTreeElementTriangle*>(posElement->GetTreeElement());

			widthPosition = xPos + posElement->GetWidth();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = RenderCheckBox(data,
				xPos,
				yPos,
				GetCheckState(data),
				isVisible,
				mode);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = RenderText(data,
				xPos,
				yPos,
				isVisible,
				mode);

			AdvanceRow(xPos, posElement);

			if(mode == RenderMode::Create)
			{
				RenderChildElement(it, mode);
			}
			else
			{
				const bool isOpen = tree_element_triangle->GetOpen();
				if (isOpen)
					RenderChildElement(it, mode);
			}
		}
		++it;
	}
}

void CCategoryWnd::RenderChildElement(tree<CTreeData*>::sibling_iterator& parent, RenderMode mode)
{
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		bool isVisible = true;
		if (mode == RenderMode::Create)
			isVisible = false;

		CPositionElement* posElement = nullptr;
		const int profondeur = tr.depth(it);
		auto data = static_cast<CTreeDataCategory*>(*it);
		if (profondeur == 1 && (!data->GetValue().empty() || it.number_of_children() == 0))
		{
			int xPos = widthPosition * (profondeur + 1);

			posElement = RenderText(data,
				xPos,
				yPos,
				isVisible,
				mode);

			AdvanceRow(xPos, posElement);
		}
		else if (!data->GetValue().empty() || it.number_of_children() == 0)
		{
			int xPos = widthPosition * (profondeur + 1);

			posElement = RenderCheckBox(data,
				xPos,
				yPos,
				GetCheckState(data),
				isVisible,
				mode);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = RenderText(data,
				xPos,
				yPos,
				isVisible,
				mode);

			AdvanceRow(xPos, posElement);
		}
		else
		{
			CTreeElementTriangle* tree_element_triangle = nullptr;
			int xPos = widthPosition * profondeur;

			posElement = RenderTriangle(data,
				xPos,
				yPos,
				isVisible,
				mode);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			tree_element_triangle = dynamic_cast<CTreeElementTriangle*>(posElement->GetTreeElement());

			posElement = RenderCheckBox(data,
				xPos,
				yPos,
				GetCheckState(data),
				isVisible,
				mode);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = RenderText(data,
				xPos,
				yPos,
				isVisible,
				mode);

			AdvanceRow(xPos, posElement);

			if (mode == RenderMode::Create)
			{
				RenderChildElement(it, mode);
			}
			else
			{
				const bool isShow = tree_element_triangle->GetOpen();
				if (isShow)
					RenderChildElement(it, mode);
			}

		}
		++it;
	}
}
