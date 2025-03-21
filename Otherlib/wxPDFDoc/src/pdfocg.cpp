///////////////////////////////////////////////////////////////////////////////
// Name:        pdfocg.cpp
// Purpose:
// Author:      Ulrich Telle
// Created:     2009-07-02
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfocg.cpp Implementation of the wxPdfDocument classes for handling optional content groups

// For compilers that support precompilation, includes <wx.h>.
#include <header.h>

// includes

#include "wx/pdfdocument.h"
#include "wx/pdflayer.h"
#include "wx/pdfobjects.h"

// --- OCG handling

wxPdfLayer*
wxPdfDocument::AddLayer(const wxString& layerName)
{
	auto layer = new wxPdfLayer(layerName);
	int index = static_cast<int>(m_ocgs->size() + 1);
	layer->SetIndex(index);
	(*m_ocgs)[index] = layer;
	return layer;
}

wxPdfLayer*
wxPdfDocument::AddLayerTitle(const wxString& layerTitle)
{
	wxPdfLayer* layer = wxPdfLayer::CreateTitle(layerTitle);
	int index = static_cast<int>(m_ocgs->size() + 1);
	layer->SetIndex(index);
	(*m_ocgs)[index] = layer;
	return layer;
}

wxPdfLayerMembership*
wxPdfDocument::AddLayerMembership()
{
	auto layerMembership = new wxPdfLayerMembership();
	int index = static_cast<int>(m_ocgs->size() + 1);
	layerMembership->SetIndex(index);
	(*m_ocgs)[index] = layerMembership;
	return layerMembership;
}

void
wxPdfDocument::AddLayerRadioGroup(const wxPdfLayerGroup& radioGroup)
{
	size_t n = m_rgLayers->size() + 1;
	(*m_rgLayers)[n] = new wxPdfLayerGroup(radioGroup);
}

void
wxPdfDocument::LockLayer(wxPdfLayer* layer)
{
	if (m_lockedLayers == nullptr)
	{
		m_lockedLayers = new wxPdfLayerGroup();
	}
	m_lockedLayers->Add(layer);
}

void
wxPdfDocument::EnterLayer(wxPdfLayer* layer)
{
	wxPdfLayer* currentLayer = layer;
	if (currentLayer->GetType() == wxPDF_OCG_TYPE_LAYER)
	{
		int n = 0;
		while (currentLayer != nullptr)
		{
			if (currentLayer->GetType() == wxPDF_OCG_TYPE_LAYER)
			{
				Out("/OC ", false);
				OutAscii(wxString::Format(wxS("/L%d"), currentLayer->GetIndex()), false);
				Out(" BDC");
				++n;
			}
			currentLayer = currentLayer->GetParent();
		}
		m_layerDepth.Add(n);
	}
	else
	{
		wxLogError(wxString(wxS("wxPdfDocument::EnterLayer: ")) +
			wxString(_("A title is not a layer.")));
	}
}

void
wxPdfDocument::EnterLayer(wxPdfLayerMembership* layer)
{
	m_layerDepth.Add(1);
	Out("/OC ", false);
	OutAscii(wxString::Format(wxS("/L%d"), layer->GetIndex()), false);
	Out(" BDC");
}

void
wxPdfDocument::LeaveLayer()
{
	int n = 1;
	if (!m_layerDepth.IsEmpty())
	{
		n = m_layerDepth.Last();
		m_layerDepth.RemoveAt(m_layerDepth.GetCount() - 1);
	}
	else
	{
		wxLogError(wxString(wxS("wxPdfDocument::LeaveLayer: ")) +
			wxString(_("Unbalanced layer operators.")));
	}
	while (n-- > 0)
	{
		Out("EMC");
	}
}

void
wxPdfDocument::PutLayers()
{
	wxPdfOcgMap::iterator ocgIter;

	// Layers
	for (ocgIter = m_ocgs->begin(); ocgIter != m_ocgs->end(); ++ocgIter)
	{
		if (ocgIter->second->GetType() == wxPDF_OCG_TYPE_LAYER)
		{
			auto layer = static_cast<wxPdfLayer*>(ocgIter->second);
			NewObj();
			layer->SetObjIndex(m_n);
			Out("<<");
			Out("/Type /OCG");
			Out("/Name ", false);
			OutTextstring(layer->GetName());
			int intent = layer->GetIntent();
			if (intent != wxPDF_OCG_INTENT_DEFAULT)
			{
				Out("/Intent [");
				if (intent & wxPDF_OCG_INTENT_VIEW)
				{
					Out("/View", false);
				}
				if (intent & wxPDF_OCG_INTENT_DESIGN)
				{
					Out("/Design", false);
				}
				Out("]");
			}
			wxPdfDictionary* usage = layer->GetUsage();
			if (usage != nullptr)
			{
				Out("/Usage ", false);
				WriteObjectValue(usage);
			}
			Out(">>");
			Out("endobj");
		}
	}

	// Layer memberships
	for (ocgIter = m_ocgs->begin(); ocgIter != m_ocgs->end(); ++ocgIter)
	{
		if (ocgIter->second->GetType() == wxPDF_OCG_TYPE_MEMBERSHIP)
		{
			auto layer = static_cast<wxPdfLayerMembership*>(ocgIter->second);
			NewObj();
			layer->SetObjIndex(m_n);
			Out("<<");
			Out("/Type /OCMD");
			wxPdfArrayLayer members = layer->GetMembers();
			if (!members.IsEmpty())
			{
				Out("/OCGs [", false);
				size_t j;
				for (j = 0; j < members.GetCount(); ++j)
				{
					OutAscii(wxString::Format(wxS(" %d 0 R"), members[j]->GetObjIndex()), false);
				}
				Out("]");
			}
			if (layer->GetVisibilityPolicy() != wxPDF_OCG_POLICY_ANYON)
			{
				Out("/P ", false);
				switch (layer->GetVisibilityPolicy())
				{
				case wxPDF_OCG_POLICY_ALLOFF:
					Out("/AllOff");
					break;
				case wxPDF_OCG_POLICY_ALLON:
					Out("/AllOn");
					break;
				case wxPDF_OCG_POLICY_ANYOFF:
					Out("/AnyOff");
					break;
				case wxPDF_OCG_POLICY_ANYON:
				default:
					Out("/AnyOn");
					break;
				}
			}
			Out(">>");
			Out("endobj");
		}
	}
}

void
wxPdfDocument::PutOCGOrder(wxPdfLayer* layer)
{
	if (layer->IsOnPanel())
	{
		if (layer->GetType() != wxPDF_OCG_TYPE_TITLE)
		{
			OutAscii(wxString::Format(wxS("%d 0 R "), layer->GetObjIndex()), false);
		}
		if (layer->HasChildren())
		{
			Out("[", false);
			if (layer->GetType() == wxPDF_OCG_TYPE_TITLE)
			{
				OutTextstring(layer->GetTitle());
			}
			wxPdfArrayLayer children = layer->GetChildren();
			size_t k;
			for (k = 0; k < children.GetCount(); ++k)
			{
				PutOCGOrder(children[k]);
			}
			Out("]", false);
		}
	}
}

void
wxPdfDocument::PutASEvent(const wxString& situation, const wxString& category, bool& first)
{
	wxArrayInt layerIds;
	size_t n = m_ocgs->size();
	size_t j;
	for (j = 1; j <= n; ++j)
	{
		wxPdfOcgType type = (*m_ocgs)[j]->GetType();
		if (type == wxPDF_OCG_TYPE_LAYER || type == wxPDF_OCG_TYPE_TITLE)
		{
			auto layer = static_cast<wxPdfLayer*>((*m_ocgs)[j]);
			wxPdfDictionary* usage = layer->GetUsage();
			if (usage != nullptr && usage->Get(category) != nullptr)
			{
				layerIds.Add(layer->GetObjIndex());
			}
		}
	}
	if (layerIds.size() > 0)
	{
		if (first)
		{
			Out("/AS [");
			first = false;
		}
		Out("<<", false);
		Out("/Event /", false);
		OutAscii(situation, false);
		Out("/Category[/", false);
		OutAscii(category, false);
		Out("]", false);
		Out("/OCGs [", false);
		for (j = 0; j < layerIds.size(); ++j)
		{
			OutAscii(wxString::Format(wxS(" %d 0 R"), layerIds[j]), false);
		}
		Out("]>>");
	}
}

void
wxPdfDocument::PutOCProperties()
{
	Out("/OCProperties <<");

	Out(" /OCGs [", false);
	wxPdfOcgMap::iterator ocgIter;
	for (ocgIter = m_ocgs->begin(); ocgIter != m_ocgs->end(); ++ocgIter)
	{
		wxPdfOcg* ocg = ocgIter->second;
		if (ocg->GetType() == wxPDF_OCG_TYPE_LAYER)
		{
			OutAscii(wxString::Format(wxS(" %d 0 R"), ocg->GetObjIndex()), false);
		}
	}
	Out("]");
	Out("/D <<");

	int offCount = 0;
	Out("/Order [");
	size_t n = m_ocgs->size();
	for (size_t j = 1; j <= n; ++j)
	{
		wxPdfOcgType ocgType = (*m_ocgs)[j]->GetType();
		if (ocgType == wxPDF_OCG_TYPE_LAYER || ocgType == wxPDF_OCG_TYPE_TITLE)
		{
			auto layer = static_cast<wxPdfLayer*>((*m_ocgs)[j]);
			if (ocgType == wxPDF_OCG_TYPE_LAYER && !layer->IsOn())
			{
				++offCount;
			}
			if (layer->GetParent() == nullptr)
			{
				PutOCGOrder(layer);
			}
		}
	}
	Out("]");

	if (offCount > 0)
	{
		Out("/OFF [", false);
		for (size_t j = 1; j <= n; ++j)
		{
			if ((*m_ocgs)[j]->GetType() == wxPDF_OCG_TYPE_LAYER)
			{
				auto layer = static_cast<wxPdfLayer*>((*m_ocgs)[j]);
				if (!layer->IsOn())
				{
					OutAscii(wxString::Format(wxS("%d 0 R "), layer->GetObjIndex()), false);
				}
			}
		}
		Out("]");
	}

	if (m_rgLayers->size() > 0)
	{
		Out("/RBGroups [", false);
		for (size_t j = 1; j <= m_rgLayers->size(); ++j)
		{
			Out("[", false);
			wxPdfArrayLayer layers = (*m_rgLayers)[j]->GetGroup();
			size_t k;
			for (k = 0; k < layers.size(); ++k)
			{
				OutAscii(wxString::Format(wxS("%d 0 R "), layers[k]->GetObjIndex()), false);
			}
			Out("]", false);
		}
		Out("]");
	}

	if (m_lockedLayers != nullptr)
	{
		wxPdfArrayLayer layers = m_lockedLayers->GetGroup();
		Out("/Locked [", false);
		for (size_t j = 0; j < layers.GetCount(); ++j)
		{
			OutAscii(wxString::Format(wxS("%d 0 R "), layers[j]->GetObjIndex()), false);
		}
		Out("]");
	}

	bool first = true;
	PutASEvent(wxS("View"), wxS("Zoom"), first);
	PutASEvent(wxS("View"), wxS("View"), first);
	PutASEvent(wxS("Print"), wxS("Print"), first);
	PutASEvent(wxS("Export"), wxS("Export"), first);
	if (!first)
	{
		Out("]");
	}

	Out("/ListMode /VisiblePages");

	Out(">>"); // End of D dictionary

	Out(">>"); // End of OCProperties dictionary
}
