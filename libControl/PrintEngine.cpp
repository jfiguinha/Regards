#include <header.h>
#include "PrintEngine.h"
using namespace Regards::Print;

wxPrintData * CPrintEngine::g_printData = nullptr;
wxPageSetupDialogData * CPrintEngine::g_pageSetupData = nullptr;

void CPrintEngine::Initialize()
{
	if (g_printData == nullptr)
		g_printData = new wxPrintData();

	if (g_pageSetupData == nullptr)
		g_pageSetupData = new wxPageSetupDialogData();

	// init global objects
	// -------------------

	// You could set an initial paper size here
#if 0
	g_printData->SetPaperId(wxPAPER_LETTER); // for Americans
	g_printData->SetPaperId(wxPAPER_A4);    // for everyone else
#endif

	// copy over initial paper size from print record
	(*g_pageSetupData) = *g_printData;

	// Set some initial page margins in mm.
	g_pageSetupData->SetMarginTopLeft(wxPoint(15, 15));
	g_pageSetupData->SetMarginBottomRight(wxPoint(15, 15));
}

void CPrintEngine::Kill()
{
	if (g_printData != nullptr)
		delete(g_printData);

	if (g_pageSetupData != nullptr)
		delete(g_pageSetupData);
}

wxPrintData * CPrintEngine::GetPrintData()
{
	return g_printData;
}

wxPageSetupDialogData * CPrintEngine::GetPageSetupDialogData()
{
	return g_pageSetupData;
}