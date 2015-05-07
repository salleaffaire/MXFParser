#include "hex_out.hpp"

void hex_bulk_output(std::ostream &os, const int width, uint8_t *p, size_t size) {
   int column = 0;
   for (uint32_t i=0;i<size;++i) {
      os << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)p[i] << " ";
      
      if (width == ++column) {
         os << std::endl;
         column = 0;
      }
   }
   os << std::endl;
 
}
