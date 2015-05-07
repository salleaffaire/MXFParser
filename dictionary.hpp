#ifndef DICTIONARY_HPP___
#define DICTIONARY_HPP___

#include <map>

template <int T>
struct Key {
   uint8_t mKey[T];

   uint8_t &operator[](uint8_t index) {
      return mKey[index];
   }

   uint8_t operator[](uint8_t index) const {
      return mKey[index];
   }
};

// Needed to be able to use UL as a Key in std::map
// ------------------------------------------------------------------------------
template <int T>
bool operator<(const Key<T> &x1, const Key<T> &x2) {
   bool rval = false;
   
   int i=0;
   for (int i=0;i<16;i++) {
      if (x1[i] != x2[i]) {
         rval = (x1[i] < x2[i]);
         break;
      }
   }
   
   return rval;
}


template <int T>
struct Signature { 
   uint16_t mPath[T];

   uint16_t &operator[](uint16_t index) {
      return mPath[index];
   }

   uint16_t operator[](uint16_t index) const {
      return mPath[index];
   }
};

struct DictionaryNode {
   std::string mValue;
   std::map<uint16_t, DictionaryNode *> mChildrenNodes;

   ~DictionaryNode() {
      for (auto &e: mChildrenNodes) {
         delete e.second;
      }
   }
};

template <int T>
struct Dictionary {
   
   Dictionary() {}

   ~Dictionary() {
      for (auto &e: mRootNodes) {
         delete e.second;
      }      
   }
   
   std::map<uint16_t, DictionaryNode *> mRootNodes;
 
   void add(const Signature<T> x, std::string name) {
      DictionaryNode *current_node;

      // Adding the root
      auto it = mRootNodes.find(x[0]);
      if (it == mRootNodes.end()) {
         current_node = new DictionaryNode;
         mRootNodes[x[0]] = current_node;
         //std::cout << "Creating a new root node for " << x[0] << std::endl;
      }
      else {
         current_node = it->second;
         //std::cout << "Following root node for " << x[0] << std::endl; 
      }
      // Adding all other node
      for (int i=1;i<T;i++) {
         auto it = current_node->mChildrenNodes.find(x[i]);
         if (it == current_node->mChildrenNodes.end()) {
            DictionaryNode *new_node = new DictionaryNode;
            current_node->mChildrenNodes[x[i]] = new_node;
            current_node = new_node;
            //std::cout << "Creating a new node for " << x[i] << std::endl;
         }
         else {
            current_node = it->second;
            //std::cout << "Following node for " << x[i] << std::endl;
         }         
      }
      current_node->mValue = name;
   }

   std::string find(const Key<T> &x) {
      std::string rval = "NULL";
      bool found = true;
      // Find the root
      auto it = mRootNodes.find(x[0]);
      if (it != mRootNodes.end()) {
         DictionaryNode *current_node = it->second;
         for (int i=1;i<T;i++) {
            // Test if there is a pass-all vertex
            auto pa_it = current_node->mChildrenNodes.find(0xFFFF);
            // Test if there is a matching vertex
            auto it = current_node->mChildrenNodes.find(x[i]);

            // If there is a matching node - take it
            if (it != current_node->mChildrenNodes.end()) {              
               current_node = it->second;
            }
            // Else if there is a pass-all node - take it
            else if (pa_it != current_node->mChildrenNodes.end()) { 
               current_node = pa_it->second;
            }
            // No Match
            else {
               found = false;
               break;
            }
         }
         if (found) {
            rval = current_node->mValue;
         }
      }
      return rval;
   }
};


#endif
