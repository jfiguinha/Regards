#include "Bm3dDlg.h"
#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(CBm3dDlg)
	//*)
#endif
//(*InternalHeaders(CopyFileDlg)
#include <wx/xrc/xmlres.h>
#include <wx/dir.h>
#include <RegardsBitmap.h>
#include "bm3d.h"
#include <window_id.h>

//*)

//(*IdInit(CopyFileDlg)
//*)

BEGIN_EVENT_TABLE(CBm3dDlg, wxDialog)
//(*EventTable(CopyFileDlg)
	EVT_IDLE(CBm3dDlg::OnIdle)
	EVT_SIZE(CBm3dDlg::OnSize)
	//*)
END_EVENT_TABLE()

CBm3dDlg::CBm3dDlg(wxWindow* parent, IMultiThreadFiltre * multiThreadProcess)
{
	wxXmlResource::Get()->LoadObject(this,parent,_T("Bm3dDlg"),_T("wxDialog"));
	Gauge1 = (wxGauge*)FindWindow(XRCID("ID_GAUGE1"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	btOK = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	Gauge1->SetRange(100);
	Gauge1->SetValue(0);
	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CBm3dDlg::OnButton1Click);
	Connect(wxEVT_INIT_DIALOG,(wxObjectEventFunction)&CBm3dDlg::OnInit);
    Connect(EVENT_ENDNEWPICTURETHREAD, wxCommandEventHandler(CBm3dDlg::EndProcess));
    Connect(wxEVT_PAINT, wxPaintEventHandler(CBm3dDlg::OnPaint));
	//*)
    this->multiThreadProcess = multiThreadProcess;
	this->SetBackgroundColour(wxColour(64, 64, 64));
    
    int nbProcess = thread::hardware_concurrency();
    
    if(multiThreadProcess->DetermineData(nbProcess) == 0)
    {
        start = true;
        nbLoop = multiThreadProcess->GetNbLoop();
        StaticText2->SetLabel(multiThreadProcess->FilterName());
    }
}

void CBm3dDlg::EndProcess(wxCommandEvent& event)
{
    int numThread = event.GetInt();
    multiThreadProcess->EndThread(numThread);
    start = true;
    nbtache++;
    if(nbtache == nbLoop)
        processEnd = true;
}

CBm3dDlg::~CBm3dDlg()
{
	//(*Destroy(CBm3dDlg)
	//*)
}

void CBm3dDlg::OnSize(wxSizeEvent& evt)
{

}

void CBm3dDlg::OnPaint(wxPaintEvent& event)
{
    start = true;
}

void CBm3dDlg::OnIdle(wxIdleEvent& evt)
{
    if (start && i < nbLoop && !cancel)
    {
        multiThreadProcess->NextStep(this);
        i = multiThreadProcess->GetNumLoop();
        //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        //i++;
        Gauge1->SetRange(nbLoop);
        Gauge1->SetValue(i);
        StaticText2->SetLabel(multiThreadProcess->FilterName() + to_string(i) + "/" + to_string(nbLoop));
        start = false;
    }
    else if (processEnd || cancel)
    {       
        start = false;
        cancel = false;
        //wxMessageBox(finalMessage, caption, wxICON_INFORMATION);
        this->Close();
    }
    
}

bool CBm3dDlg::IsProcessCancel()
{
    return processCancel;
}

void CBm3dDlg::OnInit(wxInitDialogEvent& event)
{
    
}

void CBm3dDlg::OnButton1Click(wxCommandEvent& event)
{
	//this->Close();
    processCancel = true;
    cancel = true;
   
}
