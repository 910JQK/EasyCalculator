#ifndef EASY_CALCULATOR_PARSERS_HPP
#define EASY_CALCULATOR_PARSERS_HPP
#include <gmpxx.h>
#include <complex>
#include "expression.hpp"


typedef std::complex<double> complex;


namespace Parsers {
  namespace Int {
    extern Expr::Parser<mpz_class> parser;
    extern void init();
    extern inline mpz_class eval(const std::string &expression){
      return parser.parse(expression);
    }
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
    double cbrt_(const std::vector<double> &argv);
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
  namespace Complex {
    extern Expr::Parser<complex> parser;
    extern void init();
    extern inline complex eval(const std::string &expression){
      return parser.parse(expression);
    }
    complex convert(const std::string &str);
    complex power(const complex &left, const complex &right);
    complex abs(const std::vector<complex> &argv);
    complex arg(const std::vector<complex> &argv);
    complex conj(const std::vector<complex> &argv);
    complex sqrt_(const std::vector<complex> &argv);
    complex sin_(const std::vector<complex> &argv);
    complex cos_(const std::vector<complex> &argv);
    complex tan_(const std::vector<complex> &argv);
    complex asin_(const std::vector<complex> &argv);
    complex acos_(const std::vector<complex> &argv);
    complex atan_(const std::vector<complex> &argv);
    complex exp_(const std::vector<complex> &argv);
    complex log_(const std::vector<complex> &argv);
  }
}


#endif /* EASY_CALCULATOR_PARSERS_HPP */
