#include <iostream>
#include <vector>
#include "parsers.hpp"
#include "commands.hpp"


#define CATCH \
  catch(std::string err) {    \
    std::cerr << err << '\n'; \
  } catch(const char *err) {  \
    std::cerr << err << '\n'; \
  }


Mode mode = Float;


Command commands[] = {
  {"mode", cmd_mode, "mode [int|float]\n\tSwitch mode or display current mode"},
  {"eval", eval, "eval expression\n\tCalculate expressions."},
  {"set", set_var, "set name = expression\n\tSet value for variables."},
  {"const", set_const, "const name = expression\n\tDefine constants."},
  {"defun", defun, "defun name([arg, ...]) = [condition:]experssion; [condition:expression; ...]\n\tDefine functions."},
  {"seq", sequence,"seq n: expression\n\tGenerate sequences and calculate the sum as well as the product. Expression will be parsed n times with variable \"_\" changing from 0 to n-1."},
  {"unset", unset, "unset name\n\tUnset variables, constants and functions."},
  {"help", help, "help command\n\tDisplay information about commands."},
  {"END_OF_LIST", NULL, ""}
};


const std::regex IDENT("[A-Za-z_][A-Za-z0-9_]*");
const std::regex INTEGER("[0-9]+");


inline bool check(const std::string &str){
  if(!std::regex_match(str, IDENT)){
    std::cerr << "ILLEGAL identifier " << str << '\n';
    return false;
  }
  return true;
}


void eval(const std::string &expr){
  using namespace Parsers;
  try {
    switch(mode){
    case Integer:
      std::cout << Int::eval(expr) << '\n';
      break;
    case Float:
      std::cout << Double::eval(expr) << '\n';
	break;
    }
  } 
  CATCH
}


void cmd_mode(const std::string &str){
  if(str == "int")
    mode = Integer;
  else if(str == "float")
    mode = Float;
  else if(mode == Integer)
    std::cout << "int" << '\n';
  else if(mode == Float)
    std::cout << "float" << '\n';
}


void help(const std::string &command){
  int i = 0;
  while(commands[i].exec != NULL){
    if(commands[i].name == command){
      std::cout << commands[i].help << "\n\n";
      break;
    }
    i++;
  }
  if(commands[i].exec == NULL)
    std::cerr << "No such command: " << command << '\n';
}


inline Pair set_check(const std::string &str){
  int p = str.find("=");
  if(p == std::string::npos){
    std::cerr << "Missing \"=\"\n";
    return (Pair){"",""};
  }
  if(p == 0){
    std::cerr << "Missing identifier\n";
    return (Pair){"",""};    
  }
  if(p == str.size()-1){
    std::cerr << "Missing expression\n";
    return (Pair){"",""};
  }
  std::string id = str.substr(0, p);
  std::string expr = str.substr(p+1, str.size());  
  if(!check(id))
    return (Pair){"",""};
  return (Pair){id, expr};
}


void set_var(const std::string &str){
  using namespace Parsers;
  Pair pair = set_check(str);
  if(!pair.id.size())
    return;
  const std::string &id = pair.id;
  const std::string &expr = pair.expr;
  mpz_class value;
  double value_f;
  try {
    switch(mode){
    case Integer:
      value = Int::eval(expr);
      Int::parser.set_var(id, value);
      std::cout << value << '\n';
      break;
    case Float:
      value_f = Double::eval(expr);
      Double::parser.set_var(id, value_f);
      std::cout << value_f << '\n';
      break;
    }
  }
  CATCH
}


void set_const(const std::string &str){
  using namespace Parsers;
  Pair pair = set_check(str);
  if(!pair.id.size())
    return;
  const std::string &id = pair.id;
  const std::string &expr = pair.expr;
  mpz_class value;
  double value_f;
  try {
    switch(mode){
    case Integer:
      value = Int::eval(expr);
      Int::parser.set_const(id, value);
      std::cout << value << '\n';
      break;
    case Float:
      value_f = Double::eval(expr);
      Double::parser.set_var(id, value_f);
      std::cout << value_f << '\n';
      break;
    }
  }
  CATCH
}


void unset(const std::string &str){
  using namespace Parsers;
  try {
    switch(mode){
    case Integer:
      Int::parser.unset(str);
      break;
    case Float:
      Double::parser.unset(str);
      break;
    }
  }
  CATCH
}


void sequence(const std::string &str){
  using namespace Parsers;
  int p =str.find(":");
  if(p == std::string::npos){
    std::cerr << "Missing \":\"\n";
    return;
  }
  if(p == 0){
    std::cerr << "Missing the number of repetitions\n";
    return;
  }
  if(p == str.size()-1){
    std::cerr << "Missing expression\n";
    return;
  }
  std::string count = str.substr(0, p);
  std::string expr = str.substr(p+1, str.size());
  if(!std::regex_match(count, INTEGER)){
    std::cerr << "ILLEGAL integer " << count << '\n';
    return;
  }
  int i, l = std::stoi(count);
  if(l <= 1)
    return;
  mpz_class sum, product;
  mpz_class &v = Int::parser.variables["_"];
  double sum_f, product_f;
  double &v_f = Double::parser.variables["_"];
  switch(mode){
  case Integer:
    try{
      sum = 0;
      product = 1;
      v = 0;
      mpz_class t;
      for(i=0; i<l; i++){
	t = Int::eval(expr);
	v = v + 1;
	sum += t;
	product *= t;
	std::cout << i << ": " << t << '\n';
      }
      std::cout << "sum = " << sum << '\n';
      std::cout << "product = " << product << '\n';
      Int::parser.variables.erase("_");
    }
    CATCH
    break;
  case Float:
    try{
      sum_f = 0;
      product_f = 1;
      v_f = 0;
      double t_f;
      for(i=0; i<l; i++){
	t_f = Double::eval(expr);
	v_f = v_f + 1;
	sum_f += t_f;
	product_f *= t_f;
	std::cout << i << ": " << t_f << '\n';
      }
      std::cout << "sum = " << sum_f << '\n';
      std::cout << "product = " << product_f << '\n';
      Double::parser.variables.erase("_");
    }
    CATCH
    break;
  }
}


inline bool split(std::vector<std::string> &tokens, const std::string &text, char separator, bool check_ident) {
  int start = 0, end = 0;
  std::string temp;
  while((end = text.find(separator, start)) != std::string::npos) {
    temp = text.substr(start, end - start);
    if(!temp.size()){
      std::cerr << "Unexpected empty entry\n";
      return false;
    }
    if((check_ident && !check(temp)) )
      return false;
    tokens.push_back(temp);
    start = end + 1;
  }
  temp = text.substr(start, text.size());
  if(!temp.size())
    return true;
  if((check_ident && !check(temp)) )
    return false;
  tokens.push_back(temp);
  return true;
}


void defun(const std::string &str){
  using namespace Parsers;
  std::string id;
  std::vector<std::string> arguments;
  std::vector<std::string> conditions;
  std::vector<std::string> expressions;
  std::vector<std::string> temp;

  int p = str.find('=');
  if(p == std::string::npos){
    std::cerr << "Missing \"=\"\n";
    return;
  }
  if(p == 0){
    std::cerr << "Missing identifier\n";
    return;
  }
  if(p == str.size()-1){
    std::cerr << "Missing expression\n";
    return;
  }
  std::string left = str.substr(0, p);
  std::string right = str.substr(p+1, str.size());
  p = left.find('(');
  if(p == 0){
    std::cerr << "Missing identifier";
    return;
  }
  if(p == left.size()-1){
    std::cerr << "Missing \")\"";
    return;
  }
  if(p == std::string::npos){
    if(!check(id = left))
      return;
  }else{
    if(left[left.size()-1] != ')'){
      std::cerr << "Missing \")\"";
      return;
    }
    if(!check(id = left.substr(0, p)) )
      return;
    if(!split(arguments, left.substr(p+1, left.size()-p-2), ',', true))
      return;
  }
  if(!split(temp, right, ';', false))
    return;
  int i;
  for(i=0; i<temp.size(); i++){
    p = temp[i].find(':');
    if(p == std::string::npos){
      conditions.push_back("1");
      expressions.push_back(temp[i]);
      continue;
    }
    if(p == 0){
      std::cerr << "Missing condition\n";
      return;
    }
    if(p == temp[i].size()-1){
      std::cerr << "Missing expression\n";
      return;
    }
    conditions.push_back(temp[i].substr(0, p));
    expressions.push_back(temp[i].substr(p+1, temp[i].size()) );
  }

  try{
    switch(mode){
    case Integer:
      Int::parser.set_function(id, conditions, expressions, arguments);
      break;
    case Float:
      Double::parser.set_function(id, conditions, expressions, arguments); 
      break;
    }
  }
  CATCH
}
