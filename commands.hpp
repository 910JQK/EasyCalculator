#ifndef EASY_CALCULATOR_COMMANDS_HPP
#define EASY_CALCULATOR_COMMANDS_HPP
#include <string>


const double PI = 3.141592653589793;


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
  Integer, Float
};


enum AngleUnit {
  rad, deg, grad
};


extern Mode mode;

extern AngleUnit angle_unit;

extern void eval(const std::string &expr);

extern double to_rad(double value);

extern double angle(double value);

void eval_int(const std::string &expr);

void factor(const std::string &expr);

void eval_float(const std::string &expr);

void cmd_mode(const std::string &str);

void cmd_angle(const std::string &str);

void help(const std::string &command);

void set_var(const std::string &str);

void set_const(const std::string &str);

void unset(const std::string &str);

void sequence(const std::string &str);

void defun(const std::string &str);


#endif /* EASY_CALCULATOR_COMMANDS_HPP */
