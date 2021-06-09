#pragma once
#include <BitmapInterface.h>
class CImageLoadingFormat;

enum
{
	TIMER_TRANSITION = 1,
	TIMER_SELECTEFFECT = 2,
	TIMER_CLICK = 3
};


class CBitmapViewerImpl
{

public:

	CBitmapViewerImpl()
	{
		etape = 0;
		m_bTransition = false;
		isDiaporama = false;
		invertColor = false;
		etape = 0;
		fixArrow = true;
		nextPicture = nullptr;
		startTransition = false;
		bitmapInterface = bitmapInterface;
		oldMouse.x = 0;
		oldMouse.y = 0;
		

	}

	~CBitmapViewerImpl()
	{

		
	}

	wxCursor hCursorCross;
	
	CImageLoadingFormat* nextPicture;
	wxImage arrowPrevious;
	wxImage arrowNext;
	bool startTransition;
	int etape;
	bool fixArrow;
	CBitmapInterface* bitmapInterface;

	wxPoint oldMouse;

	//Thread Parameter
	bool m_bTransition;

	bool isDiaporama;
	bool isInUse;
	bool invertColor;

	int typeClick = 0;
	wxTimer* transitionTimer;
	wxTimer* selectEffectTimer;
	wxTimer* clickTimer;
	bool isNext = false;

};