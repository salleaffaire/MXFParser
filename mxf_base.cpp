#include "mxf_base.hpp"

Dictionary<16>  mxf_file::m_UL_key_dictionary;
bool            mxf_file::m_is_UL_key_dictionary_init = false;

void mxf_file::init_UL_key_dictionary() {

   m_UL_key_dictionary.add( {0x0006, 0x000E, 0x002B, 0x0034, 
                             0x0001, 0x0001, 0x0001, 0xFFFF, 
                             0x0003, 0x0001, 0x0002, 0x0010, 
                             0x0010, 0x0000, 0x0000, 0x0000}, "Fill Item");

   m_UL_key_dictionary.add( {0x0006, 0x000E, 0x002B, 0x0034, 
                             0x0002, 0x0005, 0x0001, 0xFFFF, 
                             0x000D, 0x0001, 0x0002, 0x0001, 
                             0x0001, 0xFFFF, 0xFFFF, 0x0000}, "Partition Pack");

   
} 
   



