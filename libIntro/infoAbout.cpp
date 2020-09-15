#include <header.h>
#include "infoAbout.h"
#include "TreeElementTexte.h"
#include "TreeElementTexteClick.h"
#include "TreeDataLink.h"
#include <TreeElementControlInterface.h>
#include <TreeElementTriangle.h>
using namespace Regards::Introduction;

CInfoAbout::CInfoAbout(const CThemeTree & theme)
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
    printf("CCategoryWnd::UpdateScreenRatio() \n");
    eventControl->UpdateTreeControl();
}

CInfoAbout::~CInfoAbout()
{
	
}

void CInfoAbout::Init()
{
	top = tr.begin();
	tree<CTreeData *>::iterator top;
//	int item = 0;
//	int index = 0;

	top = tr.begin();

	tree<CTreeData *>::iterator child = top;

	CTreeData * treeDataPicture = new CTreeData();
	treeDataPicture->SetKey("Informations");
	child = tr.insert(top, treeDataPicture);

	CTreeData * treeDataFileName = new CTreeData();
	treeDataFileName->SetIsParent(false);
	treeDataFileName->SetKey("Developper");
	treeDataFileName->SetValue("Figuinha Jacques");
	tr.append_child(child, treeDataFileName);

	CTreeData * treeDataTest = new CTreeData();
	treeDataTest->SetIsParent(false);
	treeDataTest->SetKey("Test By");
	treeDataTest->SetValue("SPADOTTO Sebastien");
	tr.append_child(child, treeDataTest);

	CTreeData * treeDataWidth = new CTreeData();
	treeDataWidth->SetIsParent(false);
	treeDataWidth->SetKey("Version");
	treeDataWidth->SetValue("2.61.4");
	tr.append_child(child, treeDataWidth);

	CTreeData * treeDataAnnee = new CTreeData();
	treeDataAnnee->SetIsParent(false);
	treeDataAnnee->SetKey("Copyright");
	treeDataAnnee->SetValue("2014 - 2020");
	tr.append_child(child, treeDataAnnee);

	CTreeData * treeDataLib = new CTreeData();
	treeDataLib->SetKey("External Library");
	child = tr.insert(top, treeDataLib);

	CTreeDataLink * treeFFMPEG = new CTreeDataLink();
	treeFFMPEG->SetIsParent(false);
	treeFFMPEG->SetKey("LibCairo");
    treeFFMPEG->SetLinkType(1);
    treeFFMPEG->SetLinkPath("mpl11.pdf");
	treeFFMPEG->SetValue("1.14.2 - Mozilla Public License (MPL) - Click to see licence");
	tr.append_child(child, treeFFMPEG);

	CTreeData * treeSQLite = new CTreeData();
	treeSQLite->SetIsParent(false);
	treeSQLite->SetKey("SQLite");
	treeSQLite->SetValue("3.26.0");
	tr.append_child(child, treeSQLite);

	CTreeData * treeRapipXML = new CTreeData();
	treeRapipXML->SetIsParent(false);
	treeRapipXML->SetKey("Rapid XML");
	treeRapipXML->SetValue("1.13");
	tr.append_child(child, treeRapipXML);


	CTreeData * libMediaInfo = new CTreeData();
	libMediaInfo->SetIsParent(false);
	libMediaInfo->SetKey("Lib Media Info");
	libMediaInfo->SetValue("0.7.75");
	tr.append_child(child, libMediaInfo);

	CTreeData * treeHH = new CTreeData();
	treeHH->SetIsParent(false);
	treeHH->SetKey("tree.hh");
	treeHH->SetValue("2.81");
	tr.append_child(child, treeHH);

	CTreeData * treeCxImage = new CTreeData();
	treeCxImage->SetIsParent(false);
	treeCxImage->SetKey("CxImage ");
	treeCxImage->SetValue("7.0");
	tr.append_child(child, treeCxImage);

	CTreeData * treelibRaw = new CTreeData();
	treelibRaw->SetIsParent(false);
	treelibRaw->SetKey("Libraw");
	treelibRaw->SetValue("0.18.7");
	tr.append_child(child, treelibRaw);

	CTreeData * treelibExiv = new CTreeData();
	treelibExiv->SetIsParent(false);
	treelibExiv->SetKey("Exiv 2");
	treelibExiv->SetValue("0.26");
	tr.append_child(child, treelibExiv);

	CTreeData * treelibFFmpeg = new CTreeData();
	treelibFFmpeg->SetIsParent(false);
	treelibFFmpeg->SetKey("FFMpeg");
	treelibFFmpeg->SetValue("4.1");
	tr.append_child(child, treelibFFmpeg);
	
	CTreeData * treelibBPG = new CTreeData();
	treelibBPG->SetIsParent(false);
	treelibBPG->SetKey("wxWidget");
	treelibBPG->SetValue("3.1.3");
	tr.append_child(child, treelibBPG);

	CTreeData * treelibTurboJpeg = new CTreeData();
	treelibTurboJpeg->SetIsParent(false);
	treelibTurboJpeg->SetKey("Turbo JPEG");
	treelibTurboJpeg->SetValue("1.5.1");
	tr.append_child(child, treelibTurboJpeg);
    
	CTreeData * treelibDlib = new CTreeData();
	treelibDlib->SetIsParent(false);
	treelibDlib->SetKey("dlib");
	treelibDlib->SetValue("19.7");
	tr.append_child(child, treelibDlib);    

	CTreeData * treelibGlew = new CTreeData();
	treelibGlew->SetIsParent(false);
	treelibGlew->SetKey("glew");
	treelibGlew->SetValue("2.1.0");
	tr.append_child(child, treelibGlew);  
    
	CTreeData * treelibWebp = new CTreeData();
	treelibWebp->SetIsParent(false);
	treelibWebp->SetKey("FreeImage");
	treelibWebp->SetValue("3.2");
	tr.append_child(child, treelibWebp);   

	CTreeData * treelib265 = new CTreeData();
	treelib265->SetIsParent(false);
	treelib265->SetKey("x265");
	treelib265->SetValue("2.5");
	tr.append_child(child, treelib265);     
    
	CTreeData * treelibSDL = new CTreeData();
	treelibSDL->SetIsParent(false);
	treelibSDL->SetKey("SDL 2");
	treelibSDL->SetValue("2.0.5");
	tr.append_child(child, treelibSDL);     
    
	CTreeData * treelibJasper = new CTreeData();
	treelibJasper->SetIsParent(false);
	treelibJasper->SetKey("jasper");
	treelibJasper->SetValue("1.900.1");
	tr.append_child(child, treelibJasper);       
   
	CTreeData * treelibde265 = new CTreeData();
	treelibde265->SetIsParent(false);
	treelibde265->SetKey("libde265");
	treelibde265->SetValue("1.0");
	tr.append_child(child, treelibde265); 

	CTreeData * treelibheif = new CTreeData();
	treelibheif->SetIsParent(false);
	treelibheif->SetKey("heif");
	treelibheif->SetValue("1.0");
	tr.append_child(child, treelibheif); 

    CTreeData * treePiccante = new CTreeData();
	treePiccante->SetIsParent(false);
	treePiccante->SetKey("Piccante");
	treePiccante->SetValue("0.4");
	tr.append_child(child, treePiccante);    
    
    CTreeData * treeOpenEXR = new CTreeData();
	treeOpenEXR->SetIsParent(false);
	treeOpenEXR->SetKey("QPDF");
	treeOpenEXR->SetValue("9.0.2");
	tr.append_child(child, treeOpenEXR);     

    CTreeData * treewxSVG = new CTreeData();
	treewxSVG->SetIsParent(false);
	treewxSVG->SetKey("wxSVG");
	treewxSVG->SetValue("1.5.15");
	tr.append_child(child, treewxSVG);  

	CTreeData * poppler = new CTreeData();
	poppler->SetIsParent(false);
	poppler->SetKey("libPoppler");
	poppler->SetValue("0.8");
	tr.append_child(child, poppler);

	CreateElement();
}


void CInfoAbout::CreateElement()
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


            if(data->GetType() == 2)
            {
                CTreeDataLink * dataLink = (CTreeDataLink *)data;
                treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(), dataLink->GetLinkPath(), dataLink->GetLinkType());
            }
            else
                treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
            
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

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

void CInfoAbout::ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
    CTreeElement * treeElement = element->GetTreeElement();
    if (element->GetType() == ELEMENT_TEXTEVALUE)
    {
        CTreeElementTexteClick * treeElementTexte= (CTreeElementTexteClick *)treeElement;
        treeElementTexte->ClickElement(window, x, y);
    }
    
}

void CInfoAbout::CreateChildTree(tree<CTreeData *>::sibling_iterator &parent)
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

            if(data->GetType() == 2)
            {
                CTreeDataLink * dataLink = (CTreeDataLink *)data;
                treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(), dataLink->GetLinkPath(), dataLink->GetLinkType());
            }
            else
                treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			
            treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

			widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			if (data->GetValue() != "")
			{
				xPos = themeTree.GetMargeX();
                
                if(data->GetType() == 2)
                {
                    CTreeDataLink * dataLink = (CTreeDataLink *)data;
                    treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetValue(), dataLink->GetLinkPath(), dataLink->GetLinkType());
                }
                else
                    treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetValue());
				
                treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 1, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTEVALUE, treeElementTexte, data, (data->GetType() == 2));
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