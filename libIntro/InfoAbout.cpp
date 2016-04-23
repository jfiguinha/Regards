#include "InfoAbout.h"
#include "TreeElementTexte.h"
using namespace Regards::Introduction;

CInfoAbout::CInfoAbout(CThemeTree * theme)
{
	themeTree = *theme;
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	themeTree.SetMargeX(5);
	rowWidth.push_back(0);
	rowWidth.push_back(0);
};



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

	CTreeData * treeDataWidth = new CTreeData();
	treeDataWidth->SetIsParent(false);
	treeDataWidth->SetKey("Version");
	treeDataWidth->SetValue("2.0.0");
	tr.append_child(child, treeDataWidth);

	CTreeData * treeDataAnnee = new CTreeData();
	treeDataAnnee->SetIsParent(false);
	treeDataAnnee->SetKey("Copyright");
	treeDataAnnee->SetValue("2015");
	tr.append_child(child, treeDataAnnee);

	
	CTreeData * treeDataLib = new CTreeData();
	treeDataLib->SetKey("External Library");
	child = tr.insert(top, treeDataLib);

/*
	CTreeData * treeFFMPEG = new CTreeData();
	treeFFMPEG->SetIsParent(false);
	treeFFMPEG->SetKey("FFMPEG");
	treeFFMPEG->SetValue("2.6.2");
	tr.append_child(child, treeFFMPEG);
*/
	CTreeData * treeSQLite = new CTreeData();
	treeSQLite->SetIsParent(false);
	treeSQLite->SetKey("SQLite");
	treeSQLite->SetValue("3.8.5");
	tr.append_child(child, treeSQLite);

	CTreeData * treeExiv = new CTreeData();
	treeExiv->SetIsParent(false);
	treeExiv->SetKey("Exiv");
	treeExiv->SetValue("0.24");
	tr.append_child(child, treeExiv);

	CTreeData * treeRapipXML = new CTreeData();
	treeRapipXML->SetIsParent(false);
	treeRapipXML->SetKey("Rapid XML");
	treeRapipXML->SetValue("1.13");
	tr.append_child(child, treeRapipXML);

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
	treelibRaw->SetKey("libRaw");
	treelibRaw->SetValue("0.17");
	tr.append_child(child, treelibRaw);
/*
	CTreeData * treelibBPG = new CTreeData();
	treelibBPG->SetIsParent(false);
	treelibBPG->SetKey("libBPG");
	treelibBPG->SetValue("0.9.5");
	tr.append_child(child, treelibBPG);
*/
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

			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

			widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			if (data->GetValue() != "")
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