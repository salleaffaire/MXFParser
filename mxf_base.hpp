#ifndef MXF_BASE_HPP___
#define MXF_BASE_HPP___

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <map>

#include "klv.hpp"
#include "bit_extractor.hpp"

class mxf_base {
public:
   uint64_t m_offset;
};

// Simple Data Types
// ----------------------------------------

class mfx_auid {
public:
   uint8_t m_uid[16];
   bool    m_is_ul;
};

class mxf_idau {
public:
   uint8_t m_uid[16];
   bool    m_is_ul;
};

class mfx_ber_length {
public:
   uint8_t m_l[32];
};
 
// Counpound Data Type 
class mxf_rational {
public:
   int32_t m_numerator;
   int32_t m_denominator;
};

class mxf_time_stamp {
public:
   int16_t m_year;
   uint8_t m_month;
   uint8_t m_day;
   uint8_t m_hour;
   uint8_t m_minute;
   uint8_t m_second;
   uint8_t m_milisecond_divided_by_4;
}; 

// KLV Pack and KLV Definition Tables

// Overall Data Structure
class mxf_header_partition_pack : public mxf_base {
public:
};

class mxf_header_metadata : public mxf_base {
public:
};

class mxf_file_header : public mxf_base {
public:
   mxf_header_partition_pack m_hpp;
   mxf_header_metadata       m_hmd;
};

class mxf_essence_container : public mxf_base {
public:
   
};

class mxf_body_partition_pack : public mxf_base {
public:
};

class mxf_file_body : public mxf_base {
public:
   mxf_essence_container     m_ess_container0;
   mxf_essence_container     m_ess_container1;
   mxf_body_partition_pack   m_bpp;
};

class mxf_footer_partition_pack : public mxf_base {
public:
};

class mxf_footer_metadata : public mxf_base {
public:
};

class mxf_file_footer : public mxf_base {
public:
   mxf_footer_partition_pack m_fpp;
   mxf_footer_metadata       m_fmd;
};


class mxf_file : public mxf_base {
public:
   mxf_file() : m_file_name(""), m_p_data(0), m_state(0) {
      if (!msIsUL2NameInit) {
         init_ul_2_name();
         msIsUL2NameInit = true;
      }
   }
   
   mxf_file(std::string file_name) : m_file_name(file_name) {
      if (!msIsUL2NameInit) {
         init_ul_2_name();
         msIsUL2NameInit = true;
      }
      std::ifstream in(m_file_name, std::ifstream::ate | std::ifstream::binary);
      if (in.fail()) {
         m_state = -1;
      }
      else {
         m_state = 0;
         
         m_file_size = in.tellg();

         // return at the begining
         in.seekg(0, in.beg);

         std::cout << "Opening : " << m_file_name
                   << " " << m_file_size << " bytes." << std::endl;

         // Allocate mem ory for file
         m_p_data = new uint8_t [m_file_size];

         // Read 
         in.read((char *)m_p_data, m_file_size);
         in.close();
      }
   }
   
   ~mxf_file() {
      if (m_p_data) {
         delete [] m_p_data;
      }
   }

   bool build_klv_list() {
      bool rval = true;
      if (m_p_data) {
         // Initialize Bit Extractor
         bit_extractor be(m_p_data, m_file_size);
         // Need a KLV parser instance
         klv_parser klvp;

         klvp.set_verbosity(true);
         klvp.set_validation(true);

         bool klv_ret_val;
         do {
            auto p_klvi = std::shared_ptr<klv_item>(new klv_item);
            klv_ret_val = klvp.get_klv_item(*p_klvi, be);

            if (klv_ret_val) {
               m_klv_list.push_back(p_klvi);
            }
            std::cout << "  Bytes left: " << be.get_bytes_left() << std::endl; 
         } while (true == klv_ret_val);
      }
      return rval;
   }

   void output_klv_list(std::ostream &os) {
      for (const auto &e: m_klv_list) {
         e->output(std::cout, "");
      }
   }

   mxf_file_header m_header;
   mxf_file_body   m_body;
   mxf_file_footer m_footer;

   int decode_file_header();
   int decode_file_body();
   int decode_file_footer();

   std::string   m_file_name;
   uint32_t      m_file_size;
   uint8_t      *m_p_data;

   int32_t       m_state;
 
private:
   std::list<std::shared_ptr<klv_item>> m_klv_list;
   
   static std::map<UL, std::string> msUL2Name;
   static bool                      msIsUL2NameInit;
   static void init_ul_2_name();
};



#endif
