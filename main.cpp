#include <iostream>
#include "mxf.hpp"

#include "dictionary.hpp"

int
main(int argc, char *argv[]) {

   #if 0
   for (int i=1;i<33;i++) {
      int mask = (1 << (i & 0x1F)) - 1;        
      std::cout << "0x" << std::hex << mask << ",";   
   } 
   #endif

   #if 0
   // Test Dictionary
   // ------------------------------------------------
   Dictionary<16> mxf_kv_dic;
   mxf_kv_dic.add( {0x0006, 0x000E, 0x002B, 0x0034, 
                    0x0001, 0x0001, 0x0001, 0x0002, 
                    0x0003, 0x0001, 0x0002, 0x0010, 
                    0x0010, 0x0000, 0x0000, 0x0000}, "Fill Item");

   mxf_kv_dic.add( {0x0006, 0x000E, 0x002B, 0x0034, 
                    0x0001, 0x0001, 0xFFFF, 0x0002, 
                    0x0005, 0x0001, 0x0003, 0x0010, 
                    0x0010, 0x0001, 0x0000, 0x0000}, "Another Item 1");

   mxf_kv_dic.add( {0x0006, 0x000E, 0x002B, 0x0034, 
                    0x0001, 0x0001, 0xFFFF, 0x0002, 
                    0x0005, 0x0001, 0x0004, 0x0010, 
                    0x0010, 0x0001, 0x0000, 0x0000}, "Another Item 2");

   mxf_kv_dic.add( {0x0006, 0x000E, 0x002B, 0x0034, 
                    0xFFFF, 0x0001, 0x0006, 0x0002, 
                    0x0005, 0x0001, 0x0004, 0x0010, 
                    0x0010, 0x0001, 0x0000, 0x0000}, "Another Item 3");

   mxf_kv_dic.add( {0x0006, 0x000E, 0x002B, 0x0034, 
                    0xFFFF, 0x0002, 0xFFFF, 0xFFFF, 
                    0x0005, 0x0001, 0x0004, 0x0010, 
                    0x0010, 0x0001, 0x0000, 0x0000}, "Another Item 4");

   std::cout << mxf_kv_dic.find({0x06, 0x0E, 0x2B, 0x34, 
                                 0x08, 0x01, 0x06, 0x02, 
                                 0x05, 0x01, 0x04, 0x10, 
                                 0x10, 0x01, 0x00, 0x00}) << std::endl;
   
   // ------------------------------------------------
   #endif
   
   #if 1
   //mxf_file file("freeMXF-mxf1.mxf");
   mxf_file file("GoneGirl_32K.mxf");

   file.build_klv_list();
   file.parse();

   //file.output_klv_list(std::cout);

   return 0;
   #endif
}
