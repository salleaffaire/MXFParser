#include <iostream>
#include "mxf.hpp"

int
main(int argc, char *argv[]) {

   // Test UL Key dictionary
   // ------------------------------------------------
   key_value_dictionary mxf_kv_dic;
   mxf_kv_dic.add( {0x0006, 0x000E, 0x002B, 0x0034, 
                    0x0001, 0x0001, 0x0001, 0x0002, 
                    0x0003, 0x0001, 0x0002, 0x0010, 
                    0x0010, 0x0000, 0x0000, 0x0000}, "Fill Item");

   mxf_kv_dic.add( {0x0006, 0x000E, 0x002B, 0x0034, 
                    0x0001, 0x0001, 0x0001, 0x0002, 
                    0x0005, 0x0001, 0x0002, 0x0010, 
                    0x0010, 0x0001, 0x0000, 0x0000}, "Another Item");

   

   // ------------------------------------------------

   mxf_file file("GoneGirl_32K.mxf");

   file.build_klv_list();

   file.output_klv_list(std::cout);

   return 0;
}
