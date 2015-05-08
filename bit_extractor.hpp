#ifndef BIT_EXTRACTOR_HPP___
#define BIT_EXTRACTOR_HPP___

#include <cstdint>
#include <iostream>

//#include <stdint.h>

#define BE_BIG_ENDIAN true
#define BE_LITTLE_ENDIAN false

extern unsigned int g_mask[32];

struct bit_code {
   unsigned int mValue;
   unsigned int mSize;
};

std::ostream& operator<<(std::ostream& os, const bit_code bc);

class bit_extractor
{
public:
   explicit bit_extractor() : 
      mBuffer(0), mSizeInBytes(0), mContextBitPos(0), 
      mContextBytePos(0), mEndianess(BE_LITTLE_ENDIAN)
   {}

   bit_extractor(unsigned char *buffer, unsigned int size_in_bytes, bool endianess) :
      mBuffer(buffer), mSizeInBytes(size_in_bytes), mContextBitPos(0), 
      mContextBytePos(0), mEndianess(endianess)
   {}
   
   ~bit_extractor() 
   {}

   bool init(unsigned char *buffer, unsigned int size_in_bytes, bool endianess) {
      mBuffer = buffer;
      mSizeInBytes = size_in_bytes;
      mEndianess = endianess;
   }

   // NOTE ::: Only works for an alligned (8-bit) buffer 
   uint64_t get_64bits() {
      uint64_t rval = 0;

      if (mEndianess) {
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+0) << 56);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+1) << 48);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+2) << 40);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+3) << 32);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+4) << 24);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+5) << 16);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+6) << 8);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+7));
      }
      else {
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+7));
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+6) << 8);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+5) << 16);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+4) << 24);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+3) << 32);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+2) << 40);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+1) << 48);
         rval |= ((uint64_t)*(mBuffer+mContextBytePos+0) << 56);
      }

      mContextBytePos += 8;
      
      return rval;
   }

   unsigned int show_bits(unsigned char n) {
      unsigned int rval = 0;

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
      
      unsigned int mask = g_mask[n-1]; 
      
      rval &= mask;

      if (mEndianess) {
         unsigned int temp1 = 0;
         unsigned int temp2 = 0;
         unsigned int temp3 = 0;
         unsigned int temp4 = 0;

         if (n == 16) {
            temp1 = rval >> 8;

            rval = temp1 | ((rval & 0xFF) << 8);
         }
         else if (n == 24) {
            temp1 = rval & 0xFF;     // First byte
            temp2 = rval & 0xFF0000; // Third byte

            rval  = (temp1 << 16) | (rval & 0xFF00) | (temp2 >> 16);  
         }
         else if (n == 32) {
            temp1 = rval & 0xFF;        // First byte
            temp2 = rval & 0xFF00;      // Second byte            
            temp3 = rval & 0xFF0000;    // Third byte
            temp4 = rval & 0xFF000000;  // Fourth byte
            
            rval = (temp1 << 24) | (temp2 << 8) | (temp3 >> 8) | (temp4 >> 24);
         }
      }

      return rval;
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

   // Big Endian : true
   // Little Endian : false
   bool           mEndianess;
};

#endif

