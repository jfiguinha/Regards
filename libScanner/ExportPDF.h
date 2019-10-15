#pragma once



class CExportPDF
{
public:
	CExportPDF();
	~CExportPDF();
	static int ExportPDFToHTML(wxString pdfFile, wxString fileout);
	static int ExportPDFToTXT(wxString pdfFile, wxString fileout);
};
