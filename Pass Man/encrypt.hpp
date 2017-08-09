//
//  encrypt.hpp
//  Pass Man
//
//  Created by Indi Kernick on 9/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef encrypt_hpp
#define encrypt_hpp

#include <string>

std::string decryptFile(uint64_t, const char *);
void encryptFile(uint64_t, const char *, const std::string &);

uint64_t makeKey();

#endif
