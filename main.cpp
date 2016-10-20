#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "readline.hpp"
#include "commands.hpp"
#include "completion.hpp"
#include "parsers.hpp"


const char *PROMPT = ">>> ";


bool quiet = false;


void init_parsers() {
  Parsers::Int::init();
  Parsers::Double::init();
}


void print_help() {
  std::cout << "Usage: EasyCalculator [options] [input file]...\n\n";
  std::cout << "  -q, --quiet\tDisable interactive command line.\n";
}


bool parse_args(int argc, char **argv) {
  bool has_file = false;
  for(int i=1; i<argc; i++) {
    if(!strcmp(argv[i], "-q") || !strcmp(argv[i], "--quiet")) {
      quiet = true;
    }else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      print_help();
      exit(0);
    }else{
      std::ifstream f(argv[i]);
      if(f.good()) {
	std::string line;
	while(std::getline(f, line))
	  parse_line(line);
      }else{
	std::cerr << "Invalid file " << argv[i] << '\n';
	exit(1);
      }
      has_file = true;
    }
  }
  return has_file;
}


int main(int argc, char **argv) {
  init_parsers();
  if(parse_args(argc, argv))
    return 0;
  if(!quiet) {
    rl_readline_name = "EasyCalculator";
    rl_attempted_completion_function = calc_completion;
    std::string line;
    char *input;
    while(input = readline(PROMPT)) {
      line = std::string(input);
      add_history(input);
      free(input);
      parse_line(line);
    }
  }else{
    std::string line;
    while(std::getline(std::cin, line))
      parse_line(line);
  }
  return 0;
}


