/* poppler-input-stream.cc: glib interface to poppler
 *
 * Copyright (C) 2012 Carlos Garcia Campos <carlosgc@gnome.org>
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

#include "config.h"
#include "poppler-input-stream.h"

PopplerInputStream::PopplerInputStream(GInputStream *inputStreamA, GCancellable *cancellableA,
                                       Goffset startA, bool limitedA, Goffset lengthA, Object &&dictA)
  : BaseStream(std::move(dictA), lengthA)
{
  inputStream = (GInputStream *)g_object_ref(inputStreamA);
  cancellable = cancellableA ? (GCancellable *)g_object_ref(cancellableA) : nullptr;
  start = startA;
  limited = limitedA;
  length = lengthA;
  bufPtr = bufEnd = buf;
  bufPos = start;
  savePos = 0;
  saved = false;
}

PopplerInputStream::~PopplerInputStream()
{
  close();
  g_object_unref(inputStream);
  if (cancellable)
    g_object_unref(cancellable);
}

BaseStream *PopplerInputStream::copy() {
  return new PopplerInputStream(inputStream, cancellable, start, limited, length, dict.copy());
}

Stream *PopplerInputStream::makeSubStream(Goffset startA, bool limitedA,
                                          Goffset lengthA, Object &&dictA)
{
  return new PopplerInputStream(inputStream, cancellable, startA, limitedA, lengthA, std::move(dictA));
}

void PopplerInputStream::reset()
{
  GSeekable *seekable = G_SEEKABLE(inputStream);

  savePos = (unsigned int)g_seekable_tell(seekable);
  g_seekable_seek(seekable, start, G_SEEK_SET, cancellable, nullptr);
  saved = true;
  bufPtr = bufEnd = buf;
  bufPos = start;
}

void PopplerInputStream::close()
{
  if (!saved)
    return;
  g_seekable_seek(G_SEEKABLE(inputStream), savePos, G_SEEK_SET, cancellable, nullptr);
  saved = false;
}

void PopplerInputStream::setPos(Goffset pos, int dir)
{
  unsigned int size;
  GSeekable *seekable = G_SEEKABLE(inputStream);

  if (dir >= 0) {
    g_seekable_seek(seekable, pos, G_SEEK_SET, cancellable, nullptr);
    bufPos = pos;
  } else {
    g_seekable_seek(seekable, 0, G_SEEK_END, cancellable, nullptr);
    size = (unsigned int)g_seekable_tell(seekable);

    if (pos > size)
      pos = size;

    g_seekable_seek(seekable, -(goffset)pos, G_SEEK_END, cancellable, nullptr);
    bufPos = (unsigned int)g_seekable_tell(seekable);
  }
  bufPtr = bufEnd = buf;
}

void PopplerInputStream::moveStart(Goffset delta)
{
  start += delta;
  bufPtr = bufEnd = buf;
  bufPos = start;
}

bool PopplerInputStream::fillBuf()
{
  int n;

  bufPos += bufEnd - buf;
  bufPtr = bufEnd = buf;
  if (limited && bufPos >= start + length) {
    return false;
  }

  if (limited && bufPos + inputStreamBufSize > start + length) {
    n = start + length - bufPos;
  } else {
    n = inputStreamBufSize - (bufPos % inputStreamBufSize);
  }

  n = g_input_stream_read(inputStream, buf, n, cancellable, nullptr);
  bufEnd = buf + n;
  if (bufPtr >= bufEnd) {
    return false;
  }

  return true;
}

int PopplerInputStream::getChars(int nChars, unsigned char *buffer)
{
  int n, m;

  n = 0;
  while (n < nChars) {
    if (bufPtr >= bufEnd) {
      if (!fillBuf()) {
        break;
      }
    }
    m = (int)(bufEnd - bufPtr);
    if (m > nChars - n) {
      m = nChars - n;
    }
    memcpy(buffer + n, bufPtr, m);
    bufPtr += m;
    n += m;
  }
  return n;
}
