//
//  interpret commands.hpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef interpret_commands_hpp
#define interpret_commands_hpp

#include "passwords.hpp"
#include <experimental/optional>
#include <experimental/string_view>

class CommandInterpreter {
public:
  CommandInterpreter();
  ~CommandInterpreter();
  
  void prefix();
  void interpret(std::experimental::string_view);
  bool shouldContinue() const;

private:
  size_t key = 0;
  std::string file;
  std::experimental::optional<Passwords> passwords;
  bool quit = false;
  
  void openCommand(std::experimental::string_view);
  void clearCommand();
  void flushCommand();
  void quitCommand();
};

#endif
