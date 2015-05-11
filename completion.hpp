#ifndef EASY_CALCULATOR_COMPLETION_HPP
#define EASY_CALCULATOR_COMPLETION_HPP


char* dupstr(const char *s);

char* command_generator(const char *text, int state);

extern char** calc_completion(const char *text, int start, int end);


#endif /* EASY_CALCULATOR_COMPLETION_HPP */
