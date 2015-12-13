#include <iostream>
#include <cstring>
#include <cstdlib>
#include "readline.hpp"
#include "commands.hpp"
#include "completion.hpp"
#include "parsers.hpp"


const char *PROMPT = ">>> ";


bool quiet = false;


void init_parsers(){
  Parsers::Int::init();
  Parsers::Double::init();
}


void print_help(){
  std::cout << "Usage: EasyCalculator [options]\n\n";
  std::cout << "  -q, --quiet\tDisable interactive command line.\n";
}


void parse_args(int argc, char **argv){
  int i;
  for(i=1; i<argc; i++){
    if(!strcmp(argv[i], "-q") || !strcmp(argv[i], "--quiet")){
      quiet = true;
    }else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")){
      print_help();
      exit(0);
    }else{
      std::cerr << "Invalid argument " << argv[i] << '\n';
    }
  }
}


int main(int argc, char **argv){
  init_parsers();
  parse_args(argc, argv);
  if(!quiet){
    rl_readline_name = "EasyCalculator";
    rl_attempted_completion_function = calc_completion;
    std::string line;
    char *input;
    while(input = readline(PROMPT)){
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


