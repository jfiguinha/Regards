#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "ofxScannerKit.h"

class wxImagePanel;

class CScannerWindow: public wxDialog
{
	
public:
    CScannerWindow(wxWindow *parent, wxWindowID id,
        const wxString &title,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxSize(500, 400),
        const long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    ~CScannerWindow();
	//void setup();
	//void update();
	//void draw();
	//void exit();

    void OnCameraRemoved(wxCommandEvent& event);
    void OnCameraReady(wxCommandEvent& event);
    void OnPhotoDownloaded(wxCommandEvent& event);
    void OnCameraFound(wxCommandEvent& event);
    void OnOpenScanner(wxCommandEvent& event);
    void OnSelectScanner(wxCommandEvent& event);
    void OnScan(wxCommandEvent& event);
    void OnPreview(wxCommandEvent& event);
    //void OnSize(wxSizeEvent &event);
    //void OnPaint(wxPaintEvent &event);
    /*
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);		
    void deviceEvent(ofxDeviceEvent &e);
    */
    //vector <SimpleButton> buttons;
    
private:
    wxPanel * MakeSettingsPanel(wxWindow *parent);
     wxPanel * MakePreviewPanel(wxWindow *parent);
    wxImagePanel * panelPreview;
    ofxScannerKit          camera;
    ofxScannerDevice *     activeDevice;
    wxImage               latestImage;
    wxChoice *choice;
};

