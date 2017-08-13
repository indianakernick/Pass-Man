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
  void flushCommand() const;
  void quitCommand();
  
  void expectInit() const;
  
  void searchCommand(std::experimental::string_view);
  void listCommand() const;
  void countCommand() const;
  void genCommand(std::experimental::string_view) const;
  
  StrToStrMap::iterator uniqueSearch(std::experimental::string_view);
  
  void createCommand(std::experimental::string_view);
  void createGenCommand(std::experimental::string_view);
  void changeCommand(std::experimental::string_view);
  void changeSCommand(std::experimental::string_view);
};

#endif
