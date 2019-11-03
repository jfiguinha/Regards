#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "ofxCameraKit.h"

class wxImagePanel;

class CScannerWindow : public wxWindow
{
	
public:
    CScannerWindow(wxString name, wxWindow* parent, wxWindowID id);
    
	//void setup();
	//void update();
	//void draw();
	//void exit();

    void OnCameraRemoved(wxCommandEvent& event);
    void OnCameraReady(wxCommandEvent& event);
    void OnPhotoDownloaded(wxCommandEvent& event);
    void OnCameraFound(wxCommandEvent& event);
    
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
    ofxCameraKit          camera;
    ofxCameraDevice *     activeDevice;
    wxImage               latestImage;
    wxChoice *choice;
};

