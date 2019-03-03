#include <header.h>
#include "Rotation.h"
#include "RotateByShearRGB.h"
#include <RegardsBitmap.h>
using namespace Regards::FiltreEffet;

CRotation::CRotation(void)
{
}


CRotation::~CRotation(void)
{
}

void CRotation::Rotate(CRegardsBitmap * pBitmap,const float &angle, const CRgbaquad &clrBack)
{
	CRgbaquad * m_OriginalBitmapBits = (CRgbaquad *)pBitmap->GetPtBitmap();
	wxSize m_size = wxSize(pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
	wxSize m_sizedst;

	CRotateByShearRGB Rot(CRotateByShearRGB::Progress);
	CRgbaquad * m_ScaledBitmapBits = Rot.AllocAndRotate (
										m_OriginalBitmapBits,
										m_size,
										angle,
										&m_sizedst,
										clrBack
										);


	pBitmap->SetBitmap(m_ScaledBitmapBits, m_sizedst.x, m_sizedst.y);

	delete[] m_ScaledBitmapBits;
}

