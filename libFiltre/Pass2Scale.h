#pragma once

class CPass2Scale
{
public:

    double GetWidth()                   { return m_dWidth; }
    void   SetWidth (double dWidth)     { m_dWidth = dWidth; }

    CPass2Scale()  
	{
		m_dWidth = 1.0f;
	};

    CPass2Scale(double dWidth)  
	{
		m_dWidth = dWidth;
	};

    COLORREF * AllocAndScale(COLORREF *pOrigImage, UINT uOrigWidth, UINT uOrigHeight, UINT uNewWidth, UINT uNewHeight);
    COLORREF * Scale(COLORREF *pOrigImage, UINT uOrigWidth, UINT uOrigHeight, COLORREF *pDstImage, UINT uNewWidth, UINT uNewHeight);

private:

	typedef struct 
	{ 
	   double *Weights;  // Normalized weights of neighboring pixels
	   int Left,Right;   // Bounds of source pixels window
	} ContributionType;  // Contirbution information for a single pixel

	typedef struct 
	{ 
	   ContributionType *ContribRow; // Row (or column) of contribution weights 
	   UINT WindowSize,              // Filter window size (of affecting source pixels) 
			LineLength;              // Length of line (no. or rows / cols) 
	} LineContribType;               // Contribution information for an entire line (row or column)

    LineContribType *AllocContributions (UINT uLineLength, UINT uWindowSize);

    void FreeContributions (LineContribType * p);

    LineContribType *CalcContributions(UINT uLineSize, UINT uSrcSize, double  dScale);

    void ScaleRow(COLORREF *pSrc, UINT uSrcWidth,COLORREF *pRes, UINT uResWidth,UINT uRow, LineContribType *Contrib);
    void HorizScale(COLORREF *pSrc, UINT uSrcWidth,UINT uSrcHeight,COLORREF *pDst,UINT uResWidth,UINT uResHeight);
    void ScaleCol(COLORREF *pSrc, UINT uSrcWidth,COLORREF *pRes, UINT uResWidth,UINT uResHeight,UINT uCol, LineContribType *Contrib);
    void VertScale(COLORREF *pSrc, UINT uSrcWidth, UINT uSrcHeight, COLORREF *pDst, UINT uResWidth, UINT uResHeight);

protected:

	double m_dWidth;

	virtual double Filter(const double &dVal);
};
