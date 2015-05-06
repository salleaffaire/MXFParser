#ifndef KLV_HPP___
#define KLV_HPP___

#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iomanip>

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

// Octet 5 
enum klv_registry_category {
   KLV_RC_DICTIONARY = 0x01,
   KLV_RC_GROUP      = 0x02,
   KLV_RC_WRAPPER    = 0x03,
   KLV_RC_LABEL      = 0x04
};

class klv_item {
public:
   klv_item() {
      for (int i=0;i<16;++i) {
         mKey[i] = 0;
      }
      mLength = 0;
      mValue = 0;
   }
   ~klv_item() {
   }
   
   uint8_t  mKey[16];
   uint32_t mLength;
   uint8_t *mValue;
   uint8_t *mPointer;

   void output() {
      std::cout << " KEY" << std::endl;
      std::cout << " -------------------------------------------" << std::endl;
      std::cout << std::hex << std::setfill('0') << std::setw(2);
      std::cout << " IOD     = " << "0x" << std::hex << std::setfill('0') << std::setw(2) 
                << (uint32_t)mKey[0] << std::endl;
      std::cout << " UL Size = " << "0x" << std::hex << std::setfill('0') << std::setw(2) 
                << (uint32_t)mKey[1] << std::endl;
      std::cout << " UL Code = " << "0x" << std::hex << std::setfill('0') << std::setw(2)
                << (uint32_t)mKey[2] << std::endl;
      std::cout << " SMPTE D = " << "0x" << std::hex << std::setfill('0') << std::setw(2)
                << (uint32_t)mKey[3] << std::endl;
      std::cout << " Registry Category Designator = " 
                << "0x" << std::hex << std::setfill('0') << std::setw(2)
                << (uint32_t)mKey[4];
      if (0x02 == mKey[4]) {
         std::cout << " [Group]" << std::endl; 
      }
      else {
         std::cout << std::endl;
      }
      std::cout << " Registry Designator          = " 
                << "0x" << std::hex << std::setfill('0') << std::setw(2)
                << (uint32_t)mKey[5] << std::endl;
      std::cout << " Structure Designator         = " 
                << "0x" << std::hex << std::setfill('0') << std::setw(2)
                << (uint32_t)mKey[6] << std::endl;
      std::cout << " Version Number               = " 
                << "0x" << std::hex << std::setfill('0') << std::setw(2)
                << (uint32_t)mKey[7] << std::endl;
      
      std::cout << " Item Designator              = "
                << "0x" 
                << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[9]
                << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[10]
                << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[11]
                << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[12]
                << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[13]
                << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[14]
                << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[15] 
                << std::endl;
      std::cout << " Length = " << mLength << std::endl;      
   }
   
   bool validate() {
      bool rval = true;
      if (mKey[0] != KLV_UL_HEADER_IOD) {
         std::cout << " Invalid OID, should be equal to " 
                   << std::hex << std::setfill('0') << std::setw(2) 
                   << KLV_UL_HEADER_IOD << std::endl;
         rval = false;
      } 
      if (mKey[1] != KLV_UL_HEADER_ULSIZE) {
         std::cout << " Invalid UL Size, should be equal to " 
                   << std::hex << std::setfill('0') << std::setw(2) 
                   << KLV_UL_HEADER_ULSIZE << std::endl;
         rval = false;
      }
      if (mKey[2] != KLV_UL_DESIGNATOR_ULCODE) {
         std::cout << " Invalid UL Code, should be equal to " 
                   << std::hex << std::setfill('0') << std::setw(2) 
                   << KLV_UL_DESIGNATOR_ULCODE << std::endl;
         rval = false;
      }
      if (mKey[3] != KLV_UL_DESIGNATOR_SMPTE_DESIGNATOR) {
         std::cout << " Invalid SMPTE Designator, should be equal to " 
                   << std::hex << std::setfill('0') << std::setw(2) 
                   << KLV_UL_DESIGNATOR_SMPTE_DESIGNATOR << std::endl;
         rval = false;
      }
      
      if (rval) {
         std::cout << " KLV Key is valid" << std::endl; 
      }
      
      return rval;
   }

   int get_klv_item(klv_item &klvi) {
      int rval = 0;
      if ((mPointer - mValue) == mLength) {
         rval = -1;
      }
      else {
         switch (mKey[5]) {
         case KLV_RC_DICTIONARY:
            break;
         case KLV_RC_GROUP:
            if (mKey[6] == KLV_GROUP_UNIVERSAL_SET) {
               
            }
            else if (mKey[6] == KLV_GROUP_GLOBAL_SET) {

            }
            else if (mKey[6] == KLV_GROUP_LOCAL_SET) {

            }
            else if (mKey[6] == KLV_GROUP_VARIABLE_LENGTH_PACK) {

            }
            else if (mKey[6] == KLV_GROUP_FIXED_LENGTH_PACK) {

            }
            break;
         case KLV_RC_WRAPPER:
            break;           
         case KLV_RC_LABEL:
            break;            
         }
      }
      return rval;
   }
};

class klv_parser {
public:
   klv_parser() {
      mVerbose = false;
      mValidate = false;
   }

   bool get_klv_item(klv_item &klvi, bit_extractor &be) {
      bool rval = true;
      
      // Read Key
      // ------------------------------------------------------------------------------
      // Test for enough byte for a LU Key
      if (be.get_bytes_left() >= 16) {
         for (int i=0;i<16;++i) {
            klvi.mKey[i] = be.get_bits(8);
         }
      }
      else {
         std::cout << "Need 16 bytes for a KLV Key but only " << be.get_bytes_left()
                   << " bytes left in the file." << std::endl;
         rval = false;
      }

      uint8_t l0;
      if (rval && (be.get_bytes_left() >= 1)) {
         // Read Length (BER encoded)
         l0 = be.get_bits(8);
      }
      else {
         std::cout << "Need 1 byte for a BER first byte file is empty" << std::endl;
         rval = false;
      }
      
      if (rval) {
         // Short form
         if (!(l0 & 0x80)) {
            klvi.mLength = l0;
         } 
         // Long form
         else {
            int num_of_octets = l0 & 0x7F;
            if (be.get_bytes_left() >= num_of_octets) {
               if (num_of_octets > 4) {
                  std::cout << " Error : This library doesn't support BER long form with number of octets "
                            << "bigger than 4." << std::endl;
               }
               int i;
               klvi.mLength = 0;
               for (i=num_of_octets-1;i>=0;--i) {
                  uint32_t o = be.get_bits(8);
                  klvi.mLength |= (o << (i*8));
               }
            }
            else {
               std::cout << "Need " << num_of_octets
                         << " bytes for a KLV Length BER but only " << be.get_bytes_left()
                         << " bytes left in the file." << std::endl;
               rval = false;
            }
         }
         
      }

      if (rval && (be.get_bytes_left() >= klvi.mLength)) {
         klvi.mValue = klvi.mPointer = be.get_buffer();
         be.skip_n_bytes(klvi.mLength);
      }
      else {
         std::cout << "Need " << klvi.mLength
                   << " bytes for a KLV Value but only " << be.get_bytes_left()
                   << " bytes left in the file." << std::endl;
         rval = false;
      }
      
      return rval;
   }

   void set_verbosity(bool v) {
      mVerbose = v;
   }
   void set_validation(bool v) {
      mValidate = v;
   }

private:
   int32_t       mState;

   bool          mVerbose;
   bool          mValidate;
};


#endif
