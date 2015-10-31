#ifndef EASY_CALCULATOR_PARSERS_HPP
#define EASY_CALCULATOR_PARSERS_HPP
#include <gmpxx.h>
#include "expression.hpp"


namespace Parsers {
  namespace Int {
    extern Expr::Parser<mpz_class> parser;
    extern void init();
    extern inline mpz_class eval(const std::string &expression){
      return parser.parse(expression);
    }
    extern void factor(mpz_class n);
    mpz_class convert(const std::string &str);
    mpz_class power(const mpz_class &left, const mpz_class &right);
    mpz_class mod(const mpz_class &left, const mpz_class &right);
    mpz_class abs(const std::vector<mpz_class> &argv);
    mpz_class sgn(const std::vector<mpz_class> &argv);
    mpz_class fac(const std::vector<mpz_class> &argv);
    mpz_class P(const std::vector<mpz_class> &argv);
    mpz_class C(const std::vector<mpz_class> &argv);
    mpz_class max(const std::vector<mpz_class> &argv);
    mpz_class min(const std::vector<mpz_class> &argv);
    mpz_class gcd(const std::vector<mpz_class> &argv);
    mpz_class lcm(const std::vector<mpz_class> &argv);
    mpz_class rand(const std::vector<mpz_class> &argv);
  }
  namespace Double {
    extern Expr::Parser<double> parser;
    extern void init();
    extern inline double eval(const std::string &expression){
      return parser.parse(expression);
    }
    double convert(const std::string &str);
    double power(const double &left, const double &right);
    double mod(const double &left, const double &right);
    double abs(const std::vector<double> &argv);
    double sgn(const std::vector<double> &argv);
    double sqrt_(const std::vector<double> &argv);
    double floor_(const std::vector<double> &argv);
    double ceil_(const std::vector<double> &argv);    
    double sin_(const std::vector<double> &argv);
    double cos_(const std::vector<double> &argv);
    double tan_(const std::vector<double> &argv);
    double asin_(const std::vector<double> &argv);
    double acos_(const std::vector<double> &argv);
    double atan_(const std::vector<double> &argv);
    double exp_(const std::vector<double> &argv);
    double log_(const std::vector<double> &argv);
    double log2_(const std::vector<double> &argv);
    double log10_(const std::vector<double> &argv);
    double rand_(const std::vector<double> &argv);
  }
}


#endif /* EASY_CALCULATOR_PARSERS_HPP */
