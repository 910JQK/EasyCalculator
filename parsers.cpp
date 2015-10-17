#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "parsers.hpp"


namespace Parsers {


namespace Int {


Expr::Parser<mpz_class> parser(convert, "%^");


gmp_randclass random(gmp_randinit_default);  


inline bool assert_positive(std::string func_name, const mpz_class &n){
  if(n <= 0)
    throw func_name + "(): Out of function definition";
}


inline bool assert_non_negative(std::string func_name, const mpz_class &n){
  if(n < 0)
    throw func_name + "(): Out of function definition";
}
  

mpz_class convert(const std::string &str){
  return mpz_class(str);
}


mpz_class mod(const mpz_class &left, const mpz_class &right){
  if(right == 0)
    throw "Math Error: Division by zero";
  mpz_class result = left % right;
  return (result < 0)? (result + right): result;
}


mpz_class power(const mpz_class &left, const mpz_class &right){
  if(right < 0 && left == -1)
    return -1 - 2*(-right%2 - 1);
  if(right < 0 && left != 1)
    return 0;
  if(right == 0 && left == 0)
    throw "Math Error: 0 to the power of 0 is undefined";
  mpz_class x = left;
  mpz_class n = right;
  mpz_class result(1);
  while(n){
    if(n % 2 == 1)
      result *= x;
    x *= x;
    n /= 2;
  }
  return result;
}


mpz_class abs(const std::vector<mpz_class> &argv){
  return abs(argv[0]);
}


mpz_class sgn(const std::vector<mpz_class> &argv){
  return sgn(argv[0]);
}

  
mpz_class fac(const std::vector<mpz_class> &argv){
  assert_non_negative("fac", argv[0]);
  mpz_class result = 1;
  mpz_class n = argv[0];
  while(n){
    result *= n;
    n--;
  }
  return result;
}


mpz_class P(const std::vector<mpz_class> &argv){
  assert_positive("P", argv[0]);
  assert_positive("P", argv[1]);
  if(argv[0] < argv[1])
    throw "P(): Out of function definition";
  mpz_class n = argv[0];
  mpz_class r = argv[1];
  mpz_class result = 1;
  while(r--){
    result *= n;
    n--;
  }
  return result;
}


mpz_class C(const std::vector<mpz_class> &argv){
  assert_non_negative("C", argv[0]);
  assert_non_negative("C", argv[1]);
  if(argv[0] < argv[1])
    throw "C(): Out of function definition";
  mpz_class n = argv[0];
  mpz_class r = argv[1];

  if(n == 0)
    return 1;
  if(r == 0)
    return 1;

  if(r > n/2)
    r = n - r;
  mpz_class result = 1;
  mpz_class i;
  for(i=1; i<=r; i++)
    result = result*(n-i+1)/i;
  
  return result;
}


mpz_class max(const std::vector<mpz_class> &argv){
  return (argv[0] > argv[1])? argv[0]: argv[1];
}


mpz_class min(const std::vector<mpz_class> &argv){
  return (argv[0] < argv[1])? argv[0]: argv[1];
}


mpz_class gcd(const mpz_class &left, const mpz_class &right){
  assert_non_negative("gcd", left);
  assert_non_negative("gcd", right);
  if(right == 0)
    return left;
  return gcd(right, left % right);
}


mpz_class gcd(const std::vector<mpz_class> &argv){
  return gcd(argv[0], argv[1]);
}


mpz_class lcm(const std::vector<mpz_class> &argv){
  return (argv[0] * argv[1])/gcd(argv[0], argv[1]);
}


mpz_class rand(const std::vector<mpz_class> &argv){
  assert_positive("rand", argv[0]);
  return random.get_z_range(argv[0]);
}


void init(){
  parser.operators["%"] = Expr::Operator<mpz_class>(Expr::R, 80, mod);
  parser.operators["^"] = Expr::Operator<mpz_class>(Expr::R, 85, power);
  parser.functions["abs"] = Expr::Function<mpz_class>(abs, 1);
  parser.functions["sgn"] = Expr::Function<mpz_class>(sgn, 1);
  parser.functions["fac"] = Expr::Function<mpz_class>(fac, 1);
  parser.functions["P"] = Expr::Function<mpz_class>(P, 2);
  parser.functions["C"] = Expr::Function<mpz_class>(C, 2);
  parser.functions["max"] = Expr::Function<mpz_class>(max, 2);
  parser.functions["min"] = Expr::Function<mpz_class>(min, 2);
  parser.functions["gcd"] = Expr::Function<mpz_class>(gcd, 2);
  parser.functions["lcm"] = Expr::Function<mpz_class>(lcm, 2);
  random.seed(time(NULL));
  parser.functions["rand"] = Expr::Function<mpz_class>(rand, 1);
}


}


namespace Double {


Expr::Parser<double> parser(convert, "%^");


double convert(const std::string &str){
  return std::stod(str);
}

  
double power(const double &left, const double &right){
  if(left == 0 && right == 0)
    throw "Math Error: 0 to the power of 0 is undefined";
  return pow(left, right);
}


double mod(const double &left, const double &right){
  if(right == 0)
    throw "Math Error: Division by zero";
  return fmod(left, right);
}


double abs(const std::vector<double> &argv){
  return fabs((double)argv[0]);
}


double sgn(const std::vector<double> &argv){
  if(argv[0] > 0)
    return 1;
  if(argv[0] < 0)
    return -1;
  return 0;
}


double sqrt_(const std::vector<double> &argv){
  return sqrt(argv[0]);
}


double cbrt_(const std::vector<double> &argv){
  return cbrt(argv[0]);
}


double floor_(const std::vector<double> &argv){
  return floor(argv[0]);
}


double ceil_(const std::vector<double> &argv){
  return ceil(argv[0]);
}


double sin_(const std::vector<double> &argv){
  return sin(argv[0]);
}


double cos_(const std::vector<double> &argv){
  return cos(argv[0]);
}


double tan_(const std::vector<double> &argv){
  return tan(argv[0]);
}


double asin_(const std::vector<double> &argv){
  return asin(argv[0]);
}


double acos_(const std::vector<double> &argv){
  return acos(argv[0]);
}


double atan_(const std::vector<double> &argv){
  return atan(argv[0]);
}


double exp_(const std::vector<double> &argv){
  return exp(argv[0]);
}


double log_(const std::vector<double> &argv){
  return log(argv[0]);
}


double log2_(const std::vector<double> &argv){
  return log2(argv[0]);
}


double log10_(const std::vector<double> &argv){
  return log10(argv[0]);
}


double rand_(const std::vector<double> &argv){
  return (double)rand()/RAND_MAX;
}


void init(){
  parser.operators["%"] = Expr::Operator<double>(Expr::R, 80, mod);
  parser.operators["^"] = Expr::Operator<double>(Expr::R, 85, power);
  parser.functions["abs"] = Expr::Function<double>(abs, 1);
  parser.functions["sgn"] = Expr::Function<double>(sgn, 1);
  parser.functions["sqrt"] = Expr::Function<double>(sqrt_, 1);
  parser.functions["cbrt"] = Expr::Function<double>(cbrt_, 1);
  parser.functions["floor"] = Expr::Function<double>(floor_, 1);
  parser.functions["ceil"] = Expr::Function<double>(ceil_, 1);
  parser.functions["sin"] = Expr::Function<double>(sin_, 1);
  parser.functions["cos"] = Expr::Function<double>(cos_, 1);
  parser.functions["tan"] = Expr::Function<double>(tan_, 1);
  parser.functions["asin"] = Expr::Function<double>(asin_, 1);
  parser.functions["acos"] = Expr::Function<double>(acos_, 1);
  parser.functions["atan"] = Expr::Function<double>(atan_, 1);
  parser.functions["exp"] = Expr::Function<double>(exp_, 1);
  parser.functions["log"] = Expr::Function<double>(log_, 1);
  parser.functions["log2"] = Expr::Function<double>(log2_, 1);
  parser.functions["log10"] = Expr::Function<double>(log10_, 1);
  parser.functions["rand"] = Expr::Function<double>(rand_, 0);
  parser.constants["PI"] = 3.141592653589793;
  parser.scientific_notation_enabled = true;
  parser.decimal_point_enabled = true;
  srand(time(NULL));
}

  
}


}
