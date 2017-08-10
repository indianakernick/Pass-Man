//
//  interpret commands.cpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "interpret commands.hpp"

#include <iostream>

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

void CommandInterpreter::interpret(const std::experimental::string_view command) {
  
}

bool CommandInterpreter::shouldContinue() const {
  return !quit;
}
