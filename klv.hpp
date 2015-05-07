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

#define KLV_UL_HEADER_IOD    0x06
#define KLV_UL_HEADER_ULSIZE 0x0E
#define KLV_UL_DESIGNATOR_ULCODE           0x2B
#define KLV_UL_DESIGNATOR_SMPTE_DESIGNATOR 0x34

#define KLV_GROUP_UNIVERSAL_SET        0x01
#define KLV_GROUP_GLOBAL_SET           0x02
#define KLV_GROUP_LOCAL_SET            0x03
#define KLV_GROUP_VARIABLE_LENGTH_PACK 0x04
#define KLV_GROUP_FIXED_LENGTH_PACK    0x05 

struct UL {
   uint8_t mKey[16];

   uint8_t &operator[](uint8_t index) {
      return mKey[index];
   }

   uint8_t operator[](uint8_t index) const {
      return mKey[index];
   }
};

bool operator<(const UL &x1, const UL &x2);

struct UL_signature { 
   uint16_t mValue[16];
   uint16_t &operator[](uint16_t index) {
      return mValue[index];
   }

   uint16_t operator[](uint16_t index) const {
      return mValue[index];
   }
};

struct key_value_dictionary_node {
   std::string mValue;
   std::map<uint16_t, key_value_dictionary_node *> mChildrenNodes;

   ~key_value_dictionary_node() {
      for (auto &e: mChildrenNodes) {
         delete e.second;
      }
   }
};

struct key_value_dictionary {
   
   key_value_dictionary() {}

   ~key_value_dictionary() {
      for (auto &e: mRootNodes) {
         delete e.second;
      }      
   }
   
   std::map<uint16_t, key_value_dictionary_node *> mRootNodes;

   void add(const UL_signature x, std::string name) {
      key_value_dictionary_node *current_node;

      // Adding the root
      auto it = mRootNodes.find(x[0]);
      if (it == mRootNodes.end()) {
         current_node = new key_value_dictionary_node;
         mRootNodes[x[0]] = current_node; 
      }
      else {
         current_node = it->second;
      }
      // Adding all other node
      for (int i=1;i<16;i++) {
         auto it = current_node->mChildrenNodes.find(x[i]);
         if (it == current_node->mChildrenNodes.end()) {
            key_value_dictionary_node *new_node = new key_value_dictionary_node;
            mRootNodes[x[0]] = new_node;
            current_node = new_node;
         }
         else {
            current_node = it->second;
         }         
      }
      current_node->mValue = name;
   }

   std::string find(const UL &x) {
      std::string rval = "NULL";
      // Find the root
      auto it = mRootNodes.find(x[0]);
      if (it != mRootNodes.end()) {
         key_value_dictionary_node *current_node = it->second;
         for (int i=1;i<16;i++) {
            auto it = current_node->mChildrenNodes.find(x[i]);
            if (it != current_node->mChildrenNodes.end()) {
               

            }
            else {
               break;
            }
         }
      }
      return rval;
   }
};

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
   
   UL       mKey;
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
