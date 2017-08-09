//
//  encrypt.cpp
//  Pass Man
//
//  Created by Indi Kernick on 9/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "encrypt.hpp"

#include <memory>
#include <random>

using File = std::unique_ptr<std::FILE, decltype(&std::fclose)>;

File openFile(const char *path, const char *options) {
  std::FILE *file = std::fopen(path, options);
  if (file == nullptr) {
    throw std::runtime_error(std::string("Failed to open file \"") + path + "\"");
  } else {
    return {file, &std::fclose};
  }
}

std::string decryptFile(const uint64_t key, const char *path) {
  File file = openFile(path, "rb");
  
  std::mt19937_64 gen(key);
  std::uniform_int_distribution<uint8_t> dist;
  
  std::string str;
  
  std::fseek(file.get(), 0, SEEK_END);
  str.reserve(std::ftell(file.get()));
  std::rewind(file.get());
  
  uint8_t b = std::fgetc(file.get());
  while (b != uint8_t(EOF)) {
    str.push_back(dist(gen) ^ b);
    b = std::fgetc(file.get());
  }
  
  return str;
}

void encryptFile(const uint64_t key, const char *path, const std::string &str) {
  File file = openFile(path, "wb");
  
  std::mt19937_64 gen(key);
  std::uniform_int_distribution<uint8_t> dist;
  
  for (auto b = str.cbegin(); b != str.cend(); ++b) {
    std::fputc(dist(gen) ^ *b, file.get());
  }
}

uint64_t makeKey() {
  static std::random_device gen;
  std::uniform_int_distribution<uint64_t> dist;
  return dist(gen);
}
