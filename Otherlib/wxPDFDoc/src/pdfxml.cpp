///////////////////////////////////////////////////////////////////////////////
// Name:        pdfxml.cpp
// Purpose:
// Author:      Ulrich Telle
// Created:     2005-12-03
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence + RDS Data Security license
///////////////////////////////////////////////////////////////////////////////

/// \file pdfxml.cpp Implementation of the wxPdfDocument markup

// For compilers that support precompilation, includes <wx.h>.
#include <header.h>
// includes
#include <wx/sstream.h>

#include "wx/pdfdocument.h"
#include "wx/pdffontdetails.h"
#include "wx/pdfutility.h"
#include "wx/pdfxml.h"

static wxString
GetNodeContent(const wxXmlNode* node)
{
	const wxXmlNode* n = node;
	if (n == nullptr) return wxEmptyString;
	n = n->GetChildren();
	while (n)
	{
		if (n->GetType() == wxXML_TEXT_NODE ||
			n->GetType() == wxXML_CDATA_SECTION_NODE)
			return n->GetContent();
		n = n->GetNext();
	}
	return wxEmptyString;
}

static wxString
GetXmlAttribute(const wxXmlNode* node, const wxString& attrName, const wxString& defaultVal)
{
#if wxCHECK_VERSION(2, 9, 0)
	return node->GetAttribute(attrName, defaultVal);
#else
  return node->GetPropVal(attrName, defaultVal);
#endif
}

// --- wxPdfCellContext

wxPdfCellContext::wxPdfCellContext(double maxWidth, wxPdfAlignment hAlign, wxPdfAlignment vAlign)
{
	m_maxWidth = maxWidth;
	m_hAlign = hAlign;
	m_vAlign = vAlign;
	m_currentLine = 0;
	m_currentContext = 0;
	m_aligned = false;
	m_fillStyle = 0;
	m_lastChar = 0;
	m_spaceWidth = 0;
	m_lineDelta = 0;
	m_height = 0;
	m_table = nullptr;
	AddLine();
}

wxPdfCellContext::~wxPdfCellContext()
{
	size_t j;
	for (j = 0; j < m_contexts.GetCount(); j++)
	{
		auto context = static_cast<wxPdfCellContext*>(m_contexts[j]);
		delete context;
	}
	if (m_table != nullptr)
	{
		delete m_table;
	}
}

void
wxPdfCellContext::AddLine()
{
	m_linewidth.Add(0);
	m_spaces.Add(0);
}

void
wxPdfCellContext::AddLastLineValues(double width, int spaces)
{
	m_linewidth.Last() += width;
	m_spaces.Last() += spaces;
}

void
wxPdfCellContext::AddCurrentLineWidth(double width)
{
	if (m_currentLine < m_linewidth.GetCount())
	{
		m_linewidth[m_currentLine] += width;
	}
}

void
wxPdfCellContext::AddCurrentLineSpaces(int spaces)
{
	if (m_currentLine < m_spaces.GetCount())
	{
		m_spaces[m_currentLine] += spaces;
	}
}

void
wxPdfCellContext::MarkLastLine()
{
	if (m_spaces.Last() > 0)
	{
		m_spaces.Last() *= -1;
	}
}

bool
wxPdfCellContext::IsCurrentLineMarked()
{
	bool marked = false;
	if (m_currentLine < m_linewidth.GetCount())
	{
		marked = (m_spaces[m_currentLine] < 0);
	}
	return marked;
}


void
wxPdfCellContext::IncrementCurrentLine()
{
	if (m_currentLine < m_linewidth.GetCount() - 1)
	{
		m_currentLine++;
	}
	m_aligned = false;
}

double
wxPdfCellContext::GetLineDelta()
{
	if (!m_aligned)
	{
		m_lineDelta = m_maxWidth - GetCurrentLineWidth();
	}
	return m_lineDelta;
}

double
wxPdfCellContext::GetCurrentLineWidth()
{
	double linewidth = 0;
	if (m_currentLine < m_linewidth.GetCount())
	{
		linewidth = m_linewidth[m_currentLine];
	}
	return linewidth;
}

double
wxPdfCellContext::GetLastLineWidth()
{
	return m_linewidth.Last();
}

int
wxPdfCellContext::GetCurrentLineSpaces()
{
	int spaces = 0;
	if (m_currentLine < m_spaces.GetCount())
	{
		spaces = m_spaces[m_currentLine];
		if (spaces < 0) spaces = -spaces;
	}
	return spaces;
}

void
wxPdfCellContext::IncrementCurrentContext()
{
	if (m_currentContext < m_contexts.GetCount() - 1)
	{
		m_currentContext++;
	}
}

wxPdfCellContext*
wxPdfCellContext::GetCurrentContext()
{
	wxPdfCellContext* context = nullptr;
	if (m_currentContext < m_contexts.GetCount())
	{
		context = static_cast<wxPdfCellContext*>(m_contexts[m_currentContext]);
	}
	return context;
}

void
wxPdfCellContext::AppendContext(wxPdfCellContext* context)
{
	m_contexts.Add(context);
}

// --- wxPdfTableCell

wxPdfTableCell::wxPdfTableCell(wxXmlNode* cell, unsigned int row, unsigned int col, unsigned int rows,
                               unsigned int cols)
{
	m_cell = cell;
	m_width = -1.0;
	m_height = -1.0;
	m_row = row;
	m_col = col;
	m_rowSpan = rows;
	m_colSpan = cols;

	SetBorder(wxPDF_BORDER_NONE);
	m_context = nullptr;
	m_hasCellColour = false;
}

wxPdfTableCell::~wxPdfTableCell()
{
	if (m_context != nullptr)
	{
		delete m_context;
	}
}

// --- wxPdfTable

wxPdfTable::wxPdfTable(wxPdfDocument* document)
{
	m_document = document;
	m_nRows = 0;
	m_nCols = 0;
	m_totalWidth = 0;
	m_totalHeight = 0;
	m_headHeight = 0;
	m_pad = 0;
	m_headRowFirst = 0;
	m_headRowLast = 0;
	m_bodyRowFirst = 0;
	m_bodyRowLast = 0;
	m_borderWidth = -1;
}

wxPdfTable::~wxPdfTable()
{
	// Free all cells
	wxPdfCellHashMap::iterator cell;
	for (cell = m_table.begin(); cell != m_table.end(); ++cell)
	{
		if (cell->second != nullptr)
		{
			delete cell->second;
		}
	}
}

void
wxPdfTable::SetCellDimensions(double maxWidth)
{
	unsigned int row, col;
	double defaultWidth = (m_nCols > 0) ? maxWidth / m_nCols : 0;
	for (col = 0; col < m_nCols; col++)
	{
		if (col >= m_colWidths.size() || m_colWidths[col] <= 0)
		{
			SetColumnWidth(col, defaultWidth);
		}
	}

	if (m_totalWidth > maxWidth)
	{
		double factor = maxWidth / m_totalWidth;
		for (col = 0; col < m_colWidths.size(); col++)
		{
			m_colWidths[col] /= factor;
		}
	}

	double rowHeight;
	wxPdfBoolHashMap rowSpans;
	unsigned int rowSpan;
	unsigned int maxRowSpan = 1;
	for (row = 0; row < m_nRows; row++)
	{
		rowHeight = 0;
		for (col = 0; col < m_nCols; col++)
		{
			wxPdfCellHashMap::iterator foundCell = m_table.find((row << 16) | col);
			if (foundCell != m_table.end())
			{
				wxPdfTableCell* cell = foundCell->second;
				int span;
				double cellWidth = 0;
				for (span = 0; span < static_cast<int>(cell->GetColSpan()); span++)
				{
					cellWidth += m_colWidths[col + span];
				}
				if (cellWidth > 2 * m_pad) cellWidth -= (2 * m_pad);
				cell->SetWidth(cellWidth);
				auto cellContext = new wxPdfCellContext(cellWidth, cell->GetHAlign());
				cell->SetContext(cellContext);
				m_document->PrepareXmlCell(cell->GetXmlNode(), *cellContext);
				double height = cellContext->GetHeight() + 2 * m_pad;
				cell->SetHeight(height);
				rowSpan = cell->GetRowSpan();
				rowSpans[rowSpan] = true;
				if (rowSpan == 1 && height > rowHeight)
				{
					rowHeight = height;
				}
				else
				{
					if (rowSpan > maxRowSpan)
					{
						maxRowSpan = rowSpan;
					}
				}
			}
		}
		m_rowHeights[row] = (rowHeight < m_minHeights[row]) ? m_minHeights[row] : rowHeight;
	}

	for (rowSpan = 2; rowSpan <= maxRowSpan; rowSpan++)
	{
		wxPdfBoolHashMap::iterator currentRowSpan = rowSpans.find(rowSpan);
		if (currentRowSpan != rowSpans.end())
		{
			for (row = 0; row < m_nRows; row++)
			{
				rowHeight = 0;
				for (col = 0; col < m_nCols; col++)
				{
					wxPdfCellHashMap::iterator foundCell = m_table.find((row << 16) | col);
					if (foundCell != m_table.end())
					{
						wxPdfTableCell* cell = foundCell->second;
						if (rowSpan == cell->GetRowSpan())
						{
							rowHeight = 0;
							int span;
							for (span = cell->GetRowSpan() - 1; span >= 0; span--)
							{
								rowHeight += m_rowHeights[row + span];
							}
							if (cell->GetHeight() > rowHeight)
							{
								double delta = (cell->GetHeight() - rowHeight) / cell->GetRowSpan();
								for (span = cell->GetRowSpan() - 1; span >= 0; span--)
								{
									m_rowHeights[row + span] += delta;
								}
							}
						}
					}
				}
			}
		}
	}
	m_headHeight = 0;
	for (row = m_headRowFirst; row < m_headRowLast; row++)
	{
		m_headHeight += m_rowHeights[row];
	}
	m_totalHeight = 0;
	for (row = m_bodyRowFirst; row < m_bodyRowLast; row++)
	{
		m_totalHeight += m_rowHeights[row];
	}

	// Determine max row heights taking row spans into account
	row = m_bodyRowFirst;
	unsigned int firstRow = row;
	unsigned int nextRow = row + 1;
	while (row < m_bodyRowLast)
	{
		// Initialize max row height
		m_maxHeights[row] = m_rowHeights[row];

		// Determine max row span of current row
		maxRowSpan = 1;
		for (col = 0; col < m_nCols; col++)
		{
			wxPdfCellHashMap::iterator foundCell = m_table.find((row << 16) | col);
			if (foundCell != m_table.end())
			{
				wxPdfTableCell* cell = foundCell->second;
				rowSpan = cell->GetRowSpan();
				if (rowSpan > maxRowSpan)
				{
					maxRowSpan = rowSpan;
				}
			}
		}

		// Adjust next row not affected by row spans
		if (nextRow < row + maxRowSpan)
		{
			nextRow = row + maxRowSpan;
		}

		// Advance row
		if (row < nextRow - 1)
		{
			// Next row still in row block affected by row spans
			++row;
		}
		else
		{
			// Adjust max row heights of current row block affected by row spans
			for (row = nextRow - 2; row >= firstRow; --row)
			{
				m_maxHeights[row] += m_maxHeights[row + 1];
			}

			// Next row starts new row block
			firstRow = row = nextRow;
			nextRow = firstRow + 1;
		}
	}
}

void
wxPdfTable::InsertCell(wxPdfTableCell* cell)
{
	unsigned int x = cell->GetCol();
	unsigned int y = cell->GetRow();
	unsigned int w = cell->GetColSpan();
	unsigned int h = cell->GetRowSpan();
	m_table[(y << 16) | x] = cell;
	if (x + w > m_nCols)
	{
		m_nCols = x + w;
	}
	if (y + h > m_nRows)
	{
		m_nRows = y + h;
	}
}

void
wxPdfTable::Write()
{
	bool writeHeader = m_headRowLast > m_headRowFirst;
	bool newPage = false;
	double saveLeftMargin = m_document->GetLeftMargin();
	double x, y;
	unsigned int row, headRow;
	y = m_document->GetY();
	double breakMargin = m_document->GetBreakMargin();
	double pageHeight = m_document->GetPageHeight();
	double yMax = pageHeight - breakMargin;
	if (y + m_headHeight + m_maxHeights[m_bodyRowFirst] > yMax)
	{
		newPage = true;
	}
	for (row = m_bodyRowFirst; row < m_bodyRowLast; row++)
	{
		if (!newPage && (y + m_maxHeights[row] > yMax))
		{
			newPage = true;
		}
		if (newPage)
		{
			newPage = false;
			m_document->AddPage();
			writeHeader = m_headRowLast > m_headRowFirst;
			y = m_document->GetY();
		}
		if (writeHeader)
		{
			writeHeader = false;
			for (headRow = m_headRowFirst; headRow < m_headRowLast; headRow++)
			{
				x = saveLeftMargin;
				WriteRow(headRow, x, y);
				y += m_rowHeights[headRow];
			}
		}
		x = saveLeftMargin;
		WriteRow(row, x, y);
		y += m_rowHeights[row];
	}
	m_document->SetXY(saveLeftMargin, y);
}

void
wxPdfTable::WriteRow(unsigned int row, double x, double y)
{
	bool isHeaderRow = (row >= m_headRowFirst && row < m_headRowLast);
	unsigned int col;
	unsigned int rowspan, colspan;
	double w, h;
	m_document->SetXY(x, y + m_pad);
	for (col = 0; col < m_nCols; col++)
	{
		wxPdfCellHashMap::iterator foundCell = m_table.find((row << 16) | col);
		if (foundCell != m_table.end())
		{
			wxPdfTableCell* cell = foundCell->second;
			w = 0;
			for (colspan = 0; colspan < cell->GetColSpan(); colspan++)
			{
				w += m_colWidths[col + colspan];
			}
			h = 0;
			for (rowspan = 0; rowspan < cell->GetRowSpan(); rowspan++)
			{
				h += m_rowHeights[row + rowspan];
			}
			if (cell->HasCellColour())
			{
				wxPdfColour saveFillColour = m_document->GetFillColour();
				m_document->SetFillColour(cell->GetCellColour());
				m_document->Rect(x, y, w, h, wxPDF_STYLE_FILL);
				m_document->SetFillColour(saveFillColour);
			}
			int border = cell->GetBorder();
			if (border != wxPDF_BORDER_NONE)
			{
				double savedLineWidth = m_document->GetLineWidth();
				wxPdfColour savedDrawingColour = m_document->GetDrawColour();
				if (m_borderWidth > 0)
				{
					m_document->SetLineWidth(m_borderWidth);
				}
				if (m_borderColour.GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
				{
					m_document->SetDrawColour(m_borderColour);
				}
				if ((border & wxPDF_BORDER_FRAME) == wxPDF_BORDER_FRAME)
				{
					m_document->Rect(x, y, w, h);
				}
				else
				{
					if (border & wxPDF_BORDER_LEFT) m_document->Line(x, y, x, y + h);
					if (border & wxPDF_BORDER_TOP) m_document->Line(x, y, x + w, y);
					if (border & wxPDF_BORDER_BOTTOM) m_document->Line(x, y + h, x + w, y + h);
					if (border & wxPDF_BORDER_RIGHT) m_document->Line(x + w, y, x + w, y + h);
				}
				if (m_borderColour.GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
				{
					m_document->SetDrawColour(savedDrawingColour);
				}
				if (m_borderWidth > 0)
				{
					m_document->SetLineWidth(savedLineWidth);
				}
			}
			m_document->SetLeftMargin(x + m_pad);
			double delta = h - cell->GetHeight();
			switch (cell->GetVAlign())
			{
			case wxPDF_ALIGN_BOTTOM:
				m_document->SetXY(x + m_pad, y + m_pad + delta);
				break;
			case wxPDF_ALIGN_MIDDLE:
				m_document->SetXY(x + m_pad, y + m_pad + 0.5 * delta);
				break;
			case wxPDF_ALIGN_TOP:
			default:
				m_document->SetXY(x + m_pad, y + m_pad);
				break;
			}
			m_document->WriteXmlCell(cell->GetXmlNode(), *(cell->GetContext()));
			if (isHeaderRow)
			{
				// For header rows it is necessary to prepare the cells for reprocessing
				delete cell->GetContext();
				auto cellContext = new wxPdfCellContext(cell->GetWidth(), cell->GetHAlign());
				cell->SetContext(cellContext);
				m_document->PrepareXmlCell(cell->GetXmlNode(), *cellContext);
			}
		}
		x += m_colWidths[col];
	}
}

void
wxPdfTable::SetColumnWidth(int col, double width)
{
	m_colWidths[col] = width;
	m_totalWidth += width;
}

// ----- wxPdfDocument

void
wxPdfDocument::PrepareXmlTable(wxXmlNode* node, wxPdfCellContext& context)
{
	wxPdfTable* table = context.GetTable();
	double maxWidth = context.GetMaxWidth();
	wxPdfBoolHashMap cellused;
	int coldef = 0;
	int colundef = 0;
	int row = 0;
	int col;
	int i, j;

	wxXmlNode* child = node->GetChildren();
	while (child)
	{
		wxString name = (child->GetName()).Lower();
		if (name == wxS("colgroup"))
		{
			wxXmlNode* colChild = child->GetChildren();
			while (colChild)
			{
				if ((colChild->GetName()).Lower() == wxS("col"))
				{
					long colspan;
					wxString span = GetXmlAttribute(colChild, wxS("span"), wxS("1"));
					if (span.Length() > 0 && span.ToLong(&colspan))
					{
						if (colspan < 1) colspan = 1;
					}
					else
					{
						colspan = 1;
					}
					double colwidth = 0;
					wxString width = GetXmlAttribute(colChild, wxS("width"), wxS("0"));
					if (width.Length() > 0)
					{
						colwidth = wxPdfUtility::String2Double(width);
						if (colwidth < 0) colwidth = 0;
					}
					for (col = 0; col < colspan; col++)
					{
						table->SetColumnWidth(coldef++, colwidth);
						if (colwidth <= 0) colundef++;
					}
				}
				colChild = colChild->GetNext();
			}
		}
		else if (name == wxS("thead") || name == wxS("tbody"))
		{
			wxString oddColour = GetXmlAttribute(child, wxS("odd"), wxS("")).Lower();
			wxString evenColour = GetXmlAttribute(child, wxS("even"), wxS("")).Lower();
			bool isHeader = name == wxS("thead");
			if (isHeader)
			{
				table->SetHeadRowFirst(row);
			}
			else
			{
				table->SetBodyRowFirst(row);
			}
			wxXmlNode* rowChild = child->GetChildren();
			int rowCount = 0;
			while (rowChild)
			{
				if ((rowChild->GetName()).Lower() == wxS("tr"))
				{
					wxString rowColour = GetXmlAttribute(rowChild, wxS("bgcolor"), wxS("")).Lower();
					rowCount++;
					if (rowColour.Length() == 0)
					{
						if (rowCount % 2 == 0)
						{
							// even row numbers
							if (evenColour.Length() > 0) rowColour = evenColour;
						}
						else
						{
							// odd row numbers
							if (oddColour.Length() > 0) rowColour = oddColour;
						}
					}
					double rowHeight = 0;
					wxString height = GetXmlAttribute(rowChild, wxS("height"), wxS("0")).Lower();
					if (height.Length() > 0)
					{
						rowHeight = wxPdfUtility::String2Double(height);
						if (rowHeight < 0) rowHeight = 0;
					}
					table->SetMinRowHeight(row, rowHeight);
					wxXmlNode* colChild = rowChild->GetChildren();
					col = 0;
					while (colChild)
					{
						if ((colChild->GetName()).Lower() == wxS("td"))
						{
							int border = wxPDF_BORDER_NONE;
							wxString strBorder = GetXmlAttribute(colChild, wxS("border"), wxS("")).Upper();
							if (strBorder.Length() > 0)
							{
								if (strBorder.Contains(wxS("L"))) border |= wxPDF_BORDER_LEFT;
								if (strBorder.Contains(wxS("T"))) border |= wxPDF_BORDER_TOP;
								if (strBorder.Contains(wxS("B"))) border |= wxPDF_BORDER_BOTTOM;
								if (strBorder.Contains(wxS("R"))) border |= wxPDF_BORDER_RIGHT;
							}
							else if (table->HasBorder())
							{
								border = wxPDF_BORDER_FRAME;
							}

							wxString align = GetXmlAttribute(colChild, wxS("align"), wxS("left")).Lower();
							wxPdfAlignment hAlignment = wxPDF_ALIGN_LEFT;
							if (align == wxS("right")) hAlignment = wxPDF_ALIGN_RIGHT;
							else if (align == wxS("center")) hAlignment = wxPDF_ALIGN_CENTER;
							align = GetXmlAttribute(colChild, wxS("valign"), wxS("top")).Lower();
							wxPdfAlignment vAlignment = wxPDF_ALIGN_TOP;
							if (align == wxS("bottom")) vAlignment = wxPDF_ALIGN_BOTTOM;
							else if (align == wxS("middle")) vAlignment = wxPDF_ALIGN_MIDDLE;
							else if (align == wxS("center")) vAlignment = wxPDF_ALIGN_MIDDLE;

							wxString bgColour = GetXmlAttribute(colChild, wxS("bgcolor"), wxS("")).Lower();
							wxString rowspan = GetXmlAttribute(colChild, wxS("rowspan"), wxS("1"));
							long rs;
							if (rowspan.Length() > 0 && rowspan.ToLong(&rs))
							{
								if (rs < 1) rs = 1;
							}
							else
							{
								rs = 1;
							}
							long cs;
							wxString colspan = GetXmlAttribute(colChild, wxS("colspan"), wxS("1"));
							if (colspan.Length() > 0 && colspan.ToLong(&cs))
							{
								if (cs < 1) cs = 1;
							}
							else
							{
								cs = 1;
							}
							while (cellused.find((row << 16) | col) != cellused.end())
							{
								++col;
							}
							for (i = 0; i < cs; i++)
							{
								for (j = 0; j < rs; j++)
								{
									cellused[((j + row) << 16) | (i + col)] = true;
								}
							}
							auto cell = new wxPdfTableCell(colChild, row, col, rs, cs);
							cell->SetHAlign(hAlignment);
							cell->SetVAlign(vAlignment);
							cell->SetBorder(border);
							if (bgColour.Length() > 0)
							{
								cell->SetCellColour(wxPdfColour(bgColour));
							}
							else if (rowColour.Length() > 0)
							{
								cell->SetCellColour(wxPdfColour(rowColour));
							}
							table->InsertCell(cell);
						}
						colChild = colChild->GetNext();
					}
					row++;
				}
				if (isHeader)
				{
					table->SetHeadRowLast(row);
				}
				else
				{
					table->SetBodyRowLast(row);
				}
				rowChild = rowChild->GetNext();
			}
		}
		child = child->GetNext();
	}
	table->SetCellDimensions(maxWidth);
	context.AddHeight(table->GetTotalHeight());
}

void
wxPdfDocument::WriteXmlTable(wxPdfCellContext& context)
{
	double saveLeftMargin = GetLeftMargin();
	double saveRightMargin = GetRightMargin();
	wxPdfTable* table = context.GetTable();
	double maxWidth = context.GetMaxWidth();
	double tableWidth = table->GetTotalWidth();
	double delta = 0;
	if (tableWidth < maxWidth)
	{
		wxPdfAlignment align = context.GetHAlign();
		switch (align)
		{
		case wxPDF_ALIGN_CENTER:
			delta = 0.5 * (maxWidth - tableWidth);
			break;
		case wxPDF_ALIGN_RIGHT:
			delta = maxWidth - tableWidth;
			break;
		case wxPDF_ALIGN_LEFT:
		default:
			delta = 0;
			break;
		}
	}
	// Set left and right margin
	SetLeftMargin(saveLeftMargin + delta);
	SetRightMargin(GetPageWidth() - saveLeftMargin - tableWidth - delta);
	SetXY(saveLeftMargin + delta, GetY());

	// Check alignment

	table->Write();

	SetLeftMargin(saveLeftMargin);
	SetRightMargin(saveRightMargin);
}

void
wxPdfDocument::PrepareXmlCell(wxXmlNode* node, wxPdfCellContext& context)
{
	wxPdfCellContext* newContext;
	wxXmlNode* child = node->GetChildren();
	while (child)
	{
		wxString name = (child->GetName()).Lower();

		if (name == wxS("b") || name == wxS("i") || name == wxS("u") ||
			name == wxS("o") || name == wxS("s") ||
			name == wxS("strong") || name == wxS("em"))
		{
			// --- Bold, Italic, Underline, Overline, Strikeout
			// --- Strong (= bold), Emphasize (= italic)
			wxString addStyle = name.Upper();
			if (name.Length() > 1)
			{
				if (name == wxS("strong")) addStyle = wxS("B");
				else if (name == wxS("em")) addStyle = wxS("I");
			}
			wxString style = GetFontStyle();
			if (!style.Contains(addStyle))
			{
				SelectFont(wxS(""), style + addStyle, 0, false);
				PrepareXmlCell(child, context);
				SelectFont(wxS(""), style, 0, false);
			}
			else
			{
				PrepareXmlCell(child, context);
			}
		}
		else if (name == wxS("small"))
		{
			// --- Small font size
			static double ratio = 2. / 3.;
			double currentSize = GetFontSize();
			SetFontSize(currentSize * ratio, false);
			double delta = (currentSize - GetFontSize()) * 0.5 * ratio / GetScaleFactor();
			SetXY(GetX(), GetY() + delta);
			PrepareXmlCell(child, context);
			SetXY(GetX(), GetY() - delta);
			SetFontSize(currentSize, false);
		}
		else if (name == wxS("sup"))
		{
			// --- Superscript
			static double ratio = 2. / 3.;
			double currentSize = GetFontSize();
			SetFontSize(currentSize * ratio, false);
			double delta = (currentSize - GetFontSize()) * ratio / GetScaleFactor();
			SetXY(GetX(), GetY() - delta);
			PrepareXmlCell(child, context);
			SetXY(GetX(), GetY() + delta);
			SetFontSize(currentSize, false);
		}
		else if (name == wxS("sub"))
		{
			// --- Subscript
			static double ratio = 2. / 3.;
			double currentSize = GetFontSize();
			SetFontSize(currentSize * ratio, false);
			double delta = (currentSize - GetFontSize()) * ratio / GetScaleFactor();
			SetXY(GetX(), GetY() + delta);
			PrepareXmlCell(child, context);
			SetXY(GetX(), GetY() - delta);
			SetFontSize(currentSize, false);
		}
		else if (name == wxS("ul"))
		{
			// --- Unordered list
			double leftMargin = GetLeftMargin();
			double indent = GetFontSize() / GetScaleFactor();
			SetLeftMargin(leftMargin + indent);
			wxXmlNode* listChild = child->GetChildren();
			while (listChild)
			{
				if ((listChild->GetName()).Lower() == wxS("li"))
				{
					Ln();
					newContext = new wxPdfCellContext(context.GetMaxWidth() - indent, wxPDF_ALIGN_LEFT);
					//# newContext->AddHeight(GetLineHeight());
					context.AppendContext(newContext);
					PrepareXmlCell(listChild, *newContext);
					context.AddHeight(newContext->GetHeight());
					Ln();
				}
				listChild = listChild->GetNext();
			}
			SetLeftMargin(leftMargin);
		}
		else if (name == wxS("ol"))
		{
			// --- Ordered list
			double leftMargin = GetLeftMargin();
			double indent = GetStringWidth(wxS(" 00. "));
			wxString type = GetXmlAttribute(child, wxS("type"), wxS("1"));
			if (type.Length() > 0)
			{
				if (type[0] == wxS('a'))
					indent = GetStringWidth(wxS(" xx. "));
				else if (type[0] == wxS('A'))
					indent = GetStringWidth(wxS(" XX. "));
				else if (type[0] == wxS('i'))
					indent = GetStringWidth(wxS(" xxx. "));
				else if (type[0] == wxS('I'))
					indent = GetStringWidth(wxS(" XXX. "));
				else if (type[0] == wxS('z') &&
					type.Length() > 1 && type[1] >= wxS('1') && type[1] <= wxS('4'))
					indent = 1.1 * GetFontSize() / GetScaleFactor();
			}
			SetLeftMargin(leftMargin + indent);
			wxXmlNode* listChild = child->GetChildren();
			while (listChild)
			{
				if ((listChild->GetName()).Lower() == wxS("li"))
				{
					Ln();
					newContext = new wxPdfCellContext(context.GetMaxWidth() - indent, wxPDF_ALIGN_LEFT);
					//# newContext->AddHeight(GetLineHeight());
					context.AppendContext(newContext);
					PrepareXmlCell(listChild, *newContext);
					context.AddHeight(newContext->GetHeight());
					Ln();
				}
				listChild = listChild->GetNext();
			}
			SetLeftMargin(leftMargin);
		}
		else if (name == wxS("br"))
		{
			// --- Line break
			Ln();
			//# context.AddHeight(GetLineHeight());
			context.MarkLastLine();
			context.AddLine();
		}
		else if (name == wxS("p"))
		{
			// --- Paragraph
			wxString align = GetXmlAttribute(child, wxS("align"), wxS("left")).Lower();
			wxPdfAlignment alignment = wxPDF_ALIGN_LEFT;
			if (align == wxS("right")) alignment = wxPDF_ALIGN_RIGHT;
			else if (align == wxS("center")) alignment = wxPDF_ALIGN_CENTER;
			else if (align == wxS("justify")) alignment = wxPDF_ALIGN_JUSTIFY;

			Ln();
			newContext = new wxPdfCellContext(context.GetMaxWidth(), alignment);
			context.AppendContext(newContext);
			PrepareXmlCell(child, *newContext);
			newContext->MarkLastLine();
			context.AddHeight(newContext->GetHeight() + GetLineHeight());
			Ln();
			Ln();
		}
		else if (name == wxS("hr"))
		{
			// --- Horizontal rule
			//      double hrWidth = GetPageWidth() - GetLeftMargin() - GetRightMargin();
			//      long widthAttr;
			Ln();
			//      wxString strWidth = child->GetPropVal(wxS("width"), wxS(""));
			//      if (strWidth.Length() > 0 && strWidth.ToLong(&widthAttr))
			//      {
			//        hrWidth = hrWidth * 0.01 * widthAttr;
			//      }
			//      double x = GetX();
			//      double y = GetY();
			//      double wLine = GetLineWidth();
			//      SetLineWidth(0.2);
			//      Line(x, y, x + hrWidth, y);
			//      SetLineWidth(wLine);
			//      Ln();
			context.AddHeight(GetLineHeight());
			//context.AddLine();
		}
		else if (name == wxS("a"))
		{
			// --- Anchor
			PrepareXmlCell(child, context);
		}
		else if (name == wxS("img"))
		{
			// --- Image
			wxString src = GetXmlAttribute(child, wxS("src"), wxS(""));
			if (src.Length() > 0)
			{
				wxSize imageSize = GetImageSize(src);
				double wImage = static_cast<double>(imageSize.GetWidth()) / (GetImageScale() * GetScaleFactor());
				double hImage = static_cast<double>(imageSize.GetHeight()) / (GetImageScale() * GetScaleFactor());
				long width;
				long height;
				wxString strWidth = GetXmlAttribute(child, wxS("width"), wxS("0"));
				wxString strHeight = GetXmlAttribute(child, wxS("height"), wxS("0"));
				if (!strWidth.ToLong(&width)) width = 0;
				if (!strHeight.ToLong(&height)) height = 0;
				double w = static_cast<double>(width) / (GetImageScale() * GetScaleFactor());
				double h = static_cast<double>(height) / (GetImageScale() * GetScaleFactor());
				// TODO: handle image
				// line height, position, margins etc.
				if (h <= 0 && wImage > 0)
				{
					h = (w <= 0) ? hImage : hImage * (w / wImage);
				}
				context.AddHeight(h);
			}
		}
		else if (name == wxS("span"))
		{
			PrepareXmlCell(child, context);
		}
		else if (name == wxS("font"))
		{
			// --- Font
			wxString saveFamily = GetFontFamily();
			wxString saveStyle = GetFontStyle();
			double saveSize = GetFontSize();
			wxString strFace = GetXmlAttribute(child, wxS("face"), wxS(""));
			double size = 0;
			wxString strSize = GetXmlAttribute(child, wxS("size"), wxS(""));
			if (strSize.Length() > 0)
			{
				size = wxPdfUtility::String2Double(strSize);
			}
			if (size <= 0) size = saveSize;
			SelectFont(strFace, saveStyle, size, false);
			PrepareXmlCell(child, context);
			SelectFont(saveFamily, saveStyle, saveSize, false);
		}
		else if (name == wxS("code"))
		{
			// --- Code section
			wxString saveFamily = GetFontFamily();
			wxString saveStyle = GetFontStyle();
			double saveSize = GetFontSize();
			wxString strFace = GetXmlAttribute(child, wxS("face"), wxS("courier"));
			double size = 0;
			wxString strSize = GetXmlAttribute(child, wxS("size"), wxS("10"));
			if (strSize.Length() > 0)
			{
				size = wxPdfUtility::String2Double(strSize);
			}
			if (size <= 0) size = saveSize;
			SelectFont(strFace, wxS(""), size, false);
			Ln();
			context.MarkLastLine();
			context.AddLine();
			PrepareXmlCell(child, context);
			Ln();
			SelectFont(saveFamily, saveStyle, saveSize, false);
		}
		else if (name == wxS("h1") || name == wxS("h2") || name == wxS("h3") ||
			name == wxS("h4") || name == wxS("h5") || name == wxS("h6"))
		{
			// --- Header
			wxString align = GetXmlAttribute(child, wxS("align"), wxS("left")).Lower();
			wxPdfAlignment alignment = wxPDF_ALIGN_LEFT;
			if (align == wxS("right")) alignment = wxPDF_ALIGN_RIGHT;
			else if (align == wxS("center")) alignment = wxPDF_ALIGN_CENTER;
			else if (align == wxS("justify")) alignment = wxPDF_ALIGN_JUSTIFY;

			if (context.GetLastLineWidth() > 0)
			{
				Ln();
			}
			Ln();
			context.AddHeight(GetLineHeight());
			newContext = new wxPdfCellContext(context.GetMaxWidth(), alignment);
			context.AppendContext(newContext);

			double headsize = (wxS('4') - name[1]) * 2;
			double currentFontSize = GetFontSize();
			SetFontSize(currentFontSize + headsize, false);
			wxString addStyle = wxS("B");
			wxString style = GetFontStyle();
			if (!style.Contains(addStyle))
			{
				SelectFont(wxS(""), style + addStyle, 0, false);
				PrepareXmlCell(child, *newContext);
				SelectFont(wxS(""), style, 0, false);
			}
			else
			{
				PrepareXmlCell(child, *newContext);
			}
			context.AddHeight(newContext->GetHeight());
			// reset
			SetFontSize(currentFontSize, false);
			Ln();
		}
		else if (name == wxS("table"))
		{
			// --- Table
			wxString border = GetXmlAttribute(child, wxS("border"), wxS("0")).Lower();
			bool hasBorder = (border != wxS("0"));
			wxString strBorderWidth = GetXmlAttribute(child, wxS("borderwidth"), wxS(""));
			double borderWidth = -1;
			if (hasBorder && strBorderWidth.Length() > 0)
			{
				borderWidth = wxPdfUtility::String2Double(strBorderWidth);
				if (borderWidth < 0) borderWidth = -1;
			}
			wxString strBorderColour = GetXmlAttribute(child, wxS("bordercolor"), wxS(""));
			wxPdfColour borderColour;
			if (strBorderColour.Length() > 0)
			{
				borderColour.SetColour(strBorderColour);
			}

			wxString align = GetXmlAttribute(child, wxS("align"), wxS("")).Lower();
			wxPdfAlignment hAlignment = context.GetHAlign();
			if (align == wxS("right")) hAlignment = wxPDF_ALIGN_RIGHT;
			else if (align == wxS("center")) hAlignment = wxPDF_ALIGN_CENTER;
			else if (align == wxS("justify")) hAlignment = wxPDF_ALIGN_JUSTIFY;
			align = GetXmlAttribute(child, wxS("valign"), wxS("top")).Lower();
			wxPdfAlignment vAlignment = wxPDF_ALIGN_TOP;
			if (align == wxS("bottom")) vAlignment = wxPDF_ALIGN_BOTTOM;
			else if (align == wxS("middle")) vAlignment = wxPDF_ALIGN_MIDDLE;
			else if (align == wxS("center")) vAlignment = wxPDF_ALIGN_MIDDLE;
			double pad = 1.5 / GetScaleFactor();
			wxString padding = GetXmlAttribute(child, wxS("cellpadding"), wxS("")).Lower();
			if (padding.Length() > 0)
			{
				pad = wxPdfUtility::String2Double(padding);
				if (pad < 0) pad = 0;
			}

			auto table = new wxPdfTable(this);
			table->SetPad(pad);
			table->SetBorder(hasBorder);
			table->SetBorderWidth(borderWidth);
			if (borderColour.GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
			{
				table->SetBorderColour(borderColour);
			}
			newContext = new wxPdfCellContext(context.GetMaxWidth(), hAlignment, vAlignment);
			context.AppendContext(newContext);
			newContext->SetTable(table);
			PrepareXmlTable(child, *newContext);
			context.AddHeight(newContext->GetHeight());
		}
		else
		{
			// --- Content
			if (child->GetType() == wxXML_TEXT_NODE || name == wxS("msg"))
			{
				if (context.GetLastLineWidth() == 0)
				{
					context.AddHeight(GetLineHeight());
				}
				double wmax = context.GetMaxWidth() - context.GetLastLineWidth();
				wxString s;
				if (name == wxS("msg"))
				{
					s = GetNodeContent(child);
					if (m_translate) s = wxGetTranslation(s);
				}
				else
				{
					s = child->GetContent();
				}
				s.Replace(wxS("\r"),wxS("")); // remove carriage returns
				int nb = static_cast<int>(s.Length());
				if (nb > 0 && s[nb - 1] == wxS('\n'))
				{
					--nb;
				}
				int sep = -1;
				int i = 0;
				int j = 0;
				double len = 0;
				double ls = 0;
				int ns = 0;
				int nl = 1;
				wxChar c = 0;
				while (i < nb)
				{
					// Get next character
					c = s[i];
					if (c == wxS('\n'))
					{
						// Explicit line break
						i++;
						context.AddLastLineValues(len, ns);
						sep = -1;
						j = i;
						len = 0;
						ns = 0;
						nl++;
						context.MarkLastLine();
						context.AddLine();
						context.AddHeight(GetLineHeight());
						wmax = context.GetMaxWidth();
						continue;
					}
					if (c == wxS(' '))
					{
						sep = i;
						ls = len;
						ns++;
					}
					double lastlen = len;
					len = GetStringWidth(s.SubString(j, i));

					if (len > wmax)
					{
						// Automatic line break
						if (sep == -1)
						{
							ls = lastlen;
							if (context.GetLastLineWidth() > 0)
							{
								if (context.GetLastChar() == wxS(' '))
								{
									context.AddLastLineValues(-context.GetLastSpaceWidth(), -1);
								}
								i = j;
							}
							else
							{
								if (i == j)
								{
									i++;
								}
							}
						}
						else
						{
							i = sep + 1;
							ns--;
						}
						context.AddLastLineValues(ls, ns);
						sep = -1;
						j = i;
						len = 0;
						ns = 0;
						nl++;
						context.AddLine();
						context.AddHeight(GetLineHeight());
						wmax = context.GetMaxWidth();
					}
					else
					{
						i++;
					}
				}
				// Last chunk
				context.AddLastLineValues(len, ns);
				context.SetLastChar(c);
				context.SetLastSpaceWidth(GetStringWidth(wxS(" ")));
			}
		}
		child = child->GetNext();
	}
}

void
wxPdfDocument::WriteXml(const wxString& xmlString)
{
	if (GetLineHeight() == 0)
	{
		SetLineHeight(GetFontSize() * 1.25 / GetScaleFactor());
	}
	wxString xmlStringWithXmlRoot(wxS("<xml>") + xmlString + wxS("</xml>"));
	wxStringInputStream xmlStream(xmlStringWithXmlRoot);
	wxXmlDocument xmlDocument;
	bool loaded = xmlDocument.Load(xmlStream);
	if (loaded)
	{
		if (xmlDocument.IsOk())
		{
			wxXmlNode* root = xmlDocument.GetRoot();
			double maxWidth = GetPageWidth() - GetRightMargin() - GetX();
			wxPdfCellContext context(maxWidth, wxPDF_ALIGN_LEFT);
			double saveX = GetX();
			double saveY = GetY();
			PrepareXmlCell(root, context);
			SetXY(saveX, saveY);
			WriteXmlCell(root, context);
		}
		else
		{
			wxLogDebug(wxString(wxS("wxPdfDocument::WriteXml: ")) +
				wxString(_("Markup invalid.")));
		}
	}
	else
	{
		// TODO: Error handling
		wxLogDebug(wxString(wxS("wxPdfDocument::WriteXml: ")) +
			wxString(_("Unable to load markup string.")));
	}
}

void
wxPdfDocument::WriteXml(wxXmlNode* node)
{
	if (GetLineHeight() == 0)
	{
		SetLineHeight(GetFontSize() * 1.25 / GetScaleFactor());
	}
	double maxWidth = GetPageWidth() - GetRightMargin() - GetX();
	wxPdfCellContext context(maxWidth, wxPDF_ALIGN_LEFT);
	double saveX = GetX();
	double saveY = GetY();
	PrepareXmlCell(node, context);
	SetXY(saveX, saveY);
	WriteXmlCell(node, context);
}

void
wxPdfDocument::WriteXmlCell(wxXmlNode* node, wxPdfCellContext& context)
{
	wxPdfCellContext* newContext;
	wxXmlNode* child = node->GetChildren();
	while (child)
	{
		wxString name = (child->GetName()).Lower();
		// parse the children

		if (name == wxS("b") || name == wxS("i") || name == wxS("u") ||
			name == wxS("o") || name == wxS("s") ||
			name == wxS("strong") || name == wxS("em"))
		{
			// --- Bold, Italic, Underline, Overline, Strikeout
			// --- Strong (= bold), Emphasize (= italic)
			wxString addStyle = name.Upper();
			if (name.Length() > 1)
			{
				if (name == wxS("strong")) addStyle = wxS("B");
				else if (name == wxS("em")) addStyle = wxS("I");
			}
			wxString style = GetFontStyle();
			if (!style.Contains(addStyle))
			{
				SetFont(wxS(""), style + addStyle);
				WriteXmlCell(child, context);
				SetFont(wxS(""), style);
			}
			else
			{
				WriteXmlCell(child, context);
			}
		}
		else if (name == wxS("small"))
		{
			// --- Small font
			static double ratio = 2. / 3.;
			double currentSize = GetFontSize();
			SetFontSize(currentSize * ratio);
			double delta = (currentSize - GetFontSize()) * 0.5 * ratio / GetScaleFactor();
			SetXY(GetX(), GetY() + delta);
			WriteXmlCell(child, context);
			SetXY(GetX(), GetY() - delta);
			SetFontSize(currentSize);
		}
		else if (name == wxS("sup"))
		{
			// --- Superscript
			static double ratio = 2. / 3.;
			double currentSize = GetFontSize();
			SetFontSize(currentSize * ratio);
			double delta = (currentSize - GetFontSize()) * ratio / GetScaleFactor();
			SetXY(GetX(), GetY() - delta);
			WriteXmlCell(child, context);
			SetXY(GetX(), GetY() + delta);
			SetFontSize(currentSize);
		}
		else if (name == wxS("sub"))
		{
			// --- Subscript
			static double ratio = 2. / 3.;
			double currentSize = GetFontSize();
			SetFontSize(currentSize * ratio);
			double delta = (currentSize - GetFontSize()) * ratio / GetScaleFactor();
			SetXY(GetX(), GetY() + delta);
			WriteXmlCell(child, context);
			SetXY(GetX(), GetY() - delta);
			SetFontSize(currentSize);
		}
		else if (name == wxS("ul"))
		{
			// --- Unordered list
#if wxUSE_UNICODE
			static wxChar bulletChar = 0x2022;
			static wxChar dashChar = 0x2013;
#else
      static wxChar bulletChar = '\x95'; // dec 149
      static wxChar dashChar   = '\x96'; // dec 150
#endif
			wxChar itemChar = bulletChar;
			bool useZapfDingBats = false;
			long zapfChar = 0;
			wxString type = GetXmlAttribute(child, wxS("type"), wxS("bullet"));

			if (type.IsNumber() && type.ToLong(&zapfChar))
			{
				if (zapfChar >= 0 && zapfChar <= 255)
				{
					itemChar = zapfChar;
					useZapfDingBats = true;
				}
			}
			else if (type.Lower() == wxS("dash"))
			{
				itemChar = dashChar;
			}
			double leftMargin = GetLeftMargin();
			double indent = GetFontSize() / GetScaleFactor();
			SetLeftMargin(leftMargin + indent);
			wxXmlNode* listChild = child->GetChildren();
			while (listChild)
			{
				if ((listChild->GetName()).Lower() == wxS("li"))
				{
					wxString saveFont = GetFontFamily();
					wxString saveStyle = GetFontStyle();
					double saveSize = GetFontSize();
					if (useZapfDingBats)
					{
						SetFont(wxS("zapfdingbats"), wxS(""), 0.7 * saveSize);
					}
					else
					{
						SetFont(wxS("Helvetica"), wxS(""), saveSize);
					}
					SetLeftMargin(leftMargin);
					SetXY(leftMargin, GetY());
					WriteCell(GetLineHeight(), wxString(itemChar));
					SetLeftMargin(leftMargin + indent);
					SetXY(leftMargin + indent, GetY());
					SetFont(saveFont, saveStyle, saveSize);
					//Ln();
					newContext = context.GetCurrentContext();
					context.IncrementCurrentContext();
					WriteXmlCell(listChild, *newContext);
					Ln();
				}
				listChild = listChild->GetNext();
			}
			SetLeftMargin(leftMargin);
			SetXY(leftMargin, GetY());
		}
		else if (name == wxS("ol"))
		{
			// --- Ordered list
			bool useZapfDingBats = false;
			int listCount = 0;
			int listType = 1;
			int listCycle = 0;
			long listStart;
			wxString start = GetXmlAttribute(child, wxS("start"), wxS("1"));
			if (start.IsNumber() && start.ToLong(&listStart))
			{
				listCount += (listStart - 1);
			}
			double indent = GetStringWidth(wxS(" 00. "));
			wxString type = GetXmlAttribute(child, wxS("type"), wxS("1"));
			if (type.Length() > 0)
			{
				if (type[0] == wxS('1'))
				{
					listType = 1;
				}
				else if (type[0] == wxS('a'))
				{
					listType = 2;
					listCycle = 26;
					indent = GetStringWidth(wxS(" xx. "));
				}
				else if (type[0] == wxS('A'))
				{
					listType = 3;
					listCycle = 26;
					indent = GetStringWidth(wxS(" XX. "));
				}
				else if (type[0] == wxS('i'))
				{
					listType = 4;
					indent = GetStringWidth(wxS(" xxx. "));
				}
				else if (type[0] == wxS('I'))
				{
					listType = 5;
					indent = GetStringWidth(wxS(" XXX. "));
				}
				else if (type[0] == wxS('z'))
				{
					if (type.Length() > 1 && type[1] >= wxS('1') && type[1] <= wxS('4'))
					{
						useZapfDingBats = true;
						listType = type[1] - wxS('1');
						listCycle = 10;
						indent = 1.1 * GetFontSize() / GetScaleFactor();
					}
				}
			}
			double leftMargin = GetLeftMargin();
			SetLeftMargin(leftMargin + indent);
			Ln();
			wxXmlNode* listChild = child->GetChildren();
			while (listChild)
			{
				if ((listChild->GetName()).Lower() == wxS("li"))
				{
					wxString saveFont = GetFontFamily();
					wxString saveStyle = GetFontStyle();
					double saveSize = GetFontSize();
					SetLeftMargin(leftMargin);
					SetXY(leftMargin, GetY());
					if (useZapfDingBats)
					{
						SetFont(wxS("zapfdingbats"), wxS(""), 0.85 * saveSize);
						wxChar itemChar = 172 + 10 * listType + listCount % listCycle;
						WriteCell(GetLineHeight(), wxString(itemChar));
					}
					else
					{
						wxChar itemChar;
						SetFont(wxS("Helvetica"), wxS(""), saveSize);
						wxString item;
						switch (listType)
						{
						case 2:
							itemChar = wxS('a') + listCount % listCycle;
							item = wxString(itemChar) + wxS(". ");
							break;
						case 3:
							itemChar = wxS('A') + listCount % listCycle;
							item = wxString(itemChar) + wxS(". ");
							break;
						case 4:
							item = wxPdfUtility::Convert2Roman(listCount + 1).Lower() + wxS(". ");
							break;
						case 5:
							item = wxPdfUtility::Convert2Roman(listCount + 1) + wxS(". ");
							break;
						case 1:
						default:
							item = wxString::Format(wxS("%d. "), listCount + 1);
							break;
						}
						Cell(indent, GetLineHeight(), item, wxPDF_BORDER_NONE, 0, wxPDF_ALIGN_RIGHT);
					}

					SetLeftMargin(leftMargin + indent);
					SetXY(leftMargin + indent, GetY());
					SetFont(saveFont, saveStyle, saveSize);
					listCount++;
					newContext = context.GetCurrentContext();
					context.IncrementCurrentContext();
					WriteXmlCell(listChild, *newContext);
					Ln();
				}
				listChild = listChild->GetNext();
			}
			SetLeftMargin(leftMargin);
			SetXY(leftMargin, GetY());
		}
		else if (name == wxS("br"))
		{
			// --- Line break
			Ln();
			context.IncrementCurrentLine();
		}
		else if (name == wxS("p"))
		{
			// --- Paragraph
			if (GetX() > GetLeftMargin())
			{
				Ln();
			}
			newContext = context.GetCurrentContext();
			context.IncrementCurrentContext();
			WriteXmlCell(child, *newContext);
			if (GetX() > GetLeftMargin())
			{
				Ln();
			}
			Ln();
		}
		else if (name == wxS("hr"))
		{
			// --- Horizontal rule
			double hrWidth = GetPageWidth() - GetLeftMargin() - GetRightMargin();
			long widthAttr;
			Ln();
			wxString strWidth = GetXmlAttribute(child, wxS("width"), wxS(""));
			if (strWidth.Length() > 0 && strWidth.ToLong(&widthAttr))
			{
				hrWidth = hrWidth * 0.01 * widthAttr;
			}
			double x = GetX();
			double y = GetY();
			double wLine = GetLineWidth();
			SetLineWidth(0.2);
			Line(x, y, x + hrWidth, y);
			SetLineWidth(wLine);
			Ln();
		}
		else if (name == wxS("a"))
		{
			// --- Anchor
			wxString href = GetXmlAttribute(child, wxS("href"), wxS(""));
			wxString nameAttr = GetXmlAttribute(child, wxS("name"), wxS(""));
			if (href.Length() > 0)
			{
				if (href[0] == wxS('#'))
				{
					nameAttr = href.Right(href.Length() - 1);
					if (nameAttr.Length() > 0)
					{
						if (m_namedLinks->find(nameAttr) == m_namedLinks->end())
						{
							(*m_namedLinks)[nameAttr] = AddLink();
						}
					}
				}
				context.SetHRef(href);
				wxPdfColour saveColour = GetTextColour();
				SetTextColour(0, 0, 255);
				wxString style = GetFontStyle();
				if (!style.Contains(wxS("U")))
				{
					SetFont(wxS(""), style + wxS("U"));
					WriteXmlCell(child, context);
					SetFont(wxS(""), style);
				}
				else
				{
					WriteXmlCell(child, context);
				}
				SetTextColour(saveColour);
				context.SetHRef(wxS(""));
			}
			else if (nameAttr.Length() > 0)
			{
				int link;
				if (m_namedLinks->find(nameAttr) == m_namedLinks->end())
				{
					link = AddLink();
					(*m_namedLinks)[nameAttr] = link;
				}
				else
				{
					link = (*m_namedLinks)[nameAttr];
				}
				SetLink(link, -1, -1);
				WriteXmlCell(child, context);
			}
			else
			{
				WriteXmlCell(child, context);
			}
		}
		else if (name == wxS("img"))
		{
			// --- Image
			wxString src = GetXmlAttribute(child, wxS("src"), wxS(""));
			if (src.Length() > 0)
			{
				wxSize imageSize = GetImageSize(src);
				double wImage = static_cast<double>(imageSize.GetWidth()) / (GetImageScale() * GetScaleFactor());
				double hImage = static_cast<double>(imageSize.GetHeight()) / (GetImageScale() * GetScaleFactor());
				long width;
				long height;
				wxString strWidth = GetXmlAttribute(child, wxS("width"), wxS("0"));
				wxString strHeight = GetXmlAttribute(child, wxS("height"), wxS("0"));
				if (!strWidth.ToLong(&width)) width = 0;
				if (!strHeight.ToLong(&height)) height = 0;
				double x = GetX();
				double y = GetY();
				double w = static_cast<double>(width) / (GetImageScale() * GetScaleFactor());
				double h = static_cast<double>(height) / (GetImageScale() * GetScaleFactor());
				if (width <= 0 && wImage > 0)
				{
					w = (height <= 0) ? wImage : wImage * (h / hImage);
				}
				if (height <= 0 && wImage > 0)
				{
					h = (width <= 0) ? hImage : hImage * (w / wImage);
				}
				wxString align = GetXmlAttribute(child, wxS("align"), wxS("left")).Lower();
				double delta;
				if (align == wxS("right"))
				{
					delta = context.GetMaxWidth() - w;
				}
				else if (align == wxS("center"))
				{
					delta = 0.5 * (context.GetMaxWidth() - w);
				}
				else
				{
					delta = 0;
				}
				// line height, position, margins etc.
				if (GetX() > GetLeftMargin())
				{
					Ln();
				}
				Image(src, x + delta, y, w, h);
				SetXY(x, y + h);
			}
		}
		else if (name == wxS("span"))
		{
			int saveFillStyle = context.GetFillStyle();
			wxPdfColour saveColor = GetFillColour();
			wxString strColor = GetXmlAttribute(child, wxS("color"), wxS(""));
			if (strColor.Length() > 0)
			{
				SetFillColour(wxPdfColour(strColor));
				context.SetFillStyle(1);
			}
			WriteXmlCell(child, context);
			if (strColor.Length() > 0)
			{
				context.SetFillStyle(saveFillStyle);
				SetFillColour(saveColor);
			}
		}
		else if (name == wxS("font"))
		{
			// --- Font
			wxString saveFamily = GetFontFamily();
			wxString saveStyle = GetFontStyle();
			double saveSize = GetFontSize();
			wxPdfColour saveColour = GetTextColour();
			wxString strFace = GetXmlAttribute(child, wxS("face"), wxS(""));
			wxString strSize = GetXmlAttribute(child, wxS("size"), wxS(""));
			wxString strColour = GetXmlAttribute(child, wxS("color"), wxS(""));
			double size = 0;
			if (strSize.Length() > 0)
			{
				size = wxPdfUtility::String2Double(strSize);
			}
			if (size <= 0) size = saveSize;
			SetFont(strFace, saveStyle, size);
			if (strColour.Length() > 0)
			{
				SetTextColour(wxPdfColour(strColour));
			}
			WriteXmlCell(child, context);
			if (strColour.Length() > 0)
			{
				SetTextColour(saveColour);
			}
			SetFont(saveFamily, saveStyle, saveSize);
		}
		else if (name == wxS("code"))
		{
			// --- Code section
			wxString saveFamily = GetFontFamily();
			wxString saveStyle = GetFontStyle();
			double saveSize = GetFontSize();
			wxPdfColour saveColour = GetTextColour();
			wxString strFace = GetXmlAttribute(child, wxS("face"), wxS("courier"));
			wxString strSize = GetXmlAttribute(child, wxS("size"), wxS("10"));
			wxString strColour = GetXmlAttribute(child, wxS("color"), wxS(""));
			double size = 0;
			if (strSize.Length() > 0)
			{
				size = wxPdfUtility::String2Double(strSize);
			}
			if (size <= 0) size = saveSize;
			SetFont(strFace, wxS(""), size);
			if (strColour.Length() > 0)
			{
				SetTextColour(wxPdfColour(strColour));
			}
			Ln();
			context.IncrementCurrentLine();
			WriteXmlCell(child, context);
			Ln();
			if (strColour.Length() > 0)
			{
				SetTextColour(saveColour);
			}
			SetFont(saveFamily, saveStyle, saveSize);
		}
		else if (name == wxS("h1") || name == wxS("h2") || name == wxS("h3") ||
			name == wxS("h4") || name == wxS("h5") || name == wxS("h6"))
		{
			// --- Header
			newContext = context.GetCurrentContext();
			context.IncrementCurrentContext();

			double headsize = (wxS('4') - name[1]) * 2;
			double currentFontSize = GetFontSize();
			if (GetX() > GetLeftMargin())
			{
				Ln();
			}
			Ln();
			wxString addStyle = wxS("B");
			wxString style = GetFontStyle();
			if (!style.Contains(addStyle))
			{
				SetFont(wxS(""), style + addStyle, currentFontSize + headsize);
				WriteXmlCell(child, *newContext);
				SetFont(wxS(""), style, currentFontSize);
			}
			else
			{
				SetFontSize(currentFontSize + headsize);
				WriteXmlCell(child, *newContext);
				SetFontSize(currentFontSize);
			}
			Ln();
		}
		else if (name == wxS("table"))
		{
			// --- Table
			newContext = context.GetCurrentContext();
			context.IncrementCurrentContext();
			WriteXmlTable(*newContext);
		}
		else
		{
			// --- Content
			if (child->GetType() == wxXML_TEXT_NODE || name == wxS("msg"))
			{
				auto link = wxPdfLink(-1);
				wxString href = context.GetHRef();
				if (href.Length() > 0)
				{
					if (href[0] == wxS('#'))
					{
						wxString nameAttr = href.Right(href.Length() - 1);
						if (nameAttr.Length() > 0)
						{
							if (m_namedLinks->find(nameAttr) != m_namedLinks->end())
							{
								link = wxPdfLink((*m_namedLinks)[nameAttr]);
							}
						}
					}
					else
					{
						link = wxPdfLink(href);
					}
				}

				double h = GetLineHeight();
				double wmax = context.GetCurrentLineWidth() + wxPDF_EPSILON;
				DoXmlAlign(context);

				wxString s;
				if (name == wxS("msg"))
				{
					s = GetNodeContent(child);
					if (m_translate) s = wxGetTranslation(s);
				}
				else
				{
					s = child->GetContent();
				}
				s.Replace(wxS("\r"),wxS("")); // remove carriage returns
				int nb = static_cast<int>(s.Length());
				if (nb > 0 && s[nb - 1] == wxS('\n'))
				{
					--nb;
				}

				int sep = -1;
				int i = 0;
				int j = 0;
				double len = 0;
				int ns = 0;
				int nl = 1;
				wxChar c;
				while (i < nb)
				{
					// Get next character
					c = s[i];
					if (c == wxS('\n'))
					{
						// Explicit line break
						if (m_ws > 0)
						{
							m_ws = 0;
							Out("0 Tw");
						}
						WriteCell(h, s.SubString(j, i - 1), wxPDF_BORDER_NONE, context.GetFillStyle(), link);
						i++;
						sep = -1;
						j = i;
						len = 0;
						ns = 0;
						nl++;
						context.IncrementCurrentLine();
						Ln();
						DoXmlAlign(context);
						wmax = context.GetCurrentLineWidth();
						continue;
					}
					if (c == wxS(' '))
					{
						sep = i;
						ns++;
					}
					len = GetStringWidth(s.SubString(j, i));

					if (len > wmax)
					{
						// Automatic line break
						if (sep == -1)
						{
							// Case 1: current line length exactly matches the maximum with
							// Case 2: current line contains no spaces and line length plus epsilon exactly matches the maximum width
							//         (line break in the middle of a word if the word is too long to fit on the line)
							if (wmax == context.GetCurrentLineWidth() ||
								((wmax == context.GetCurrentLineWidth() + wxPDF_EPSILON) && context.
									GetCurrentLineSpaces() == 0))
							{
								if (i == j)
								{
									i++;
								}
								WriteCell(h, s.SubString(j, i - 1), wxPDF_BORDER_NONE, context.GetFillStyle(), link);
							}
							else
							{
								i = j;
							}
						}
						else
						{
							WriteCell(h, s.SubString(j, sep - 1), wxPDF_BORDER_NONE, context.GetFillStyle(), link);
							i = sep + 1;
						}
						sep = -1;
						j = i;
						len = 0;
						ns = 0;
						nl++;
						context.IncrementCurrentLine();
						Ln();
						DoXmlAlign(context);
						wmax = context.GetCurrentLineWidth();
					}
					else
					{
						i++;
					}
				}
				// Last chunk
				WriteCell(h, s.SubString(j, i - 1), wxPDF_BORDER_NONE, context.GetFillStyle(), link);
				context.AddCurrentLineWidth(-len);
				if (context.GetHAlign() == wxPDF_ALIGN_JUSTIFY && m_ws > 0)
				{
					double delta = ns * m_ws;
					SetXY(GetX() + delta, GetY());
				}
			}
		}
		child = child->GetNext();
	}
}

void
wxPdfDocument::DoXmlAlign(wxPdfCellContext& context)
{
	if (!context.GetAligned())
	{
		if (m_ws > 0 && context.GetHAlign() != wxPDF_ALIGN_JUSTIFY)
		{
			m_ws = 0;
			Out("0 Tw");
			m_wsApply = false;
		}
		switch (context.GetHAlign())
		{
		case wxPDF_ALIGN_JUSTIFY:
			{
				m_ws = (!context.IsCurrentLineMarked() && context.GetCurrentLineSpaces() > 0)
					       ? (context.GetMaxWidth() - context.GetCurrentLineWidth()) / context.GetCurrentLineSpaces()
					       : 0;
				wxString fontType = m_currentFont->GetType();
				m_wsApply = (fontType == wxS("TrueTypeUnicode")) || (fontType == wxS("OpenTypeUnicode"));
				if (!m_wsApply)
				{
					OutAscii(wxPdfUtility::Double2String(m_ws * m_k, 3) + wxString(wxS(" Tw")));
				}
			}
			break;
		case wxPDF_ALIGN_CENTER:
			{
				double delta = 0.5 * (context.GetMaxWidth() - context.GetCurrentLineWidth());
				SetXY(GetX() + delta, GetY());
			}
			break;
		case wxPDF_ALIGN_RIGHT:
			{
				double delta = context.GetMaxWidth() - context.GetCurrentLineWidth();
				SetXY(GetX() + delta, GetY());
			}
			break;
		case wxPDF_ALIGN_LEFT:
		default:
			break;
		}
	}
	context.SetAligned();
}
