//
//  interpret commands.cpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "interpret commands.hpp"

#include <cstdlib>
#include <iostream>
#include "parse.hpp"
#include "encrypt.hpp"
#include "generate key.hpp"
#include "write to clipboard.hpp"
#include "generate password.hpp"

namespace {
  const char HELP_TEXT[] =
R"(help
  Prints a list of all commands and what they do.

gen_key
  Generates an encryption key

gen_key_copy
  Generates an encryption key and copies it to the clipboard

open <key> <file>
  Opens a file and decrypts it. Once opened, the file can be manipulated. If the
  file either doesn't exist or is empty, then a new database is created.

clear
  Removes every entry from the database.

flush
  Writes all changes to the file (if it exists).

quit
  Writes all changes to the file (if it exists) and exits.

search <sub_string>
  Searchs for passwords by name.

list
  Lists the names of every password.

count
  Prints the number of passwords in the database.

gen <length>
  Prints a randomly generated string

create <name> <password>
  Creates a new entry in the database.

create_gen <name> <length>
  Generates a passwords and puts it into the database.

change <name> <new_password>
  If name is an unambiguous substring then that password is changed. If name is
  a number then the password in the most recent search with that index is
  changed.

rename <name> <new_name>
  Renames a password. Name is as described in "change".

get <name>
  Prints a password. Name is as described in "change".

copy <name>
  Copies a password to the clipboard. Name is as described in "change".

rem <name>
  Removes a password from the database. Name is as described in "change".
)";

  bool commandIs(const std::experimental::string_view command, const std::experimental::string_view name) {
    if (command.size() < name.size()) {
      return false;
    }
    
    const size_t space = command.find_first_of(' ');
    constexpr size_t npos = std::experimental::string_view::npos;
    const size_t compareLength = space == npos ? command.size() : space;
    
    if (compareLength != name.size()) {
      return false;
    }
    
    return command.compare(0, compareLength, name) == 0;
  }
  
  void unknownCommand(const std::experimental::string_view command) {
    std::cout << "Unknown command \"";
    const size_t space = command.find_first_of(' ');
    std::cout.write(
      command.data(),
      space == std::experimental::string_view::npos
      ? command.size()
      : space
    );
    std::cout << "\"\n";
  }

  void helpCommand() {
    std::cout << HELP_TEXT;
  }
  
  void genKeyCommand() {
    std::cout << "Encryption key: " << generateKey() << '\n';
  }
  
  void genKeyCopyCommand() {
    const uint64_t key = generateKey();
    writeToClipboard(std::to_string(key));
    std::cout << "Success!\n";
  }
}

CommandInterpreter::CommandInterpreter() {
  std::cout << "Welcome to PassMan!\n";
  std::cout << "Type \"help\" for a list of commands.\n";
  std::cout << '\n';
}

CommandInterpreter::~CommandInterpreter() {
  //goodbye message?
};

void CommandInterpreter::prefix() {
  std::cout << "> ";
}

constexpr std::experimental::string_view operator""_sv(const char *data, const size_t size) {
  return {data, size};
}

void CommandInterpreter::interpret(const std::experimental::string_view command) {
  #define COMMAND_IS(COMMAND_NAME) \
    const auto name = #COMMAND_NAME##_sv; \
    commandIs(command, name)
  
  if (COMMAND_IS(help)) {
    helpCommand();
  } else if (COMMAND_IS(gen_key)) {
    genKeyCommand();
  } else if (COMMAND_IS(gen_key_copy)) {
    genKeyCopyCommand();
  } else if (COMMAND_IS(open)) {
    openCommand(command.substr(name.size()));
  } else if (COMMAND_IS(clear)) {
    clearCommand();
  } else if (COMMAND_IS(flush)) {
    flushCommand();
  } else if (COMMAND_IS(quit)) {
    quitCommand();
  } else if (COMMAND_IS(search)) {
    searchCommand(command.substr(name.size()));
  } else if (COMMAND_IS(list)) {
    listCommand();
  } else if (COMMAND_IS(count)) {
    countCommand();
  } else if (COMMAND_IS(gen)) {
    genCommand(command.substr(name.size()));
  } else {
    unknownCommand(command);
  }
  
  std::cout.flush();
  
  #undef COMMAND_IS
}

bool CommandInterpreter::shouldContinue() const {
  return !quit;
}

namespace {
  uint64_t readNumber(std::experimental::string_view &args, bool &failed) {
    if (args.size() == 0) {
      failed = true;
      return 0;
    }
    char *end;
    const uint64_t arg = std::strtoull(args.data(), &end, 0);
    if (errno == ERANGE) {
      failed = true;
      return 0;
    }
    args.remove_prefix(end - args.data());
    return arg;
  }

  std::string readString(std::experimental::string_view &args) {
    if (args.size() == 0) {
      return {};
    }
    
    std::string arg;
    bool prevBackSlash = false;
    
    for (size_t i = 0; i != args.size(); ++i) {
      const char c = args[i];
      if (c == ' ') {
        if (prevBackSlash) {
          arg.push_back(' ');
        } else {
          break;
        }
      } else if (c == '\\') {
        if (prevBackSlash) {
          arg.push_back('\\');
          prevBackSlash = false;
        } else {
          prevBackSlash = true;
        }
      } else {
        arg.push_back(c);
        prevBackSlash = false;
      }
    }
    
    return arg;
  }

  bool fileExists(const char *const path) {
    std::unique_ptr<std::FILE, decltype(&std::fclose)> file(
      fopen(path, "r"),
      &std::fclose
    );
    return bool(file);
  }

  bool nextArg(std::experimental::string_view &args, const char *signature) {
    if (args.size() == 0 || args[0] != ' ') {
      std::cout << "Command signature is:\n" << signature << '\n';
      return false;
    }
    args.remove_prefix(1);
    return true;
  }
}

void CommandInterpreter::openCommand(
  std::experimental::string_view arguments
) {
  if (!nextArg(arguments, "open <key> <file>")) {
    return;
  }

  bool failed = false;
  const uint64_t newKey = readNumber(arguments, failed);
  if (newKey == 0 || failed) {
    std::cout << "Invalid key\n";
    return;
  }
  
  if (!nextArg(arguments, "open <key> <file>")) {
    return;
  }
  
  const std::string newFile = readString(arguments);
  if (newFile.size() == 0) {
    std::cout << "Invalid file path\n";
    return;
  }
  
  if (!fileExists(newFile.c_str())) {
    std::FILE *fileStream = std::fopen(newFile.c_str(), "w");
    if (fileStream == nullptr) {
      std::cout << "Failed to create file \"" << newFile.c_str() << "\"\n";
      return;
    } else {
      std::cout << "Created a new file named \"" << newFile.c_str() << "\"\n";
    }
    std::fclose(fileStream);
  }
  
  if (passwords) {
    flushCommand();
  }
  
  key = newKey;
  file = std::move(newFile);
  passwords.emplace(readPasswords(decryptFile(key, file)));
  
  std::cout << "Success!\n";
}

void CommandInterpreter::clearCommand() {
  if (passwords) {
    passwords->map.clear();
    passwords->searchResults.clear();
    std::cout << "Database cleared\n";
  }
}

void CommandInterpreter::flushCommand() {
  if (passwords) {
    encryptFile(key, file, writePasswords(*passwords));
    std::cout << "Database flushed\n";
  }
}

void CommandInterpreter::quitCommand() {
  flushCommand();
  quit = true;
}

void CommandInterpreter::searchCommand(std::experimental::string_view arguments) {
  if (!passwords) {
    std::cout << "Database is uninitialized. Use the open command to initialize.\n";
    return;
  }
  
  if (!nextArg(arguments, "search <sub_string>")) {
    return;
  }
  
  const std::string subString = readString(arguments);
  if (subString.empty()) {
    std::cout << "Invalid substring\n";
    return;
  }
  
  passwords->searchResults.clear();
  
  for (auto p = passwords->map.cbegin(); p != passwords->map.cend(); ++p) {
    constexpr size_t npos = std::experimental::string_view::npos;
    if (p->first.find(subString) != npos) {
      std::cout.width(8);
      std::cout << passwords->searchResults.size() << " - " << p->first << '\n';
      passwords->searchResults.push_back(p->first);
    }
  }
  
  if (passwords->searchResults.empty()) {
    std::cout << "No password names where found containing the substring:\n\"";
    std::cout << subString << "\"\n";
  }
}

void CommandInterpreter::listCommand() {
  if (!passwords) {
    std::cout << "Database is uninitialized. Use the open command to initialize.\n";
  } else if (passwords->map.empty()) {
    std::cout << "Database is empty\n";
  } else {
    for (auto p = passwords->map.cbegin(); p != passwords->map.cend(); ++p) {
      std::cout << p->first << '\n';
    }
  }
}

void CommandInterpreter::countCommand() {
  if (!passwords) {
    std::cout << "Database is uninitialized. Use the open command to initialize.\n";
  } else if (passwords->map.empty()) {
    std::cout << "Database is empty\n";
  } else if (passwords->map.size() == 1) {
    std::cout << "Database contains 1 password\n";
  } else {
    std::cout << "Database contains " << passwords->map.size() << "passwords\n";
  }
}

void CommandInterpreter::genCommand(std::experimental::string_view arguments) {
  if (!nextArg(arguments, "gen <length>")) {
    return;
  }
  
  bool failed = false;
  const uint64_t size = readNumber(arguments, failed);
  if (size == 0 || failed) {
    std::cout << "Invalid length\n";
    return;
  }
  
  std::cout << "Random password: " << generatePassword(size) << '\n';
}
