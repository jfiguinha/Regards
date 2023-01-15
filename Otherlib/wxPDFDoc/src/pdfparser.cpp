///////////////////////////////////////////////////////////////////////////////
// Name:        pdfparser.cpp
// Purpose:
// Author:      Ulrich Telle
// Created:     2006-10-15
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfparser.cpp Implementation of PDF parser

// For compilers that support precompilation, includes <wx.h>.
#include <header.h>

// includes
#include <wx/uri.h>
#include <wx/url.h>

#include "wx/pdfencrypt.h"
#include "wx/pdfinfo.h"
#include "wx/pdfobjects.h"
#include "wx/pdfparser.h"
#include "wx/pdftemplate.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxPdfXRef);

wxPdfXRefEntry::wxPdfXRefEntry()
{
	m_type = -1;
	m_ofs_idx = 0;
	m_gen_ref = 0;
}

wxPdfXRefEntry::~wxPdfXRefEntry()
{
}

void
wxPdfParser::ReserveXRef(size_t count)
{
	size_t currentCount = m_xref.GetCount();
	if (count > currentCount)
	{
		m_xref.Add(wxPdfXRefEntry(), count - currentCount);
	}
}

wxFileSystem* wxPdfParser::ms_fileSystem = nullptr;

wxFileSystem*
wxPdfParser::GetFileSystem()
{
	if (ms_fileSystem == nullptr)
	{
		static wxFileSystem fileSystem;
		ms_fileSystem = &fileSystem;
	}
	return ms_fileSystem;
}

wxPdfParser::wxPdfParser(const wxString& filename, const wxString& password)
{
	m_objectQueue = new wxPdfObjectQueue();
	m_objectQueueLast = m_objectQueue;
	m_objectMap = new wxPdfObjectMap();
	m_objStmCache = new wxPdfObjStmMap();
	m_tokens = nullptr;
	m_trailer = nullptr;
	m_root = nullptr;
	m_useRawStream = false;
	m_cacheObjects = true;

	m_encrypted = false;
	m_decryptor = nullptr;

	m_filename = filename;
	m_password = password;

	wxString fileURL = filename;
	wxURI uri(filename);
	if (!uri.HasScheme())
	{
		fileURL = wxFileSystem::FileNameToURL(filename);
	}
	m_pdfFile = GetFileSystem()->OpenFile(fileURL);
	if (m_pdfFile != nullptr)
	{
		m_tokens = new wxPdfTokenizer(m_pdfFile->GetStream());
		m_initialized = ParseDocument();
	}
}

wxPdfParser::~wxPdfParser()
{
	wxPdfObjectQueue* entry = m_objectQueue;
	wxPdfObjectQueue* next;
	while (entry != nullptr)
	{
		wxPdfObject* object = entry->GetObject();
		if (object != nullptr && object->IsCreatedIndirect())
		{
			delete object;
		}
		next = entry->GetNext();
		delete entry;
		entry = next;
	}
	delete m_objectMap;

	wxPdfObjStmMap::iterator objStm = m_objStmCache->begin();
	for (objStm = m_objStmCache->begin(); objStm != m_objStmCache->end(); ++objStm)
	{
		if (objStm->second != nullptr)
		{
			delete objStm->second;
		}
	}
	delete m_objStmCache;

	size_t j;
	for (j = 0; j < m_pages.GetCount(); j++)
	{
		auto obj = static_cast<wxPdfObject*>(m_pages.Item(j));
		delete obj;
	}
	m_pages.Clear();

	if (m_trailer != nullptr)
	{
		delete m_trailer;
	}
	if (m_root != nullptr)
	{
		delete m_root;
	}

	delete m_tokens;
	if (m_pdfFile != nullptr)
	{
		delete m_pdfFile;
	}

	if (m_decryptor != nullptr)
	{
		delete m_decryptor;
	}
}

bool
wxPdfParser::IsOk()
{
	return (m_pdfFile != nullptr && m_initialized);
}

void
wxPdfParser::AppendObject(int originalObjectId, int actualObjectId, wxPdfObject* obj)
{
	auto newEntry = new wxPdfObjectQueue(originalObjectId, actualObjectId, obj);
	m_objectQueueLast->SetNext(newEntry);
	m_objectQueueLast = newEntry;
	(*m_objectMap)[originalObjectId] = newEntry;
}

unsigned int
wxPdfParser::GetPageCount()
{
	return static_cast<unsigned int>(m_pages.GetCount());
}

static const wxChar* gs_entryList[] = {
	wxS("Title"), wxS("Author"), wxS("Subject"), wxS("Keywords"),
	wxS("Creator"), wxS("Producer"), wxS("CreationDate"), wxS("ModDate"),
	nullptr
}; //, "Trapped")

bool
wxPdfParser::GetSourceInfo(wxPdfInfo& info)
{
	bool ok = false;
	auto infoDict = static_cast<wxPdfDictionary*>(ResolveObject(m_trailer->Get(wxS("Info"))));
	if (infoDict != nullptr && infoDict->GetType() == OBJTYPE_DICTIONARY)
	{
		using InfoSetter = void(wxPdfInfo::*)(const wxString& value);
		InfoSetter entryFunc[] = {
			&wxPdfInfo::SetTitle, &wxPdfInfo::SetAuthor, &wxPdfInfo::SetSubject,
			&wxPdfInfo::SetKeywords, &wxPdfInfo::SetCreator, &wxPdfInfo::SetProducer,
			&wxPdfInfo::SetCreationDate, &wxPdfInfo::SetModDate,
			nullptr
		};
		wxString value;
		size_t j;
		for (j = 0; gs_entryList[j] != nullptr; j++)
		{
			auto entry = static_cast<wxPdfString*>(infoDict->Get(gs_entryList[j]));
			if (entry != nullptr)
			{
				value = entry->GetValue();
#if wxUSE_UNICODE
				if ((value.Length() >= 2) && (value.GetChar(0) == 254) && (value.GetChar(1) == 255))
				{
					wxMBConvUTF16BE conv;
					size_t k;
					size_t len = value.Length() - 2;
					auto mbstr = new char[len + 2];
					for (k = 0; k < len; k++)
					{
						mbstr[k] = static_cast<int>(value.GetChar(k + 2));
					}
					mbstr[len] = 0;
					mbstr[len + 1] = 0;
					value = conv.cMB2WC(mbstr);
					delete [] mbstr;
				}
#endif
				(info.*entryFunc[j])(value);
			}
		}
		if (infoDict->IsCreatedIndirect())
		{
			delete infoDict;
		}
		ok = true;
	}
	return ok;
}

bool
wxPdfParser::ParseDocument()
{
	bool ok = false;
	m_fileSize = m_tokens->GetLength();
	m_pdfVersion = m_tokens->CheckPdfHeader();
	if (m_pdfVersion != wxEmptyString)
	{
		if (ParseXRef())
		{
			if (SetupDecryptor())
			{
				m_root = static_cast<wxPdfDictionary*>(m_trailer->Get(wxS("Root")));
				m_root = static_cast<wxPdfDictionary*>(ResolveObject(m_root));
				if (m_root != nullptr)
				{
					auto versionEntry = static_cast<wxPdfName*>(ResolveObject(m_root->Get(wxS("Version"))));
					if (versionEntry != nullptr)
					{
						wxString version = versionEntry->GetName();
						version = version.Mid(1, 3);
						if (m_pdfVersion < version)
						{
							m_pdfVersion = version;
						}
						if (versionEntry->IsCreatedIndirect())
						{
							delete versionEntry;
						}
					}
					auto pages = static_cast<wxPdfDictionary*>(ResolveObject(m_root->Get(wxS("Pages"))));
					ok = ParsePageTree(pages);
					delete pages;
				}
			}
		}
	}
	return ok;
}

bool
wxPdfParser::SetupDecryptor()
{
	bool ok = true;
	wxPdfObject* encDic = m_trailer->Get(wxS("Encrypt"));
	if (encDic == nullptr || encDic->GetType() == OBJTYPE_NULL)
	{
		return true;
	}
	auto enc = static_cast<wxPdfDictionary*>(ResolveObject(encDic));
	wxPdfObject* obj;
	auto documentIDs = static_cast<wxPdfArray*>(ResolveObject(m_trailer->Get(wxS("ID"))));
	wxString documentID;
	if (documentIDs != nullptr)
	{
		obj = documentIDs->Get(0);
		if (obj->GetType() == OBJTYPE_STRING)
		{
			documentID = static_cast<wxPdfString*>(obj)->GetValue();
		}
		if (documentIDs->IsCreatedIndirect())
		{
			delete documentIDs;
		}
	}

	wxString uValue = wxEmptyString;
	obj = enc->Get(wxS("U"));
	if (obj->GetType() == OBJTYPE_STRING)
	{
		uValue = static_cast<wxPdfString*>(obj)->GetValue();
		if (uValue.Length() != 32)
		{
			wxLogError(wxString(wxS("wxPdfParser::SetupDecryptor: ")) +
				wxString(_("Invalid length of U value.")));
			ok = false;
		}
	}

	wxString oValue = wxEmptyString;
	obj = enc->Get(wxS("O"));
	if (obj->GetType() == OBJTYPE_STRING)
	{
		oValue = static_cast<wxPdfString*>(obj)->GetValue();
		if (oValue.Length() != 32)
		{
			wxLogError(wxString(wxS("wxPdfParser::SetupDecryptor: ")) +
				wxString(_("Invalid length of O value.")));
			ok = false;
		}
	}

	int rValue = 0;
	obj = enc->Get(wxS("R"));
	if (obj->GetType() == OBJTYPE_NUMBER)
	{
		rValue = static_cast<wxPdfNumber*>(obj)->GetInt();
		if (rValue != 2 && rValue != 3)
		{
			wxLogError(wxString(wxS("wxPdfParser::SetupDecryptor: ")) +
				wxString::Format(_("Unknown encryption type (%d)."), rValue));
			ok = false;
		}
	}
	else
	{
		wxLogError(wxString(wxS("wxPdfParser::SetupDecryptor: ")) +
			wxString(_("Illegal R value.")));
		ok = false;
	}

	int vValue = 0;
	obj = enc->Get(wxS("V"));
	if (obj != nullptr && obj->GetType() == OBJTYPE_NUMBER)
	{
		vValue = static_cast<wxPdfNumber*>(obj)->GetInt();
		if (!((rValue == 2 && vValue == 1) || (rValue == 3 && vValue == 2)))
		{
			wxLogError(wxString(wxS("wxPdfParser::SetupDecryptor: ")) +
				wxString(_("Unsupported V value.")));
			ok = false;
		}
	}
	else
	{
		wxLogError(wxString(wxS("wxPdfParser::SetupDecryptor: ")) +
			wxString(_("Illegal V value.")));
		ok = false;
	}

	int pValue = 0;
	obj = enc->Get(wxS("P"));
	if (obj->GetType() == OBJTYPE_NUMBER)
	{
		pValue = static_cast<wxPdfNumber*>(obj)->GetInt();
		// Check required permissions (Applications MUST respect the permission settings)
		if ((pValue & REQUIRED_PERMISSIONS) != REQUIRED_PERMISSIONS)
		{
			wxLogError(wxString(wxS("wxPdfParser::SetupDecryptor: ")) +
				wxString(_("Import of document not allowed due to missing permissions.")));
			ok = false;
		}
	}
	else
	{
		wxLogError(wxString(wxS("wxPdfParser::SetupDecryptor: ")) +
			wxString(_("Illegal P value.")));
		ok = false;
	}

	int lengthValue = 40; // Default for revisison 2
	if (rValue == 3)
	{
		// Get the key length if revision is 3
		obj = enc->Get(wxS("Length"));
		if (obj->GetType() == OBJTYPE_NUMBER)
		{
			lengthValue = static_cast<wxPdfNumber*>(obj)->GetInt();
			if (lengthValue > 128 || lengthValue < 40 || lengthValue % 8 != 0)
			{
				wxLogError(wxString(wxS("wxPdfParser::SetupDecryptor: ")) +
					wxString(_("Illegal Length value.")));
				ok = false;
			}
		}
		else
		{
			wxLogError(wxString(wxS("wxPdfParser::SetupDecryptor: ")) +
				wxString(_("Illegal Length value.")));
			ok = false;
		}
	}

	if (enc->IsCreatedIndirect())
	{
		delete enc;
	}

	if (ok)
	{
		m_encrypted = true;
		m_decryptor = new wxPdfEncrypt();
		if (!m_decryptor->Authenticate(documentID, m_password, uValue, oValue, pValue, lengthValue, rValue))
		{
			wxLogError(wxString(wxS("wxPdfParser::SetupDecryptor: ")) +
				wxString(_("Bad password.")));
			ok = false;
		}
	}

	return ok;
}

bool
wxPdfParser::ParsePageTree(wxPdfDictionary* pages)
{
	bool ok = false;
	// Get the kids dictionary
	auto kids = static_cast<wxPdfArray*>(ResolveObject(pages->Get(wxS("Kids"))));
	if (kids != nullptr)
	{
		size_t nKids = kids->GetSize();
		size_t j;
		ok = true;
		for (j = 0; j < nKids; j++)
		{
			auto page = static_cast<wxPdfDictionary*>(ResolveObject(kids->Get(j)));
			auto type = static_cast<wxPdfName*>(page->Get(wxS("Type")));
			if (type->GetName() == wxS("Pages"))
			{
				// If one of the kids is an embedded
				// /Pages array, resolve it as well.
				ok = ok && ParsePageTree(page);
				delete page;
			}
			else
			{
				m_pages.Add(page);
			}
		}
		if (kids->IsCreatedIndirect())
		{
			delete kids;
		}
	}
	else
	{
		wxLogError(wxString(wxS("wxPdfParser::ParsePageTree: ")) +
			wxString(_("Cannot find /Kids in current /Page-Dictionary")));
	}
	return ok;
}

wxPdfObject*
wxPdfParser::GetPageResources(unsigned int pageno)
{
	wxPdfObject* resources = nullptr;
	if (pageno < GetPageCount())
	{
		resources = GetPageResources(static_cast<wxPdfObject*>(m_pages[pageno]));
	}
	return resources;
}

wxPdfObject*
wxPdfParser::GetPageResources(wxPdfObject* page)
{
	wxPdfObject* resources = nullptr;
	auto dic = static_cast<wxPdfDictionary*>(ResolveObject(page));

	// If the current object has a resources dictionary associated with it,
	// we use it. Otherwise, we move back to its parent object.
	wxPdfObject* resourceRef = ResolveObject(dic->Get(wxS("Resources")));
	if (resourceRef != nullptr)
	{
		resources = ResolveObject(resourceRef);
	}
	else
	{
		wxPdfObject* parent = ResolveObject(dic->Get(wxS("Parent")));
		if (parent != nullptr)
		{
			resources = GetPageResources(parent);
			delete parent;
		}
	}
	return resources;
}

void
wxPdfParser::GetContent(unsigned int pageno, wxArrayPtrVoid& contents)
{
	if (pageno < GetPageCount())
	{
		wxPdfObject* content = static_cast<wxPdfDictionary*>(m_pages[pageno])->Get(wxS("Contents"));
		if (content != nullptr)
		{
			GetPageContent(content, contents);
		}
	}
}

void
wxPdfParser::GetPageContent(wxPdfObject* contentRef, wxArrayPtrVoid& contents)
{
	int type = contentRef->GetType();
	if (type == OBJTYPE_INDIRECT)
	{
		wxPdfObject* content = ResolveObject(contentRef);
		if (content->GetType() == OBJTYPE_ARRAY)
		{
			GetPageContent(content, contents);
			delete content;
		}
		else
		{
			contents.Add(content);
		}
	}
	else if (type == OBJTYPE_ARRAY)
	{
		auto contentArray = static_cast<wxPdfArray*>(contentRef);
		size_t n = contentArray->GetSize();
		size_t j;
		for (j = 0; j < n; j++)
		{
			GetPageContent(contentArray->Get(j), contents);
		}
	}
}

wxPdfArrayDouble*
wxPdfParser::GetPageMediaBox(unsigned int pageno)
{
	wxPdfArrayDouble* box = GetPageBox(static_cast<wxPdfDictionary*>(m_pages[pageno]), wxS("MediaBox"));
	return box;
}

wxPdfArrayDouble*
wxPdfParser::GetPageCropBox(unsigned int pageno)
{
	wxPdfArrayDouble* box = GetPageBox(static_cast<wxPdfDictionary*>(m_pages[pageno]), wxS("CropBox"));
	if (box == nullptr)
	{
		box = GetPageBox(static_cast<wxPdfDictionary*>(m_pages[pageno]), wxS("MediaBox"));
	}
	return box;
}

wxPdfArrayDouble*
wxPdfParser::GetPageBleedBox(unsigned int pageno)
{
	wxPdfArrayDouble* box = GetPageBox(static_cast<wxPdfDictionary*>(m_pages[pageno]), wxS("BleedBox"));
	if (box == nullptr)
	{
		box = GetPageCropBox(pageno);
	}
	return box;
}

wxPdfArrayDouble*
wxPdfParser::GetPageTrimBox(unsigned int pageno)
{
	wxPdfArrayDouble* box = GetPageBox(static_cast<wxPdfDictionary*>(m_pages[pageno]), wxS("TrimBox"));
	if (box == nullptr)
	{
		box = GetPageCropBox(pageno);
	}
	return box;
}

wxPdfArrayDouble*
wxPdfParser::GetPageArtBox(unsigned int pageno)
{
	wxPdfArrayDouble* box = GetPageBox(static_cast<wxPdfDictionary*>(m_pages[pageno]), wxS("ArtBox"));
	if (box == nullptr)
	{
		box = GetPageCropBox(pageno);
	}
	return box;
}

wxPdfArrayDouble*
wxPdfParser::GetPageBox(wxPdfDictionary* page, const wxString& boxIndex)
{
	wxPdfArrayDouble* pageBox = nullptr;
	auto box = static_cast<wxPdfArray*>(ResolveObject(page->Get(boxIndex)));
	if (box == nullptr)
	{
		auto parent = static_cast<wxPdfDictionary*>(ResolveObject(page->Get(wxS("Parent"))));
		if (parent != nullptr)
		{
			pageBox = GetPageBox(parent, boxIndex);
			delete parent;
		}
	}
	else
	{
		pageBox = new wxPdfArrayDouble();
		size_t j;
		for (j = 0; j < box->GetSize(); j++)
		{
			auto item = static_cast<wxPdfNumber*>(box->Get(j));
			pageBox->Add(item->GetValue());
		}
	}
	return pageBox;
}

int
wxPdfParser::GetPageRotation(unsigned int pageno)
{
	return GetPageRotation(static_cast<wxPdfDictionary*>(m_pages[pageno]));
}

int
wxPdfParser::GetPageRotation(wxPdfDictionary* page)
{
	int pageRotation = 0;
	auto rotation = static_cast<wxPdfNumber*>(ResolveObject(page->Get(wxS("Rotate"))));
	if (rotation == nullptr)
	{
		auto parent = static_cast<wxPdfDictionary*>(ResolveObject(page->Get(wxS("Parent"))));
		if (parent != nullptr)
		{
			pageRotation = GetPageRotation(parent);
			delete parent;
		}
	}
	else
	{
		pageRotation = rotation->GetInt();
	}
	return pageRotation;
}

bool
wxPdfParser::ParseXRef()
{
	m_tokens->Seek(m_tokens->GetStartXRef());
	m_tokens->NextToken();
	if (m_tokens->GetStringValue() != wxS("startxref"))
	{
		wxLogError(wxString(wxS("wxPdfParser::ParseXRef: ")) +
			wxString(_("'startxref' not found.")));
		return false;
	}
	m_tokens->NextToken();
	if (m_tokens->GetTokenType() != /*PRTokeniser.*/ TOKEN_NUMBER)
	{
		wxLogError(wxString(wxS("wxPdfParser::ParseXRef: ")) +
			wxString(_("'startxref' is not followed by a number.")));
		return false;
	}
	int startxref = m_tokens->GetIntValue();

	if (!ParseXRefStream(startxref, true))
	{
		m_xref.Clear();
		m_tokens->Seek(startxref);
		m_trailer = ParseXRefSection();
		wxPdfDictionary* trailer1 = m_trailer;
		wxPdfDictionary* trailer2 = nullptr;
		while (trailer1 != nullptr)
		{
			auto prev = static_cast<wxPdfNumber*>(trailer1->Get(wxS("Prev")));
			trailer2 = trailer1;
			if (prev != nullptr)
			{
				m_tokens->Seek(prev->GetInt());
				trailer1 = ParseXRefSection();
			}
			else
			{
				trailer1 = nullptr;
			}
			if (trailer2 != m_trailer)
			{
				delete trailer2;
			}
		}
	}
	return (m_trailer != nullptr);
}

wxPdfDictionary*
wxPdfParser::ParseXRefSection()
{
	m_tokens->NextValidToken();
	if (m_tokens->GetStringValue() != wxS("xref"))
	{
		wxLogError(wxString(wxS("wxPdfParser::ParseXRefSection: ")) +
			wxString(_("xref subsection not found.")));
		return nullptr;
	}
	int start = 0;
	int end = 0;
	int pos = 0;
	int gen = 0;
	while (true)
	{
		m_tokens->NextValidToken();
		if (m_tokens->GetStringValue() == wxS("trailer"))
			break;
		if (m_tokens->GetTokenType() != TOKEN_NUMBER)
		{
			wxLogError(wxString(wxS("wxPdfParser::ParseXRefSection: ")) +
				wxString(_("Object number of the first object in this xref subsection not found.")));
			return nullptr;
		}
		start = m_tokens->GetIntValue();
		m_tokens->NextValidToken();
		if (m_tokens->GetTokenType() != TOKEN_NUMBER)
		{
			wxLogError(wxString(wxS("wxPdfParser::ParseXRefSection: ")) +
				wxString(_("Number of entries in this xref subsection not found.")));
			return nullptr;
		}
		end = m_tokens->GetIntValue() + start;
		if (start == 1)
		{
			// fix incorrect start number
			int back = m_tokens->Tell();
			m_tokens->NextValidToken();
			pos = m_tokens->GetIntValue();
			m_tokens->NextValidToken();
			gen = m_tokens->GetIntValue();
			if (pos == 0 && gen == 65535)
			{
				--start;
				--end;
			}
			m_tokens->Seek(back);
		}
		ReserveXRef(end);

		int k;
		for (k = start; k < end; ++k)
		{
			wxPdfXRefEntry& xrefEntry = m_xref[k];
			m_tokens->NextValidToken();
			pos = m_tokens->GetIntValue();
			m_tokens->NextValidToken();
			gen = m_tokens->GetIntValue();
			m_tokens->NextValidToken();
			if (m_tokens->GetStringValue() == wxS("n"))
			{
				if (xrefEntry.m_ofs_idx == 0 && xrefEntry.m_gen_ref == 0)
				{
					// TODO: if (pos == 0)
					//   wxLogError(wxS("File position 0 cross-reference entry in this xref subsection"));
					xrefEntry.m_ofs_idx = pos;
					xrefEntry.m_gen_ref = gen;
					xrefEntry.m_type = 1;
				}
			}
			else if (m_tokens->GetStringValue() == wxS("f"))
			{
				if (xrefEntry.m_ofs_idx == 0 && xrefEntry.m_gen_ref == 0)
				{
					xrefEntry.m_ofs_idx = -1;
					xrefEntry.m_gen_ref = 0;
					xrefEntry.m_type = 0;
				}
			}
			else
			{
				wxLogError(wxString(wxS("wxPdfParser:ReadXRefSection: ")) +
					wxString(_("Invalid cross-reference entry in this xref subsection.")));
				return nullptr;
			}
		}
	}
	auto trailer = static_cast<wxPdfDictionary*>(ParseObject());
	auto xrefSize = static_cast<wxPdfNumber*>(trailer->Get(wxS("Size")));
	ReserveXRef(xrefSize->GetInt());

	wxPdfObject* xrs = trailer->Get(wxS("XRefStm"));
	if (xrs != nullptr && xrs->GetType() == OBJTYPE_NUMBER)
	{
		int loc = static_cast<wxPdfNumber*>(xrs)->GetInt();
		ParseXRefStream(loc, false);
	}
	return trailer;
}

bool
wxPdfParser::ParseXRefStream(int ptr, bool setTrailer)
{
	int idx, k;

	m_tokens->Seek(ptr);
	int streamRef = 0;
	if (!m_tokens->NextToken())
	{
		return false;
	}
	if (m_tokens->GetTokenType() != TOKEN_NUMBER)
	{
		return false;
	}
	streamRef = m_tokens->GetIntValue();
	if (!m_tokens->NextToken() || m_tokens->GetTokenType() != TOKEN_NUMBER)
	{
		return false;
	}
	if (!m_tokens->NextToken() || m_tokens->GetStringValue() != wxS("obj"))
	{
		return false;
	}
	wxPdfObject* object = ParseObject();
	wxPdfStream* stm = nullptr;
	if (object->GetType() == OBJTYPE_STREAM)
	{
		stm = static_cast<wxPdfStream*>(object);
		if (static_cast<wxPdfName*>(stm->Get(wxS("Type")))->GetName() != wxS("XRef"))
		{
			delete object;
			return false;
		}
	}
	int size = static_cast<wxPdfNumber*>(stm->Get(wxS("Size")))->GetInt();
	bool indexAllocated = false;
	wxPdfArray* index;
	wxPdfObject* obj = stm->Get(wxS("Index"));
	if (obj == nullptr)
	{
		indexAllocated = true;
		index = new wxPdfArray();
		index->Add(0);
		index->Add(size);
	}
	else
	{
		index = static_cast<wxPdfArray*>(obj);
	}
	auto w = static_cast<wxPdfArray*>(stm->Get(wxS("W")));
	int prev = -1;
	obj = stm->Get(wxS("Prev"));
	if (obj != nullptr)
	{
		prev = static_cast<wxPdfNumber*>(obj)->GetInt();
	}
	// Each xref pair is a position
	// type 0 -> -1, 0
	// type 1 -> offset, 0
	// type 2 -> index, obj num
	ReserveXRef(size);

	GetStreamBytes(stm);
	wxMemoryOutputStream* streamBuffer = stm->GetBuffer();
	wxMemoryInputStream streamBytes(*streamBuffer);
	size_t inLength = streamBytes.GetSize();
	auto buffer = new char[inLength];
	streamBytes.Read(buffer, inLength);

	int bptr = 0;
	int wc[3];
	for (k = 0; k < 3; ++k)
	{
		wc[k] = static_cast<wxPdfNumber*>(w->Get(k))->GetInt();
	}
	for (idx = 0; static_cast<size_t>(idx) < index->GetSize(); idx += 2)
	{
		int start = static_cast<wxPdfNumber*>(index->Get(idx))->GetInt();
		int length = static_cast<wxPdfNumber*>(index->Get(idx + 1))->GetInt();
		ReserveXRef(start + length);
		while (length-- > 0)
		{
			wxPdfXRefEntry& xrefEntry = m_xref[start];
			int type = 1;
			if (wc[0] > 0)
			{
				type = 0;
				for (k = 0; k < wc[0]; ++k)
				{
					type = (type << 8) + (buffer[bptr++] & 0xff);
				}
			}
			int field2 = 0;
			for (k = 0; k < wc[1]; ++k)
			{
				field2 = (field2 << 8) + (buffer[bptr++] & 0xff);
			}
			int field3 = 0;
			for (k = 0; k < wc[2]; ++k)
			{
				field3 = (field3 << 8) + (buffer[bptr++] & 0xff);
			}
			if (xrefEntry.m_ofs_idx == 0 && xrefEntry.m_gen_ref == 0)
			{
				switch (type)
				{
				case 0:
					xrefEntry.m_type = 0;
					xrefEntry.m_ofs_idx = -1;
					xrefEntry.m_gen_ref = 0;
					break;
				case 1:
					xrefEntry.m_type = 1;
					xrefEntry.m_ofs_idx = field2;
					xrefEntry.m_gen_ref = field3;
					break;
				case 2:
					xrefEntry.m_type = 2;
					xrefEntry.m_ofs_idx = field3;
					xrefEntry.m_gen_ref = field2;
					break;
				}
			}
			start++;
		}
	}
	delete [] buffer;
	if (static_cast<size_t>(streamRef) < m_xref.GetCount())
	{
		m_xref[streamRef].m_ofs_idx = -1;
	}
	if (indexAllocated)
	{
		delete index;
	}

	// Set the first xref stream dictionary as the trailer dictionary
	if (setTrailer && m_trailer == nullptr)
	{
		m_trailer = stm->GetDictionary();
		stm->SetDictionary(nullptr);
	}
	delete stm;

	if (prev == -1)
	{
		return true;
	}
	return ParseXRefStream(prev, false);
}

wxPdfDictionary*
wxPdfParser::ParseDictionary()
{
	auto dic = new wxPdfDictionary();
	while (true)
	{
		m_tokens->NextValidToken();
		if (m_tokens->GetTokenType() == TOKEN_END_DICTIONARY)
			break;
		if (m_tokens->GetTokenType() != TOKEN_NAME)
		{
			wxLogError(wxString(wxS("wxPdfParser::ParseDictionary: ")) +
				wxString(_("Dictionary key is not a name.")));
			break;
		}
		auto name = new wxPdfName(m_tokens->GetStringValue());
		wxPdfObject* obj = ParseObject();
		int type = obj->GetType();
		if (-type == TOKEN_END_DICTIONARY)
		{
			wxLogError(wxString(wxS("wxPdfParser::ParseDictionary: ")) +
				wxString(_("Unexpected '>>'.")));
			delete obj;
			delete name;
			break;
		}
		if (-type == TOKEN_END_ARRAY)
		{
			wxLogError(wxString(wxS("wxPdfParser::ParseDictionary: ")) +
				wxString(_("Unexpected ']'.")));
			delete obj;
			delete name;
			break;
		}
		dic->Put(name, obj);
		delete name;
	}
	return dic;
}

wxPdfArray*
wxPdfParser::ParseArray()
{
	auto array = new wxPdfArray();
	while (true)
	{
		wxPdfObject* obj = ParseObject();
		int type = obj->GetType();
		if (-type == TOKEN_END_ARRAY)
		{
			delete obj;
			break;
		}
		if (-type == TOKEN_END_DICTIONARY)
		{
			wxLogError(wxString(wxS("wxPdfParser::ParseArray: ")) +
				wxString(_("Unexpected '>>'.")));
			delete obj;
			break;
		}
		array->Add(obj);
	}
	return array;
}

wxPdfObject*
wxPdfParser::ParseObject()
{
	wxPdfObject* obj;
	m_tokens->NextValidToken();
	int type = m_tokens->GetTokenType();
	switch (type)
	{
	case TOKEN_START_DICTIONARY:
		{
			wxPdfDictionary* dic = ParseDictionary();
			int pos = m_tokens->Tell();
			// be careful in the trailer. May not be a "next" token.
			if (m_tokens->NextToken() && m_tokens->GetStringValue() == wxS("stream"))
			{
				int ch = m_tokens->ReadChar();
				if (ch != '\n')
					ch = m_tokens->ReadChar();
				if (ch != '\n')
					m_tokens->BackOnePosition(ch);
				auto stream = new wxPdfStream(m_tokens->Tell());
				stream->SetDictionary(dic);
				obj = stream;
			}
			else
			{
				m_tokens->Seek(pos);
				obj = dic;
			}
		}
		break;

	case TOKEN_START_ARRAY:
		{
			obj = ParseArray();
		}
		break;

	case TOKEN_NUMBER:
		{
			obj = new wxPdfNumber(m_tokens->GetStringValue());
		}
		break;

	case TOKEN_STRING:
		{
			wxString token = m_tokens->GetStringValue();
			// Decrypt if necessary
			if (m_encrypted)
			{
				m_decryptor->Encrypt(m_objNum, m_objGen, token);
			}

			auto strObj = new wxPdfString(token);
			strObj->SetIsHexString(m_tokens->IsHexString());
			obj = strObj;
		}
		break;

	case TOKEN_NAME:
		{
			obj = new wxPdfName(m_tokens->GetStringValue());
		}
		break;

	case TOKEN_REFERENCE:
		{
			int num = m_tokens->GetReference();
			obj = new wxPdfIndirectReference(num, m_tokens->GetGeneration());
		}
		break;

	case TOKEN_BOOLEAN:
		{
			obj = new wxPdfBoolean((m_tokens->GetStringValue() == wxS("true")));
		}
		break;

	case TOKEN_NULL:
		{
			obj = new wxPdfNull();
		}
		break;

	default:
		{
			wxString token = m_tokens->GetStringValue();
			obj = new wxPdfLiteral(-type, m_tokens->GetStringValue());
		}
		break;
	}
	return obj;
}

wxPdfObject*
wxPdfParser::ResolveObject(wxPdfObject* obj)
{
	if (obj != nullptr && obj->GetType() == OBJTYPE_INDIRECT)
	{
		auto ref = static_cast<wxPdfIndirectReference*>(obj);
		int idx = ref->GetNumber();
		obj = ParseSpecificObject(idx);
		obj->SetCreatedIndirect(true);
	}
	return obj;
}

wxPdfObject*
wxPdfParser::ParseSpecificObject(int idx)
{
	wxPdfObject* obj = nullptr;
	if (static_cast<size_t>(idx) >= m_xref.GetCount())
	{
		return nullptr;
	}
	obj = ParseDirectObject(idx);
	return obj;
}

wxPdfObject*
wxPdfParser::ParseDirectObject(int k)
{
	int objIndex = 0;
	int objStreamIndex = 0;
	bool isCached = false;
	wxPdfObject* obj = nullptr;

	// Check for free object
	if (m_xref[k].m_type == 0)
	{
		return nullptr;
	}
	int pos = m_xref[k].m_ofs_idx;
	if (m_xref[k].m_type == 2)
	{
		objIndex = m_xref[k].m_gen_ref;
		wxPdfObjStmMap::iterator objStm = m_objStmCache->find(objIndex);
		if (objStm != m_objStmCache->end())
		{
			obj = objStm->second;
			isCached = true;
		}
		else
		{
			objStreamIndex = m_xref[k].m_gen_ref;
			pos = m_xref[objStreamIndex].m_ofs_idx;
		}
	}
	if (!isCached)
	{
		m_tokens->Seek(pos);
		m_tokens->NextValidToken();
		if (m_tokens->GetTokenType() != TOKEN_NUMBER)
		{
			wxLogError(wxString(wxS("wxPdfParser::ParseSingleObject: ")) +
				wxString(_("Invalid object number.")));
			return nullptr;
		}
		m_objNum = m_tokens->GetIntValue();
		m_tokens->NextValidToken();
		if (m_tokens->GetTokenType() != TOKEN_NUMBER)
		{
			wxLogError(wxString(wxS("wxPdfParser::ParseSingleObject: ")) +
				wxString(_("Invalid generation number.")));
			return nullptr;
		}
		m_objGen = m_tokens->GetIntValue();
		m_tokens->NextValidToken();
		if (m_tokens->GetStringValue() != wxS("obj"))
		{
			wxLogError(wxString(wxS("wxPdfParser::ParseSingleObject: ")) +
				wxString(_("Token 'obj' expected.")));
			return nullptr;
		}
		obj = ParseObject();
	}

	// TODO: Check for valid 'endstream'

	if (m_xref[k].m_type == 2)
	{
		m_objNum = k;
		m_objGen = 0;
		auto objStream = static_cast<wxPdfStream*>(obj);
		obj = ParseObjectStream(static_cast<wxPdfStream*>(obj), m_xref[k].m_ofs_idx);
		if (m_cacheObjects)
		{
			if (!isCached)
			{
				(*m_objStmCache)[objIndex] = objStream;
			}
		}
		else
		{
			delete objStream;
		}
	}

	if (obj != nullptr)
	{
		obj->SetObjNum(m_objNum, m_objGen);
	}
	if (obj->GetType() == OBJTYPE_STREAM)
	{
		GetStreamBytes(static_cast<wxPdfStream*>(obj));
	}
	return obj;
}

wxPdfObject*
wxPdfParser::ParseObjectStream(wxPdfStream* objStm, int idx)
{
	wxPdfObject* obj = nullptr;

	auto firstNumber = static_cast<wxPdfNumber*>(ResolveObject(objStm->Get(wxS("First"))));
	int first = firstNumber->GetInt();
	if (objStm->GetBuffer() == nullptr)
	{
		bool saveUseRawStream = m_useRawStream;
		m_useRawStream = false;
		GetStreamBytes(objStm);
		m_useRawStream = saveUseRawStream;
	}

	bool saveEncrypted = m_encrypted;
	m_encrypted = false;
	wxPdfTokenizer* saveTokens = m_tokens;
	wxMemoryInputStream objStream(*(objStm->GetBuffer()));
	m_tokens = new wxPdfTokenizer(&objStream);

	int address = 0;
	bool ok = true;
	if (!objStm->HasObjOffsets())
	{
		// Read object offsets
		wxArrayInt* objOffsets = objStm->GetObjOffsets();
		int objCount = idx + 1;
		if (m_cacheObjects)
		{
			auto objCountNumber = static_cast<wxPdfNumber*>(ResolveObject(objStm->Get(wxS("N"))));
			objCount = objCountNumber->GetInt();
		}
		int offset;
		int k;
		for (k = 0; k < objCount; ++k)
		{
			ok = m_tokens->NextToken();
			if (!ok)
				break;
			if (m_tokens->GetTokenType() != TOKEN_NUMBER)
			{
				ok = false;
				break;
			}
			ok = m_tokens->NextToken();
			if (!ok)
				break;
			if (m_tokens->GetTokenType() != TOKEN_NUMBER)
			{
				ok = false;
				break;
			}
			offset = m_tokens->GetIntValue() + first;
			if (m_cacheObjects)
			{
				objOffsets->Add(offset);
			}
			if (k == idx)
			{
				address = offset;
			}
		}
		if (ok)
		{
			objStm->SetHasObjOffsets(m_cacheObjects);
		}
	}
	else
	{
		address = objStm->GetObjOffset(idx);
		ok = (address > 0);
	}
	if (ok)
	{
		m_tokens->Seek(address);
		obj = ParseObject();
	}
	else
	{
		wxLogError(wxString(wxS("wxPdfParser::ParseOneObjStm: ")) +
			wxString(_("Error reading ObjStm.")));
	}

	delete m_tokens;
	m_tokens = saveTokens;
	m_encrypted = saveEncrypted;

	return obj;
}

void
wxPdfParser::GetStreamBytes(wxPdfStream* stream)
{
	GetStreamBytesRaw(stream);

	// Do not decode the content of resource object streams
	if (m_useRawStream) return;

	// Check whether the stream buffer is empty
	wxMemoryOutputStream* osIn = stream->GetBuffer();
	if (osIn->GetLength() == 0) return;

	size_t j;
	wxArrayPtrVoid filters;
	wxPdfObject* filter = ResolveObject(stream->Get(wxS("Filter")));
	if (filter != nullptr)
	{
		int type = filter->GetType();
		if (type == OBJTYPE_NAME)
		{
			filters.Add(filter);
		}
		else if (type == OBJTYPE_ARRAY)
		{
			auto filterArray = static_cast<wxPdfArray*>(filter);
			size_t size = filterArray->GetSize();
			for (j = 0; j < size; j++)
			{
				filters.Add(filterArray->Get(j));
			}
		}

		// Read decode parameters if available
		wxArrayPtrVoid dp;
		wxPdfObject* dpo = ResolveObject(stream->Get(wxS("DecodeParms")));
		if (dpo == nullptr || (dpo->GetType() != OBJTYPE_DICTIONARY && dpo->GetType() != OBJTYPE_ARRAY))
		{
			dpo = ResolveObject(stream->Get(wxS("DP")));
		}
		if (dpo != nullptr)
		{
			if (dpo->GetType() == OBJTYPE_DICTIONARY)
			{
				dp.Add(dpo);
			}
			else if (dpo->GetType() == OBJTYPE_ARRAY)
			{
				auto dpArray = static_cast<wxPdfArray*>(dpo);
				size_t size = dpArray->GetSize();
				for (j = 0; j < size; j++)
				{
					dp.Add(dpArray->Get(j));
				}
			}
		}

		wxPdfObject* dicParam = nullptr;
		wxMemoryOutputStream* osOut = nullptr;
		for (j = 0; j < filters.GetCount(); j++)
		{
			osIn = stream->GetBuffer();
			auto name = static_cast<wxPdfName*>(filters[j]);
			if (name->GetName() == wxS("FlateDecode") || name->GetName() == wxS("Fl"))
			{
				osOut = FlateDecode(osIn);
				if (j < dp.GetCount())
				{
					wxMemoryOutputStream* osIn2 = osOut;
					dicParam = static_cast<wxPdfObject*>(dp[j]);
					osOut = DecodePredictor(osIn2, dicParam);
					if (osOut != osIn2)
					{
						delete osIn2;
					}
				}
			}
			else if (name->GetName() == wxS("ASCIIHexDecode") || name->GetName() == wxS("AHx"))
			{
				osOut = ASCIIHexDecode(osIn);
			}
			else if (name->GetName() == wxS("ASCII85Decode") || name->GetName() == wxS("A85"))
			{
				osOut = ASCII85Decode(osIn);
			}
			else if (name->GetName() == wxS("LZWDecode"))
			{
				osOut = LZWDecode(osIn);
				if (j < dp.GetCount())
				{
					wxMemoryOutputStream* osIn2 = osOut;
					dicParam = static_cast<wxPdfObject*>(dp[j]);
					osOut = DecodePredictor(osIn2, dicParam);
					if (osOut != osIn2)
					{
						delete osIn2;
					}
				}
			}
			else
			{
				wxLogError(wxString(wxS("wxPdfParser::GetStreamBytes: ")) +
					wxString::Format(_("Filter '%s' not supported."), name->GetName().c_str()));
			}
			if (osOut != nullptr)
			{
				stream->SetBuffer(osOut);
				if (osIn != osOut)
				{
					delete osIn;
				}
			}
		}
	}
}

void
wxPdfParser::GetStreamBytesRaw(wxPdfStream* stream)
{
	auto streamLength = static_cast<wxPdfNumber*>(ResolveObject(stream->Get(wxS("Length"))));
	size_t size = streamLength->GetInt();
	m_tokens->Seek(stream->GetOffset());
	wxMemoryOutputStream* memoryBuffer = nullptr;
	wxMemoryOutputStream* streamBuffer = m_tokens->ReadBuffer(size);

	if (m_encrypted && size > 0)
	{
		wxMemoryInputStream inData(*streamBuffer);
		delete streamBuffer;
		memoryBuffer = new wxMemoryOutputStream();
		auto buffer = new unsigned char[size];
		inData.Read(buffer, size);
		if (inData.LastRead() == size)
		{
			m_decryptor->Encrypt(stream->GetNumber(), stream->GetGeneration(), buffer, static_cast<unsigned int>(size));
			memoryBuffer->Write(buffer, size);
		}
		delete [] buffer;
		memoryBuffer->Close();
	}
	else
	{
		memoryBuffer = streamBuffer;
	}

	stream->SetBuffer(memoryBuffer);
	if (streamLength->IsCreatedIndirect())
	{
		delete streamLength;
	}
}

// --- Tokenizer

wxPdfTokenizer::wxPdfTokenizer(wxInputStream* inputStream)
{
	m_inputStream = inputStream;
}

wxPdfTokenizer::~wxPdfTokenizer()
{
}

off_t
wxPdfTokenizer::Seek(off_t pos)
{
	return m_inputStream->SeekI(pos);
}

off_t
wxPdfTokenizer::Tell()
{
	return m_inputStream->TellI();
}

void
wxPdfTokenizer::BackOnePosition(int ch)
{
	if (ch != -1)
	{
		off_t pos = Tell();
		if (pos > 0) pos--;
		Seek(pos);
	}
}

off_t
wxPdfTokenizer::GetLength()
{
	return m_inputStream->GetLength();
}

int
wxPdfTokenizer::ReadChar()
{
	int readChar;
	char ch = m_inputStream->GetC();
	readChar = (m_inputStream->LastRead() > 0) ? static_cast<unsigned char>(ch) : -1;
	return readChar;
}

wxMemoryOutputStream*
wxPdfTokenizer::ReadBuffer(size_t size)
{
	auto memoryBuffer = new wxMemoryOutputStream();
	if (size > 0)
	{
		auto buffer = new char[size];
		m_inputStream->Read(buffer, size);
		if (m_inputStream->LastRead() == size)
		{
			memoryBuffer->Write(buffer, size);
		}
		delete [] buffer;
	}
	memoryBuffer->Close();
	return memoryBuffer;
}

off_t
wxPdfTokenizer::GetStartXRef()
{
	char buffer[1024];
	int idx, found;
	off_t size = GetLength();
	if (size > 1024) size = 1024;
	off_t pos = GetLength() - size;
	do
	{
		m_inputStream->SeekI(pos);
		m_inputStream->Read(buffer, size);
		idx = size - 9;
		do
		{
			found = memcmp(buffer + idx, "startxref", 9);
			--idx;
		}
		while (found != 0 && idx >= 0);
		if (found == 0) break;
		pos = (pos > 1) ? (pos > (size - 9)) ? pos - size + 9 : 1 : 0;
	}
	while (pos > 0);
	if (found == 0)
	{
		pos = pos + idx + 1;
	}
	else
	{
		wxLogError(wxString(wxS("wxPdfTokenizer::GetStartXRef: ")) +
			wxString(_("PDF startxref not found.")));
	}
	return pos;
}

wxString
wxPdfTokenizer::CheckPdfHeader()
{
	wxString version = wxEmptyString;
	m_inputStream->SeekI(0);
	wxString str = ReadString(1024);
	int idx = str.Find(wxS("%PDF-1."));
	if (idx >= 0)
	{
		m_inputStream->SeekI(idx);
		version = str.Mid(idx + 5, 3);
	}
	else
	{
		m_inputStream->SeekI(0);
		wxLogError(wxString(wxS("wxPdfTokenizer::GetStartXref: ")) +
			wxString(_("PDF header signature not found.")));
	}
	return version;
}

wxString
wxPdfTokenizer::ReadString(int size)
{
	wxString buf;
	int ch;
	while (size > 0)
	{
		size--;
		ch = ReadChar();
		if (ch == -1)
			break;
		buf += ch;
	}
	return buf;
}

bool
wxPdfTokenizer::NextToken()
{
	wxString buffer = wxEmptyString;
	m_stringValue = wxEmptyString;
	int ch = 0;
	do
	{
		ch = ReadChar();
	}
	while (ch != -1 && IsWhitespace(ch));

	if (ch == -1)
		return false;

	switch (ch)
	{
	case '[':
		m_type = TOKEN_START_ARRAY;
		break;
	case ']':
		m_type = TOKEN_END_ARRAY;
		break;
	case '/':
		{
			m_type = TOKEN_NAME;
			// The slash is not part of the name
			// buffer += ch;
			while (true)
			{
				ch = ReadChar();
				if (IsDelimiterOrWhitespace(ch))
					break;
				buffer += ch;
			}
			BackOnePosition(ch);
			break;
		}
	case '>':
		ch = ReadChar();
		if (ch != '>')
		{
			wxLogError(wxString(wxS("wxPdfTokenizer::NextToken: ")) +
				wxString(_("'>' not expected.")));
			return false;
		}
		m_type = TOKEN_END_DICTIONARY;
		break;
	case '<':
		{
			int v1 = ReadChar();
			if (v1 == '<')
			{
				m_type = TOKEN_START_DICTIONARY;
				break;
			}
			m_type = TOKEN_STRING;
			m_hexString = true;
			int v2 = 0;
			while (true)
			{
				while (IsWhitespace(v1))
				{
					v1 = ReadChar();
				}
				if (v1 == '>')
					break;
				v1 = GetHex(v1);
				if (v1 < 0)
					break;
				v2 = ReadChar();
				while (IsWhitespace(v2))
				{
					v2 = ReadChar();
				}
				if (v2 == '>')
				{
					ch = v1 << 4;
					buffer += ch;
					break;
				}
				v2 = GetHex(v2);
				if (v2 < 0)
					break;
				ch = (v1 << 4) + v2;
				buffer += ch;
				v1 = ReadChar();
			}
			if (v1 < 0 || v2 < 0)
			{
				wxLogError(wxString(wxS("wxPdfTokenizer::NextToken: ")) +
					wxString(_("Error reading string.")));
				return false;
			}
			break;
		}
	case '%':
		m_type = TOKEN_COMMENT;
		do
		{
			ch = ReadChar();
		}
		while (ch != -1 && ch != '\r' && ch != '\n');
		break;
	case '(':
		{
			m_type = TOKEN_STRING;
			m_hexString = false;
			int nesting = 0;
			while (true)
			{
				ch = ReadChar();
				if (ch == -1)
					break;
				if (ch == '(')
				{
					++nesting;
				}
				else if (ch == ')')
				{
					--nesting;
				}
				else if (ch == '\\')
				{
					bool lineBreak = false;
					ch = ReadChar();
					switch (ch)
					{
					case 'n':
						ch = '\n';
						break;
					case 'r':
						ch = '\r';
						break;
					case 't':
						ch = '\t';
						break;
					case 'b':
						ch = '\b';
						break;
					case 'f':
						ch = '\f';
						break;
					case '(':
					case ')':
					case '\\':
						break;
					case '\r':
						lineBreak = true;
						ch = ReadChar();
						if (ch != '\n')
							BackOnePosition(ch);
						break;
					case '\n':
						lineBreak = true;
						break;
					default:
						{
							if (ch < '0' || ch > '7')
							{
								break;
							}
							int octal = ch - '0';
							ch = ReadChar();
							if (ch < '0' || ch > '7')
							{
								BackOnePosition(ch);
								ch = octal;
								break;
							}
							octal = (octal << 3) + ch - '0';
							ch = ReadChar();
							if (ch < '0' || ch > '7')
							{
								BackOnePosition(ch);
								ch = octal;
								break;
							}
							octal = (octal << 3) + ch - '0';
							ch = octal & 0xff;
							break;
						}
					}
					if (lineBreak)
						continue;
					if (ch < 0)
						break;
				}
				else if (ch == '\r')
				{
					ch = ReadChar();
					if (ch < 0)
						break;
					if (ch != '\n')
					{
						BackOnePosition(ch);
						ch = '\n';
					}
				}
				if (nesting == -1)
					break;
				buffer += ch;
			}
			if (ch == -1)
			{
				wxLogError(wxString(wxS("wxPdfTokenizer::NextToken: ")) +
					wxString(_("Error reading string.")));
				return false;
			}
			break;
		}
	default:
		{
			if (ch == '-' || ch == '+' || ch == '.' || (ch >= '0' && ch <= '9'))
			{
				m_type = TOKEN_NUMBER;
				do
				{
					buffer += ch;
					ch = ReadChar();
				}
				while (ch != -1 && ((ch >= '0' && ch <= '9') || ch == '.'));
			}
			else
			{
				m_type = TOKEN_OTHER;
				do
				{
					buffer += ch;
					ch = ReadChar();
				}
				while (!IsDelimiterOrWhitespace(ch));
			}
			BackOnePosition(ch);
			break;
		}
	}
	if (buffer != wxEmptyString)
	{
		m_stringValue.Append(buffer);
		if (m_type == TOKEN_OTHER && (m_stringValue == wxS("true") || m_stringValue == wxS("false")))
		{
			m_type = TOKEN_BOOLEAN;
		}
	}
	return true;
}

void
wxPdfTokenizer::NextValidToken()
{
	int level = 0;
	wxString n1 = wxEmptyString;
	wxString n2 = wxEmptyString;
	int ptr = 0;
	while (NextToken())
	{
		if (m_type == TOKEN_COMMENT)
			continue;
		switch (level)
		{
		case 0:
			{
				if (m_type != TOKEN_NUMBER)
					return;
				ptr = Tell();
				n1 = m_stringValue;
				++level;
				break;
			}
		case 1:
			{
				if (m_type != TOKEN_NUMBER)
				{
					Seek(ptr);
					m_type = TOKEN_NUMBER;
					m_stringValue = n1;
					return;
				}
				n2 = m_stringValue;
				++level;
				break;
			}
		default:
			{
				if (m_type != TOKEN_OTHER || m_stringValue != wxS("R"))
				{
					Seek(ptr);
					m_type = TOKEN_NUMBER;
					m_stringValue = n1;
					return;
				}
				m_type = TOKEN_REFERENCE;
				long value;
				n1.ToLong(&value);
				m_reference = value;
				n2.ToLong(&value);
				m_generation = value;
				return;
			}
		}
	}
	wxLogError(wxString(wxS("wxPdfTokenizer::NextValidToken: ")) +
		wxString(_("Unexpected end of file.")));
}

int
wxPdfTokenizer::GetTokenType()
{
	return m_type;
}

wxString
wxPdfTokenizer::GetStringValue()
{
	return m_stringValue;
}

int
wxPdfTokenizer::GetIntValue()
{
	long value;
	m_stringValue.ToLong(&value);
	return value;
}

int
wxPdfTokenizer::GetReference()
{
	return m_reference;
}

int
wxPdfTokenizer::GetGeneration()
{
	return m_generation;
}

bool
wxPdfTokenizer::IsWhitespace(int ch)
{
	return (ch == 0 || ch == 9 || ch == 10 || ch == 12 || ch == 13 || ch == 32);
}

bool
wxPdfTokenizer::IsDelimiter(int ch)
{
	return (ch == '(' || ch == ')' || ch == '<' || ch == '>' || ch == '[' || ch == ']' || ch == '/' || ch == '%');
}

bool
wxPdfTokenizer::IsDelimiterOrWhitespace(int ch)
{
	return IsWhitespace(ch) || IsDelimiter(ch) || (ch == -1);
}

int
wxPdfTokenizer::GetHex(int v)
{
	if (v >= '0' && v <= '9')
		return v - '0';
	if (v >= 'A' && v <= 'F')
		return v - 'A' + 10;
	if (v >= 'a' && v <= 'f')
		return v - 'a' + 10;
	return -1;
}
