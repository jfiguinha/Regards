#include <header.h>
#include "infoAbout.h"
#include "TreeElementTexte.h"
#include "TreeElementTexteClick.h"
#include "TreeDataLink.h"
#include <TreeElementControlInterface.h>
#include <TreeElementTriangle.h>
#include <LibResource.h>
using namespace Regards::Introduction;

CInfoAbout::CInfoAbout(const CThemeTree& theme)
{
	themeTree = theme;
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	themeTree.SetMargeX(5);
	rowWidth.push_back(0);
	rowWidth.push_back(0);
	index = 0;
	yPos = 0;
	widthPosition = 0;
};

void CInfoAbout::UpdateScreenRatio()
{
	//printf("CCategoryWnd::UpdateScreenRatio() \n");
	eventControl->UpdateTreeControl();
}

CInfoAbout::~CInfoAbout()
{
}

void CInfoAbout::Init()
{
	top = tr.begin();
	tree<CTreeData*>::iterator top;
	top = tr.begin();

	auto treeDataPicture = new CTreeData();
	treeDataPicture->SetKey("Informations");
	auto child = tr.insert(top, treeDataPicture);

	auto treeDataFileName = new CTreeData();
	treeDataFileName->SetIsParent(false);
	treeDataFileName->SetKey("Developper");
	treeDataFileName->SetValue("Figuinha Jacques");
	tr.append_child(child, treeDataFileName);

	wxString localVersion = CLibResource::LoadStringFromResource("REGARDSVERSION", 1);

	auto treeDataWidth = new CTreeData();
	treeDataWidth->SetIsParent(false);
	treeDataWidth->SetKey("Version");
	treeDataWidth->SetValue(localVersion);
	tr.append_child(child, treeDataWidth);

	auto treeDataAnnee = new CTreeData();
	treeDataAnnee->SetIsParent(false);
	treeDataAnnee->SetKey("Copyright");
	treeDataAnnee->SetValue("2014 - 2026");
	tr.append_child(child, treeDataAnnee);

	auto treeDataLib = new CTreeData();
	treeDataLib->SetKey("External Library");
	child = tr.insert(top, treeDataLib);

	auto treeFFMPEG = new CTreeDataLink();
	treeFFMPEG->SetIsParent(false);
	treeFFMPEG->SetKey("LibCairo");
	treeFFMPEG->SetLinkType(1);
	treeFFMPEG->SetLinkPath("mpl11.pdf");
	treeFFMPEG->SetValue("1.14.2 - Mozilla Public License (MPL) - Click to see licence");
	tr.append_child(child, treeFFMPEG);

	auto treeSQLite = new CTreeData();
	treeSQLite->SetIsParent(false);
	treeSQLite->SetKey("SQLite");
	treeSQLite->SetValue("3.30.0");
	tr.append_child(child, treeSQLite);

	auto treeRapipXML = new CTreeData();
	treeRapipXML->SetIsParent(false);
	treeRapipXML->SetKey("Rapid XML");
	treeRapipXML->SetValue("1.13");
	tr.append_child(child, treeRapipXML);
	
	auto libMediaInfo = new CTreeData();
	libMediaInfo->SetIsParent(false);
	libMediaInfo->SetKey("Lib Media Info");
	libMediaInfo->SetValue("0.7.75");
	tr.append_child(child, libMediaInfo);

	auto treeHH = new CTreeData();
	treeHH->SetIsParent(false);
	treeHH->SetKey("tree.hh");
	treeHH->SetValue("2.81");
	tr.append_child(child, treeHH);

	auto treeCxImage = new CTreeData();
	treeCxImage->SetIsParent(false);
	treeCxImage->SetKey("CxImage ");
	treeCxImage->SetValue("7.0");
	tr.append_child(child, treeCxImage);

	auto treelibRaw = new CTreeData();
	treelibRaw->SetIsParent(false);
	treelibRaw->SetKey("Libraw");
	treelibRaw->SetValue("0.21.0");
	tr.append_child(child, treelibRaw);

	auto treelibExiv = new CTreeData();
	treelibExiv->SetIsParent(false);
	treelibExiv->SetKey("Exiv 2");
	treelibExiv->SetValue("0.28.3");
	tr.append_child(child, treelibExiv);

	auto treelibFFmpeg = new CTreeData();
	treelibFFmpeg->SetIsParent(false);
	treelibFFmpeg->SetKey("FFMpeg");
	treelibFFmpeg->SetValue("5.1.2");
	tr.append_child(child, treelibFFmpeg);

	auto treelibBPG = new CTreeData();
	treelibBPG->SetIsParent(false);
	treelibBPG->SetKey("wxWidget");
	treelibBPG->SetValue("3.2.0");
	tr.append_child(child, treelibBPG);

	auto treelibTurboJpeg = new CTreeData();
	treelibTurboJpeg->SetIsParent(false);
	treelibTurboJpeg->SetKey("Turbo JPEG");
	treelibTurboJpeg->SetValue("1.5.1");
	tr.append_child(child, treelibTurboJpeg);

	auto treelibWebp = new CTreeData();
	treelibWebp->SetIsParent(false);
	treelibWebp->SetKey("FreeImage");
	treelibWebp->SetValue("3.2");
	tr.append_child(child, treelibWebp);

	auto treelibSDL = new CTreeData();
	treelibSDL->SetIsParent(false);
	treelibSDL->SetKey("SDL 2");
	treelibSDL->SetValue("2.0.5");
	tr.append_child(child, treelibSDL);

	auto treelibJasper = new CTreeData();
	treelibJasper->SetIsParent(false);
	treelibJasper->SetKey("jasper");
	treelibJasper->SetValue("1.900.1");
	tr.append_child(child, treelibJasper);

	auto treelibde265 = new CTreeData();
	treelibde265->SetIsParent(false);
	treelibde265->SetKey("libde265");
	treelibde265->SetValue("1.0.0");
	tr.append_child(child, treelibde265);

	auto treelibheif = new CTreeData();
	treelibheif->SetIsParent(false);
	treelibheif->SetKey("heif");
	treelibheif->SetValue("1.0");
	tr.append_child(child, treelibheif);

	auto treelibAvif = new CTreeData();
	treelibAvif->SetIsParent(false);
	treelibAvif->SetKey("Avif");
	treelibAvif->SetValue("1.0");
	tr.append_child(child, treelibAvif);

	auto treeOpenEXR = new CTreeData();
	treeOpenEXR->SetIsParent(false);
	treeOpenEXR->SetKey("QPDF");
	treeOpenEXR->SetValue("9.0.2");
	tr.append_child(child, treeOpenEXR);

	auto treewxSVG = new CTreeData();
	treewxSVG->SetIsParent(false);
	treewxSVG->SetKey("wxSVG");
	treewxSVG->SetValue("1.5.15");
	tr.append_child(child, treewxSVG);

	auto poppler = new CTreeData();
	poppler->SetIsParent(false);
	poppler->SetKey("libPoppler");
	poppler->SetValue("0.8");
	tr.append_child(child, poppler);

	auto tesseract = new CTreeData();
	tesseract->SetIsParent(false);
	tesseract->SetKey("Tesseract");
	tesseract->SetValue("5.2");
	tr.append_child(child, tesseract);

	auto opencv = new CTreeData();
	opencv->SetIsParent(false);
	opencv->SetKey("OpenCV");
	opencv->SetValue("4.8");
	tr.append_child(child, opencv);

	CreateElement();
}


void CInfoAbout::CreateElement()
{
	RenderMode mode = RenderMode::Create;
	CTreeElementTriangle* tree_element_triangle = nullptr;
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
			CTreeElementTexte* tree_element_texte;

			posElement = RenderTriangle(data,
				xPos,
				yPos,
				isVisible,
				mode);

			tree_element_triangle = static_cast<CTreeElementTriangle*>(posElement->GetTreeElement());

			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			widthPosition = posElement->GetWidth() + themeTree.GetMargeX();


			if (data->GetType() == 2)
			{

				posElement = RenderTextLink(data,
					xPos,
					yPos,
					isVisible,
					mode);
			}
			else
			{
				posElement = RenderText(data,
					xPos,
					yPos,
					isVisible,
					mode);
			}

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			const bool isOpen = tree_element_triangle->GetOpen();
			if (isOpen)
				CreateChildTree(it);
		}
		++it;
	}
}

void CInfoAbout::ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
                                const int& posLargeur, const int& posHauteur)
{
	CTreeElement* treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_TEXTEVALUE)
	{
		auto treeElementTexte = static_cast<CTreeElementTexteClick*>(treeElement);
		treeElementTexte->ClickElement(window, x, y);
	}
}

void CInfoAbout::CreateChildTree(tree<CTreeData*>::sibling_iterator& parent)
{
	RenderMode mode = RenderMode::Create;
	CPositionElement* posElement;
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;
	//int i =

	for (int i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		CTreeData* data = *it;

		if (data->GetValue().size() > 0 || it.number_of_children() == 0)
		{
			int xPos = widthPosition * (profondeur + 1);
			int widthElementColumn2 = 0;
			if (data->GetType() == 2)
			{

				posElement = RenderTextLink(data,
					xPos,
					yPos,
					isVisible,
					mode);
			}
			else
			{
				posElement = RenderText(data,
					xPos,
					yPos,
					isVisible,
					mode);
			}

			const int widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			if (data->GetValue() != "")
			{
				xPos = themeTree.GetMargeX();

				if (data->GetType() == 2)
				{

					posElement = RenderTextLink(data,
						xPos,
						yPos,
						isVisible,
						mode);
				}
				else
				{
					posElement = RenderText(data,
						xPos,
						yPos,
						isVisible,
						mode);
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
			posElement = RenderTriangle(data,
				xPos,
				yPos,
				isVisible,
				mode);

			CTreeElementTriangle * tree_element_triangle = static_cast<CTreeElementTriangle*>(posElement->GetTreeElement());

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			if (data->GetType() == 2)
			{

				posElement = RenderTextLink(data,
					xPos,
					yPos,
					isVisible,
					mode);
			}
			else
			{
				posElement = RenderText(data,
					xPos,
					yPos,
					isVisible,
					mode);
			}

			int widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			CreateChildTree(it);
		}
		++it;
	}
}
