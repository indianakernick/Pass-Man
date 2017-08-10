//
//  main.cpp
//  Pass Man
//
//  Created by Indi Kernick on 9/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include <iostream>

#include "generate password.hpp"

int main(int, const char **) {
  
  std::cout << generatePassword(32) << '\n';
  
  return 0;
}
