#include "header.h"
#include "2PassScale.h"
extern double value[256];

#define BYTE unsigned char

LineContribType* C2PassScale::ContribV = nullptr;
int C2PassScale::olduResHeight = 0;
int C2PassScale::olduSrcHeight = 0;
LineContribType* C2PassScale::ContribH = nullptr;
int C2PassScale::olduResWidth = 0;
int C2PassScale::olduSrcWidth = 0;
unsigned char* C2PassScale::pTemp  = nullptr;
int C2PassScale::olduNewWidth= 0;
int C2PassScale::olduOrigHeight= 0;
    
void C2PassScale::Execute(const cv::Mat& in, cv::Mat& Out)
{
    if (in.empty() || in.channels() != 3)
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
    if (p != nullptr)
    {
        for (unsigned int u = 0; u < p->LineLength; u++)
        {
            // Free contribs for every pixel
            delete[] p->ContribRow[u].Weights;
        }
        delete[] p->ContribRow;    // Free list of pixels contribs
        delete p;                   // Free contribs header
        p = nullptr;
    }

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
    //LineContribType* Contrib = CalcContributions(uResWidth, uSrcWidth, double(uResWidth) / double(uSrcWidth));
       
    // Allocate and calculate the contributions
    if (uResWidth != olduResWidth || olduSrcWidth != uSrcWidth)
    {
        FreeContributions(ContribH);
        ContribH = CalcContributions(uResWidth, uSrcWidth, double(uResWidth) / double(uSrcWidth));
        olduResWidth = uResWidth;
        olduSrcWidth = uSrcWidth;
    }

    const unsigned int uSrcRowBytes = uSrcWidth * 3;
    const unsigned int uResRowBytes = uResWidth * 3;

    for (unsigned int u = 0; u < uResHeight; u++)
    {
        unsigned char* pSrcRow = pSrc + u * uSrcRowBytes;
        unsigned char* pDstRow = pDst + u * uResRowBytes;

        for (unsigned int x = 0; x < uResWidth; x++)
        {
            double r = 0.0;
            double g = 0.0;
            double b = 0.0;

            const int iLeft = ContribH->ContribRow[x].Left;
            const int iRight = ContribH->ContribRow[x].Right;
            double* pWeights = ContribH->ContribRow[x].Weights;

            for (int i = iLeft; i <= iRight; i++)
            {
                const double weight = pWeights[i - iLeft];
                const unsigned char* pPixel = pSrcRow + i * 3;

                r += weight * value[*pPixel];
                g += weight * value[*(pPixel + 1)];
                b += weight * value[*(pPixel + 2)];
            }

            unsigned char* pDstPixel = pDstRow + x * 3;
            pDstPixel[0] = static_cast<BYTE>(min(255.0, max(0.0, r + 0.5)));
            pDstPixel[1] = static_cast<BYTE>(min(255.0, max(0.0, g + 0.5)));
            pDstPixel[2] = static_cast<BYTE>(min(255.0, max(0.0, b + 0.5)));
        }
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
    if (uResHeight != olduResHeight || olduSrcHeight != uSrcHeight)
    {
        FreeContributions(ContribV);
        ContribV = CalcContributions(uResHeight, uSrcHeight, double(uResHeight) / double(uSrcHeight));
		olduResHeight = uResHeight;
		olduSrcHeight = uSrcHeight;
    }
   

    const unsigned int uSrcRowBytes = uSrcWidth * 3;
    const unsigned int uResRowBytes = uResWidth * 3;

    // Improved loop order for better cache locality
    for (unsigned int y = 0; y < uResHeight; y++)
    {
        const int iLeft = ContribV->ContribRow[y].Left;
        const int iRight = ContribV->ContribRow[y].Right;
        double* pWeights = ContribV->ContribRow[y].Weights;

        unsigned char* pDstRow = pDst + y * uResRowBytes;

        for (unsigned int u = 0; u < uResWidth; u++)
        {
            double r = 0.0;
            double g = 0.0;
            double b = 0.0;

            for (int i = iLeft; i <= iRight; i++)
            {
                const double weight = pWeights[i - iLeft];
                const unsigned char* pPixel = pSrc + (i * uSrcWidth + u) * 3;

                r += weight * value[*pPixel];
                g += weight * value[*(pPixel + 1)];
                b += weight * value[*(pPixel + 2)];
            }

            unsigned char* pDstPixel = pDstRow + u * 3;
            pDstPixel[0] = static_cast<BYTE>(min(255.0, max(0.0, r + 0.5)));
            pDstPixel[1] = static_cast<BYTE>(min(255.0, max(0.0, g + 0.5)));
            pDstPixel[2] = static_cast<BYTE>(min(255.0, max(0.0, b + 0.5)));
        }
    }
        // Free contributions structure
}

void C2PassScale::Scale(
    unsigned char* pOrigImage,
    unsigned int        uOrigWidth,
    unsigned int        uOrigHeight,
    unsigned char* pDstImage,
    unsigned int        uNewWidth,
    unsigned int        uNewHeight)
{
    if(olduNewWidth != uNewWidth || olduOrigHeight != uOrigHeight)
    {
        if(pTemp != nullptr)
            free(pTemp);
        pTemp = (unsigned char*)malloc(uNewWidth * uOrigHeight * 3);
        olduNewWidth = uNewWidth;
        olduOrigHeight = uOrigHeight;
    }

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
    

}


