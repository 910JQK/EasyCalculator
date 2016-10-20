#include <cstdlib>
#include <cstring>
#include "readline.hpp"
#include "commands.hpp"
#include "parsers.hpp"
#include "completion.hpp"


char* dupstr(const char *s) {
  char *r;
  r = (char*)malloc(strlen(s) + 1);
  strcpy(r, s);
  return r;
}


char* generator_command(const char *text, int state) {
  static int list_index, len;
  const char *name;
  if(!state) {
    list_index = 0;
    len = strlen(text);
  }
  while(commands[list_index].exec != NULL) {
    name = commands[list_index].name.c_str();
    list_index++;
    if(strncmp(name, text, len) == 0)
      return dupstr(name);
  }
  return ((char*)NULL);
}


char** calc_completion(const char *text, int start, int end) {
  char **matches;

  matches = (char**)NULL;

  matches = rl_completion_matches(text, generator_command);    

  /*
  if(start == 0) {
    matches = rl_completion_matches(text, generator_command);    
  } else{
    matches = rl_completion_matches(text, generator_parser);
    rl_completion_suppress_append = 1;
  }
  */
  
  return matches;

}
