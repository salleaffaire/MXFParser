#include "klv.hpp"
#include "mxf.hpp"

int
main(int argc, char *argv[]) {
   klv_parser klvp("GoneGirl_32K.mxf");
 
   klv_item klvi;
   
   klvp.set_verbosity(true);
   klvp.set_validity_test(true);

   klvp.get_klv_item(klvi);
   klvp.get_klv_item(klvi);
   klvp.get_klv_item(klvi);
   

   return 0;
}
