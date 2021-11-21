#include "header.h"
#include "BitmapInfosGps.h"
#include <wx/dcbuffer.h>
#include <wx/sstream.h>
using namespace Regards::Window;
//using namespace Regards::Internet;

CBitmapInfosGps::CBitmapInfosGps(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos& theme)
	: CWindowMain("CBitmapInfosGps", parent, id)
{
	bitmapInfosTheme = theme;
	Connect(wxEVT_PAINT, wxPaintEventHandler(CBitmapInfosGps::on_paint));
}


void CBitmapInfosGps::SetInfos(const wxString& localisation, const wxString& latitude, const wxString& longitude)
{
	this->localisation = localisation;
	this->latitude = latitude;
	this->longitude = longitude;
}


CBitmapInfosGps::~CBitmapInfosGps()
{
	//delete fileGeolocalisation;
}

int CBitmapInfosGps::GetHeight()
{
	return bitmapInfosTheme.GetHeight();
}

void CBitmapInfosGps::UpdateScreenRatio()
{
	Refresh();
}

void CBitmapInfosGps::Redraw()
{
	wxClientDC dc(this);
	DrawInformations(&dc);
}

void CBitmapInfosGps::DrawInformations(wxDC* dc)
{
	wxRect rc = GetWindowRect();
	FillRect(dc, rc, bitmapInfosTheme.colorBack);

	wxString refLat = "N";
	wxString refLong = "E";

	if (latitude[0] == '-')
		refLat = "S";

	if (longitude[0] == '-')
		refLong = "W";

	wxString message = L"Latitude : " + latitude + "-" + refLat + " - Longitude : " + longitude + "-" + refLong;

	wxSize size = GetSizeTexte(dc, localisation, bitmapInfosTheme.themeFont);
	wxSize sizeMessage = GetSizeTexte(dc, message, bitmapInfosTheme.themeFont);

	DrawTexte(dc, localisation, (GetWindowWidth() - size.x) / 2, ((GetWindowHeight() / 2) - size.y) / 2,
	          bitmapInfosTheme.themeFont);

	DrawTexte(dc, message, (GetWindowWidth() - sizeMessage.x) / 2,
	          (GetWindowHeight() / 2) + ((GetWindowHeight() / 2) - sizeMessage.y) / 2, bitmapInfosTheme.themeFont);
}

void CBitmapInfosGps::on_paint(wxPaintEvent& event)
{
	int width = GetWindowWidth();
	int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;

	wxBufferedPaintDC dc(this);
	DrawInformations(&dc);
}
