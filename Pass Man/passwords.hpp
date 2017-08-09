//
//  passwords.hpp
//  Pass Man
//
//  Created by Indi Kernick on 9/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef passwords_hpp
#define passwords_hpp

#include <string>
#include <vector>
#include <unordered_map>

class Passwords {
public:
  explicit Passwords(std::unordered_map<std::string, std::string> &&);
  
  //searchs the map for a substring
  std::vector<std::string> search(const std::string &) const;
  //get a password from the search results
  std::string get(size_t) const;
  //searchs the map for a substring and if one entry contains the substring,
  //the password will be returned, otherwise, an exception is thrown
  std::string get(const std::string &) const;
  
  
  
  std::unordered_map<std::string, std::string> &getMap() const;
  
private:
  std::unordered_map<std::string, std::string> map;
  std::vector<std::string> searchResults;
};

#endif
