#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include <cmath>
#include "parsers.hpp"
#include "commands.hpp"


#define CATCH \
  catch(std::string err) {    \
    std::cerr << err << '\n'; \
  } catch(const char *err) {  \
    std::cerr << err << '\n'; \
  }

Mode mode = Float;
AngleUnit angle_unit = rad;


Command commands[] = {
  {"mode", cmd_mode, "mode [int|float]\n\tSwitch mode or display current mode."},
  {"angle", cmd_angle, "angle [rad|deg|grad]\n\tSwitch angle unit or display current angle unit."},
  {"import", import, "import file\n\tImport a script file."},
  {"eval", eval, "eval expression\n\tCalculate value expressions."},
  {"int", eval_int, "int expression\n\tCalculate value of expressions in int mode."},
  {"factor", factor, "factor expression\n\tFactor integer numbers."},
  {"float", eval_float, "float expression\n\tCalculate value of expressions in float mode."},
  {"solve", solve, "solve n\n\tSolve linear equation system of n dimensions."},
  {"set", set_var, "set name = expression\n\tSet value for variables."},
  {"const", set_const, "const name = expression\n\tDefine constants."},
  {"def", def, "def name([arg, ...]) = [condition:]experssion; [condition:expression; ...]\n\tDefine functions."},
  {"root", root, "root variable: expression = expression @ x0\n\tFind a root of the equation using Newton's Method."},
  {"seq", sequence,"seq n: [assignment1; assignment2...]: [expression1 | expression2 ...]\n\tGenerate a sequence of length n with variable \"_\" changing from 0 to n-1. Assignments and expressions are evaluated sequently."},
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


void eval_int(const std::string &expr){
  try {
    mpz_class ans = Parsers::Int::eval(expr);
    Parsers::Int::parser.set_var("ans", ans);
    std::cout << ans << '\n';
  }
  CATCH
}


void factor(const std::string &expr){
  try {
    Parsers::Int::factor(Parsers::Int::eval(expr));
  }
  CATCH
}


void eval_float(const std::string &expr){
  try {
    double ans = Parsers::Double::eval(expr);
    Parsers::Double::parser.set_var("ans", ans);
    std::cout << ans << '\n';
  }
  CATCH
}


void eval(const std::string &expr){
  switch(mode){
  case Integer:
    eval_int(expr);
    break;
  case Float:
    eval_float(expr);
    break;
  }
}


void solve(const std::string &str){
  int n;
  double **matrix;
  double *solution;
  try {
    n = std::stoi(str);
  }
  catch(const std::exception &err) {
    std::cerr << "dimension count: invalid value" << '\n';
    return;
  }
  matrix = new double*[n];
  solution = new double[n];
  for(int i=0; i<n; i++)
    matrix[i] = new double[n+1];
  auto is_zero = [](double x) {
    return (abs(x) < 1e-9);
  };
  auto exchange = [matrix, n](int a, int b) {
    for(int i=0; i<n+1; i++) {
      double t = matrix[a][i];
      matrix[a][i] = matrix[b][i];
      matrix[b][i] = t;
    }
  };
  auto add = [matrix, n](int a, int b, double k) {
    for(int i=0; i<n+1; i++)
      matrix[a][i] += matrix[b][i]*k;
  };
  auto clean = [matrix, solution, n]() {
    delete[] solution;
    for(int i=0; i<n; i++)
      delete[] matrix[i];
    delete[] matrix;
  };
  for(int i=0; i<n; i++)
    for(int j=0; j<n+1; j++)
      std::cin >> matrix[i][j];
  for(int i=0; i<n; i++) {
    if(is_zero(matrix[i][i])) {
      bool ok = false;
      for(int j=i+1; j<n; j++) {
	if(!is_zero(matrix[j][i])) {
	  exchange(j, i);
	  ok = true;
	  break;
	}
      }
      if(!ok) {
	std::cerr << "invalid augmented matrix" << '\n';
	clean();
	return;
      }
    }
    double p = 1/matrix[i][i];
    for(int j=i; j<n+1; j++)
      matrix[i][j] *= p;
    for(int j=i+1; j<n; j++)
      add(j, i, -matrix[j][i]);
  }
  for(int i=n-1; i>=0; i--) {
    double x = matrix[i][n];
    for(int j=i+1; j<n+1; j++)
      x -= matrix[i][j]*solution[j];
    solution[i] = x;
  }
  for(int i=0; i<n; i++) {
    std::cout << solution[i];
    if(i != n-1)
      std::cout << ' ';
  }
  std::cout << '\n';
  clean();
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


void cmd_angle(const std::string &str){
  if(str == "rad")
    angle_unit = rad;
  else if(str == "deg")
    angle_unit = deg;
  else if(str == "grad")
    angle_unit = grad;
  else if(angle_unit == rad)
    std::cout << "rad" << '\n';
  else if(angle_unit == deg)
    std::cout << "deg" << '\n';
  else if(angle_unit == grad)
    std::cout << "grad" << '\n';
}


double to_rad(double value){
  if(angle_unit == deg)
    return value*(PI/180);
  else if(angle_unit == grad)
    return value*(PI/200);
  return value;
}


double angle(double value){
  if(angle_unit == deg)
    return value*(180/PI);
  else if(angle_unit == grad)
    return value*(200/PI);
  return value;
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


inline bool split(std::vector<std::string> &tokens, const std::string &text, char separator, bool check_ident = false) {
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
  temp = text.substr(start);
  if(!temp.size())
    return true;
  if((check_ident && !check(temp)) )
    return false;
  tokens.push_back(temp);
  return true;
}


inline Pair split_equation(const std::string &str){
  int p = str.find("=");
  if(p == std::string::npos){
    std::cerr << "Missing \"=\"\n";
    return (Pair){"",""};
  }
  if(p == 0){
    std::cerr << "Missing left side\n";
    return (Pair){"",""};    
  }
  if(p == str.size()-1){
    std::cerr << "Missing right side\n";
    return (Pair){"",""};
  }
  std::string left = str.substr(0, p);
  std::string right = str.substr(p+1);
  return (Pair){left, right};
}


void set_var(const std::string &str){
  using namespace Parsers;
  Pair pair = split_equation(str);
  if(!check(pair.left))
    return;
  const std::string &id = pair.left;
  const std::string &expr = pair.right;
  mpz_class value;
  double value_f;
  try {
    switch(mode){
    case Integer:
      value = Int::eval(expr);
      Int::parser.set_var(id, value);
      // std::cout << value << '\n';
      break;
    case Float:
      value_f = Double::eval(expr);
      Double::parser.set_var(id, value_f);
      // std::cout << value_f << '\n';
      break;
    }
  }
  CATCH
}


void set_const(const std::string &str){
  using namespace Parsers;
  Pair pair = split_equation(str);
  if(!check(pair.left))
    return;
  const std::string &id = pair.left;
  const std::string &expr = pair.right;
  mpz_class value;
  double value_f;
  try {
    switch(mode){
    case Integer:
      value = Int::eval(expr);
      Int::parser.set_const(id, value);
      // std::cout << value << '\n';
      break;
    case Float:
      value_f = Double::eval(expr);
      Double::parser.set_const(id, value_f);
      // std::cout << value_f << '\n';
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


void root(const std::string &str){
  using namespace Parsers;
  int p0 = str.find(":");
  if(p0 == std::string::npos){
    std::cerr << "Missing separator \":\"\n";
    return;
  } else if(p0 == 0){
    std::cerr << "Missing unknown number\n";
    return;
  } else if(p0 == str.size()-1){
    std::cerr << "Missing equation\n";
    return;
  }
  std::string var = str.substr(0, p0);
  if(!check(var))
    return;
  int p1 = str.find("@");
  if(p1 == std::string::npos){
    std::cerr << "Missing separator \"@\"\n";
    return;
  } else if(p1 == str.size()-1){
    std::cerr << "Missing initial value\n";
    return;
  }
  std::string equation = str.substr(p0+1, p1-p0-1);
  std::string x0 = str.substr(p1+1);
  Pair pair = split_equation(equation);
  const std::string &left = pair.left;
  const std::string &right = pair.right;
  double x, x_last, fx, fx1, fx2, d;
  int count = 0;
  if(mode != Float){
    std::cerr << "Finding root is only available in float mode\n";
    return;
  }
  try {
      x = Double::eval(x0);
  }
  CATCH
  try {
    do {
      x_last = x;
      Double::parser.set_var(var, x);
      fx = Double::eval(left) - Double::eval(right);
      x += 5e-6;
      Double::parser.set_var(var, x);
      fx1 = Double::eval(left) - Double::eval(right);
      x -= 1e-5;
      Double::parser.set_var(var, x);
      fx2 = Double::eval(left) - Double::eval(right);
      d = (fx1 - fx2)/1e-5;
      x += 5e-6;
      x -= fx/d;
      if(++count > 1000){
	std::cerr << "Cannot get proper result after an 1000-time-loop\n";
	break;
      }	
    } while (fabs(x - x_last) >= 1e-6);
    std::cout << var << " = " << x << '\n';
    Double::parser.set_var(var, x);
  }
  CATCH
}


void sequence(const std::string &str){
  using namespace Parsers;
  int p = str.find(":");
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
  std::string args = str.substr(p+1);
  mpz_class l;
  switch(mode){
  case Integer:
    try {
      l = Int::eval(count);
    }
    CATCH
    break;
  case Float:
    try {
      l = round(Double::eval(count));
    }
    CATCH
    break;
  }
  if(l <= 0) {
    std::cerr << "seq: count of items must be positive\n";
    return;
  }
  std::string assign_str, expr_str = "";
  p = args.find(":");
  if(p != std::string::npos) {
    assign_str = args.substr(0, p);
    expr_str = args.substr(p+1);
  } else {
    assign_str = args;
  }
  std::vector<std::string> assignments;
  std::vector<std::string> expressions;
  if(!split(assignments, assign_str, ';'))
    return;
  if(expr_str.size() != 0)
    if(!split(expressions, expr_str, '|'))
      return;
  mpz_class &I = Int::parser.variables["_"];
  double &I_f = Double::parser.variables["_"];
  try {
    for(mpz_class i=0; i<l; i++) {
      switch(mode){
      case Integer:
	I = i;
	break;
      case Float:
	I_f = i.get_d();
	break;
      }
      for(const std::string &I: assignments) {
	Pair pair = split_equation(I);
	if(!check(pair.left))
	  return;
	const std::string &id = pair.left;
	const std::string &expr = pair.right;
	switch(mode){
	case Integer:
	  Int::parser.set_var(id, Int::eval(expr));
	  break;
	case Float:
	  Double::parser.set_var(id, Double::eval(expr));
	  break;
	}
      }
      for(const std::string &I: expressions) {
	switch(mode){
	case Integer:
	  std::cout << Int::eval(I) << ' ';
	  break;
	case Float:
	  std::cout << Double::eval(I) << ' ';
	  break;
	}
      }
      if(expr_str.size() != 0)
	std::cout << '\n';
    }
  }
  CATCH
}


void def(const std::string &str){
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
  std::string right = str.substr(p+1);
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


void parse_logical_line(const std::string &raw_line){
  if(!raw_line.size())
    return;

  if(raw_line[0] == '#')
    return;
  
  int end = raw_line.find("#");
  if(end == std::string::npos)
    end = raw_line.size();
  const std::string line = raw_line.substr(0, end);

  int start = line.find(" ");
  if(start == std::string::npos || start == line.size()-1){
    eval(std::regex_replace(line, Expr::BLANK, ""));
    return;
  }
  std::string cmd = line.substr(0, start);
  int i = 0;
  /* NULL means end of list */
  while(commands[i].exec != NULL){
    if(commands[i].name == cmd){
      commands[i].exec(std::regex_replace(line.substr(start+1, line.size()-start-1), Expr::BLANK, ""));
      break;
    }
    i++;
  }
  if(commands[i].exec == NULL)
    eval(std::regex_replace(line, Expr::BLANK, ""));
}


void parse_line(const std::string &line){
  /* split logical lines */
  int start = 0, end = 0;
  std::string temp;
  while((end = line.find(";;", start)) != std::string::npos) {
    temp = line.substr(start, end - start);
    parse_logical_line(temp);
    start = end + 2;
  }
  temp = line.substr(start, line.size());
  parse_logical_line(temp);
}


bool read_file(const std::string &file_name){
  std::ifstream file(file_name);
  if(!file.good()){
    return false;
  }
  std::string line;
  while(std::getline(file, line))
    parse_line(line);
  return true;
}


void import(const std::string &str){
  Mode mode_prev = mode;
  mode = Float;
  if(!read_file(str+".ecs"))
    if(!read_file(str))
      std::cerr << "Import \""<< str << "\": Unable to read file\n";
  mode = mode_prev;
}
