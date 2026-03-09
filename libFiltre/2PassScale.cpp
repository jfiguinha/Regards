#include "header.h"
#include "2PassScale.h"
extern double value[256];

#ifndef WIN32
#define RGB(r,g,b)          ((uint32_t)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))
#endif

void C2PassScale::Execute(const cv::Mat& in, cv::Mat& Out)
{
    if (in.empty() || Out.empty() || in.rows <= 0 || in.cols <= 0 || Out.rows <= 0 || Out.cols <= 0)
    {
        throw std::invalid_argument("Les matrices d'entrée ou de sortie sont invalides.");
    }
	Scale((uint32_t*)in.data, in.cols, in.rows, (uint32_t*)Out.data, Out.cols, Out.rows);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void C2PassScale::Scale(uint32_t *pOrigImage, int uOrigWidth, int uOrigHeight, uint32_t *pDstImage, int uNewWidth, int uNewHeight)
{
    // Scale source image horizontally into temporary image
    uint32_t *pTemp = new uint32_t [uNewWidth * uOrigHeight];
    HorizScale (pOrigImage, 
                uOrigWidth,
                uOrigHeight,
                pTemp,
                uNewWidth,
                uOrigHeight);

    // Scale temporary image vertically into result image    
    VertScale ( pTemp,
                uNewWidth,
                uOrigHeight,
                pDstImage,
                uNewWidth,
                uNewHeight);
    delete [] pTemp;

}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////
C2PassScale::LineContribType * C2PassScale::AllocContributions (int uLineLength, int uWindowSize)
{
    LineContribType *res = new LineContribType; 
        // Init structure header 
    res->WindowSize = uWindowSize; 
    res->LineLength = uLineLength; 
        // Allocate list of contributions 
    res->ContribRow = new ContributionType[uLineLength];
    for (int u = 0 ; u < uLineLength ; u++) 
    {
        // Allocate contributions for every pixel
        res->ContribRow[u].Weights = new double[uWindowSize];
    }
    return res; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void C2PassScale::FreeContributions (LineContribType * p)
{
    for (int u = 0; u < p->LineLength; u++) 
    {
        // Free contribs for every pixel
        delete [] p->ContribRow[u].Weights;
    }
    delete [] p->ContribRow;    // Free list of pixels contribs
    delete p;                   // Free contribs header
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////
double C2PassScale::Filter(const double &dVal)
{
    double m_dLocalVal = fabs(dVal); 
    return (m_dLocalVal < m_dWidth ? m_dWidth - m_dLocalVal : 0.0); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////
C2PassScale::LineContribType * C2PassScale::CalcContributions(int uLineSize, int uSrcSize, double  dScale)
{
    double dWidth;
    double dFScale = 1.0;

    if (dScale < 1.0) 
    {    // Minification
        dWidth = m_dWidth / dScale; 
        dFScale = dScale; 
    } 
    else
    {    // Magnification
        dWidth= m_dWidth; 
    }
 
    // Window size is the number of sampled pixels
    int iWindowSize = 2 * (int)ceil(dWidth) + 1; 
 
    // Allocate a new line contributions strucutre
    LineContribType *res = AllocContributions (uLineSize, iWindowSize); 
 
    for (int u = 0; u < uLineSize; u++) 
    {   // Scan through line of contributions
        double dCenter = (double)u / dScale;   // Reverse mapping
        // Find the significant edge points that affect the pixel
        int iLeft = max (0, (int)floor (dCenter - dWidth)); 
        res->ContribRow[u].Left = iLeft; 
        int iRight = min ((int)ceil (dCenter + dWidth), int(uSrcSize) - 1); 
        res->ContribRow[u].Right = iRight;
 
        // Cut edge points to fit in filter window in case of spill-off
        if (iRight - iLeft + 1 > iWindowSize) 
        {
            if (iLeft < (int(uSrcSize) - 1 / 2)) 
            {
                iLeft++; 
            }
            else 
            {
                iRight--; 
            }
        }
        double dTotalWeight = 0.0;  // Zero sum of weights
        for (int iSrc = iLeft; iSrc <= iRight; iSrc++)
        {   // Calculate weights
            dTotalWeight += (res->ContribRow[u].Weights[iSrc-iLeft] =  
                                dFScale * Filter(dFScale * (dCenter - (double)iSrc))); 
        }
        //ASSERT (dTotalWeight >= 0.0);   // An error in the filter function can cause this 
        if (dTotalWeight > 0.0)
        {   // Normalize weight of neighbouring points
            for (int iSrc = iLeft; iSrc <= iRight; iSrc++)
            {   // Normalize point
                res->ContribRow[u].Weights[iSrc-iLeft] /= dTotalWeight; 
            }
        }
   } 
   return res; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void C2PassScale::HorizScale(uint32_t *pSrc, int uSrcWidth,int uSrcHeight,uint32_t *pDst,int uResWidth,int uResHeight)
{
    //TRACE ("Performing horizontal scaling...\n"); 
    if (uResWidth == uSrcWidth)
    {   // No scaling required, just copy
		memcpy(pDst, pSrc, sizeof (uint32_t) * uSrcHeight * uSrcWidth);
    }
    // Allocate and calculate the contributions
    LineContribType * Contrib = CalcContributions (uResWidth, uSrcWidth, double(uResWidth) / double(uSrcWidth)); 

    double r = 0;
    double g = 0;
    double b = 0;
    int iLeft = 0;    // Retrieve left boundries
    int iRight = 0;  // Retrieve right boundries

    for (int u = 0; u < uResHeight; u++)
    {   // Step through rows        
        uint32_t* pSrcRow = &(pSrc[u * uSrcWidth]);
        uint32_t* pDstRow = &(pDst[u * uResWidth]);

        for (int x = 0; x < uResWidth; x++)
        {   // Loop through row
            r = 0;
            g = 0;
            b = 0;
            iLeft = Contrib->ContribRow[x].Left;    // Retrieve left boundries
            iRight = Contrib->ContribRow[x].Right;  // Retrieve right boundries
            for (int i = iLeft; i <= iRight; i++)
            {   // Scan between boundries
                // Accumulate weighted effect of each neighboring pixel

                r += (Contrib->ContribRow[x].Weights[i - iLeft] * value[GetRValue(pSrcRow[i])]);
                g += (Contrib->ContribRow[x].Weights[i - iLeft] * value[GetGValue(pSrcRow[i])]);
                b += (Contrib->ContribRow[x].Weights[i - iLeft] * value[GetBValue(pSrcRow[i])]);
            }

            pDstRow[x] = RGB(GetValue(r), GetValue(g), GetValue(b)); // Place result in destination pixel
        }
    }
    FreeContributions (Contrib);  // Free contributions structure
}

uint8_t C2PassScale::GetValue(double r)
{
    if (r < 0)
        return 0;
    else if (r > 255)
        return 255;
    else
        return (uint8_t)r;

}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void C2PassScale::VertScale(uint32_t *pSrc, int uSrcWidth, int uSrcHeight, uint32_t *pDst, int uResWidth, int uResHeight)
{
    if (uSrcHeight == uResHeight)
    {   // No scaling required, just copy
        memcpy(pDst, pSrc, sizeof (uint32_t) * uSrcHeight * uSrcWidth);
    }
    // Allocate and calculate the contributions
    LineContribType * Contrib = CalcContributions (uResHeight, uSrcHeight, double(uResHeight) / double(uSrcHeight)); 

    double r = 0;
    double g = 0;
    double b = 0;
    int iLeft = 0;    // Retrieve left boundries
    int iRight = 0;  // Retrieve right boundries

    for (int u = 0; u < uResWidth; u++)
    {   // Step through columns
        for (int y = 0; y < uResHeight; y++)
        {    // Loop through column
            r = 0;
            g = 0;
            b = 0;
            iLeft = Contrib->ContribRow[y].Left;    // Retrieve left boundries
            iRight = Contrib->ContribRow[y].Right;  // Retrieve right boundries
            for (int i = iLeft; i <= iRight; i++)
            {   // Scan between boundries
                // Accumulate weighted effect of each neighboring pixel

                //A mettre dans un tableau pour Traitement SSE

                uint32_t pCurSrc = pSrc[i * uSrcWidth + u];
                r += (Contrib->ContribRow[y].Weights[i - iLeft] * value[GetRValue(pCurSrc)]);
                g += (Contrib->ContribRow[y].Weights[i - iLeft] * value[GetGValue(pCurSrc)]);
                b += (Contrib->ContribRow[y].Weights[i - iLeft] * value[GetBValue(pCurSrc)]);
            }


            pDst[y * uResWidth + u] = RGB(GetValue(r), GetValue(g), GetValue(b));   // Place result in destination pixel
        }
    }
    FreeContributions (Contrib);     // Free contributions structure
}
