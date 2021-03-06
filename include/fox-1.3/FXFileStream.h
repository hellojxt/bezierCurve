/********************************************************************************
*                                                                               *
*                       F i l e   S t r e a m   C l a s s                       *
*                                                                               *
*********************************************************************************
* Copyright (C) 1997,2004 by Jeroen van der Zijp.   All Rights Reserved.        *
*********************************************************************************
* This library is free software; you can redistribute it and/or                 *
* modify it under the terms of the GNU Lesser General Public                    *
* License as published by the Free Software Foundation; either                  *
* version 2.1 of the License, or (at your option) any later version.            *
*                                                                               *
* This library is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
* Lesser General Public License for more details.                               *
*                                                                               *
* You should have received a copy of the GNU Lesser General Public              *
* License along with this library; if not, write to the Free Software           *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.    *
*********************************************************************************
* $Id: FXFileStream.h,v 1.8 2004/10/21 20:17:52 fox Exp $                       *
********************************************************************************/
#ifndef FXFILESTREAM_H
#define FXFILESTREAM_H

#ifndef FXSTREAM_H
#include "FXStream.h"
#endif

namespace FX {


/// File Store Definition
class FXAPI FXFileStream : public FXStream {
private:
  FXint file;
protected:
  virtual unsigned long writeBuffer(unsigned long count);
  virtual unsigned long readBuffer(unsigned long count);
public:

  /// Create file store
  FXFileStream(const FXObject* cont=NULL);

  /**
  * Open binary data file stream; allocate a buffer of the given size
  * for the file I/O; the buffer must be at least 16 bytes.
  */
  FXbool open(const FXString& filename,FXStreamDirection save_or_load,unsigned long size=8192);

  /// Close file store
  virtual FXbool close();

  /// Get position
  unsigned long position() const { return FXStream::position(); }

  /// Move to position
  virtual FXbool position(long offset,FXWhence whence=FXFromStart);

  /// Save single items to stream
  FXFileStream& operator<<(const FXuchar& v){ FXStream::operator<<(v); return *this; }
  FXFileStream& operator<<(const FXchar& v){ FXStream::operator<<(v); return *this; }
  FXFileStream& operator<<(const FXushort& v){ FXStream::operator<<(v); return *this; }
  FXFileStream& operator<<(const FXshort& v){ FXStream::operator<<(v); return *this; }
  FXFileStream& operator<<(const FXuint& v){ FXStream::operator<<(v); return *this; }
  FXFileStream& operator<<(const FXint& v){ FXStream::operator<<(v); return *this; }
  FXFileStream& operator<<(const FXfloat& v){ FXStream::operator<<(v); return *this; }
  FXFileStream& operator<<(const FXdouble& v){ FXStream::operator<<(v); return *this; }
#ifdef FX_LONG
  FXFileStream& operator<<(const FXlong& v){ FXStream::operator<<(v); return *this; }
  FXFileStream& operator<<(const FXulong& v){ FXStream::operator<<(v); return *this; }
#endif

  /// Save arrays of items to stream
  FXFileStream& save(const FXuchar* p,unsigned long n){ FXStream::save(p,n); return *this; }
  FXFileStream& save(const FXchar* p,unsigned long n){ FXStream::save(p,n); return *this; }
  FXFileStream& save(const FXushort* p,unsigned long n){ FXStream::save(p,n); return *this; }
  FXFileStream& save(const FXshort* p,unsigned long n){ FXStream::save(p,n); return *this; }
  FXFileStream& save(const FXuint* p,unsigned long n){ FXStream::save(p,n); return *this; }
  FXFileStream& save(const FXint* p,unsigned long n){ FXStream::save(p,n); return *this; }
  FXFileStream& save(const FXfloat* p,unsigned long n){ FXStream::save(p,n); return *this; }
  FXFileStream& save(const FXdouble* p,unsigned long n){ FXStream::save(p,n); return *this; }
#ifdef FX_LONG
  FXFileStream& save(const FXlong* p,unsigned long n){ FXStream::save(p,n); return *this; }
  FXFileStream& save(const FXulong* p,unsigned long n){ FXStream::save(p,n); return *this; }
#endif

  /// Load single items from stream
  FXFileStream& operator>>(FXuchar& v){ FXStream::operator>>(v); return *this; }
  FXFileStream& operator>>(FXchar& v){ FXStream::operator>>(v); return *this; }
  FXFileStream& operator>>(FXushort& v){ FXStream::operator>>(v); return *this; }
  FXFileStream& operator>>(FXshort& v){ FXStream::operator>>(v); return *this; }
  FXFileStream& operator>>(FXuint& v){ FXStream::operator>>(v); return *this; }
  FXFileStream& operator>>(FXint& v){ FXStream::operator>>(v); return *this; }
  FXFileStream& operator>>(FXfloat& v){ FXStream::operator>>(v); return *this; }
  FXFileStream& operator>>(FXdouble& v){ FXStream::operator>>(v); return *this; }
#ifdef FX_LONG
  FXFileStream& operator>>(FXlong& v){ FXStream::operator>>(v); return *this; }
  FXFileStream& operator>>(FXulong& v){ FXStream::operator>>(v); return *this; }
#endif

  /// Load arrays of items from stream
  FXFileStream& load(FXuchar* p,unsigned long n){ FXStream::load(p,n); return *this; }
  FXFileStream& load(FXchar* p,unsigned long n){ FXStream::load(p,n); return *this; }
  FXFileStream& load(FXushort* p,unsigned long n){ FXStream::load(p,n); return *this; }
  FXFileStream& load(FXshort* p,unsigned long n){ FXStream::load(p,n); return *this; }
  FXFileStream& load(FXuint* p,unsigned long n){ FXStream::load(p,n); return *this; }
  FXFileStream& load(FXint* p,unsigned long n){ FXStream::load(p,n); return *this; }
  FXFileStream& load(FXfloat* p,unsigned long n){ FXStream::load(p,n); return *this; }
  FXFileStream& load(FXdouble* p,unsigned long n){ FXStream::load(p,n); return *this; }
#ifdef FX_LONG
  FXFileStream& load(FXlong* p,unsigned long n){ FXStream::load(p,n); return *this; }
  FXFileStream& load(FXulong* p,unsigned long n){ FXStream::load(p,n); return *this; }
#endif

  /// Save object
  FXFileStream& saveObject(const FXObject* v){ FXStream::saveObject(v); return *this; }

  /// Load object
  FXFileStream& loadObject(FXObject*& v){ FXStream::loadObject(v); return *this; }

  /// Destructor
  virtual ~FXFileStream();
  };

}

#endif
