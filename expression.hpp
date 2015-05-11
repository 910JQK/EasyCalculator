#ifndef EASY_CALCULATOR_EXPRESSION_PARSER_HPP
#define EASY_CALCULATOR_EXPRESSION_PARSER_HPP
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>


namespace Expr {
  extern const std::regex BLANK;
  enum Associativity {
    L, R
  };
  template <class T>
  struct Operator {
    Associativity assoc;
    int priority;
    bool unary;
    union {
      T (*exec)(const T &operand_left, const T &operand_right);
      T (*exec_unary)(const T &operand);
    };
    Operator();
    Operator(Associativity assoc_init, int priority_init, T (*exec_init)(const T &operand_left, const T &operand_right));
    Operator(Associativity assoc_init, int priority_init, T (*exec_init)(const T &operand));
  };
  struct FunctionExpression {
    std::string expr_condition;
    std::string expr_value;
    FunctionExpression(std::string condition, std::string value){
      expr_condition = condition;
      expr_value = value;
    }
  };
  template <class T>
  struct Function {
    int argc;
    bool builtin;
    T (*exec)(const std::vector<T> &argv);
    std::vector<FunctionExpression> expr;
    Function();
    Function(T (*exec_function)(const std::vector<T> &argv), int num_args);
    Function(const std::vector<FunctionExpression> &expressions, const std::vector<std::string> &args);
  };
  template <class T>
  class Parser {
  protected:
    std::vector<T> empty_vector;
  public:
    Parser(T (*convert_function)(const std::string &str), std::string operator_chars_str_add = "");
    ~Parser();
    std::unordered_map<std::string, Operator<T>> operators;
    std::unordered_map<std::string, T> constants;
    std::unordered_map<std::string, T> variables;
    std::unordered_map<std::string, Function<T>> functions;
    std::string operator_chars;
    bool is_operator_char(char c);
    T (*convert)(const std::string &str);
    void set_const(const std::string &id, T value);
    void set_var(const std::string &id, T value);
    void set_function(const std::string &id, const std::vector<std::string> &conditions, const std::vector<std::string> &expressions, const std::vector<std::string> &arguments);
    void unset(const std::string &id);
    bool scientific_notation_enabled;
    bool decimal_point_enabled;
    bool blank_recheck_enabled;
    T parse(const std::string &str);
    T parse(std::string str, std::vector<T> &local_variables);
  };
}


#endif /* EASY_CALCULATOR_EXPRESSION_PARSER_HPP */
