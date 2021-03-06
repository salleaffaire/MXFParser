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
#include "dictionary.hpp"
#include "bit_extractor.hpp"
#include "hex_out.hpp"

class mxf_base {
public:
   uint64_t m_offset;
};

// Simple Data Types
// ----------------------------------------

class mxf_auid {
public:
   bool    m_is_ul;
   Key<16> m_id;
};

class mxf_idau {
public:
   bool    m_is_ul;
   Key<16> m_id;
};

class mxf_ber_length {
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
   mxf_header_partition_pack() : m_essence_containers(0) {}

   ~mxf_header_partition_pack() {
      if (m_essence_containers) {
         delete [] m_essence_containers;
      }
   }

   uint16_t m_major_version;
   uint16_t m_minor_version;
   uint32_t m_kag_size;
   uint64_t m_this_partition;
   uint64_t m_previous_partition;
   uint64_t m_footer_partition;
   uint64_t m_header_byte_count;
   uint64_t m_index_byte_count;
   uint32_t m_index_sid;
   uint64_t m_body_offset;
   uint32_t m_body_sid;
   Key<16>  m_operational_pattern;
   Key<16> *m_essence_containers;
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
   mxf_body_partition_pack() : m_essence_containers(0) {}

   ~mxf_body_partition_pack() {
      if (m_essence_containers) {
         delete [] m_essence_containers;
      }
   }

   uint16_t m_major_version;
   uint16_t m_minor_version;
   uint32_t m_kag_size;
   uint64_t m_this_partition;
   uint64_t m_previous_partition;
   uint64_t m_footer_partition;
   uint64_t m_header_byte_count;
   uint64_t m_index_byte_count;
   uint32_t m_index_sid;
   uint64_t m_body_offset;
   uint32_t m_body_sid;
   Key<16>  m_operational_pattern;
   Key<16> *m_essence_containers;
};

class mxf_file_body : public mxf_base {
public:
   mxf_essence_container     m_ess_container0;
   mxf_essence_container     m_ess_container1;
   mxf_body_partition_pack   m_bpp;
};

class mxf_footer_partition_pack : public mxf_base {
public:
   mxf_footer_partition_pack() : m_essence_containers(0) {}

   ~mxf_footer_partition_pack() {
      if (m_essence_containers) {
         delete [] m_essence_containers;
      }
   }

   uint16_t m_major_version;
   uint16_t m_minor_version;
   uint32_t m_kag_size;
   uint64_t m_this_partition;
   uint64_t m_previous_partition;
   uint64_t m_footer_partition;
   uint64_t m_header_byte_count;
   uint64_t m_index_byte_count;
   uint32_t m_index_sid;
   uint64_t m_body_offset;
   uint32_t m_body_sid;
   Key<16>  m_operational_pattern;
   Key<16> *m_essence_containers;
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
      if (!m_is_UL_key_dictionary_init) {
         init_UL_key_dictionary();
         m_is_UL_key_dictionary_init = true;
      }
   }
   
   mxf_file(std::string file_name) : m_file_name(file_name) {
      if (!m_is_UL_key_dictionary_init) {
         init_UL_key_dictionary();
         m_is_UL_key_dictionary_init = true;
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
         bit_extractor be(m_p_data, m_file_size, BE_BIG_ENDIAN);
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
            //std::cout << "  Bytes left: " << be.get_bytes_left() << std::endl; 
         } while (true == klv_ret_val);
      }
      return rval;
   }
   
   void parse() {
      for (const auto &e: m_klv_list) {
         std::string name =  m_UL_key_dictionary.find(e->mKey);
         if (name == m_partition_pack_name) {
            decode_partition_pack(*e);        
         }
         else if ((name == m_fill_item_name) || 
                  (name == m_operational_pattern_name) || 
                  (name == m_primer_pack_name)) {
         
         }
         e->output(std::cout, "");
         std::cout << " NAME : " << name << std::endl << std::endl;
         std::cout << " PAYLOAD = " << std::endl;
         hex_bulk_output(std::cout, 32, e->mValue, e->mLength);
         std::cout << std::endl; 
      }
   }

   void output_klv_list(std::ostream &os) {
      for (const auto &e: m_klv_list) {
         e->output(std::cout, "");
         std::string name =  m_UL_key_dictionary.find(e->mKey);
         std::cout << "  NAME : " << name << std::endl << std::endl;
         if (name != "NULL") {
            std::cout << "PAYLOAD = " << std::endl;
            hex_bulk_output(std::cout, 32, e->mValue, e->mLength);
            std::cout << std::endl;
         }
      }
   }
   
   // Core structures
   // ------------------------------------------------------------
   mxf_file_header m_header;
   mxf_file_body   m_body;
   mxf_file_footer m_footer;
   
   int decode_partition_pack(klv_item &klv) {
      int rval = 0;
      switch (klv.mKey[13]) {
      case 0x02:
         decode_header_partition_pack(klv);
         break;
      case 0x03:
         decode_body_partition_pack(klv);
         break;
      case 0x04:
         decode_footer_partition_pack(klv);
         break;
      }
      return rval;
   }
   
   int decode_header_partition_pack(klv_item &klv) {
      int rval = 0;
      
      bit_extractor be(klv.mValue, klv.mLength, BE_BIG_ENDIAN);
      
      m_header.m_hpp.m_major_version      = be.get_bits(16);
      m_header.m_hpp.m_minor_version      = be.get_bits(16);
      m_header.m_hpp.m_kag_size           = be.get_bits(32);
      m_header.m_hpp.m_this_partition     = be.get_64bits();
      m_header.m_hpp.m_previous_partition = be.get_64bits();
      m_header.m_hpp.m_footer_partition   = be.get_64bits();
      m_header.m_hpp.m_header_byte_count  = be.get_64bits();
      m_header.m_hpp.m_index_byte_count   = be.get_64bits();
      m_header.m_hpp.m_index_sid          = be.get_bits(32);
      m_header.m_hpp.m_body_offset        = be.get_64bits();
      m_header.m_hpp.m_body_sid           = be.get_bits(32);
      
      // UL of operational Pattern
      for (int i=0;i<16;++i) {
         m_header.m_hpp.m_operational_pattern.mKey[i] = be.get_bits(8);
      }

      // UL Batch - Skip the 8 bytes 
      be.get_64bits();
      
      uint32_t nunmber_of_ul = be.get_bytes_left();
      m_header.m_hpp.m_essence_containers = new Key<16>[nunmber_of_ul];

      for (int j=0;j<nunmber_of_ul;j++) {
         for (int i=0;i<16;++i) {
            m_header.m_hpp.m_essence_containers[j].mKey[i] = be.get_bits(8);
         }  
      }

      
#if 1
      
      std::cout << "  Major Version        = " << hex_to_string(m_header.m_hpp.m_major_version)
                << std::endl;
      std::cout << "  Minor Version        = " << hex_to_string(m_header.m_hpp.m_minor_version)
                << std::endl;
      std::cout << "  KAG Size             = " << hex_to_string(m_header.m_hpp.m_kag_size)
                << std::endl;
      std::cout << "  This Partition #     = " << hex_to_string(m_header.m_hpp.m_this_partition)
                << std::endl;
      std::cout << "  Previous Partition # = " << hex_to_string(m_header.m_hpp.m_previous_partition)
                << std::endl;
      std::cout << "  Footer Partition #   = " << hex_to_string(m_header.m_hpp.m_footer_partition)
                << std::endl;
      std::cout << "  Header Byte Count    = " << hex_to_string(m_header.m_hpp.m_header_byte_count)
                << std::endl;
      std::cout << "  Index Byte Count     = " << hex_to_string(m_header.m_hpp.m_index_byte_count)
                << std::endl;
      std::cout << "  Index SID            = " << hex_to_string(m_header.m_hpp.m_index_sid)
                << std::endl;
      std::cout << "  Body Offset          = " << hex_to_string(m_header.m_hpp.m_body_offset)
                << std::endl;
      std::cout << "  Body SID             = " << hex_to_string(m_header.m_hpp.m_body_sid)
                << std::endl;
      
#endif

      return rval;
   }

   int decode_body_partition_pack(klv_item &klv) {
      int rval = 0;
      
      bit_extractor be(klv.mValue, klv.mLength, BE_BIG_ENDIAN);
      
      m_body.m_bpp.m_major_version      = be.get_bits(16);
      m_body.m_bpp.m_minor_version      = be.get_bits(16);
      m_body.m_bpp.m_kag_size           = be.get_bits(32);
      m_body.m_bpp.m_this_partition     = be.get_64bits();
      m_body.m_bpp.m_previous_partition = be.get_64bits();
      m_body.m_bpp.m_footer_partition   = be.get_64bits();
      m_body.m_bpp.m_header_byte_count  = be.get_64bits();
      m_body.m_bpp.m_index_byte_count   = be.get_64bits();
      m_body.m_bpp.m_index_sid          = be.get_bits(32);
      m_body.m_bpp.m_body_offset        = be.get_64bits();
      m_body.m_bpp.m_body_sid           = be.get_bits(32);
      
      // UL of operational Pattern
      for (int i=0;i<16;++i) {
         m_body.m_bpp.m_operational_pattern.mKey[i] = be.get_bits(8);
      }

      // UL Batch - Skip the 8 bytes 
      be.get_64bits();
      
      uint32_t nunmber_of_ul = be.get_bytes_left();
      m_body.m_bpp.m_essence_containers = new Key<16>[nunmber_of_ul];

      for (int j=0;j<nunmber_of_ul;j++) {
         for (int i=0;i<16;++i) {
            m_body.m_bpp.m_essence_containers[j].mKey[i] = be.get_bits(8);
         }  
      }

      
#if 1
      
      std::cout << "  Major Version        = " << hex_to_string(m_header.m_hpp.m_major_version)
                << std::endl;
      std::cout << "  Minor Version        = " << hex_to_string(m_header.m_hpp.m_minor_version)
                << std::endl;
      std::cout << "  KAG Size             = " << hex_to_string(m_header.m_hpp.m_kag_size)
                << std::endl;
      std::cout << "  This Partition #     = " << hex_to_string(m_header.m_hpp.m_this_partition)
                << std::endl;
      std::cout << "  Previous Partition # = " << hex_to_string(m_header.m_hpp.m_previous_partition)
                << std::endl;
      std::cout << "  Footer Partition #   = " << hex_to_string(m_header.m_hpp.m_footer_partition)
                << std::endl;
      std::cout << "  Header Byte Count    = " << hex_to_string(m_header.m_hpp.m_header_byte_count)
                << std::endl;
      std::cout << "  Index Byte Count     = " << hex_to_string(m_header.m_hpp.m_index_byte_count)
                << std::endl;
      std::cout << "  Index SID            = " << hex_to_string(m_header.m_hpp.m_index_sid)
                << std::endl;
      std::cout << "  Body Offset          = " << hex_to_string(m_header.m_hpp.m_body_offset)
                << std::endl;
      std::cout << "  Body SID             = " << hex_to_string(m_header.m_hpp.m_body_sid)
                << std::endl;
      
#endif

      return rval;
      
   }

   int decode_footer_partition_pack(klv_item &klv) {
      int rval = 0;
      
      bit_extractor be(klv.mValue, klv.mLength, BE_BIG_ENDIAN);
      
      m_footer.m_fpp.m_major_version      = be.get_bits(16);
      m_footer.m_fpp.m_minor_version      = be.get_bits(16);
      m_footer.m_fpp.m_kag_size           = be.get_bits(32);
      m_footer.m_fpp.m_this_partition     = be.get_64bits();
      m_footer.m_fpp.m_previous_partition = be.get_64bits();
      m_footer.m_fpp.m_footer_partition   = be.get_64bits();
      m_footer.m_fpp.m_header_byte_count  = be.get_64bits();
      m_footer.m_fpp.m_index_byte_count   = be.get_64bits();
      m_footer.m_fpp.m_index_sid          = be.get_bits(32);
      m_footer.m_fpp.m_body_offset        = be.get_64bits();
      m_footer.m_fpp.m_body_sid           = be.get_bits(32);
      
      // UL of operational Pattern
      for (int i=0;i<16;++i) {
         m_footer.m_fpp.m_operational_pattern.mKey[i] = be.get_bits(8);
      }

      // UL Batch - Skip the 8 bytes 
      be.get_64bits();
      
      uint32_t nunmber_of_ul = be.get_bytes_left();
      m_footer.m_fpp.m_essence_containers = new Key<16>[nunmber_of_ul];

      for (int j=0;j<nunmber_of_ul;j++) {
         for (int i=0;i<16;++i) {
            m_footer.m_fpp.m_essence_containers[j].mKey[i] = be.get_bits(8);
         }  
      }

      
#if 1
      
      std::cout << "  Major Version        = " << hex_to_string(m_header.m_hpp.m_major_version)
                << std::endl;
      std::cout << "  Minor Version        = " << hex_to_string(m_header.m_hpp.m_minor_version)
                << std::endl;
      std::cout << "  KAG Size             = " << hex_to_string(m_header.m_hpp.m_kag_size)
                << std::endl;
      std::cout << "  This Partition #     = " << hex_to_string(m_header.m_hpp.m_this_partition)
                << std::endl;
      std::cout << "  Previous Partition # = " << hex_to_string(m_header.m_hpp.m_previous_partition)
                << std::endl;
      std::cout << "  Footer Partition #   = " << hex_to_string(m_header.m_hpp.m_footer_partition)
                << std::endl;
      std::cout << "  Header Byte Count    = " << hex_to_string(m_header.m_hpp.m_header_byte_count)
                << std::endl;
      std::cout << "  Index Byte Count     = " << hex_to_string(m_header.m_hpp.m_index_byte_count)
                << std::endl;
      std::cout << "  Index SID            = " << hex_to_string(m_header.m_hpp.m_index_sid)
                << std::endl;
      std::cout << "  Body Offset          = " << hex_to_string(m_header.m_hpp.m_body_offset)
                << std::endl;
      std::cout << "  Body SID             = " << hex_to_string(m_header.m_hpp.m_body_sid)
                << std::endl;
      
#endif

      return rval;
   }

   int decode_file_header();
   int decode_file_body();
   int decode_file_footer();

   std::string   m_file_name;
   uint32_t      m_file_size;
   uint8_t      *m_p_data;

   int32_t       m_state;
 
private:

   void read(bit_extractor &be, mxf_auid &auid, bool is_ul) {
      if (is_ul) {
         
      }
      else {

      }
   }

   void read(bit_extractor &be, mxf_idau &idau, bool is_ul) {
      
   }

   std::list<std::shared_ptr<klv_item>> m_klv_list;
   
   static Dictionary<16>  m_UL_key_dictionary;
   static bool            m_is_UL_key_dictionary_init;

   static void init_UL_key_dictionary();

   static std::string     m_partition_pack_name;
   static std::string     m_fill_item_name;
   static std::string     m_operational_pattern_name;
   static std::string     m_primer_pack_name;
};



#endif
