#include "header.h"
#include "2PassScale.h"
#include <appcontext.h>
extern AppContext application_context;

#define BYTE unsigned char

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

std::unique_ptr<LineContribType> C2PassScale::AllocContributions(unsigned int uLineLength, unsigned int uWindowSize)
{
    auto res = std::make_unique<LineContribType>();
    // Init structure header 
    res->WindowSize = uWindowSize;
    res->LineLength = uLineLength;
    // Allocate list of contributions 
    res->ContribRow.resize(uLineLength);
    for (unsigned int u = 0; u < uLineLength; u++)
    {
        // Allocate contributions for every pixel
        res->ContribRow[u].Weights.resize(uWindowSize);
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

std::unique_ptr<LineContribType> C2PassScale::CalcContributions(unsigned int uLineSize, unsigned int uSrcSize, double dScale)
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
    std::unique_ptr<LineContribType> res = AllocContributions(uLineSize, iWindowSize);

    for (unsigned int u = 0; u < uLineSize; u++)
    {   // Scan through line of contributions
        double dCenter = (double)u / dScale;   // Reverse mapping
        // Find the significant edge points that affect the pixel
        int iLeft = max(0, (int)floor((double)dCenter - dWidth));
        int iRight = min((int)ceil((double)dCenter + dWidth), int(uSrcSize) - 1);

        // Cut edge points to fit in filter window in case of spill-off
        if (iRight - iLeft + 1 > iWindowSize)
        {
            // BUGFIX: this was `iLeft < (int(uSrcSize) - 1 / 2)`. Integer
            // division made `1 / 2` evaluate to 0, so the condition was
            // effectively always true and iRight was never trimmed,
            // producing asymmetric edge artifacts on the right/bottom
            // border whenever the filter window spilled off the source.
            if (iLeft < (int(uSrcSize) - 1) / 2)
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
    // Allocate and calculate the contributions.
    // Cache key now also includes m_dWidth: previously a cached ContribH
    // computed for one filter (e.g. Box, m_dWidth=0.5) could be silently
    // reused by a different filter (e.g. Lanczos, m_dWidth=3.0) called on
    // an image of the same dimensions, producing wrong output with no
    // visible error.
    if (uResWidth != m_uOldResWidth || m_uOldSrcWidth != uSrcWidth || m_dOldWidthH != m_dWidth)
    {
        m_ContribH = CalcContributions(uResWidth, uSrcWidth, double(uResWidth) / double(uSrcWidth));
        m_uOldResWidth = uResWidth;
        m_uOldSrcWidth = uSrcWidth;
        m_dOldWidthH = m_dWidth;
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

            const int iLeft = m_ContribH->ContribRow[x].Left;
            const int iRight = m_ContribH->ContribRow[x].Right;
            const double* pWeights = m_ContribH->ContribRow[x].Weights.data();

            for (int i = iLeft; i <= iRight; i++)
            {
                const double weight = pWeights[i - iLeft];
                const unsigned char* pPixel = pSrcRow + i * 3;

                r += weight * application_context.value[*pPixel];
                g += weight * application_context.value[*(pPixel + 1)];
                b += weight * application_context.value[*(pPixel + 2)];
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
    // Allocate and calculate the contributions (see HorizScale for why
    // m_dWidth is part of the cache key).
    if (uResHeight != m_uOldResHeight || m_uOldSrcHeight != uSrcHeight || m_dOldWidthV != m_dWidth)
    {
        m_ContribV = CalcContributions(uResHeight, uSrcHeight, double(uResHeight) / double(uSrcHeight));
        m_uOldResHeight = uResHeight;
        m_uOldSrcHeight = uSrcHeight;
        m_dOldWidthV = m_dWidth;
    }


    const unsigned int uSrcRowBytes = uSrcWidth * 3;
    const unsigned int uResRowBytes = uResWidth * 3;

    // Improved loop order for better cache locality
    for (unsigned int y = 0; y < uResHeight; y++)
    {
        const int iLeft = m_ContribV->ContribRow[y].Left;
        const int iRight = m_ContribV->ContribRow[y].Right;
        const double* pWeights = m_ContribV->ContribRow[y].Weights.data();

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

                r += weight * application_context.value[*pPixel];
                g += weight * application_context.value[*(pPixel + 1)];
                b += weight * application_context.value[*(pPixel + 2)];
            }

            unsigned char* pDstPixel = pDstRow + u * 3;
            pDstPixel[0] = static_cast<BYTE>(min(255.0, max(0.0, r + 0.5)));
            pDstPixel[1] = static_cast<BYTE>(min(255.0, max(0.0, g + 0.5)));
            pDstPixel[2] = static_cast<BYTE>(min(255.0, max(0.0, b + 0.5)));
        }
    }
}

void C2PassScale::Scale(
    unsigned char* pOrigImage,
    unsigned int        uOrigWidth,
    unsigned int        uOrigHeight,
    unsigned char* pDstImage,
    unsigned int        uNewWidth,
    unsigned int        uNewHeight)
{
    if (m_uOldNewWidth != uNewWidth || m_uOldOrigHeight != uOrigHeight)
    {
        // std::vector handles (re)allocation; no manual malloc/free, no
        // leak, and it stays consistent with the RAII style used
        // elsewhere (ContribH/ContribV are now unique_ptr-owned too).
        m_Temp.resize(static_cast<size_t>(uNewWidth) * uOrigHeight * 3);
        m_uOldNewWidth = uNewWidth;
        m_uOldOrigHeight = uOrigHeight;
    }

    HorizScale(pOrigImage,
        uOrigWidth,
        uOrigHeight,
        m_Temp.data(),
        uNewWidth,
        uOrigHeight);

    // Scale temporary image vertically into result image    
    VertScale(m_Temp.data(),
        uNewWidth,
        uOrigHeight,
        pDstImage,
        uNewWidth,
        uNewHeight);
}