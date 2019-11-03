#include "ScannerWindow.h"
#include "event_scanner.h"
#include <wxImagePanel.h>
#include <FileUtility.h>
#include <wx/gbsizer.h>
#include <libPicture.h>
#include <ImageLoadingFormat.h>
enum
{
    ID_BUT_SCAN = 3000,
    ID_BUT_PREVIEW,
};


CScannerWindow::CScannerWindow(wxString name, wxWindow* parent, wxWindowID id)
    : wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0)
{
	//ofSetVerticalSync(true);
	wxString file;
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
    if (!wxMkDir(tempFolder)) {
#else
	wxString tempFolder = documentPath + "/temp";
    if (!wxMkDir(tempFolder,  wxS_DIR_DEFAULT)) {
#endif
	
		// handle the error here
	}
    
    activeDevice = NULL;
    
    camera.listCameras();
    camera.setDownloadFolder(tempFolder.ToStdString());
    camera.parent = this;
    //ofAddListener(camera.deviceEvents, this, &testApp::deviceEvent);
    
    cout << "download path: " << camera.downloadPath << endl;
    
    wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
    hsizer->Add(MakeSettingsPanel(this), 1, wxEXPAND | wxALL, 5);
    hsizer->Add(MakePreviewPanel(this), 1, wxEXPAND | wxALL, 5);

    wxBoxSizer *bsizer = new wxBoxSizer(wxHORIZONTAL);
    bsizer->Add(10, 10, 1, wxEXPAND);
    wxButton *defBut;
    bsizer->Add(defBut = new wxButton(this, ID_BUT_SCAN, _("&Scan")), 0, wxALIGN_RIGHT | wxALL, 5);
    defBut->SetDefault();
    bsizer->Add(new wxButton(this, ID_BUT_PREVIEW, _("&Preview")), 0, wxALIGN_RIGHT | wxALL, 5);
    bsizer->Add(new wxButton(this, wxID_CANCEL, _("Cancel")), 0, wxALIGN_RIGHT | wxALL, 5);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(hsizer, 1, wxEXPAND | wxALL, 5);
    sizer->Add(bsizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    SetSizer(sizer);
    sizer->SetSizeHints(this);
    
    Connect(EVENT_CK_CAMERA_REMOVED, wxCommandEventHandler(CScannerWindow::OnCameraRemoved));
    Connect(EVENT_CK_CAMERA_READY, wxCommandEventHandler(CScannerWindow::OnCameraReady));
    Connect(EVENT_CK_PHOTO_DOWNLOADED, wxCommandEventHandler(CScannerWindow::OnPhotoDownloaded));
    Connect(EVENT_CK_ALL_CAMERAS_FOUND, wxCommandEventHandler(CScannerWindow::OnCameraFound));
}

wxPanel * CScannerWindow::MakeSettingsPanel(wxWindow *parent)
{
	wxPanel *panel = new wxPanel(parent);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	wxGridBagSizer *gsizer = NULL;

	wxStaticBox *sbox = new wxStaticBox(panel, wxID_ANY, wxString("List of Scanner : "));
	wxStaticBoxSizer *sbsizer = new wxStaticBoxSizer(sbox, wxVERTICAL);
	gsizer = new wxGridBagSizer(5, 5);
	sbsizer->Add(gsizer, 0, wxEXPAND | wxALL, 5);
	sizer->Add(sbsizer, 0, wxEXPAND | wxALL, 5);

	unsigned int row = gsizer->GetEffectiveRowsCount();

	gsizer->Add(new wxStaticText(panel, wxID_ANY, wxString("Scanner") + _T(":")),
		wxGBPosition(row, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);

	choice = new wxChoice(panel, wxID_ANY);

	choice->SetSelection(0);
	gsizer->Add(choice, wxGBPosition(row, 1), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
	gsizer->Add(new wxButton(panel, ID_BUT_SCAN, _("&Scan")), wxGBPosition(row, 2), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
	panel->SetSizer(sizer);
	sizer->SetSizeHints(panel);
	return panel;
}

wxPanel * CScannerWindow::MakePreviewPanel(wxWindow *parent)
{
    panelPreview = new wxImagePanel(parent);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    
    panelPreview->SetSizer(sizer);
    sizer->SetSizeHints(panelPreview);

    return panelPreview;
}

void CScannerWindow::OnCameraRemoved(wxCommandEvent& event)
{
    ofxCameraDevice * device = (ofxCameraDevice *)event.GetClientData();
    if(device != nullptr)
    {
        if(activeDevice == device->device) {
            activeDevice = NULL;
        }
    }
    delete device;
        
}

void CScannerWindow::OnCameraReady(wxCommandEvent& event)
{
    ofxCameraDevice * device = (ofxCameraDevice *)event.GetClientData();
    if(device != nullptr)
        device->enableTethering();
}

void CScannerWindow::OnPhotoDownloaded(wxCommandEvent& event)
{
    //ofxPhotoDownloaded * photoDown
    ofxPhotoDownloaded * photoDown = (ofxPhotoDownloaded *)event.GetClientData();
    if(photoDown != nullptr)
    {
        CLibPicture libPicture;
        CImageLoadingFormat * imageLoading = libPicture.LoadPicture(photoDown->photoPath);
        wxImage * image = imageLoading->GetwxImage();
        latestImage = *image;
        delete imageLoading;
        delete image;
        delete photoDown;
        //printf("downloading: %s", photoPath);
        //latestImage.LoadFile(photoPath);       
    }
}

void CScannerWindow::OnCameraFound(wxCommandEvent& event)
{
    cout << "Listing all devices %i" << camera.devices.size() << endl;
    for(int i=0; i<camera.devices.size(); i++) 
    {
        //cout << "["<<i<<"] " << camera.devices[i]->name << endl;
        choice->Append(camera.devices[i]->name);
        //SimpleButton btn;
        //btn.set(0, 0, 30, 30);
        //buttons.push_back(btn);
    }
}

/*
//--------------------------------------------------------------
void CScannerWindow::setup() {


    
}
*/
/*
//--------------------------------------------------------------
void CScannerWindow::deviceEvent(ofxDeviceEvent &e) {

    if(e.eventType == ofxCameraKit::CK_ALL_CAMERAS_FOUND) {
        
        cout << "Listing all devices %i" << camera.devices.size() << endl;
        for(int i=0; i<camera.devices.size(); i++) {
            cout << "["<<i<<"] " << camera.devices[i]->name << endl;
            
            SimpleButton btn;
            btn.set(0, 0, 30, 30);
            buttons.push_back(btn);
        }
        
    }
    
    if(e.eventType == ofxCameraKit::CK_CAMERA_REMOVED) {
        if(activeDevice == e.device->device) {
            activeDevice = NULL;
        }
    }
    
    if(e.eventType == ofxCameraKit::CK_CAMERA_READY) {
        e.device->enableTethering();
    }
    
    
    if(e.eventType == ofxCameraKit::CK_PHOTO_DOWNLOADED) {
        printf("downloading: %s", e.photoPath.c_str());
        latestImage.loadImage(e.photoPath);
    }
    
}
 * */

/*
//--------------------------------------------------------------
void CScannerWindow::exit() {
}

//--------------------------------------------------------------
void CScannerWindow::update(){
    
}


//--------------------------------------------------------------

void CScannerWindow::draw(){
    
    
    ofSetColor(25);
    ofDrawBitmapString("click the square to open the device\nthen f to take a picture from that device", 10, 20);

    for(int i=0; i<camera.devices.size(); i++) {
        
        float sy = 100;
        buttons[i].set(10, sy+(i*30), 20, 20);
        
        buttons[i].bPressed ? ofSetColor(100, 200, 0) : ofSetColor(25);
        ofFill();
        ofRect(buttons[i].x, buttons[i].y, 20, 20);
        ofSetColor(25);
        ofDrawBitmapString(camera.devices[i]->name, 35, sy+15+(i*30));
        
        
        if(camera.devices[i]->isOpen) {
            ofDrawBitmapString("press f to take picture", 235, sy+15+(i*30));
        }
    }
    
    if(latestImage.IsOk()) {
        float w = 500;
        float h = w * ((float)latestImage.getHeight()/(float)latestImage.getWidth());
        ofSetColor(255);
        latestImage.draw((ofGetWidth()-w)/2,
                         (ofGetHeight()-h)/2, w, h);              
    }
}
*/
/*
//--------------------------------------------------------------
void CScannerWindow::keyPressed(int key){
    if(key == 'f' && camera.devices.size()>0) {
        if(activeDevice) activeDevice->takePicture();    
    }
}

//--------------------------------------------------------------
void CScannerWindow::keyReleased(int key){
	
}

//--------------------------------------------------------------
void CScannerWindow::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void CScannerWindow::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void CScannerWindow::mousePressed(int x, int y, int button){
    for(int i=0; i<camera.devices.size(); i++) {
        if(buttons[i].inside(x, y)) {
            buttons[i].bPressed = !buttons[i].bPressed;
            ofxCameraDevice * c = camera.devices[i];
            activeDevice = c;
            if(buttons[i].bPressed) {
                c->openDevice();
            }
            else c->closeDevice();
        }
    }
}

//--------------------------------------------------------------
void CScannerWindow::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void CScannerWindow::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void CScannerWindow::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void CScannerWindow::dragEvent(ofDragInfo dragInfo){ 
	
}
*/