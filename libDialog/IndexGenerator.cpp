#include <header.h>
#include "IndexGenerator.h"
#include <wx/colordlg.h>
//(*InternalHeaders(IndexGenerator)
#include <wx/xrc/xmlres.h>
#include <wx/dcmemory.h>
#include <wx/msgdlg.h>
#include <LibResource.h>
//*)

//(*IdInit(IndexGenerator)
//*)

BEGIN_EVENT_TABLE(IndexGenerator, wxDialog)
	//(*EventTable(IndexGenerator)
	//*)
END_EVENT_TABLE()

IndexGenerator::IndexGenerator(wxWindow* parent)
{
	//(*Initialize(IndexGenerator)
	wxXmlResource::Get()->LoadObject(this, parent,_T("IndexGenerator"),_T("wxDialog"));
	btOK = static_cast<wxButton*>(FindWindow(XRCID("ID_BUTTON1")));
	btCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_BUTTON2")));
	StaticBox1 = static_cast<wxStaticBox*>(FindWindow(XRCID("ID_STATICBOX1")));
	StaticText1 = static_cast<wxStaticText*>(FindWindow(XRCID("ID_STATICTEXT1")));
	StaticText2 = static_cast<wxStaticText*>(FindWindow(XRCID("ID_STATICTEXT2")));
	txtWidth = static_cast<wxTextCtrl*>(FindWindow(XRCID("ID_TEXTCTRL1")));
	txtHeight = static_cast<wxTextCtrl*>(FindWindow(XRCID("ID_TEXTCTRL2")));
	StaticText3 = static_cast<wxStaticText*>(FindWindow(XRCID("ID_STATICTEXT3")));
	spPictureLine = static_cast<wxSpinCtrl*>(FindWindow(XRCID("ID_SPINCTRL1")));
	StaticBitmap1 = static_cast<wxStaticBitmap*>(FindWindow(XRCID("ID_STATICBITMAP1")));
	StaticText4 = static_cast<wxStaticText*>(FindWindow(XRCID("ID_STATICTEXT4")));
	btColor = static_cast<wxButton*>(FindWindow(XRCID("ID_BUTTON3")));
	txtTitle = static_cast<wxTextCtrl*>(FindWindow(XRCID("ID_TEXTCTRL3")));
	isOk = false;
	Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&IndexGenerator::OnbtOkClick);
	Connect(XRCID("ID_BUTTON2"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&IndexGenerator::OnbtCancelClick);
	Connect(XRCID("ID_BUTTON3"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&IndexGenerator::OnbtColorClick);

	wxBitmap bitmap(StaticBitmap1->GetSize().x, StaticBitmap1->GetSize().y);
	wxMemoryDC memdc;
	memdc.SelectObject(bitmap);
	memdc.SetBackground(*wxBLACK_BRUSH);
	memdc.Clear(); //fills the entire bitmap with green colour
	memdc.SelectObject(wxNullBitmap);
	StaticBitmap1->SetBitmap(bitmap);
	//*)
}

IndexGenerator::~IndexGenerator()
{
	//(*Destroy(IndexGenerator)
	//*)
}


wxString IndexGenerator::GetIndexTitle()
{
	return txtTitle->GetLineText(0).c_str();
}

bool IndexGenerator::IsOk()
{
	return isOk;
}

void IndexGenerator::SetThumbnailSize(const int& width, const int& height)
{
	txtWidth->SetValue(wxString::Format(wxT("%d"), width));
	txtHeight->SetValue(wxString::Format(wxT("%d"), height));
}

void IndexGenerator::OnbtOkClick(wxCommandEvent& event)
{
	isOk = true;
	//Validation data
	wxString width = txtWidth->GetLineText(0);
	wxString height = txtHeight->GetLineText(0);
	wxString title = txtTitle->GetLineText(0);
	wxString title_error = CLibResource::LoadStringFromResource(L"indexerror", 1);
	if (!isdigit(width[0]))
	{
		isOk = false;
		wxString label = CLibResource::LoadStringFromResource(L"errorwidthsize", 1);
		wxMessageBox(label, title_error, wxICON_ERROR);
	}

	if (!isdigit(height[0]))
	{
		isOk = false;
		wxString label = CLibResource::LoadStringFromResource(L"errorheightsize", 1);
		wxMessageBox(label, title_error, wxICON_ERROR);
	}

	if (title.size() < 3)
	{
		isOk = false;
		wxString label = CLibResource::LoadStringFromResource(L"errortitlesize", 1);
		wxMessageBox(label, title_error, wxICON_ERROR);
	}

	if (isOk)
		this->Close();
}

void IndexGenerator::OnbtCancelClick(wxCommandEvent& event)
{
	this->Close();
}

void IndexGenerator::OnbtColorClick(wxCommandEvent& event)
{
	wxColourDialog colorDialog(this);
	if (colorDialog.ShowModal() == wxID_OK)
	{
		wxColourData color = colorDialog.GetColourData();
		colour = color.GetColour();


		wxBitmap bitmap(StaticBitmap1->GetSize().x, StaticBitmap1->GetSize().y);
		wxMemoryDC memdc;
		memdc.SelectObject(bitmap);
		memdc.SetBackground(colour);
		memdc.Clear(); //fills the entire bitmap with green colour
		memdc.SelectObject(wxNullBitmap);
		StaticBitmap1->SetBitmap(bitmap);
	}
}

wxColour IndexGenerator::GetColor()
{
	return colour;
}

int IndexGenerator::GetThumbnailSize(int& width, int& height)
{
	width = atoi(txtWidth->GetLineText(0).c_str());
	height = atoi(txtHeight->GetLineText(0).c_str());
	return 0;
}

int IndexGenerator::GetNbPictureByLine()
{
	return spPictureLine->GetValue();
}
