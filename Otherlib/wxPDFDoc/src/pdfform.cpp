///////////////////////////////////////////////////////////////////////////////
// Name:        pdfform.cpp
// Purpose:
// Author:      Ulrich Telle
// Created:     2006-01-18
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfform.cpp Implementation of the wxPdfDocument form fields

// For compilers that support precompilation, includes <wx.h>.
#include <header.h>

// includes

#include "wx/pdfdocument.h"
#include "wx/pdffontdetails.h"
#include "wx/pdfform.h"
#include "wx/pdfutility.h"

wxPdfIndirectObject::wxPdfIndirectObject(int objectId, int generationId)
{
	m_type = wxPDF_OBJECT_INDIRECT;
	m_objectId = objectId;
	m_generationId = generationId;
}

wxPdfIndirectObject::~wxPdfIndirectObject()
{
}

wxPdfAnnotationObject::wxPdfAnnotationObject(int objectId, int generationId)
	: wxPdfIndirectObject(objectId, generationId)
{
	SetType(wxPDF_OBJECT_ANNOTATION);
}

wxPdfAnnotationObject::~wxPdfAnnotationObject()
{
}

void
wxPdfAnnotationObject::SetRectangle(double x, double y, double width, double height)
{
	m_x = x;
	m_y = y;
	m_w = width;
	m_h = height;
}

wxPdfAnnotationWidget::wxPdfAnnotationWidget(int objectId, int generationId)
	: wxPdfAnnotationObject(objectId, generationId)
{
	SetType(wxPDF_OBJECT_WIDGET);
}

wxPdfAnnotationWidget::~wxPdfAnnotationWidget()
{
}

wxPdfCheckBox::wxPdfCheckBox(int objectId, int generationId)
	: wxPdfAnnotationWidget(objectId, generationId)
{
	SetType(wxPDF_OBJECT_WIDGET_CHECKBOX);
}

wxPdfCheckBox::~wxPdfCheckBox()
{
}

wxPdfComboBox::wxPdfComboBox(int objectId,
                             int fontindex, double fontsize,
                             int generationId)
	: wxPdfAnnotationWidget(objectId, generationId)
{
	SetType(wxPDF_OBJECT_WIDGET_COMBOBOX);
	m_fontindex = fontindex;
	m_fontsize = fontsize;
}

wxPdfComboBox::~wxPdfComboBox()
{
}

wxPdfPushButton::wxPdfPushButton(int objectId,
                                 int fontindex, double fontsize,
                                 int generationId)
	: wxPdfAnnotationWidget(objectId, generationId)
{
	SetType(wxPDF_OBJECT_WIDGET_PUSHBUTTON);
	m_fontindex = fontindex;
	m_fontsize = fontsize;
}

wxPdfPushButton::~wxPdfPushButton()
{
}

wxPdfRadioButton::wxPdfRadioButton(int objectId, int index, int generationId)
	: wxPdfAnnotationWidget(objectId, generationId)
{
	SetType(wxPDF_OBJECT_WIDGET_RADIOBUTTON);
	m_index = index;
}

wxPdfRadioButton::~wxPdfRadioButton()
{
}

wxPdfRadioGroup::wxPdfRadioGroup(int objectId, const wxString& groupName, int generationId)
	: wxPdfIndirectObject(objectId, generationId)
{
	SetType(wxPDF_OBJECT_RADIOGROUP);
	m_groupName = groupName;
}

wxPdfRadioGroup::~wxPdfRadioGroup()
{
}

void
wxPdfRadioGroup::Add(wxPdfRadioButton* radio)
{
	m_radios.Add(radio);
	radio->SetParent(this);
}

wxPdfTextField::wxPdfTextField(int objectId,
                               int fontindex, double fontsize,
                               const wxString& value, int generationId)
	: wxPdfAnnotationWidget(objectId, generationId)
{
	SetType(wxPDF_OBJECT_WIDGET_TEXTFIELD);
	m_fontindex = fontindex;
	m_fontsize = fontsize;
	m_value = value;
}

wxPdfTextField::~wxPdfTextField()
{
}

// wxPdfDocument methods for forms

void
wxPdfDocument::OutIndirectObject(wxPdfIndirectObject* object)
{
	int objectId = object->GetObjectId();
	int generationId = object->GetGenerationId();
	if (m_offsets->count(objectId - 1) == 0)
	{
		(*m_offsets)[objectId - 1] = m_buffer->TellO();
		OutAscii(wxString::Format(wxS("%d %d obj"), objectId, generationId));
		switch (object->GetType())
		{
		case wxPDF_OBJECT_RADIOGROUP:
			{
				auto obj = static_cast<wxPdfRadioGroup*>(object);
				/// TODO: support for radio button groups
				// Currently radio button groups do not function
				Out("<<");
				Out("/FT /Btn");
				OutAscii(wxString::Format(wxS("/Ff %d"), (1 << 14) | (1 << 15)));
				//          Out("/F 4");
				Out("/T ", false);
				OutAsciiTextstring(obj->GetName());
#if 0
          OutAscii(wxString(wxS("/BS << /W ") +
                   Double2String(obj->GetBorderWidth() +
                   wxString(wxS("/S/")) + obj->GetBorderStyle() +
                   wxString(wxS(">>")));
#endif
				//          OutAscii(wxString(wxS("/MK <</BC [")) + obj->GetBorder() +
				//                   wxString(wxS("]/BG [")) + obj->GetBackground() + wxString(wxS("] /CA ")), false);
				//          OutAsciiTextstring(wxString(wxS("4")), false);
				//          Out(">>");
				Out("/DR 2 0 R");
				Out("/DA ", false);
				//          OutAsciiTextstring(wxString::Format(wxS("/F%d 9 Tf "), m_zapfdingbats) + obj->GetTextColour());
				//          Out("/AP <</N <</Yes <<>>>> >>");
				//          Out("/AS /Off");
				Out("/V /V1");
				Out("/Kids [", false);
				size_t j;
				wxPdfRadioButton* radio;
				wxArrayPtrVoid kids = obj->GetKids();
				for (j = 0; j < kids.GetCount(); j++)
				{
					radio = static_cast<wxPdfRadioButton*>(kids[j]);
					OutAscii(wxString::Format(wxS("%d %d R "), radio->GetObjectId(), radio->GetGenerationId()), false);
				}
				Out("]");
				Out(">>");
				Out("endobj");
				for (j = 0; j < kids.GetCount(); j++)
				{
					radio = static_cast<wxPdfRadioButton*>(kids[j]);
					OutIndirectObject(radio);
				}
			}
			break;

		case wxPDF_OBJECT_WIDGET_CHECKBOX:
			{
				auto obj = static_cast<wxPdfCheckBox*>(object);
				Out("<<");
				Out("/Type /Annot");
				Out("/Subtype /Widget");
				double yPos = obj->GetY();
				if (m_yAxisOriginTop)
				{
					yPos = m_h - yPos;
				}
				OutAscii(wxString(wxS("/Rect [")) +
					wxPdfUtility::Double2String(obj->GetX() * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String(yPos * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String((obj->GetX() + obj->GetWidth()) * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String((yPos - obj->GetHeight()) * m_k, 2) + wxString(wxS("]")));
				Out("/FT /Btn");
				Out("/Ff 0");
				Out("/F 4");
				Out("/T ", false);
				OutAsciiTextstring(obj->GetName());
				// Border style
				OutAscii(wxString(wxS("/BS << /W ")) +
					wxPdfUtility::Double2String(obj->GetBorderWidth(), 2) +
					wxString(wxS("/S/")) + obj->GetBorderStyle() +
					wxString(wxS(">>")));
				OutAscii(wxString(wxS("/MK <</BC [")) + obj->GetBorderColour() +
				         wxString(wxS("]/BG [")) + obj->GetBackgroundColour() + wxString(wxS("] /CA ")), false);
				OutAsciiTextstring(wxString(wxS("4")), false);
				Out(">>");
				Out("/DR 2 0 R");
				Out("/DA ", false);
				OutAsciiTextstring(wxString::Format(wxS("/F%d 9 Tf "), m_zapfdingbats) + obj->GetTextColour());
				Out("/AP <</N <</Yes <<>>>> >>");
				Out("/AS /Off");
				if (obj->GetValue())
				{
					Out("/DV /Yes");
					Out("/V /Yes");
				}
				else
				{
					Out("/DV /Off");
					Out("/V /Off");
				}
				Out(">>");
				Out("endobj");
			}
			break;

		case wxPDF_OBJECT_WIDGET_COMBOBOX:
			{
				auto obj = static_cast<wxPdfComboBox*>(object);
				Out("<<");
				Out("/Type /Annot");
				Out("/Subtype /Widget");
				double yPos = obj->GetY();
				if (m_yAxisOriginTop)
				{
					yPos = m_h - yPos;
				}
				OutAscii(wxString(wxS("/Rect [")) +
					wxPdfUtility::Double2String(obj->GetX() * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String(yPos * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String((obj->GetX() + obj->GetWidth()) * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String((yPos - obj->GetHeight()) * m_k, 2) + wxString(wxS("]")));
				Out("/FT /Ch");
				Out("/Ff 67764224");
				Out("/F 4");
				Out("/T ", false);
				OutAsciiTextstring(obj->GetName());
				OutAscii(wxString(wxS("/MK <</BC [")) + obj->GetBorderColour() +
					wxString(wxS("]/BG [")) + obj->GetBackgroundColour() + wxString(wxS("]>>")));
				// Border style
				OutAscii(wxString(wxS("/BS << /W ")) +
					wxPdfUtility::Double2String(obj->GetBorderWidth(), 2) +
					wxString(wxS("/S/")) + obj->GetBorderStyle() +
					wxString(wxS(">>")));
				Out("/DR 2 0 R");
				Out("/DA ", false);
				OutAsciiTextstring(wxString::Format(wxS("/F%d "), obj->GetFontIndex()) +
					wxPdfUtility::Double2String(obj->GetFontSize(), 2) +
					wxString(wxS(" Tf ")) + obj->GetTextColour());
				wxArrayString options = obj->GetValue();
				Out("/DV ", false);
				OutTextstring(options[0]);
				Out("/V ", false);
				OutTextstring(options[0]);
				// Option list alternative (transfer value, display value)
				//   /Op t [ [(1)(Socks)][(2)(Shoes)] [(3)(Pants)][(4)(Shirt)][(5)(Tie)][(6)(Belt)][(7)(Shorts)]]
				//
				Out("/Opt [", false);
				size_t j;
				for (j = 0; j < options.GetCount(); j++)
				{
					OutTextstring(options[j], false);
				}
				Out("]");
				Out(">>");
				Out("endobj");
			}
			break;

		case wxPDF_OBJECT_WIDGET_PUSHBUTTON:
			{
				auto obj = static_cast<wxPdfPushButton*>(object);
				Out("<<");
				Out("/Type /Annot");
				Out("/Subtype /Widget");
				double yPos = obj->GetY();
				if (m_yAxisOriginTop)
				{
					yPos = m_h - yPos;
				}
				OutAscii(wxString(wxS("/Rect [")) +
					wxPdfUtility::Double2String(obj->GetX() * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String(yPos * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String((obj->GetX() + obj->GetWidth()) * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String((yPos - obj->GetHeight()) * m_k, 2) + wxString(wxS("]")));
				Out("/FT /Btn");
				Out("/Ff 65536"); // (1 << 16),
				Out("/F 4");
				// Highlight button
				//        Out("/H");
				Out("/T ", false);
				OutAsciiTextstring(obj->GetName());
				Out("/BS << /W 1 /S /B >>");
				OutAscii(wxString(wxS("/MK <</BC [")) + obj->GetBorderColour() +
				         wxString(wxS("]/BG [")) + obj->GetBackgroundColour() +
				         wxString(wxS("] /CA ")), false);
				OutTextstring(obj->GetCaption(), false);
				Out(">>");
				Out("/DR 2 0 R");
				Out("/DA ", false);
				OutAsciiTextstring(wxString::Format(wxS("/F%d "), obj->GetFontIndex()) +
					wxPdfUtility::Double2String(obj->GetFontSize(), 2) +
					wxString(wxS(" Tf ")) + obj->GetTextColour());
				Out("/A <</S /JavaScript /JS", false);
				OutTextstring(obj->GetAction());
				Out(">>");
				Out(">>");
				Out("endobj");
			}
			break;

		case wxPDF_OBJECT_WIDGET_RADIOBUTTON:
			{
				auto obj = static_cast<wxPdfRadioButton*>(object);
				Out("<<");
				Out("/Type /Annot");
				Out("/Subtype /Widget");
				double yPos = obj->GetY();
				if (m_yAxisOriginTop)
				{
					yPos = m_h - yPos;
				}
				OutAscii(wxString(wxS("/Rect [")) +
					wxPdfUtility::Double2String(obj->GetX() * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String(yPos * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String((obj->GetX() + obj->GetWidth()) * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String((yPos - obj->GetHeight()) * m_k, 2) + wxString(wxS("]")));
				Out("/FT /Btn");
				Out("/Ff 49152");
				Out("/F 4");
				wxPdfRadioGroup* parent = obj->GetParent();
#if 0
// TODO: integrate radio button groups into PDF document
          OutAscii(wxString::Format(wxS("/Parent [%d %d R]"), parent->GetObjectId(), parent->GetGenerationId()));
//          Out("/P ??? 0 R /H /T ");
#endif
				Out("/T ", false);
				OutAsciiTextstring(parent->GetName());
				Out("/AS /Off");
				OutAscii(wxString(wxS("/MK <</BC [")) + obj->GetBorderColour() +
				         wxString(wxS("]/BG [")) + obj->GetBackgroundColour() +
				         wxString(wxS("] /CA ")), false);
				OutAsciiTextstring(wxString(wxS("l")), false);
				Out(">>");
				// Border style
				OutAscii(wxString(wxS("/BS << /W ")) +
					wxPdfUtility::Double2String(obj->GetBorderWidth(), 2) +
					wxString(wxS("/S/")) + obj->GetBorderStyle() +
					wxString(wxS(">>")));
				Out("/DR 2 0 R");
				Out("/DA ", false);
				OutAsciiTextstring(wxString::Format(wxS("/F%d 9 Tf "), m_zapfdingbats) + obj->GetTextColour());
				OutAscii(wxString::Format(wxS("/AP <</N <</V%d <<>> >> >>"), obj->GetIndex()));
				// Set first button in group as selected
				if (obj->GetIndex() == 1)
				{
					OutAscii(wxString::Format(wxS("/V /V%d"), obj->GetIndex()));
				}
				Out(">>");
				Out("endobj");
			}
			break;

		case wxPDF_OBJECT_WIDGET_TEXTFIELD:
			{
				auto obj = static_cast<wxPdfTextField*>(object);
				Out("<<");
				Out("/Type /Annot");
				Out("/Subtype /Widget");
				double yPos = obj->GetY();
				if (m_yAxisOriginTop)
				{
					yPos = m_h - yPos;
				}
				OutAscii(wxString(wxS("/Rect [")) +
					wxPdfUtility::Double2String(obj->GetX() * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String(yPos * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String((obj->GetX() + obj->GetWidth()) * m_k, 2) + wxString(wxS(" ")) +
					wxPdfUtility::Double2String((yPos - obj->GetHeight()) * m_k, 2) + wxString(wxS("]")));
				Out("/FT /Tx");
				if (obj->GetMultiLine())
				{
					Out("/Ff 4096");
				}
				Out("/F 4");
				Out("/T ", false);
				OutAsciiTextstring(obj->GetName());
				Out("/V ", false);
				OutTextstring(obj->GetValue()); // Current value
				Out("/DV ", false);
				OutTextstring(obj->GetValue()); // Default value
				// Maximal length of text input
				//          OutAscii(wxString::Format(wxS("/MaxLen %d"), obj->GetMaxLength());
				OutAscii(wxString(wxS("/MK <</BC [")) + obj->GetBorderColour() +
					wxString(wxS("]/BG [")) + obj->GetBackgroundColour() + wxString(wxS("]>>")));
				// Border style
				OutAscii(wxString(wxS("/BS << /W ")) +
					wxPdfUtility::Double2String(obj->GetBorderWidth(), 2) +
					wxString(wxS("/S/")) + obj->GetBorderStyle() +
					wxString(wxS(">>")));
				Out("/DR 2 0 R");
				Out("/DA ", false);
				OutAsciiTextstring(wxString::Format(wxS("/F%d "), obj->GetFontIndex()) +
					wxPdfUtility::Double2String(obj->GetFontSize(), 2) +
					wxString(wxS(" Tf ")) + obj->GetTextColour());
				Out(">>");
				Out("endobj");
			}
			break;

		case wxPDF_OBJECT_WIDGET:
		case wxPDF_OBJECT_ANNOTATION:
		case wxPDF_OBJECT_INDIRECT:
		default:
			Out("endobj");
			break;
		}
	}
}

void
wxPdfDocument::CheckBox(const wxString& name, double width, bool checked)
{
	CheckBox(name, m_x, m_y, width, checked);
}

void
wxPdfDocument::CheckBox(const wxString& name, double x, double y, double width, bool checked)
{
	auto field = new wxPdfCheckBox(GetNewObjId());
	field->SetName(name);
	field->SetValue(checked);
	field->SetRectangle(x, y, width, width);
	AddFormField(field);

	// Font ZapfDingBats is required to display check boxes
	LoadZapfDingBats();
}

void
wxPdfDocument::ComboBox(const wxString& name, double width, double height, const wxArrayString& values)
{
	ComboBox(name, m_x, m_y, width, height, values);
}

void
wxPdfDocument::ComboBox(const wxString& name,
                        double x, double y, double width, double height,
                        const wxArrayString& values)
{
	auto field = new wxPdfComboBox(GetNewObjId(), m_currentFont->GetIndex(), m_fontSizePt);
	field->SetName(name);
	field->SetValue(values);
	field->SetRectangle(x, y, width, height);
	AddFormField(field);
}

void
wxPdfDocument::PushButton(const wxString& name, double width, double height,
                          const wxString& caption, const wxString& action)
{
	PushButton(name, m_x, m_y, width, height, caption, action);
}

void
wxPdfDocument::PushButton(const wxString& name,
                          double x, double y, double w, double h,
                          const wxString& caption, const wxString& action)
{
	auto field = new wxPdfPushButton(GetNewObjId(), m_currentFont->GetIndex(), m_fontSizePt);
	field->SetName(name);
	field->SetCaption(caption);
	field->SetAction(action);
	field->SetRectangle(x, y, w, h);
	AddFormField(field);
}

void
wxPdfDocument::RadioButton(const wxString& group, const wxString& name, double width)
{
	RadioButton(group, name, m_x, m_y, width);
}

void
wxPdfDocument::RadioButton(const wxString& group, const wxString& name,
                           double x, double y, double width)
{
	wxPdfRadioGroup* currentGroup;
	wxPdfRadioGroupMap::iterator radioGroup = (*m_radioGroups).find(group);
	if (radioGroup != (*m_radioGroups).end())
	{
		currentGroup = static_cast<wxPdfRadioGroup*>(radioGroup->second);
	}
	else
	{
		//    currentGroup = new wxPdfRadioGroup(GetNewObjId(), group);
		currentGroup = new wxPdfRadioGroup(0, group);
		(*m_radioGroups)[group] = currentGroup;
#if 0
// TODO: integrate radio button groups into PDF document
    int n = (*m_formFields).size()+1;
    (*m_formFields)[n] = currentGroup;
#endif
	}

	auto field = new wxPdfRadioButton(GetNewObjId(), currentGroup->GetCount() + 1);
	field->SetName(name);
	field->SetRectangle(x, y, width, width);
	// TODO: integrate radio button groups into PDF document
	AddFormField(field /*, false*/);
	currentGroup->Add(field);

	// Font ZapfDingBats is required to display radio buttons
	LoadZapfDingBats();
}

void
wxPdfDocument::TextField(const wxString& name, double width, double height,
                         const wxString& value, bool multiline)
{
	TextField(name, m_x, m_y, width, height, value, multiline);
}

void
wxPdfDocument::TextField(const wxString& name,
                         double x, double y, double width, double height,
                         const wxString& value, bool multiline)
{
	auto field = new wxPdfTextField(GetNewObjId(), m_currentFont->GetIndex(), m_fontSizePt, value);
	field->SetName(name);
	field->SetValue(value);
	field->SetMultiLine(multiline);
	field->SetRectangle(x, y, width, height);
	AddFormField(field);
}

void
wxPdfDocument::AddFormField(wxPdfAnnotationWidget* field, bool setFormField)
{
	field->SetBorderColour(m_formBorderColour);
	field->SetBackgroundColour(m_formBackgroundColour);
	field->SetTextColour(m_formTextColour);
	field->SetBorderStyle(m_formBorderStyle);
	field->SetBorderWidth(m_formBorderWidth);
	if (setFormField)
	{
		unsigned int n = static_cast<unsigned int>((*m_formFields).size()) + 1;
		(*m_formFields)[n] = field;
	}

	wxArrayPtrVoid* annotationArray = nullptr;
	wxPdfFormAnnotsMap::iterator formAnnots = (*m_formAnnotations).find(m_page);
	if (formAnnots != (*m_formAnnotations).end())
	{
		annotationArray = formAnnots->second;
	}
	else
	{
		annotationArray = new wxArrayPtrVoid;
		(*m_formAnnotations)[m_page] = annotationArray;
	}
	annotationArray->Add(field);
}

void
wxPdfDocument::SetFormColours(const wxPdfColour& borderColour,
                              const wxPdfColour& backgroundColour,
                              const wxPdfColour& textColour)
{
	m_formBorderColour = borderColour.GetColour(false).BeforeLast(wxS(' '));
	m_formBackgroundColour = backgroundColour.GetColour(false).BeforeLast(wxS(' '));
	m_formTextColour = textColour.GetColour(false);
}

void
wxPdfDocument::SetFormBorderStyle(wxPdfBorderStyle borderStyle, double borderWidth)
{
	switch (borderStyle)
	{
	case wxPDF_BORDER_DASHED: m_formBorderStyle = wxString(wxS("D"));
		break;
	case wxPDF_BORDER_BEVELED: m_formBorderStyle = wxString(wxS("B"));
		break;
	case wxPDF_BORDER_INSET: m_formBorderStyle = wxString(wxS("I"));
		break;
	case wxPDF_BORDER_UNDERLINE: m_formBorderStyle = wxString(wxS("U"));
		break;
	case wxPDF_BORDER_SOLID:
	default: m_formBorderStyle = wxString(wxS("S"));
		break;
	}
	m_formBorderWidth = (borderWidth >= 0) ? borderWidth * m_k : 1;
}

void
wxPdfDocument::LoadZapfDingBats()
{
	if (m_zapfdingbats == 0)
	{
		// Save current font
		wxPdfFontDetails* saveFont = m_currentFont;
		wxString saveFamily = m_fontFamily;
		int saveStyle = m_fontStyle;
		double saveSize = m_fontSizePt;

		// Select font ZapfDingBats
		SelectFont(wxS("ZapfDingBats"), wxS(""), 9, false);
		m_zapfdingbats = m_currentFont->GetIndex();

		// Restore current font
		m_currentFont = saveFont;
		m_fontFamily = saveFamily;
		m_fontStyle = saveStyle;
		m_fontSizePt = saveSize;
		m_fontSize = saveSize / m_k;
	}
}
