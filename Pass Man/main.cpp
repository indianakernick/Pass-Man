//
//  main.cpp
//  Pass Man
//
//  Created by Indi Kernick on 9/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include <iostream>
#include <cstdio>

int main(int, const char **) {
  
  std::FILE *file = popen("pbcopy", "w");
  if (file != nullptr) {
    std::fputs("Hello, World!", file);
  }
  pclose(file);
  
  return 0;
}
