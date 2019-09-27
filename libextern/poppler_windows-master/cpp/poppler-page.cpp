/*
 * Copyright (C) 2009-2010, Pino Toscano <pino@kde.org>
 * Copyright (C) 2017, 2018, Albert Astals Cid <aacid@kde.org>
 * Copyright (C) 2017, Jason Alan Palmer <jalanpalmer@gmail.com>
 * Copyright (C) 2018, Suzuki Toshiya <mpsuzuki@hiroshima-u.ac.jp>
 * Copyright (C) 2018, Adam Reichold <adam.reichold@t-online.de>
 * Copyright (C) 2018, Zsombor Hollay-Horvath <hollay.horvath@gmail.com>
 * Copyright (C) 2018, Aleksey Nikolaev <nae202@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 \file poppler-page.h
 */
#include "poppler-page.h"
#include "poppler-page-transition.h"

#include "poppler-document-private.h"
#include "poppler-page-private.h"
#include "poppler-private.h"

#include "TextOutputDev.h"

#include <algorithm>
#include <memory>
#include <utility>

using namespace poppler;

page_private::page_private(document_private *_doc, int _index)
    : doc(_doc)
    , page(doc->doc->getCatalog()->getPage(_index + 1))
    , index(_index)
    , transition(nullptr)
{
}

page_private::~page_private()
{
    delete transition;
}

/**
 \class poppler::page poppler-page.h "poppler/cpp/poppler-page.h"

 A page in a PDF %document.
 */

/**
 \enum poppler::page::orientation_enum

 The possible orientation of a page.
*/

/**
 \enum poppler::page::search_direction_enum

 The direction/action to follow when performing a text search.
*/

/**
 \enum poppler::page::text_layout_enum

 A layout of the text of a page.
*/


page::page(document_private *doc, int index)
    : d(new page_private(doc, index))
{
}

/**
 Destructor.
 */
page::~page()
{
    delete d;
}

/**
 \returns the orientation of the page
 */
page::orientation_enum page::orientation() const
{
    const int rotation = d->page->getRotate();
    switch (rotation) {
    case 90:
        return landscape;
        break;
    case 180:
        return upside_down;
        break;
    case 270:
        return seascape;
        break;
    default:
        return portrait;
    }
}

/**
 The eventual duration the page can be hinted to be shown in a presentation.

 If this value is positive (usually different than -1) then a PDF viewer, when
 showing the page in a presentation, should show the page for at most for this
 number of seconds, and then switch to the next page (if any). Note this is
 purely a presentation attribute, it has no influence on the behaviour.

 \returns the duration time (in seconds) of the page
 */
double page::duration() const
{
    return d->page->getDuration();
}

/**
 Returns the size of one rect of the page.

 \returns the size of the specified page rect
 */
rectf page::page_rect(page_box_enum box) const
{
    const PDFRectangle *r = nullptr;
    switch (box) {
    case media_box:
        r = d->page->getMediaBox();
        break;
    case crop_box:
        r = d->page->getCropBox();
        break;
    case bleed_box:
        r = d->page->getBleedBox();
        break;
    case trim_box:
        r = d->page->getTrimBox();
        break;
    case art_box:
        r = d->page->getArtBox();
        break;
    }
    if (r) {
        return detail::pdfrectangle_to_rectf(*r);
    }
    return rectf();
}

/**
 \returns the label of the page, if any
 */
ustring page::label() const
{
    GooString goo;
    if (!d->doc->doc->getCatalog()->indexToLabel(d->index, &goo)) {
        return ustring();
    }

    return detail::unicode_GooString_to_ustring(&goo);
}

/**
 The transition from this page to the next one.

 If it is set, then a PDF viewer in a presentation should perform the
 specified transition effect when switching from this page to the next one.

 \returns the transition effect for the switch to the next page, if any
 */
page_transition* page::transition() const
{
    if (!d->transition) {
        Object o = d->page->getTrans();
        if (o.isDict()) {
            d->transition = new page_transition(&o);
        }
    }
    return d->transition;
}

/**
 Search the page for some text.

 \param text the text to search
 \param[in,out] r the area where to start search, which will be set to the area
                  of the match (if any)
 \param direction in which direction search for text
 \param case_sensitivity whether search in a case sensitive way
 \param rotation the rotation assumed for the page
 */
bool page::search(const ustring &text, rectf &r, search_direction_enum direction,
                  case_sensitivity_enum case_sensitivity, rotation_enum rotation) const
{
    const size_t len = text.length();
    std::vector<Unicode> u(len);
    for (size_t i = 0; i < len; ++i) {
        u[i] = text[i];
    }

    const bool sCase = case_sensitivity == case_sensitive;
    const int rotation_value = (int)rotation * 90;

    bool found = false;
    double rect_left = r.left();
    double rect_top = r.top();
    double rect_right = r.right();
    double rect_bottom = r.bottom();

    TextOutputDev td(nullptr, true, 0, false, false);
    d->doc->doc->displayPage(&td, d->index + 1, 72, 72, rotation_value, false, true, false);
    TextPage *text_page = td.takeText();

    switch (direction) {
    case search_from_top:
        found = text_page->findText(&u[0], len,
                    true, true, false, false, sCase, false, false,
                    &rect_left, &rect_top, &rect_right, &rect_bottom);
        break;
    case search_next_result:
        found = text_page->findText(&u[0], len,
                    false, true, true, false, sCase, false, false,
                    &rect_left, &rect_top, &rect_right, &rect_bottom);
        break;
    case search_previous_result:
        found = text_page->findText(&u[0], len,
                    false, true, true, false, sCase, true, false,
                    &rect_left, &rect_top, &rect_right, &rect_bottom);
        break;
    }

    text_page->decRefCnt();
    r.set_left(rect_left);
    r.set_top(rect_top);
    r.set_right(rect_right);
    r.set_bottom(rect_bottom);

    return found;
}

/**
 Returns the text in the page, in its physical layout.

 \param r if not empty, it will be extracted the text in it; otherwise, the
          text of the whole page

 \returns the text of the page in the specified rect or in the whole page
 */
ustring page::text(const rectf &r) const
{
    return text(r, physical_layout);
}

/**
 Returns the text in the page.

 \param rect if not empty, it will be extracted the text in it; otherwise, the
             text of the whole page
 \param layout_mode the layout of the text

 \returns the text of the page in the specified rect or in the whole page

 \since 0.16
 */
ustring page::text(const rectf &r, text_layout_enum layout_mode) const
{
    std::unique_ptr<GooString> s;
    const bool use_raw_order = (layout_mode == raw_order_layout);
    TextOutputDev td(nullptr, false, 0, use_raw_order, false);
    d->doc->doc->displayPage(&td, d->index + 1, 72, 72, 0, false, true, false);
    if (r.is_empty()) {
        PDFRectangle rect = *d->page->getCropBox();
        const int rotate = d->page->getRotate();
        if (rotate == 90 || rotate == 270) {
            std::swap(rect.x1, rect.y1);
            std::swap(rect.x2, rect.y2);
        }
        s.reset(td.getText(rect.x1, rect.y1, rect.x2, rect.y2));
    } else {
        s.reset(td.getText(r.left(), r.top(), r.right(), r.bottom()));
    }
    return ustring::from_utf8(s->c_str());
}

/*
 * text_box object for page::text_list()
 */
text_box_data::~text_box_data() = default;

text_box::~text_box() = default;

text_box& text_box::operator=(text_box&& a) = default;
text_box::text_box(text_box&& a) = default;

text_box::text_box(text_box_data *data) : m_data{data}
{
}

ustring text_box::text() const
{
    return m_data->text;
}

rectf text_box::bbox() const
{
    return m_data->bbox;
}

int text_box::rotation() const
{
    return m_data->rotation;
}

rectf text_box::char_bbox(size_t i) const
{
    if (i < m_data->char_bboxes.size())
        return m_data->char_bboxes[i];
    return rectf(0, 0, 0, 0);
}

bool text_box::has_space_after() const
{
    return m_data->has_space_after;
}

std::vector<text_box> page::text_list() const
{
    std::vector<text_box>  output_list;

    /* config values are same with Qt5 Page::TextList() */
    auto output_dev = std::make_unique<TextOutputDev>(
	nullptr,    /* char* fileName */
	false,  /* bool physLayoutA */
	0,       /* double fixedPitchA */
	false,  /* bool rawOrderA */
	false  /* bool append */
    );

    /*
     * config values are same with Qt5 Page::TextList(),
     * but rotation is fixed to zero.
     * Few people use non-zero values.
     */
    d->doc->doc->displayPageSlice(output_dev.get(),
                                  d->index + 1,           /* page */
                                  72, 72, 0,              /* hDPI, vDPI, rot */
                                  false, false, false,    /* useMediaBox, crop, printing */
                                  -1, -1, -1, -1,         /* sliceX, sliceY, sliceW, sliceH */
                                  nullptr, nullptr,       /* abortCheckCbk(), abortCheckCbkData */
                                  nullptr, nullptr,       /* annotDisplayDecideCbk(), annotDisplayDecideCbkData */
                                  true);                  /* copyXRef */

    if (std::unique_ptr< TextWordList > word_list{output_dev->makeWordList()}) {

        output_list.reserve(word_list->getLength());
        for (int i = 0; i < word_list->getLength(); i ++) {
            TextWord *word = word_list->get(i);

            std::unique_ptr<GooString> gooWord{word->getText()};
            ustring ustr = ustring::from_utf8(gooWord->c_str());

            double xMin, yMin, xMax, yMax;
            word->getBBox(&xMin, &yMin, &xMax, &yMax);

            text_box tb{new text_box_data{
                ustr,
                {xMin, yMin, xMax-xMin, yMax-yMin},
                word->getRotation(),
                {},
                word->hasSpaceAfter() == true
            }};

            tb.m_data->char_bboxes.reserve(word->getLength());
            for (int j = 0; j < word->getLength(); j ++) {
                word->getCharBBox(j, &xMin, &yMin, &xMax, &yMax);
                tb.m_data->char_bboxes.push_back({xMin, yMin, xMax-xMin, yMax-yMin});
            }

            output_list.push_back(std::move(tb));
        }
    }

    return output_list;
}
