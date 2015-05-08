#ifndef HEX_OUT_HPP___
#define HEX_OUT_HPP___

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

template <typename T>
std::string hex_to_string(T h) {
   std::stringstream ss;
   
   ss << sizeof(T) << " --- " << std::hex << std::setw(sizeof(T)*2) << std::setfill('0') << h;  

   return ss.str();
}

void hex_bulk_output(std::ostream &os, const int width, uint8_t *p, size_t size);

#endif
