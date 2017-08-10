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

using StrToStrMap = std::unordered_map<std::string, std::string>;

struct Passwords {
  explicit Passwords(StrToStrMap &&);
  Passwords(const Passwords &) = delete;
  Passwords(Passwords &&) = default;
  
  Passwords &operator=(const Passwords &) = delete;
  Passwords &operator=(Passwords &&) = default;
  
  StrToStrMap map;
  std::vector<std::string> searchResults;
};

#endif
