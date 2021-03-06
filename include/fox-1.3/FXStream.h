/********************************************************************************
*                                                                               *
*       P e r s i s t e n t   S t o r a g e   S t r e a m   C l a s s e s       *
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
* $Id: FXStream.h,v 1.33 2004/09/19 20:13:34 fox Exp $                          *
********************************************************************************/
#ifndef FXSTREAM_H
#define FXSTREAM_H


namespace FX {


/// Stream data flow direction
enum FXStreamDirection {
  FXStreamDead=0,               /// Unopened stream
  FXStreamSave=1,               /// Saving stuff to stream
  FXStreamLoad=2                /// Loading stuff from stream
  };


/// Stream status codes
enum FXStreamStatus {
  FXStreamOK=0,                 /// OK
  FXStreamEnd=1,                /// Try read past end of stream
  FXStreamFull=2,               /// Filled up stream buffer or disk full
  FXStreamNoWrite=3,            /// Unable to open for write
  FXStreamNoRead=4,             /// Unable to open for read
  FXStreamFormat=5,             /// Stream format error
  FXStreamUnknown=6,            /// Trying to read unknown class
  FXStreamAlloc=7,              /// Alloc failed
  FXStreamFailure=8             /// General failure
  };


/// Stream seeking
enum FXWhence {
  FXFromStart=0,                /// Seek from start position
  FXFromCurrent=1,              /// Seek from current position
  FXFromEnd=2                   /// Seek from end position
  };


/**
* A stream is a way to serialize data and objects into a byte stream.
* Each item of data that is saved or loaded from the stream may be byte-swapped,
* thus allowing little-endian machines to read data produced on big endian ones
* and vice-versa.
* Data is serialized exactly as-is.  There are no tags or other markers
* inserted into the stream; thus, the stream may be used to save or load arbitrary
* binary data.
* Objects derived from FXObjects may be serialized also; whenever a reference to an
* object is serialized, a table is consulted to determine if the same object has
* been encountered previously; if not, the object is added to the table and then
* its contents are serialized.  If the object has been encountered before, only a
* reference to the object is serialized.
* When loading back a serialized object, new instances are constructed using
* the default constructor, and subsequently the object's contents are loaded.
* A special container object may be passed in which is placed in the table
* as if it had been encountered before; this will cause only references to this
* object to be saved.  The container object is typically the top-level document
* object which manages all objects contained by it.  Additional objects may be
* added using addObject(); these will not be actually saved or loaded.
*/
class FXAPI FXStream {
protected:
  FXHash             hash;      // Hash table
  const FXObject    *parent;    // Parent object
  FXuchar           *begptr;    // Begin of buffer
  FXuchar           *endptr;    // End of buffer
  FXuchar           *wrptr;     // Write pointer
  FXuchar           *rdptr;     // Read pointer
  unsigned long      pos;       // Position
  FXStreamDirection  dir;       // Direction of current transfer
  FXStreamStatus     code;      // Status code
  FXbool             owns;      // Stream owns buffer
  FXbool             swap;      // Swap bytes on readin
protected:

  /**
  * Write at least count bytes from the buffer;
  * returns number of bytes available to be written.
  */
  virtual unsigned long writeBuffer(unsigned long count);

  /**
  * Read at least count bytes into the buffer;
  * returns number of bytes available to be read.
  */
  virtual unsigned long readBuffer(unsigned long count);

public:

  /**
  * Construct stream with given container object.  The container object
  * is an object that will itself not be saved to or loaded from the stream,
  * but which may be referenced by other objects.  These references will be
  * properly saved and restored.
  */
  FXStream(const FXObject* cont=NULL);

  /**
  * Open stream for reading (FXStreamLoad) or for writing (FXStreamSave).
  * An initial buffer size may be given, which must be at least 16 bytes.
  * If data is not NULL, it is expected to point to an external data buffer
  * of length size; otherwise stream will use an internally managed buffer.
  */
  FXbool open(FXStreamDirection save_or_load,unsigned long size=8192,FXuchar* data=NULL);

  /// Close; return TRUE if OK
  virtual FXbool close();

  /// Flush buffer
  virtual FXbool flush();

  /// Get available buffer space
  unsigned long getSpace() const;

  /// Set available buffer space
  void setSpace(unsigned long sp);

  /// Get status code
  FXStreamStatus status() const { return code; }

  /// Return TRUE if at end of file or error
  FXbool eof() const { return code!=FXStreamOK; }

  /// Set status code
  void setError(FXStreamStatus err);

  /// Obtain stream direction
  FXStreamDirection direction() const { return dir; }

  /// Get parent object
  const FXObject* container() const { return parent; }

  /// Get position
  unsigned long position() const { return pos; }

  /// Move to position relative to head, tail, or current location
  virtual FXbool position(long offset,FXWhence whence=FXFromStart);

  /**
  * Change swap bytes flag.
  */
  void swapBytes(FXbool s){ swap=s; }

  /**
  * Get state of the swap bytes flag.
  */
  FXbool swapBytes() const { return swap; }

  /// Return implementation's endianness
  static FXbool isLittleEndian(){ return !FOX_BIGENDIAN; }

  /// Save single items to stream
  FXStream& operator<<(const FXuchar& v);
  FXStream& operator<<(const FXchar& v){ return *this << reinterpret_cast<const FXuchar&>(v); }
  FXStream& operator<<(const FXushort& v);
  FXStream& operator<<(const FXshort& v){ return *this << reinterpret_cast<const FXushort&>(v); }
  FXStream& operator<<(const FXuint& v);
  FXStream& operator<<(const FXint& v){ return *this << reinterpret_cast<const FXuint&>(v); }
  FXStream& operator<<(const FXfloat& v){ return *this << reinterpret_cast<const FXuint&>(v); }
  FXStream& operator<<(const FXdouble& v);
#ifdef FX_LONG
  FXStream& operator<<(const FXlong& v){ return *this << reinterpret_cast<const FXdouble&>(v); }
  FXStream& operator<<(const FXulong& v){ return *this << reinterpret_cast<const FXdouble&>(v); }
#endif

  /// Save arrays of items to stream
  FXStream& save(const FXuchar* p,unsigned long n);
  FXStream& save(const FXchar* p,unsigned long n){ return save(reinterpret_cast<const FXuchar*>(p),n); }
  FXStream& save(const FXushort* p,unsigned long n);
  FXStream& save(const FXshort* p,unsigned long n){ return save(reinterpret_cast<const FXushort*>(p),n); }
  FXStream& save(const FXuint* p,unsigned long n);
  FXStream& save(const FXint* p,unsigned long n){ return save(reinterpret_cast<const FXuint*>(p),n); }
  FXStream& save(const FXfloat* p,unsigned long n){ return save(reinterpret_cast<const FXuint*>(p),n); }
  FXStream& save(const FXdouble* p,unsigned long n);
#ifdef FX_LONG
  FXStream& save(const FXlong* p,unsigned long n){ return save(reinterpret_cast<const FXdouble*>(p),n); }
  FXStream& save(const FXulong* p,unsigned long n){ return save(reinterpret_cast<const FXdouble*>(p),n); }
#endif

  /// Load single items from stream
  FXStream& operator>>(FXuchar& v);
  FXStream& operator>>(FXchar& v){ return *this >> reinterpret_cast<FXuchar&>(v); }
  FXStream& operator>>(FXushort& v);
  FXStream& operator>>(FXshort& v){ return *this >> reinterpret_cast<FXushort&>(v); }
  FXStream& operator>>(FXuint& v);
  FXStream& operator>>(FXint& v){ return *this >> reinterpret_cast<FXuint&>(v); }
  FXStream& operator>>(FXfloat& v){ return *this >> reinterpret_cast<FXuint&>(v); }
  FXStream& operator>>(FXdouble& v);
#ifdef FX_LONG
  FXStream& operator>>(FXlong& v){ return *this >> reinterpret_cast<FXdouble&>(v); }
  FXStream& operator>>(FXulong& v){ return *this >> reinterpret_cast<FXdouble&>(v); }
#endif

  /// Load arrays of items from stream
  FXStream& load(FXuchar* p,unsigned long n);
  FXStream& load(FXchar* p,unsigned long n){ return load(reinterpret_cast<FXuchar*>(p),n); }
  FXStream& load(FXushort* p,unsigned long n);
  FXStream& load(FXshort* p,unsigned long n){ return load(reinterpret_cast<FXushort*>(p),n); }
  FXStream& load(FXuint* p,unsigned long n);
  FXStream& load(FXint* p,unsigned long n){ return load(reinterpret_cast<FXuint*>(p),n); }
  FXStream& load(FXfloat* p,unsigned long n){ return load(reinterpret_cast<FXuint*>(p),n); }
  FXStream& load(FXdouble* p,unsigned long n);
#ifdef FX_LONG
  FXStream& load(FXlong* p,unsigned long n){ return load(reinterpret_cast<FXdouble*>(p),n); }
  FXStream& load(FXulong* p,unsigned long n){ return load(reinterpret_cast<FXdouble*>(p),n); }
#endif

  /// Save object
  FXStream& saveObject(const FXObject* v);

  /// Load object
  FXStream& loadObject(FXObject*& v);

  /// Add object without saving or loading
  FXStream& addObject(const FXObject* v);

  /// Destructor
  virtual ~FXStream();
  };

}

#endif
