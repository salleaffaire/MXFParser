#ifndef HEX_OUT_HPP___
#define HEX_OUT_HPP___

#include <cstdint>
#include <iostream>
#include <iomanip>


void hex_bulk_output(std::ostream &os, const int width, uint8_t *p, size_t size);

#endif
