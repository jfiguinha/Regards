#ifndef CONFIGREGARDS_H
#define CONFIGREGARDS_H

#ifndef WX_PRECOMP
//(*HeadersPCH(ConfigRegards)
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
#endif
//(*Headers(ConfigRegards)
#include <wx/spinctrl.h>
//*)

class ConfigRegards : public wxDialog
{
public:
	ConfigRegards(wxWindow* parent);
	~ConfigRegards() override;

	//(*Declarations(ConfigRegards)
	wxButton* btOk;
	// NOTE: ID_SBDIAPORAMA / ID_STATICBOX2 n'existent pas (encore ?) dans ConfigRegards.xrc
	// (verifie le 2026-08-18). FindWindow renverra nullptr tant que l'xrc n'est pas mis a jour
	// avec un attribut name="..." sur les wxStaticBoxSizer correspondants.
	wxStaticBox* sbDiaporama = nullptr;

	wxTextCtrl* txtMusicDiaporamaPath;
	wxButton* btMusicDiaporamaPath;

	wxSpinCtrl* scTime;
	wxSpinCtrl* scProcessFace;
	wxSpinCtrl* scProcessExif;
	wxSpinCtrl* scProcessThumbnail;

	wxComboBox* rbTransitionEffect;
	wxButton* btCancel;
	wxStaticText* stTime;
	wxStaticBox* sbThumbnail = nullptr; // voir NOTE sbDiaporama ci-dessus
	wxRadioBox* rbDatabaseInMemory;
	wxRadioBox* rbAutoRotate;
	wxRadioBox* rbContrastCorrection;


	wxRadioBox* rbUSESUPERDNN;
	wxComboBox* cbUSESUPERDNNFILTER;

	wxComboBox* rbTransitionDiaporamaEffect;
	wxComboBox* rbInterpolation;
	wxComboBox* cbHardwareAccelerator;

	wxRadioBox* rbOpenCLOpenGLInterop;
	wxComboBox* rbVideoEncoderHard;
	wxComboBox* rbVideoDecoderHard;

	//ID_RBVIDEOFACEDETECTION
	wxRadioBox* rbVideoFaceDetection;
	wxRadioBox* rbFaceDetection;
	//*)

	wxComboBox* rbSkin;

	wxTextCtrl* txtPicturePath;
	wxButton* btPicturePath;
	wxTextCtrl* txtVideoPath;
	wxButton* btVideoPath;

	wxTextCtrl* txtAPIKey;
	bool IsOk();

protected:
	//(*Identifiers(ConfigRegards)
	//*)

private:
	void OnBtnMusicDiaporamaClick(wxCommandEvent& event);
	void OnbtnPathVideoClick(wxCommandEvent& event);
	void OnBtnPathPictureClick(wxCommandEvent& event);

	// Factorise l'ouverture d'un wxFileDialog + mise a jour du wxTextCtrl cible
	// (utilise par les 3 handlers ci-dessus).
	void SelectFile(const wxString& label, const wxString& wildcard, wxTextCtrl* target,
	                 const wxString& defaultFilename = wxEmptyString);

	// Les radio box ci-dessous stockent leur valeur "inversee" par rapport au parametre
	// (selection 0 <-> parametre 1, et inversement). Ces deux helpers centralisent la
	// conversion pour eviter les erreurs de logique (cf. rbFaceDetection).
	static int InvertBinary(int value);
	static void SetInvertedRadioSelection(wxRadioBox* radio, int paramValue);
	static int GetInvertedRadioValue(wxRadioBox* radio);

	//(*Handlers(ConfigRegards)
	void init();
	void OnbtnOkClick(wxCommandEvent& event);
	void OnBtnCancelClick(wxCommandEvent& event);
	//*)
	bool isOk;
	wxString oldAccelerator;
	DECLARE_EVENT_TABLE()
};

#endif
