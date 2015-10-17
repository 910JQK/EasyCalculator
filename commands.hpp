#ifndef EASY_CALCULATOR_COMMANDS_HPP
#define EASY_CALCULATOR_COMMANDS_HPP
#include <string>


struct Command {
  const std::string name;
  void (*exec)(const std::string &str);
  const std::string help;
};


struct Pair {
  std::string id;
  std::string expr;
};


extern Command commands[];


enum Mode {
  Integer, Float, Complex
};


extern Mode mode;

extern void eval(const std::string &expr);

void cmd_mode(const std::string &str);

void help(const std::string &command);

void set_var(const std::string &str);

void set_const(const std::string &str);

void unset(const std::string &str);

void sequence(const std::string &str);

void defun(const std::string &str);


#endif /* EASY_CALCULATOR_COMMANDS_HPP */
