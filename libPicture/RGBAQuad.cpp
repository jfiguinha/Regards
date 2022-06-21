#include <header.h>
#include "RGBAQuad.h"
#include <algorithm>
float const_floatcolor[256];
bool initfloatvalue = false;

CRgbaquad::CRgbaquad(const uint8_t& red, const uint8_t& green, const uint8_t& blue, const uint8_t& alpha)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
	this->alpha = alpha;

	if (!initfloatvalue)
	{
		for (auto i = 0; i < 256; i++)
			const_floatcolor[i] = static_cast<float>(i);

		initfloatvalue = true;
	}
}

int CRgbaquad::GetIntensity() const
{
    return (red + green + blue) / 3;
}

CRgbaquad::CRgbaquad()
{
	this->red = 0;
	this->green = 0;
	this->blue = 0;
	this->alpha = 0;

	if (!initfloatvalue)
	{
		for (auto i = 0; i < 256; i++)
			const_floatcolor[i] = static_cast<float>(i);

		initfloatvalue = true;
	}
}

CRgbaquad::CRgbaquad(const wxString& hexacolor)
{
	SetHexaColor(hexacolor);
}

CRgbaquad::~CRgbaquad()
{
}


void CRgbaquad::SetColor(const uint8_t& color)
{
	this->red = color;
	this->green = color;
	this->blue = color;
	this->alpha = 0;
}

std::vector<wxString> CRgbaquad::Split(const wxString& s, char seperator)
{
	std::vector<wxString> output;

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		wxString substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}


void CRgbaquad::SetHexaColor(const wxString& hexacolor)
{
	vector<wxString> listcolor = Split(hexacolor, ';');
	if (listcolor.size() == 3)
	{
		red = atoi(listcolor[0]);
		green = atoi(listcolor[1]);
		blue = atoi(listcolor[2]);
	}
}

wxString CRgbaquad::GetHexacolor()
{
	char chex[255];
	sprintf(chex, "%d;%d;%d", red, green, blue);
	return chex;
}

void CRgbaquad::Add(const float& value)
{
	red += static_cast<uint8_t>(value);
	green += static_cast<uint8_t>(value);
	blue += static_cast<uint8_t>(value);
}

void CRgbaquad::Add(const CRgbaquad& value)
{
	red += value.red;
	green += value.green;
	blue += value.blue;
}

void CRgbaquad::Sub(const CRgbaquad& value)
{
	red -= value.red;
	green -= value.green;
	blue -= value.blue;
}

void CRgbaquad::Mul(const uint8_t& value)
{
	red *= value;
	green *= value;
	blue *= value;
}

void CRgbaquad::Mul(const float& value)
{
	red = static_cast<uint8_t>(const_floatcolor[red] * value);
	green = static_cast<uint8_t>(const_floatcolor[green] * value);
	blue = static_cast<uint8_t>(const_floatcolor[blue] * value);
}

void CRgbaquad::Div(const float& value)
{
	red = static_cast<uint8_t>(const_floatcolor[red] / value);
	green = static_cast<uint8_t>(const_floatcolor[green] / value);
	blue = static_cast<uint8_t>(const_floatcolor[blue] / value);
}

void CRgbaquad::Sub(const uint8_t& value)
{
	red = value - red;
	green = value - green;
	blue = value - blue;
}

void CRgbaquad::Min(const uint8_t& value)
{
	red = min(value, red);
	green = min(value, green);
	blue = min(value, blue);
}

void CRgbaquad::Min(CRgbaquad value)
{
	red = min(value.GetRed(), red);
	green = min(value.GetGreen(), green);
	blue = min(value.GetBlue(), blue);
}

void CRgbaquad::Max(const uint8_t& value)
{
	red = max(value, red);
	green = max(value, green);
	blue = max(value, blue);
}

void CRgbaquad::Max(CRgbaquad value)
{
	red = max(value.GetRed(), red);
	green = max(value.GetGreen(), green);
	blue = max(value.GetBlue(), blue);
}

bool CRgbaquad::operator==(const CRgbaquad& other)
{
	if (other.blue == blue && other.green == green && other.red == red)
		return true;

	return false;
}

float CRgbaquad::GetFRed() const
{
	return const_floatcolor[red];
}

float CRgbaquad::GetFGreen() const
{
	return const_floatcolor[green];
}

float CRgbaquad::GetFBlue() const
{
	return const_floatcolor[blue];
}

float CRgbaquad::GetFAlpha() const
{
	return const_floatcolor[alpha];
}

float CRgbaquad::GetGreyValue() const
{
	return static_cast<float>(const_floatcolor[red] * 0.299 + const_floatcolor[green] * 0.587 + const_floatcolor[blue] *
		0.114);
}

void CRgbaquad::SetRed(const uint8_t& red)
{
	this->red = clamp(red, 0, 255);
}

void CRgbaquad::SetGreen(const uint8_t& green)
{
	this->green = clamp(green, 0, 255);
}

void CRgbaquad::SetBlue(const uint8_t& blue)
{
	this->blue = clamp(blue, 0, 255);
}

void CRgbaquad::SetAlpha(const uint8_t& alpha)
{
	this->alpha = clamp(alpha, 0, 255);
}

void CRgbaquad::SetColorWithAlpha(CRgbaquad* color)
{
	float alpha = color->GetFAlpha() / 255.0f;
	float alphaDiff = 1.0f - alpha;
	blue = static_cast<uint8_t>(GetFBlue() * alphaDiff + color->GetFBlue() * alpha);
	green = static_cast<uint8_t>(GetFGreen() * alphaDiff + color->GetFGreen() * alpha);
	red = static_cast<uint8_t>(GetFRed() * alphaDiff + color->GetFRed() * alpha);

	this->red = clamp(red, 0, 255);
	this->green = clamp(green, 0, 255);
	this->blue = clamp(blue, 0, 255);
	this->alpha = clamp(alpha, 0, 255);
}

int CRgbaquad::clamp(const int& val, const int& minval, const int& maxval)
{
	//std::clamp(val, minval, maxval);
	return min(max(val, minval), maxval);
}

void CRgbaquad::SetColor(const uint8_t& red, const uint8_t& green, const uint8_t& blue, const uint8_t& alpha)
{
	this->red = clamp(red, 0, 255);
	this->green = clamp(green, 0, 255);
	this->blue = clamp(blue, 0, 255);
	this->alpha = clamp(alpha, 0, 255);
}


uint8_t CRgbaquad::GetRed() const
{
	return red;
}

uint8_t CRgbaquad::GetGreen() const
{
	return green;
}

uint8_t CRgbaquad::GetBlue() const
{
	return blue;
}

uint8_t CRgbaquad::GetAlpha() const
{
	return alpha;
}
