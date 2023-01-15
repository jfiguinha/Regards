#pragma once
class CRegardsBitmap;
class CRgbaquad;

class CLine
{
public:
	CLine(const int& heightMax, const int& widthMax);
	~CLine();
	void MidpointLine(cv::Mat* bitmap, const int& xFrom, const int& yFrom, const int& xTo, const int& yTo,
	                  const CRgbaquad& m_color, const float& fAlpha, const bool& m_bAntialiasing);

private:
	void SetAlphaColorValue(const int& xFrom, const int& yFrom, const int& x, const int& y, const float& fAlpha,
	                        const CRgbaquad& m_color, cv::Mat* bitmap);

	int widthMax, heightMax;
};
