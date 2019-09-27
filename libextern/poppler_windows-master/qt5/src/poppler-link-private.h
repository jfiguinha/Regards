/* poppler-link-private.h: qt interface to poppler
 * Copyright (C) 2016, 2018, Albert Astals Cid <aacid@kde.org>
 * Copyright (C) 2018 Intevation GmbH <intevation@intevation.de>
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

#ifndef _POPPLER_LINK_PRIVATE_H_
#define _POPPLER_LINK_PRIVATE_H_

class LinkOCGState;

namespace Poppler {

class Link;

class LinkPrivate
{
public:
    LinkPrivate( const QRectF &area )
        : linkArea( area )
    {
    }

    virtual ~LinkPrivate()
    {
        qDeleteAll(nextLinks);
    }

    static LinkPrivate *get( Link *link )
    {
        return link->d_ptr;
    }

    LinkPrivate(const LinkPrivate &) = delete;
    LinkPrivate& operator=(const LinkPrivate &) = delete;

    QRectF linkArea;
    QVector <Link *> nextLinks;
};



class LinkOCGStatePrivate : public LinkPrivate
{
public:
    LinkOCGStatePrivate( const QRectF &area, ::LinkOCGState *plocg )
        : LinkPrivate( area )
        , popplerLinkOCGState( plocg )
    {
    }

    ::LinkOCGState *popplerLinkOCGState;
};



class LinkHidePrivate : public LinkPrivate
{
public:
    LinkHidePrivate( const QRectF &area, const QString &tName, bool show )
        : LinkPrivate( area )
        , targetName( tName )
        , isShow( show )
    {
    }

    QString targetName;
    bool isShow;
};

}

#endif
