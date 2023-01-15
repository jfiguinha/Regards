///////////////////////////////////////////////////////////////////////////////
// Name:        pdfdc29.h
// Purpose:
// Author:      Ulrich Telle
// Created:     2010-11-28
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfdc29.h Interface of the wxPdfDC class (wxWidgets 2.9.x)

#ifndef _PDF_DC29_H_
#define _PDF_DC29_H_

/// Class representing a PDF drawing context
class WXDLLIMPEXP_PDFDOC wxPdfDC : public wxDC
{
public:
	wxPdfDC();

	// Recommended constructor
	wxPdfDC(const wxPrintData& printData);

	wxPdfDC(wxPdfDocument* pdfDocument, double templateWidth, double templateHeight);

	wxPdfDocument* GetPdfDocument();

	void SetResolution(int ppi);
	int GetResolution() const override;

	void SetImageType(wxBitmapType bitmapType, int quality = 75);

	void SetMapModeStyle(wxPdfMapModeStyle style);
	wxPdfMapModeStyle GetMapModeStyle() const;

private:
	wxDECLARE_DYNAMIC_CLASS(wxPdfDC);
};

/// Class representing the PDF drawing context implementation
class WXDLLIMPEXP_PDFDOC wxPdfDCImpl : public wxDCImpl
{
public:
	wxPdfDCImpl(wxPdfDC* owner);
	wxPdfDCImpl(wxPdfDC* owner, const wxPrintData& data);
	wxPdfDCImpl(wxPdfDC* owner, wxPdfDocument* pdfDocument, double templateWidth, double templateHeight);
	wxPdfDCImpl(wxPdfDC* owner, const wxString& file, int w = 300, int h = 200);
	~wxPdfDCImpl() override;

	void Init();

	wxPdfDocument* GetPdfDocument();
	void SetPrintData(const wxPrintData& data);
	wxPrintData& GetPrintData() { return m_printData; }

	void SetResolution(int ppi);
	int GetResolution() const override;

	void SetImageType(wxBitmapType bitmapType, int quality = 75);

	// implement base class pure virtuals

	void Clear() override;
	bool StartDoc(const wxString& message) override;
	void EndDoc() override;
	void StartPage() override;
	void EndPage() override;
	void SetFont(const wxFont& font) override;
	void SetPen(const wxPen& pen) override;
	void SetBrush(const wxBrush& brush) override;
	void SetBackground(const wxBrush& brush) override;
	void SetBackgroundMode(int mode) override;
	void SetPalette(const wxPalette& palette) override;

	void DestroyClippingRegion() override;

	wxCoord GetCharHeight() const override;
	wxCoord GetCharWidth() const override;

	bool CanDrawBitmap() const override;
	bool CanGetTextExtent() const override;
	int GetDepth() const override;
	wxSize GetPPI() const override;

	void SetMapMode(wxMappingMode mode) override;
	void SetUserScale(double x, double y) override;

	void SetLogicalScale(double x, double y) override;
	void SetLogicalOrigin(wxCoord x, wxCoord y) override;
	void SetDeviceOrigin(wxCoord x, wxCoord y) override;
	void SetAxisOrientation(bool xLeftRight, bool yBottomUp) override;
	void SetLogicalFunction(wxRasterOperationMode function) override;

	void SetTextForeground(const wxColour& colour) override;
	void ComputeScaleAndOrigin() override;

#if 0
  // RTL related functions
  // ---------------------

  // get or change the layout direction (LTR or RTL) for this dc,
  // wxLayout_Default is returned if layout direction is not supported
  virtual wxLayoutDirection GetLayoutDirection() const
        { return wxLayout_Default; }
  virtual void SetLayoutDirection(wxLayoutDirection WXUNUSED(dir))
       { }
#endif

protected:
	// the true implementations
	bool DoFloodFill(wxCoord x, wxCoord y, const wxColour& col,
	                 wxFloodFillStyle style = wxFLOOD_SURFACE) override;

	void DoGradientFillLinear(const wxRect& rect,
	                          const wxColour& initialColour,
	                          const wxColour& destColour,
	                          wxDirection nDirection = wxEAST) override;

	void DoGradientFillConcentric(const wxRect& rect,
	                              const wxColour& initialColour,
	                              const wxColour& destColour,
	                              const wxPoint& circleCenter) override;

	bool DoGetPixel(wxCoord x, wxCoord y, wxColour* col) const override;

	void DoDrawPoint(wxCoord x, wxCoord y) override;

#if wxUSE_SPLINES
	void DoDrawSpline(const wxPointList* points) override;
#endif

	void DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2) override;

	void DoDrawArc(wxCoord x1, wxCoord y1,
	               wxCoord x2, wxCoord y2,
	               wxCoord xc, wxCoord yc) override;

	void DoDrawCheckMark(wxCoord x, wxCoord y,
	                     wxCoord width, wxCoord height) override;

	void DoDrawEllipticArc(wxCoord x, wxCoord y, wxCoord w, wxCoord h,
	                       double sa, double ea) override;

	void DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height) override;
	void DoDrawRoundedRectangle(wxCoord x, wxCoord y,
	                            wxCoord width, wxCoord height,
	                            double radius) override;
	void DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height) override;

	void DoCrossHair(wxCoord x, wxCoord y) override;

	void DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y) override;
	void DoDrawBitmap(const wxBitmap& bmp, wxCoord x, wxCoord y,
	                  bool useMask = false) override;

	void DoDrawText(const wxString& text, wxCoord x, wxCoord y) override;
	void DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y,
	                       double angle) override;

	bool DoBlit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
	            wxDC* source, wxCoord xsrc, wxCoord ysrc,
	            wxRasterOperationMode rop = wxCOPY, bool useMask = false,
	            wxCoord xsrcMask = -1, wxCoord ysrcMask = -1) override;

	void DoGetSize(int* width, int* height) const override;
	void DoGetSizeMM(int* width, int* height) const override;

#if wxCHECK_VERSION(2, 9, 5)
	void DoDrawLines(int n, const wxPoint points[],
	                 wxCoord xoffset, wxCoord yoffset) override;
	void DoDrawPolygon(int n, const wxPoint points[],
	                   wxCoord xoffset, wxCoord yoffset,
	                   wxPolygonFillMode fillStyle = wxODDEVEN_RULE) override;
	void DoDrawPolyPolygon(int n, const int count[], const wxPoint points[],
	                       wxCoord xoffset, wxCoord yoffset,
	                       wxPolygonFillMode fillStyle) override;
#else
  virtual void DoDrawLines(int n, wxPoint points[],
                           wxCoord xoffset, wxCoord yoffset);
  virtual void DoDrawPolygon(int n, wxPoint points[],
                             wxCoord xoffset, wxCoord yoffset,
                             wxPolygonFillMode fillStyle = wxODDEVEN_RULE);
  virtual void DoDrawPolyPolygon(int n, int count[], wxPoint points[],
                                 wxCoord xoffset, wxCoord yoffset,
                                 int fillStyle);
#endif // wxCHECK_VERSION

	virtual void DoSetClippingRegionAsRegion(const wxRegion& region);
	void DoSetClippingRegion(wxCoord x, wxCoord y,
	                         wxCoord width, wxCoord height) override;
	void DoSetDeviceClippingRegion(const wxRegion& region) override;

	void DoGetTextExtent(const wxString& string,
	                     wxCoord* x, wxCoord* y,
	                     wxCoord* descent = nullptr,
	                     wxCoord* externalLeading = nullptr,
	                     const wxFont* theFont = nullptr) const override;

	bool DoGetPartialTextExtents(const wxString& text, wxArrayInt& widths) const override;

public:
	int GetDrawingStyle();
	bool StretchBlt(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
	                wxBitmap* bitmap);
	int IncreaseImageCounter() { return ++m_imageCount; }

	void SetMapModeStyle(wxPdfMapModeStyle style) { m_mappingModeStyle = style; }
	wxPdfMapModeStyle GetMapModeStyle() const { return m_mappingModeStyle; }

	double ScaleLogicalToPdfX(wxCoord x) const;
	double ScaleLogicalToPdfXRel(wxCoord x) const;
	double ScaleLogicalToPdfY(wxCoord y) const;
	double ScaleLogicalToPdfYRel(wxCoord y) const;
	double ScaleFontSizeToPdf(int pointSize) const;
	int ScalePdfToFontMetric(double metric) const;

private:
	int FindPdfFont(wxFont* font) const;
	void SetupPen();
	void SetupBrush();
	void SetupAlpha();
	void SetupTextAlpha();
	void CalculateFontMetrics(wxPdfFontDescription* desc, int pointSize,
	                          int* height, int* ascent, int* descent, int* extLeading) const;

	bool m_templateMode;
	double m_templateWidth;
	double m_templateHeight;
	double m_ppi;
	double m_ppiPdfFont;
	wxPdfDocument* m_pdfDocument;
	int m_imageCount;
	wxPrintData m_printData;
	wxPdfMapModeStyle m_mappingModeStyle;

	wxPdfColour m_cachedPdfColour;
	wxUint32 m_cachedRGB;

	bool m_jpegFormat;
	int m_jpegQuality;

	wxDECLARE_DYNAMIC_CLASS(wxPdfDCImpl);
};

#endif
