#include <stack>
#include <gmpxx.h>
#include "expression.hpp"


namespace Expr {


template class Parser<mpz_class>;
template class Function<mpz_class>;
template class Parser<double>;
template class Function<double>;
  

const std::regex BLANK("[ \\n\\t\\0]+");


template <class T>
T logical_not(const T &operand) {
  return !operand;
}


template <class T>
T unary_plus(const T &operand) {
  return operand;
}


template <class T>
T unary_minus(const T &operand) {
  return -operand;
}


template <class T>
T plus(const T &left, const T &right) {
  return left + right;
}


template <class T>
T minus(const T &left, const T &right) {
  return left - right;
}


template <class T>
T multiply(const T &left, const T &right) {
  return left * right;
}


template <class T>
T divide(const T &left, const T &right) {
  if(right == 0)
    throw "Math Error: Division by zero";
  return left / right;
}


template <class T>
T less_than(const T &left, const T &right) {
  return (left < right);
}


template <class T>
T greater_than(const T &left, const T &right) {
  return (right < left);
}


template <class T>
T less_than_or_equal(const T &left, const T &right) {
  return !(right < left);
}


template <class T>
T greater_than_or_equal(const T &left, const T &right) {
  return !(left < right);
}


template <class T>
T equal(const T &left, const T &right) {
  return (left == right);
}


template <class T>
T not_equal(const T &left, const T &right) {
  return !(left == right);
}


template <class T>
T logical_and(const T &left, const T &right) {
  return (left && right);
}


template <class T>
T logical_or(const T &left, const T &right) {
  return (left || right);
}


template <class T>
static T empty_function(const T &oprand) {

}


template <class T>
inline bool key(const std::unordered_map<std::string, T> &map, std::string k) {
  return (map.find(k) != map.end());
}


inline bool is_ident_char(char c) {
  return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_');  
}


inline bool is_ident_begin(char c) {
  return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}


template <class T>
Operator<T>::Operator() {

}

  
template <class T>
Operator<T>::Operator(Associativity assoc_init, int priority_init, T (*exec_init)(const T &operand_left, const T &operand_right)) {
  assoc = assoc_init;
  priority = priority_init;
  exec = exec_init;
  unary = false;
}


template <class T>
Operator<T>::Operator(Associativity assoc_init, int priority_init, T (*exec_init)(const T &operand)) {
  assoc = assoc_init;
  priority = priority_init;
  exec_unary = exec_init;
  unary = true;
}


enum TokenType {
  initial, end, op, num, ident, parens_l, parens_r, comma
};


template <class T>
struct Token {
  TokenType type;
  std::string str;
  Token(TokenType t, const std::string s) {
    type = t;
    str = s;
  }
};


inline void replace_function_args(std::string &expression, const std::vector<std::string> &args) {
  expression = std::regex_replace(expression, BLANK, "");
  int i, start, l;
  std::string s;
  for(i=0; i<args.size(); i++) {
    start = 0;
    l = args[i].size();
    while((start = expression.find(args[i], start)) != std::string::npos) {
      s = std::to_string(i);
      if(!(start > 0 && is_ident_char(expression[start-1])) && !(start + l < expression.size() && is_ident_char(expression[start+l])) ) {
	expression.replace(start, args[i].size(), std::string("_") + s);
	start += (1 + s.size());
      }
      start += l;
    } // while
  } // for
}
  

template<class T>
Function<T>::Function() {

}


template<class T>
Function<T>::Function(T (*exec_function)(const std::vector<T> &argv), int num_args) {
  exec = exec_function;
  argc = num_args;
  builtin = true;
}


template<class T>
Function<T>::Function(const std::vector<FunctionExpression> &expressions, const std::vector<std::string> &args) {
  expr = expressions;
  int i;
  for(i=0; i<expr.size(); i++) {
    replace_function_args(expr[i].expr_condition, args);
    replace_function_args(expr[i].expr_value, args);
  }
  argc = args.size();
  builtin = false;
}


template <class T>
Parser<T>::Parser(T (*convert_function)(const std::string &str), std::string operator_chars_str_add) {
  convert = convert_function;
  operator_chars = std::string("#+-*/<>=&|!") + operator_chars_str_add;
  Operator<T> sentinel(L, 0, empty_function);
  operators["#"] = sentinel;
  Operator<T> op_logical_or(L, 30, logical_or);
  operators["||"] = op_logical_or;
  Operator<T> op_logical_and(L, 31, logical_and);
  operators["&&"] = op_logical_and;
  Operator<T> op_equal(L, 40, equal);
  Operator<T> op_not_equal(L, 40, not_equal);
  operators["="] = op_equal;
  operators["!="] = op_not_equal;
  Operator<T> op_lt(L, 50, less_than);
  Operator<T> op_gt(L, 50, greater_than);
  Operator<T> op_le(L, 50, less_than_or_equal);
  Operator<T> op_ge(L, 50, greater_than_or_equal);
  operators["<"] = op_lt;
  operators[">"] = op_gt;
  operators["<="] = op_le;
  operators[">="] = op_ge;
  Operator<T> op_plus(L, 70, plus);
  Operator<T> op_minus(L, 70, minus);
  operators["+"] = op_plus;
  operators["-"] = op_minus;
  Operator<T> op_multiply(L, 80, multiply);
  Operator<T> op_divide(L, 80, divide);
  operators["*"] = op_multiply;
  operators["/"] = op_divide;
  Operator<T> op_unary_plus(R, 90, unary_plus);
  Operator<T> op_unary_minus(R, 90, unary_minus);
  Operator<T> op_logical_not(R, 90, logical_not);
  operators["(+)"] = op_unary_plus;
  operators["(-)"] = op_unary_minus;
  operators["!"] = op_logical_not;
  scientific_notation_enabled = false;
  decimal_point_enabled = false;
  blank_recheck_enabled = false;
}


template <class T>
Parser<T>::~Parser() {

}


template <class T>
bool Parser<T>::is_operator_char(char c) {
  return (operator_chars.find(c) != std::string::npos);
}


template <class T>
bool Parser<T>::is_id_available(const std::string &id) {
  return !(key(constants, id) || key(variables, id) || key(functions, id));
}


template <class T>
void Parser<T>::set_const(const std::string &id, T value) {
  if(!id.size())
    throw "Constant was set with empty identifier";
  if(id[0] == '_')
    throw "Identifiers start with \"_\" are reserved";
  if(id == "x")
    throw "Constant name cannot be \"x\", which is reserved";
  if(id == "ans")
    throw "Constant name cannot be \"ans\", which is reserved";
  if(!is_id_available(id))
    throw "This identifier is in use";
  constants[id] = value;
}


template <class T>
void Parser<T>::set_var(const std::string &id, T value) {
  if(!id.size())
    throw "Variable was set with empty identifier";
  if(id[0] == '_')
    throw "Identifiers starting with \"_\" are reserved";
  if(key(constants, id) || key(functions, id))
    throw "This identifier is in use";
  variables[id] = value;
}


template <class T>
void Parser<T>::set_function(const std::string &id, const std::vector<std::string> &conditions, const std::vector<std::string> &expressions, const std::vector<std::string> &arguments) {
  if(key(constants, id) || key(variables, id))
    throw "This identifier is in use";
  if(key(functions, id) && functions[id].builtin)
      throw id + " is builtin";
  if(id == "x")
    throw "Function name cannot be \"x\", which is reserved";
  if(id == "ans")
    throw "Function name cannot be \"ans\", which is reserved";
  std::vector<FunctionExpression> f_expressions;
  int i;
  for(i=0; i<conditions.size(); i++) {
    f_expressions.push_back((FunctionExpression) {conditions[i], expressions[i]});
  }
  functions[id] = Function<T>(f_expressions, arguments);
}


template <class T>
void Parser<T>::unset(const std::string &id) {
  if(key(variables, id)) {
    variables.erase(id);
  } else if(key(constants, id)) {
    constants.erase(id);
  } else if(key(functions, id)) {
    if(functions[id].builtin)
      throw id + " is builtin";
    else
      functions.erase(id);
  } else {
    throw "This identifier is not in use";
  }
}


inline bool check_local_var_id(std::string s) {
  int i;
  for(i=1; i<s.size(); i++)
    if(!(s[i] >= '0' && s[i] <= '9'))
      return false;
  return true;
}


template <class T>
inline void check_operand(const std::stack<T> &output) {
    if(output.empty())
      throw "Missing operand";
}


template <class T>
inline void eval_operator(Operator<T> &op, std::stack<T> &output) {
  if(!op.unary) {
    check_operand(output);
    T operand_right = output.top();
    output.pop();
    check_operand(output);
    T operand_left = output.top();
    output.pop();
    output.push(op.exec(operand_left, operand_right));
  }else{
    check_operand(output);
    T operand = output.top();
    output.pop();
    output.push(op.exec_unary(operand));
  }
}


template <class T>
T Parser<T>::parse(std::string str, std::vector<T> &local_variables) {
  if(blank_recheck_enabled)
    str = std::regex_replace(str, BLANK, "");
  #ifdef PARSER_DEBUG
  std::cerr << "#Debug: Parse String " << str << '\n';
  #endif
  
  TokenType prev = initial;
  TokenType status;
  std::string token_str = "";
  std::vector<Token<T>> tokens;
  str += '\0';
  bool not_evaluable = true;
  bool read_e = false;
  bool read_dot = false;
  
  int i, len = str.size();
  for(i=0; i<len; i++) {
    char c = str[i];
    /* There should be a special check for scientific notation of float numbers */
    if(is_operator_char(c) && !(scientific_notation_enabled && i > 0 && (c == '-' || c == '+') && (str[i-1] == 'e' || str[i-1] == 'E')) )
      status = op;
    else if(c == '(')
      status = parens_l;
    else if(c == ')')
      status = parens_r;
    else if(c == ',')
      status = comma;
    else if(c == '\0')
      status = end;
    else if(prev == ident || (!(scientific_notation_enabled && prev == num) && is_ident_begin(c) ))
      status = ident;
    else
      status = num;

    if(status == ident && !is_ident_char(c))
      throw "ILLEGAL identifier";

    if(status == num) {
      if(scientific_notation_enabled) {
	if(c == 'e' || c == 'E') {
	  if(read_e)
	    throw "ILLEGAL number";
	  read_e = true;
	} else if(c == '+' || c == '-') {
	  if(str[i-1] != 'e' && str[i-1] != 'E')
	    throw "ILLEGAL number";
	  // else ok
	} else if(c != '.' && !(c >= '0' && c <= '9')) {
	  throw "ILLEGAL number";
	}
      }
      if(decimal_point_enabled) {
	if(c == '.') {
	  if(read_dot)
	    throw "ILLEGAL number";
	  read_dot = true;
	} else if(!scientific_notation_enabled && !(c >= '0' && c <= '9')) {
	  throw "ILLEGAL number";
	}
      }
      if(
	 (!decimal_point_enabled && !scientific_notation_enabled)
	 && !(c >= '0' && c <= '9') ) {
	throw "ILLEGAL number";
      }
    }
    #ifdef PARSER_DEBUG    
    std::cerr << "#Debug: i == " << i <<  "; status == " << status << '\n';
    #endif
    if((status != prev && prev != initial) || (prev == parens_l || prev == parens_r)) {
      if(prev == num) {
	if(scientific_notation_enabled && read_e)
	  read_e = false;
	if(decimal_point_enabled && read_dot)
	  read_dot = false;
      }
      #ifdef PARSER_DEBUG    
      std::cerr << "#Debug: Read String " << token_str << '\n';
      #endif
      if((token_str == "+" || token_str == "-") && not_evaluable)
	token_str = '(' + token_str + ')';
      /* bad fix */
      if(!(op && (token_str == "*-" || token_str == "/-" || token_str == "^-")) ) {
	Token<T> t(prev, token_str);
	tokens.push_back(t);
	token_str = "";
      }else{
	Token<T> t1(op, token_str.substr(0, 1));
	Token<T> t2(op, "(-)");
	tokens.push_back(t1);
	tokens.push_back(t2);
	token_str = "";
      }
    }
    not_evaluable = (prev == initial || prev == parens_l || prev == comma || prev == op);
    token_str += c;
    prev = status;
  }

  std::stack<Token<T>> temp;
  std::stack<T> output;
  Token<T> sentinel(op, "#");
  temp.push(sentinel);
  Operator<T> o1, o2;
  Function<T> f;
  int n;
  for(i=0; i<tokens.size(); i++) {

    #ifdef PARSER_DEBUG    
    switch(tokens[i].type) {
    case op:
      std::cerr << "#Debug: Token Operator " << tokens[i].str << '\n';
      break;
    case ident:
      std::cerr << "#Debug: Token Identifier " << tokens[i].str << '\n';
      break;
    case parens_l:
      std::cerr << "#Debug: Token (\n";      
      break;
    case parens_r:
      std::cerr << "#Debug: Token )\n";
      break;
    case comma:
      std::cerr << "#Debug: Token ,\n";
      break;
    case num:
      std::cerr << "#Debug: Token Number " << convert(tokens[i].str) << '\n';
      break;
    default:
      std::cerr << "#Debug: Token Unknown " << convert(tokens[i].str) << '\n';
    }
    #endif
    
    switch(tokens[i].type) {
    case num:
      output.push(convert(tokens[i].str));
      break;
    case op:
      if(operators.find(tokens[i].str) == operators.end())	
	throw std::string("Undefined operator ") + tokens[i].str;
      if(temp.top().type == parens_l) {
	temp.push(tokens[i]);
	break;
      }
      o1 = operators[tokens[i].str];
      o2 = operators[temp.top().str];
      while((o1.assoc == L && o1.priority <= o2.priority)
	    || (o1.assoc == R && o1.priority < o2.priority)) {
	temp.pop();
	eval_operator(o2, output);
	o2 = operators[temp.top().str];
      }
      temp.push(tokens[i]);
      break;
    case parens_l:
      temp.push(tokens[i]);
      break;
    case parens_r:
      while(temp.top().type != parens_l) {
	o1 = operators[temp.top().str];
	/* sentinel */
	if(!o1.priority)	  
	  throw "Missing (";
	temp.pop();
	eval_operator(o1, output);
      }
      temp.pop();
      if(temp.top().type == ident) {
	if(functions.find(temp.top().str) == functions.end())
	  throw std::string("Undefined function ") + temp.top().str;
	f = functions[temp.top().str];
	std::vector<T> argv;
	int j;
	for(j=0; j<f.argc; j++) {
	  if(output.empty())
	    throw std::string("Expect ") + std::to_string(f.argc) + " arguments but " + std::to_string(j) + " given";
	  argv.insert(argv.begin(), output.top());
	  output.pop();
	}
	if(f.builtin) {
	  output.push(f.exec(argv));
	}else{
	  bool ok = false;
	  for(j=0; j<f.expr.size(); j++) {
	    if(this->parse(f.expr[j].expr_condition, argv)) {
	      output.push(this->parse(f.expr[j].expr_value, argv));
	      ok = true;
	      break;
	    }
	  } // for
	  if(!ok)
	    throw "Out of function definition";
	} // if
	temp.pop();
      }
      break;
    case ident:
      if(i+1 < tokens.size() && tokens[i+1].type == parens_l) {
	/* function */
	temp.push(tokens[i]);
      }else{
	if(tokens[i].str.size() > 1 && tokens[i].str[0] == '_' && check_local_var_id(tokens[i].str) && (n = std::stoi(tokens[i].str.substr(1, tokens[i].str.size()-1)) ) < local_variables.size())
	  output.push(local_variables[n]);
	else if(constants.find(tokens[i].str) != constants.end())
	  output.push(constants[tokens[i].str]);
	else if(variables.find(tokens[i].str) != variables.end())
	  output.push(variables[tokens[i].str]);
	else
	  throw std::string(tokens[i].str) + ": no such variable or constant";
      }
      break;
    case comma:
      while(temp.top().type != parens_l) {
	o1 = operators[temp.top().str];
	/* sentinel */
	if(!o1.priority)	  
	  throw "Invalid separator";
	temp.pop();
	eval_operator(o1, output);
      }
      break;
    }
  }
  while(true) {
    if(temp.top().type == parens_l)
      throw "Missing )";
    o1 = operators[temp.top().str];
    if(!o1.priority)
      break;
    temp.pop();
    eval_operator(o1, output);
  }

  if(output.size() > 1)
    throw "Syntax Error";
  #ifdef PARSER_DEBUG    
  std::cerr << "#Debug: output.top() == " << output.top() << '\n';
  #endif
  return output.top();
}


template <class T>
T Parser<T>::parse(const std::string &str) {
  return parse(str, empty_vector);
}


}
