#ifndef MXF_BASE_HPP___
#define MXF_BASE_HPP___

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

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
   mxf_file_header m_header;
   mxf_file_body   m_body;
   mxf_file_footer m_footer;

   int decode_file_header();
   int decode_file_body();
   int decode_file_footer();

   std::ifstream m_file;
   std::string   m_file_name;
private:
   
};



#endif
