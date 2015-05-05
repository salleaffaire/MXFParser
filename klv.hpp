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
      if (mValue) {
         delete [] mValue;
      }
   }
   
   uint8_t  mKey[16];
   uint32_t mLength;
   uint8_t *mValue;
};

class klv_parser {
public:
   klv_parser() {
      mFileName = "";
      mDataCurrent = mDataBegining = 0;
      mFileSize = 0;
      mState = 0;
      mVerbose = false;
      mValidate = false;
   }

   klv_parser(std::string filename) {
      mFileName = filename;
      
      // Read the file into memory
      // ------------------------------------------
      std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
      if (in.fail()) {
         mState = -1;
      }
      else {
         mState = 0;
         
         mFileSize = in.tellg();

         // return at the begining
         in.seekg(0, in.beg);

         std::cout << "Opening : " << mFileName
                   << " " << mFileSize << " bytes." << std::endl;

         // Allocate mem ory for file
         mDataCurrent = new uint8_t [mFileSize];
         mDataBegining = mDataCurrent;

         // Read 
         in.read((char *)mDataCurrent, mFileSize);
         in.close();
      }
      
      if (0 == mState) {
         mBE.init(mDataCurrent, mFileSize);
      }
   }

   bool get_klv_item(klv_item &klvi) {
      bool rval = true;
      
      // Read Key
      // -----------------------------------------------------------------------------------
      for (int i=0;i<16;++i) {
         klvi.mKey[i] = mBE.get_bits(8);

      }
      
      if (mVerbose) {
         std::cout << " KEY" << std::endl;
         std::cout << " -------------------------------------------" << std::endl;
         std::cout << std::hex << std::setfill('0') << std::setw(2);
         std::cout << " IOD     = " << "0x" << std::hex << std::setfill('0') << std::setw(2) 
                   << (uint32_t)klvi.mKey[0] << std::endl;
         std::cout << " UL Size = " << "0x" << std::hex << std::setfill('0') << std::setw(2) 
                   << (uint32_t)klvi.mKey[1] << std::endl;
         std::cout << " UL Code = " << "0x" << std::hex << std::setfill('0') << std::setw(2)
                   << (uint32_t)klvi.mKey[2] << std::endl;
         std::cout << " SMPTE D = " << "0x" << std::hex << std::setfill('0') << std::setw(2)
                   << (uint32_t)klvi.mKey[3] << std::endl;
         std::cout << " Registry Category Designator = " 
                   << "0x" << std::hex << std::setfill('0') << std::setw(2)
                   << (uint32_t)klvi.mKey[4] << std::endl;
         std::cout << " Registry Designator          = " 
                   << "0x" << std::hex << std::setfill('0') << std::setw(2)
                   << (uint32_t)klvi.mKey[5] << std::endl;
         std::cout << " Structure Designator         = " 
                   << "0x" << std::hex << std::setfill('0') << std::setw(2)
                   << (uint32_t) klvi.mKey[6] << std::endl;
         std::cout << " Version Number               = " 
                   << "0x" << std::hex << std::setfill('0') << std::setw(2)
                   << (uint32_t)klvi.mKey[7] << std::endl;
         
         std::cout << " Item Designator              = "
                   << "0x" 
                   << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)klvi.mKey[9]
                   << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)klvi.mKey[10]
                   << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)klvi.mKey[11]
                   << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)klvi.mKey[12]
                   << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)klvi.mKey[13]
                   << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)klvi.mKey[14]
                   << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)klvi.mKey[15] 
                   << std::endl;;
      }
      bool isValid = true;
      if (mValidate) {
         if (klvi.mKey[0] != KLV_UL_HEADER_IOD) {
            std::cout << " Invalid OID, should be equal to " 
                      << std::hex << std::setfill('0') << std::setw(2) 
                      << KLV_UL_HEADER_IOD << std::endl;
            isValid = false;
         } 
         if (klvi.mKey[1] != KLV_UL_HEADER_ULSIZE) {
            std::cout << " Invalid UL Size, should be equal to " 
                      << std::hex << std::setfill('0') << std::setw(2) 
                      << KLV_UL_HEADER_ULSIZE << std::endl;
            isValid = false;
         }
         if (klvi.mKey[2] != KLV_UL_DESIGNATOR_ULCODE) {
            std::cout << " Invalid UL Code, should be equal to " 
                      << std::hex << std::setfill('0') << std::setw(2) 
                      << KLV_UL_DESIGNATOR_ULCODE << std::endl;
            isValid = false;
         }
         if (klvi.mKey[3] != KLV_UL_DESIGNATOR_SMPTE_DESIGNATOR) {
            std::cout << " Invalid SMPTE Designator, should be equal to " 
                      << std::hex << std::setfill('0') << std::setw(2) 
                      << KLV_UL_DESIGNATOR_SMPTE_DESIGNATOR << std::endl;
            isValid = false;
         }
         
         if (isValid) {
            std::cout << " KLV Key is valid" << std::endl; 
         }
      }

      // Read Length (BER encoded)
      uint8_t l0 = mBE.get_bits(8);
      
      // Short form
      if (!(l0 & 0x80)) {
         klvi.mLength = l0;
      } 
      // Long form
      else {
         int num_of_octets = l0 & 0x7F;
         if (num_of_octets > 4) {
            std::cout << " Error : This library doesn't support BER long form with number of octets "
                      << "bigger than 4." << std::endl;
            
         }
         int i;
         klvi.mLength = 0;
         for (i=num_of_octets-1;i>=0;--i) {
            uint32_t o = mBE.get_bits(8);
            klvi.mLength |= (o << (i*8));
         }
      }
      
      if (mVerbose) {
         std::cout << " Length = " << klvi.mLength << std::endl;
      }

      // Read Value
      if (klvi.mValue) {
         delete [] klvi.mValue;
      }
      klvi.mValue = new uint8_t [klvi.mLength];
      
      for (int i=0;i<klvi.mLength;i++) {
         klvi.mValue[i] = mBE.get_bits(8);
      }
      
      return rval;
   } 

   void set_verbosity(bool v) {
      mVerbose = v;
   }
   void set_validity_test(bool v) {
      mValidate = v;
   }

private:
   std::string   mFileName;
   uint8_t      *mDataCurrent;
   uint8_t      *mDataBegining;
   uint32_t      mFileSize;

   int32_t       mState;

   bit_extractor mBE;

   bool          mVerbose;
   bool          mValidate;
};


#endif
