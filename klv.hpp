#ifndef KLV_HPP___
#define KLV_HPP___

#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <list>
#include <map>

#include "bit_extractor.hpp"
#include "dictionary.hpp"

#define KLV_UL_HEADER_IOD    0x06
#define KLV_UL_HEADER_ULSIZE 0x0E
#define KLV_UL_DESIGNATOR_ULCODE           0x2B
#define KLV_UL_DESIGNATOR_SMPTE_DESIGNATOR 0x34

#define KLV_GROUP_UNIVERSAL_SET        0x01
#define KLV_GROUP_GLOBAL_SET           0x02
#define KLV_GROUP_LOCAL_SET            0x03
#define KLV_GROUP_VARIABLE_LENGTH_PACK 0x04
#define KLV_GROUP_FIXED_LENGTH_PACK    0x05 

// Octet 5 
enum klv_registry_category {
   KLV_RC_DICTIONARY = 0x01,
   KLV_RC_GROUP      = 0x02,
   KLV_RC_WRAPPER    = 0x03,
   KLV_RC_LABEL      = 0x04
};

class klv_item {
public:
   klv_item();
   ~klv_item();
   
   Key<16>  mKey;
   uint32_t mLength;
   uint8_t *mValue;

   bool parse_value();
   void output(std::ostream &os, std::string indent);
   bool validate();
   
   // Used for Groups and Sets 
   std::list<std::shared_ptr<klv_item>> *mKLVList;
};

class klv_parser {
public:
   klv_parser();

   bool get_klv_item(klv_item &klvi, bit_extractor &be);
   void set_verbosity(bool v);
   void set_validation(bool v);

private:
   int32_t       mState;

   bool          mVerbose;
   bool          mValidate;
};


#endif
