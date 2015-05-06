#include "mxf.hpp"

int
main(int argc, char *argv[]) {
   mxf_file file("GoneGirl_32K.mxf");

   file.build_klv_list();

   return 0;
}
