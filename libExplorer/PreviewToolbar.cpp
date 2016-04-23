#include "PreviewToolbar.h"
#include <libResource.h>
#include <effect.h>
using namespace Regards::Window;
using namespace Regards::Explorer;


CPreviewToolbar::CPreviewToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface)
	: CToolbarWindow(parent, id, theme)
{

	this->toolbarInterface = toolbarInterface;

	wxString libelleFirst = CLibResource::LoadStringFromResource("LBLFIRST",1);
	wxString libellePrevious = CLibResource::LoadStringFromResource("LBLPREVIOUS", 1);
	wxString libelleRotate90 = CLibResource::LoadStringFromResource("LBLROTATE90", 1);
	wxString libelleRotate270 = CLibResource::LoadStringFromResource("LBLROTATE270", 1);
	wxString libelleFlipV = CLibResource::LoadStringFromResource("LBLFLIPV", 1);
	wxString libelleFlipH = CLibResource::LoadStringFromResource("LBLFLIPH", 1);
	wxString libelleNext = CLibResource::LoadStringFromResource("LBLNEXT", 1);
	wxString libelleLast = CLibResource::LoadStringFromResource("LBLLAST", 1);

	CToolbarButton * imageFirst = new CToolbarButton(themeToolbar.button);
	imageFirst->SetButtonResourceId("IDB_ARROWTRACKLPNG");
	imageFirst->SetLibelle(libelleFirst);
	imageFirst->SetCommandId(WM_IMAGES_FIRST);
	navElement.push_back(imageFirst);

	CToolbarButton * imagePrec = new CToolbarButton(themeToolbar.button);
	imagePrec->SetButtonResourceId("IDB_ARROWLPNG");
	imagePrec->SetLibelle(libellePrevious);
	imagePrec->SetCommandId(WM_IMAGES_PRCDENTE);
	imagePrec->SetRepeatable(true);
	navElement.push_back(imagePrec);

	CToolbarButton * rotate90 = new CToolbarButton(themeToolbar.button);
	rotate90->SetButtonResourceId("IDB_ROTATION90");
	rotate90->SetCommandId(WM_ROTATE90);
	rotate90->SetLibelle(libelleRotate90);
	navElement.push_back(rotate90);

	CToolbarButton * rotate180 = new CToolbarButton(themeToolbar.button);
	rotate180->SetButtonResourceId("IDB_ROTATION270");
	rotate180->SetCommandId(WM_ROTATE270);
	rotate180->SetLibelle(libelleRotate270);
	navElement.push_back(rotate180);

	CToolbarButton * flipVertical = new CToolbarButton(themeToolbar.button);
	flipVertical->SetButtonResourceId("IDB_FLIPVERT");
	flipVertical->SetCommandId(WM_FLIPVERTICAL);
	flipVertical->SetLibelle(libelleFlipV);
	navElement.push_back(flipVertical);

	CToolbarButton * flipHorizontal = new CToolbarButton(themeToolbar.button);
	flipHorizontal->SetButtonResourceId("IDB_FLIPHORZ");
	flipHorizontal->SetCommandId(WM_FLIPHORIZONTAL);
	flipHorizontal->SetLibelle(libelleFlipH);
	navElement.push_back(flipHorizontal);

	CToolbarButton * imageSuiv = new CToolbarButton(themeToolbar.button);
	imageSuiv->SetButtonResourceId("IDB_ARROWRPNG");
	imageSuiv->SetCommandId(WM_IMAGES_SUIVANTE);
	imageSuiv->SetRepeatable(true);
	imageSuiv->SetLibelle(libelleNext);
	navElement.push_back(imageSuiv);

	CToolbarButton * imageEnd = new CToolbarButton(themeToolbar.button);
	imageEnd->SetButtonResourceId("IDB_ARROWTRACKRPNG");
	imageEnd->SetCommandId(WM_IMAGES_END);
	imageEnd->SetLibelle(libelleLast);
	navElement.push_back(imageEnd);
}



CPreviewToolbar::~CPreviewToolbar()
{
}


void CPreviewToolbar::EventManager(const int &id)
{
	toolbarInterface->ClickShowButton(id);

}




