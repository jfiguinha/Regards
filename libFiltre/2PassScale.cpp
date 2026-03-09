#include "header.h"
#include "2PassScale.h"
extern double value[256];

#define BYTE unsigned char


void C2PassScale::Execute(const cv::Mat& in, cv::Mat& Out)
{
    if (in.empty())
    {
        return;
    }
    if (in.channels() != 3)
    {
        return;
    }
    if (Out.empty())
    {
        Out.create(in.size(), in.type());
    }
    Scale((unsigned char*)in.data,
        in.cols,
        in.rows,
        (unsigned char*)Out.data,
        Out.cols,
		Out.rows);
}

LineContribType* C2PassScale::AllocContributions(unsigned int uLineLength, unsigned int uWindowSize)
{
    LineContribType* res = new LineContribType;
    // Init structure header 
    res->WindowSize = uWindowSize;
    res->LineLength = uLineLength;
    // Allocate list of contributions 
    res->ContribRow = new ContributionType[uLineLength];
    for (unsigned int u = 0; u < uLineLength; u++)
    {
        // Allocate contributions for every pixel
        res->ContribRow[u].Weights = new double[uWindowSize];
    }
    return res;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////
double C2PassScale::Filter(const double& dVal)
{
    double m_dLocalVal = fabs(dVal);
    return (m_dLocalVal < m_dWidth ? m_dWidth - m_dLocalVal : 0.0);
}

void C2PassScale::FreeContributions(LineContribType* p)
{
    for (unsigned int u = 0; u < p->LineLength; u++)
    {
        // Free contribs for every pixel
        delete[] p->ContribRow[u].Weights;
    }
    delete[] p->ContribRow;    // Free list of pixels contribs
    delete p;                   // Free contribs header
}


LineContribType* C2PassScale::CalcContributions(unsigned int uLineSize, unsigned int uSrcSize, double dScale)
{
    double dWidth;
    double dFScale = 1.0;
    double dFilterWidth = m_dWidth;

    if (dScale < 1.0)
    {    // Minification
        if (dScale == 0) dScale = 1;
        dWidth = dFilterWidth / dScale;
        dFScale = dScale;
    }
    else
    {    // Magnification
        dWidth = dFilterWidth;
    }

    // Window size is the number of sampled pixels
    int iWindowSize = 2 * (int)ceil((double)dWidth) + 1;

    // Allocate a new line contributions strucutre
    LineContribType* res = AllocContributions(uLineSize, iWindowSize);

    for (unsigned int u = 0; u < uLineSize; u++)
    {   // Scan through line of contributions
        double dCenter = (double)u / dScale;   // Reverse mapping
        // Find the significant edge points that affect the pixel
        int iLeft = max(0, (int)floor((double)dCenter - dWidth));
        int iRight = min((int)ceil((double)dCenter + dWidth), int(uSrcSize) - 1);

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
        res->ContribRow[u].Left = iLeft;
        res->ContribRow[u].Right = iRight;

        double dTotalWeight = 0.0;  // Zero sum of weights
        int iSrc;
        for (iSrc = iLeft; iSrc <= iRight; iSrc++)
        {   // Calculate weights
            dTotalWeight += (res->ContribRow[u].Weights[iSrc - iLeft] =
                dFScale * Filter(dFScale * (dCenter - (double)iSrc)));
        }

        if (dTotalWeight > 0.0)
        {   // Normalize weight of neighbouring points
            for (iSrc = iLeft; iSrc <= iRight; iSrc++)
            {   // Normalize point
                res->ContribRow[u].Weights[iSrc - iLeft] /= dTotalWeight;
            }
        }
    }
    return res;
}


void C2PassScale::ScaleRow(unsigned char* pSrc,
    unsigned int                uSrcWidth,
    unsigned char* pRes,
    unsigned int                uResWidth,
    unsigned int                uRow,
    LineContribType* Contrib)
{
    unsigned char* pSrcRow = &(pSrc[uRow * uSrcWidth * 3]);
    unsigned char* pDstRow = &(pRes[uRow * uResWidth * 3]);
    for (unsigned int x = 0; x < uResWidth; x++)
    {   // Loop through row
        float r = 0;
        float g = 0;
        float b = 0;
        int iLeft = Contrib->ContribRow[x].Left;    // Retrieve left boundries
        int iRight = Contrib->ContribRow[x].Right;  // Retrieve right boundries
        for (int i = iLeft; i <= iRight; i++)
        {   // Scan between boundries
            // Accumulate weighted effect of each neighboring pixel
            r += (float)(Contrib->ContribRow[x].Weights[i - iLeft] * value[pSrcRow[i * 3]]);
            g += (float)(Contrib->ContribRow[x].Weights[i - iLeft] * value[pSrcRow[i * 3 + 1]]);
            b += (float)(Contrib->ContribRow[x].Weights[i - iLeft] * value[pSrcRow[i * 3 + 2]]);
        }
        pDstRow[x * 3] = (BYTE)(r + 0.5) & 0xff; // Place result in destination pixel
        pDstRow[x * 3 + 1] = (BYTE)(g + 0.5) & 0xff;
        pDstRow[x * 3 + 2] = (BYTE)(b + 0.5) & 0xff;
    }
}


void C2PassScale::HorizScale(unsigned char* pSrc,
    unsigned int                uSrcWidth,
    unsigned int                uSrcHeight,
    unsigned char* pDst,
    unsigned int                uResWidth,
    unsigned int                uResHeight)
{
    if (uResWidth == uSrcWidth)
    {   // No scaling required, just copy
        memcpy(pDst, pSrc, 3 * uSrcHeight * uSrcWidth);
        return;
    }
    // Allocate and calculate the contributions
    LineContribType* Contrib = CalcContributions(uResWidth, uSrcWidth, double(uResWidth) / double(uSrcWidth));
    for (unsigned int u = 0; u < uResHeight; u++)
    {   // Step through rows
        ScaleRow(pSrc,
            uSrcWidth,
            pDst,
            uResWidth,
            u,
            Contrib);    // Scale each row 
    }
    FreeContributions(Contrib);  // Free contributions structure
}


void C2PassScale::ScaleCol(unsigned char* pSrc,
    unsigned int                uSrcWidth,
    unsigned char* pRes,
    unsigned int                uResWidth,
    unsigned int                uResHeight,
    unsigned int                uCol,
    LineContribType* Contrib)
{
    for (unsigned int y = 0; y < uResHeight; y++)
    {    // Loop through column
        float r = 0;
        float g = 0;
        float b = 0;
        int iLeft = Contrib->ContribRow[y].Left;    // Retrieve left boundries
        int iRight = Contrib->ContribRow[y].Right;  // Retrieve right boundries
        for (int i = iLeft; i <= iRight; i++)
        {   // Scan between boundries
            // Accumulate weighted effect of each neighboring pixel
            unsigned char* pCurSrc = &pSrc[(i * uSrcWidth + uCol) * 3];
            r += float(Contrib->ContribRow[y].Weights[i - iLeft] * value[*pCurSrc]);
            g += float(Contrib->ContribRow[y].Weights[i - iLeft] * value[*(pCurSrc + 1)]);
            b += float(Contrib->ContribRow[y].Weights[i - iLeft] * value[*(pCurSrc + 2)]);
        }
        int off = (y * uResWidth + uCol) * 3;
        pRes[off] = (BYTE)(r + 0.5) & 0xff;   // Place result in destination pixel
        pRes[off + 1] = (BYTE)(g + 0.5) & 0xff;
        pRes[off + 2] = (BYTE)(b + 0.5) & 0xff;
    }
}



void C2PassScale::VertScale(unsigned char* pSrc,
    unsigned int                uSrcWidth,
    unsigned int                uSrcHeight,
    unsigned char* pDst,
    unsigned int                uResWidth,
    unsigned int                uResHeight)
{
    

    if (uSrcHeight == uResHeight)
    {   // No scaling required, just copy
        memcpy(pDst, pSrc, 3 * uSrcHeight * uSrcWidth);
        return;
    }
    // Allocate and calculate the contributions
    LineContribType* Contrib = CalcContributions(uResHeight, uSrcHeight, double(uResHeight) / double(uSrcHeight));
    for (unsigned int u = 0; u < uResWidth; u++)
    {   // Step through columns
        ScaleCol(pSrc,
            uSrcWidth,
            pDst,
            uResWidth,
            uResHeight,
            u,
            Contrib);   // Scale each column
    }
    FreeContributions(Contrib);     // Free contributions structure
}

void C2PassScale::Scale(
    unsigned char* pOrigImage,
    unsigned int        uOrigWidth,
    unsigned int        uOrigHeight,
    unsigned char* pDstImage,
    unsigned int        uNewWidth,
    unsigned int        uNewHeight)
{
    // Scale source image horizontally into temporary image
    unsigned char* pTemp = (unsigned char*)malloc(uNewWidth * uOrigHeight * 3);
    HorizScale(pOrigImage,
        uOrigWidth,
        uOrigHeight,
        pTemp,
        uNewWidth,
        uOrigHeight);

    // Scale temporary image vertically into result image    
    VertScale(pTemp,
        uNewWidth,
        uOrigHeight,
        pDstImage,
        uNewWidth,
        uNewHeight);
    free(pTemp);

}


