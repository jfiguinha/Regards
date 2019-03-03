#include "header.h"
#include "ScrollInterface.h"
#include "ScrollbarWnd.h"
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
using namespace Regards::Window;

CScrollInterface::CScrollInterface()
{
	defaultPageSize = 50;
	defaultLineSize = 5;
	scrollbar = nullptr;

}


CScrollInterface::~CScrollInterface()
{
}


void CScrollInterface::MoveLeft()
{
	int pageSize = defaultPageSize;
	if (scrollbar->GetHScrollbar() != nullptr)
	{
		CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();
		pageSize = scrollH->GetPageSize();
	}
	posLargeur -= pageSize;
	TestMaxX();

	this->Update();
}

void CScrollInterface::MoveRight()
{
	int pageSize = defaultPageSize;
	if (scrollbar->GetHScrollbar() != nullptr)
	{
		CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();
		pageSize = scrollH->GetPageSize();
	}
	posLargeur += pageSize;
	TestMaxX();

	this->Update();
}

void CScrollInterface::MoveTop()
{
	int pageSize = defaultPageSize;
	if (scrollbar->GetVScrollbar() != nullptr)
	{
		CScrollbarVerticalWnd * scrollV = scrollbar->GetVScrollbar();
		pageSize = scrollV->GetPageSize();
	}
	posHauteur -= pageSize;
	TestMaxY();

	this->Update();
}

void CScrollInterface::MoveBottom()
{
	int pageSize = defaultPageSize;
	if (scrollbar->GetVScrollbar() != nullptr)
	{
		CScrollbarVerticalWnd * scrollV = scrollbar->GetVScrollbar();
		pageSize = scrollV->GetPageSize();
	}
	posHauteur += pageSize;
	TestMaxY();
	this->Update();
}

void CScrollInterface::SetScrollbar(CScrollbarWnd * scrollbar)
{
	this->scrollbar = scrollbar;
}

void CScrollInterface::SetTopPosition(const int &top)
{
	posHauteur = top;
	//TestMaxY();
	this->Update();
}

void CScrollInterface::SetLeftPosition(const int &left)
{
	posLargeur = left;
	TestMaxX();
	this->Update();
}