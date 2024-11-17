#pragma once

struct Points
{
    int x, y;
};

class CRgbaquad
{
public:
	CRgbaquad(const uint8_t& red, const uint8_t& green, const uint8_t& blue, const uint8_t& alpha = 0);

	CRgbaquad(const std::string& hexacolor);

	/****Définition du constructeur de copie***/
	CRgbaquad(const CRgbaquad& source) //lobjet source est protégé par const   
	{
		/*réservation de lespace mémoire dans le tas puis copie des valeurs aux adresses correspondantes*/
		red = source.red;
		green = source.green;
		blue = source.blue;
		alpha = source.alpha;
	}


	CRgbaquad();

	~CRgbaquad();

	static int GetPosition(cv::Mat* bitmapMatrix, const int& x, const int& y);

	static CRgbaquad* GetPtColorValue(cv::Mat* bitmapMatrix, const int& x, const int& y);

	static CRgbaquad GetColorValue(cv::Mat* bitmapMatrix, const int& x, const int& y);

	void SetHexaColor(const std::string & hexacolor);

	void SetColor(const uint8_t& color);

	float GetGreyValue() const;

	std::string GetHexacolor();

	bool operator==(const CRgbaquad& other);

	CRgbaquad& operator+(const CRgbaquad& color1)
	{
		this->blue += color1.GetBlue();
		this->green += color1.GetGreen();
		this->red += color1.GetRed();
		return *this;
	}


	void Add(const float& value);

	void Add(const CRgbaquad& value);

	void Sub(const uint8_t& value);

	void Sub(const CRgbaquad& value);

	void Mul(const uint8_t& value);

	void Mul(const float& value);

	void Div(const float& value);

	void Min(const uint8_t& value);

	void Min(CRgbaquad value);

	void Max(const uint8_t& value);

	void Max(CRgbaquad value);

	float GetFRed() const;

	float GetFGreen() const;

	float GetFBlue() const;

	float GetFAlpha() const;

	void SetRed(const uint8_t& alpha);

	void SetGreen(const uint8_t& alpha);

	void SetBlue(const uint8_t& alpha);

	void SetAlpha(const uint8_t& alpha);

	void SetColorWithAlpha(CRgbaquad* color);

	void SetColor(const uint8_t& red, const uint8_t& green, const uint8_t& blue, const uint8_t& alpha = 0);

	uint8_t GetRed() const;

	uint8_t GetGreen() const;

	uint8_t GetBlue() const;

	uint8_t GetAlpha() const;

	int GetIntensity() const;

private:
	std::vector<std::string> Split(const std::string& s, char seperator);
	int clamp(const int& val, const int& minval, const int& maxval);

#if _MSC_VER >= 1800
	uint8_t blue = 0;
	uint8_t green = 0;
	uint8_t red = 0;
	uint8_t alpha = 0;
#else
	uint8_t blue;
	uint8_t green;
	uint8_t red;
	uint8_t alpha;
#endif
};


#define COLOR_RGBA_WHITE CRgbaquad(255,255,255,0)
#define COLOR_RGBA_GREY156 CRgbaquad(156, 156, 156, 0);
#define COLOR_RGBA_GREY CRgbaquad(128,128,128,0)
#define COLOR_RGBA_GREY74 CRgbaquad(74, 74, 74,0)
#define COLOR_RGBA_BLACKGREY CRgbaquad(74, 74, 74,0)
#define COLOR_RGBA_RED CRgbaquad(255, 0, 0,0)
#define COLOR_RGBA_GREY29 CRgbaquad(29, 29, 29, 0)
#define COLOR_RGBA_GREY30 CRgbaquad(30,30,30,0)
#define COLOR_RGBA_GREY34 CRgbaquad(34,34,34,0)
#define COLOR_RGBA_GREY128 CRgbaquad(128,128,128,0)
#define COLOR_RGBA_GREY222 CRgbaquad(222, 222, 222, 0)
#define COLOR_RGBA_GREY223 CRgbaquad(223, 223, 223, 0)
#define COLOR_RGBA_BLACK CRgbaquad(0,0,0,0)
#define COLOR_RGBA_GREY200 CRgbaquad(200, 200, 200, 0)
#define COLOR_RGBA_BLACKBLACKGREY  CRgbaquad(29, 29, 29,0)
