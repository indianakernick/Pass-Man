//
//  app.cpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app.hpp"

#include <iostream>
#include "interpret commands.hpp"

void runApp() {
  CommandInterpreter interpreter;
  std::string command;
  do {
    interpreter.prefix();
    std::cin >> command;
    interpreter.interpret(command);
  } while (interpreter.shouldContinue());
}
