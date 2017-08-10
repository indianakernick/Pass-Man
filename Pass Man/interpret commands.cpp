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
  } else {
    unknownCommand(command);
  }
  
  std::cout.flush();
  
  #undef COMMAND_IS
}

bool CommandInterpreter::shouldContinue() const {
  return !quit;
}
