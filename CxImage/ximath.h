#if !defined(__ximath_h)
#define __ximath_h

#include "ximadef.h"

//***bd*** simple floating point point
class DLL_EXP CxPoint2
{
public:
	CxPoint2();
	CxPoint2(float x_, float y_);
	CxPoint2(const CxPoint2& p);

	float Distance(CxPoint2 p2);
	float Distance(float x_, float y_);

	float x, y;
};

//and simple rectangle
class DLL_EXP CxRect2
{
public:
	CxRect2();
	CxRect2(float x1_, float y1_, float x2_, float y2_);
	CxRect2(const CxPoint2& bl, const CxPoint2& tr);
	CxRect2(const CxRect2& p);

	float Surface() const;
	CxRect2 CrossSection(const CxRect2& r2) const;
	CxPoint2 Center() const;
	float Width() const;
	float Height() const;

	CxPoint2 botLeft;
	CxPoint2 topRight;
};

#endif
