#include "klv.hpp"

// KLV Item Abstraction
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------

klv_item::klv_item() : mKLVList(0) {
   for (int i=0;i<16;++i) {
      mKey[i] = 0;
   }
   mLength = 0;
   mValue = 0;
}

klv_item::~klv_item() {
   if (mKLVList) {
      delete mKLVList;
   }
}


bool klv_item::parse_value() {
   int rval = 0;

   switch (mKey[4]) {
   case KLV_RC_DICTIONARY:
      break;
   case KLV_RC_GROUP:
      if (mKey[5] == KLV_GROUP_UNIVERSAL_SET) {
         if (mKLVList) {
            delete mKLVList;
         }

         mKLVList = new std::list<std::shared_ptr<klv_item>>;
         
         // Initialize Bit Extractor
         bit_extractor be(mValue, mLength);
         // Need a KLV parser instance
         klv_parser klvp;
         
         klvp.set_verbosity(true);
         klvp.set_validation(true);
         
         bool klv_ret_val;
         do {
            auto p_klvi = std::shared_ptr<klv_item>(new klv_item);
            klv_ret_val = klvp.get_klv_item(*p_klvi, be);
            if (klv_ret_val) {
               mKLVList->push_back(p_klvi);
            }
            std::cout << "    Bytes left: " << be.get_bytes_left() << std::endl;
         } while (true == klv_ret_val);
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
   
   return rval;
}

void klv_item::output(std::ostream &os, std::string indent) {
   os << indent << " KEY" << std::endl;
   os << indent << " -------------------------------------------" << std::endl;
   os << indent << std::hex << std::setfill('0') << std::setw(2);
   os << indent << " IOD     = " << "0x" << std::hex << std::setfill('0') << std::setw(2) 
      << (uint32_t)mKey[0] << std::endl;
   os << indent << " UL Size = " << "0x" << std::hex << std::setfill('0') << std::setw(2) 
      << (uint32_t)mKey[1] << std::endl;
   os << indent << " UL Code = " << "0x" << std::hex << std::setfill('0') << std::setw(2)
      << (uint32_t)mKey[2] << std::endl;
   os << indent << " SMPTE D = " << "0x" << std::hex << std::setfill('0') << std::setw(2)
      << (uint32_t)mKey[3] << std::endl;
   os << indent << " Registry Category Designator = " 
      << "0x" << std::hex << std::setfill('0') << std::setw(2)
      << (uint32_t)mKey[4];
   if (0x02 == mKey[4]) {
      os << indent << " [Group]" << std::endl; 
   }
   else {
      os << indent << std::endl;
   }
   os << indent << " Registry Designator          = " 
      << "0x" << std::hex << std::setfill('0') << std::setw(2)
      << (uint32_t)mKey[5] << std::endl;
   os << indent << " Structure Designator         = " 
      << "0x" << std::hex << std::setfill('0') << std::setw(2)
      << (uint32_t)mKey[6] << std::endl;
   os << indent << " Version Number               = " 
      << "0x" << std::hex << std::setfill('0') << std::setw(2)
      << (uint32_t)mKey[7] << std::endl;
      
   os << indent << " Item Designator              = "
      << "0x" 
      << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[8]
      << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[9]
      << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[10]
      << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[11]
      << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[12]
      << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[13]
      << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[14]
      << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)mKey[15] 
      << std::endl;
   os << indent << " Length = " << mLength << std::endl; 

   if (mKLVList) {
      
   }
}
   
bool klv_item::validate() {
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

// KLV Parser 
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------

klv_parser::klv_parser() {
   mVerbose = false;
   mValidate = false;
}

bool klv_parser::get_klv_item(klv_item &klvi, bit_extractor &be) {
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
      klvi.mValue = be.get_buffer();
      be.skip_n_bytes(klvi.mLength);
   }
   else {
      std::cout << "Need " << klvi.mLength
                << " bytes for a KLV Value but only " << be.get_bytes_left()
                << " bytes left in the file." << std::endl;
      rval = false;
   }

   if (rval)  {
      klvi.parse_value();
   }
   
   return rval;
}

void klv_parser::set_verbosity(bool v) {
   mVerbose = v;
}
void klv_parser::set_validation(bool v) {
   mValidate = v;
}
