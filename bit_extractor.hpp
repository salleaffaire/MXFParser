#ifndef BIT_EXTRACTOR_HPP___
#define BIT_EXTRACTOR_HPP___

#include <iostream>

//#include <stdint.h>

struct bit_code {
   unsigned int mValue;
   unsigned int mSize;
};

std::ostream& operator<<(std::ostream& os, const bit_code bc);

class bit_extractor
{
public:
   explicit bit_extractor() : 
      mBuffer(0), mSizeInBytes(0), mContextBitPos(0), mContextBytePos(0)
   {}

   bit_extractor(unsigned char *buffer, unsigned int size_in_bytes) :
      mBuffer(buffer), mSizeInBytes(size_in_bytes), mContextBitPos(0), mContextBytePos(0)
   {}
   
   ~bit_extractor() 
   {}

   bool init(unsigned char *buffer, unsigned int size_in_bytes) {
      mBuffer = buffer;
      mSizeInBytes = size_in_bytes;
   }

   unsigned int show_bits(unsigned char n) {
      unsigned rval = 0;

      unsigned char *p = mBuffer + mContextBytePos; 
      
      const unsigned char comp_bit_pos = 8 - mContextBitPos;
      
      rval |= ((unsigned int)*p++ >> mContextBitPos);
      //std::cout << "R0: " << std::hex << rval << std::endl;
      rval |= ((unsigned int)*p++ << comp_bit_pos);
      //std::cout << "R1: " << std::hex << rval << std::endl;
      rval |= ((unsigned int)*p++ << (comp_bit_pos+8));
      //std::cout << "R2: " << std::hex << rval << std::endl;
      rval |= ((unsigned int)*p++ << (comp_bit_pos+16));
      //std::cout << "R3: " << std::hex << rval << std::endl;
               
      if (mContextBitPos) {
         rval |= ((unsigned int)*p++ << (comp_bit_pos+24));
         //std::cout << "R4: " << std::hex << rval << std::endl;
      }

      unsigned int mask = (1 << (n & 0x1F)) - 1; 
  
      return (rval & mask);
   }

   void flush_bits(unsigned char n) {
      mContextBytePos += (n + mContextBitPos) >> 0x03;
      mContextBitPos = (n + mContextBitPos) & 0x07;

      //std::cout << "Byte Position : " << std::dec << (int)mContextBytePos << std::endl;
      //std::cout << "Bit  Position : " << std::dec << (int)mContextBitPos << std::endl;
   }

   unsigned int get_bits(unsigned char n) {
      unsigned int rval = show_bits(n);
      flush_bits(n);
      return rval;
   }
   
   unsigned char *get_buffer() {
      return mBuffer+mContextBytePos;
   }

   void skip_n_bytes(unsigned int n) {
      mContextBytePos += n;
   }

   unsigned int get_bytes_left() {
      return (mSizeInBytes - mContextBytePos);
   }

private:
   unsigned char *mBuffer;
   unsigned int   mSizeInBytes;   //  Total number of bytes in the buffer 
   
   unsigned char  mContextBitPos;
   unsigned int   mContextBytePos;
};

#endif

