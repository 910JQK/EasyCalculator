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
  std::string left;
  std::string right;
};


enum Mode {
  Integer, Float
};


enum AngleUnit {
  rad, deg, grad
};

extern Command commands[];

extern Mode mode;

extern AngleUnit angle_unit;

extern double to_rad(double value);

extern double angle(double value);

extern void parse_line(const std::string &line);

void eval(const std::string &expr);

void eval_int(const std::string &expr);

void factor(const std::string &expr);

void eval_float(const std::string &expr);

void solve(const std::string &str);

void cmd_mode(const std::string &str);

void cmd_angle(const std::string &str);

void help(const std::string &command);

void set_var(const std::string &str);

void set_const(const std::string &str);

void unset(const std::string &str);

void sequence(const std::string &str);

void root(const std::string &str);

void def(const std::string &str);

void parse_logical_line(const std::string &raw_line);

bool read_file(const std::string &file_name);

void import(const std::string &str);

#endif /* EASY_CALCULATOR_COMMANDS_HPP */
