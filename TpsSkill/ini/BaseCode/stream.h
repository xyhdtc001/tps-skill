#ifndef _STREAM_H_
#define _STREAM_H_

#include "BaseMacro.h"
#include "BaseType.h"

#define DECLARE_OVERLOADED_READ(type)  bool read(type* out_read);
#define DECLARE_OVERLOADED_WRITE(type) bool write(type in_write);

class Stream
{
public:

   enum Status
   {
      Ok = 0,           ///< Ok!
      IOError,          ///< Read or Write error
      EOS,              ///< End of Stream reached (mostly for reads)
      IllegalCall,      ///< An unsupported operation used.  Always w/ accompanied by AssertWarn
      Closed,           ///< Tried to operate on a closed stream (or detached filter)
      UnknownError      ///< Catchall
   };

   enum Capability
   {
      StreamWrite    = 1<<0, ///< Can this stream write?
      StreamRead     = 1<<1, ///< Can this stream read?
      StreamPosition = 1<<2  ///< Can this stream position?
   };

private:
   Status m_streamStatus;


protected:
   void setStatus(const Status in_newStatus) { m_streamStatus = in_newStatus; }

public:
   Stream();
   virtual ~Stream() {}

   /// Gets the status of the stream
   Stream::Status getStatus() const { return m_streamStatus; }

   /// Gets a printable string form of the status
   static const char* getStatusString(const Status in_status);


public:
     /// Checks to see if this stream has the capability of a given function
   virtual bool hasCapability(const Capability caps) const = 0;

   virtual uint32 getWritePos() const = 0;

   /// Sets the position of the stream.  Returns if the new position is valid or not
   virtual bool setWritePos( const uint32 pos ) = 0;
   virtual bool setReadPos( const uint32 pos ) = 0;

   /// Gets the size of the stream
   virtual uint32  getStreamSize() = 0;

   /// Reads a line from the stream.
   /// @param buffer buffer to be read into
   /// @param bufferSize max size of the buffer.  Will not read more than the "bufferSize"
   void readLine(uint8 *buffer, uint32 bufferSize);

   /// writes a line to the stream
   void writeLine(const uint8 *buffer);


   /// Reads a string of maximum 255 characters long
   virtual void readString(char stringBuf[256]);

   /// Reads a string that could potentially be more than 255 characters long.
   /// @param maxStringLen Maximum length to read.  If the string is longer than maxStringLen, only maxStringLen bytes will be read.
   /// @param stringBuf buffer where data is read into
   void readLongString(uint32 maxStringLen, char *stringBuf);

   /// Writes a string to the stream.  This function is slightly unstable.
   /// Only use this if you have a valid string that is not empty.
   /// writeString is safer.
   void writeLongString(uint32 maxStringLen, const char *string);

   /// Write raw text to the stream
   void writeText(const char *text);

   /// Writes a string to the stream.
   virtual void writeString( const char *stringBuf, int32 maxLen=255 );

   // read/write real strings
   void write( const char* str );
   //void read(String * str) { _read(str); }

public:
   virtual bool read( const uint32 in_numBytes,  void* out_pBuffer ) = 0;
   virtual bool write( const uint32 in_numBytes, const void* in_pBuffer ) = 0;

   DECLARE_OVERLOADED_WRITE(int8)
   DECLARE_OVERLOADED_WRITE(uint8)

   DECLARE_OVERLOADED_WRITE(int16)
   DECLARE_OVERLOADED_WRITE(int32)
   DECLARE_OVERLOADED_WRITE(uint16)
   DECLARE_OVERLOADED_WRITE(uint32)
   DECLARE_OVERLOADED_WRITE(uint64)
   DECLARE_OVERLOADED_WRITE(float32)
   DECLARE_OVERLOADED_WRITE(float64)

   DECLARE_OVERLOADED_READ(int8)
   DECLARE_OVERLOADED_READ(uint8)

   DECLARE_OVERLOADED_READ(int16)
   DECLARE_OVERLOADED_READ(int32)
   DECLARE_OVERLOADED_READ(uint16)
   DECLARE_OVERLOADED_READ(uint32)
   DECLARE_OVERLOADED_READ(uint64)
   DECLARE_OVERLOADED_READ(float32)
   DECLARE_OVERLOADED_READ(float64)

   bool read(bool* out_pRead)
   {
      uint8 translate;
      bool success = read(&translate);
      if (success == false)
         return false;

      *out_pRead = translate != 0;
      return true;
   }

   bool write(const bool& in_rWrite) 
   {
      uint8 translate = in_rWrite ? uint8(1) : uint8(0);
      return write(translate);
   }

   /// Copy the contents of another stream into this one
   bool copyFrom(Stream *other);

   /// Write a number of tabs to this stream
   void writeTabs(uint32 count)
   {
      char tab[] = "   ";
      while(count--)
         write(3, (void*)tab);
   }

   /// Create an exact replica of this stream.
   /// Return NULL if the cloning fails.
   virtual Stream* clone() const;
};

// This interface is used to provide the amount of bytes actually written/read when using the stream as a 
// file.  The Stream interface does not provide this information.  This is a lame workaround.  
class IStreamByteCount
{
public:
   virtual ~IStreamByteCount() {}

   virtual uint32 getLastBytesRead() = 0;
   virtual uint32 getLastBytesWritten() = 0;
};

#endif //_STREAM_H_
