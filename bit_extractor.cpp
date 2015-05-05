#include "bit_extractor.hpp"

std::ostream& operator<<(std::ostream& os, const bit_code bc) {
   
   unsigned int n    = bc.mSize;
   unsigned int mask = 1 << (n-1);
      
   os << "0b";
   while (n) {
      if (bc.mValue & mask) {
         os << "1";
      }
      else {
         os << "0";
      }
      mask >>= 1;
      --n;
   }
   
   return os;
}
