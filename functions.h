#ifndef FUNCTIONS
#define FUNCTIONS

void help();
int check_args(int nargs, char** args, char* str_compare);
int nc_strcmp(const char* s1, const char* s2);
int nc_strlen(const char* str);
void write_file(int nargs, char** args, const char* file_name);
char* nc_strcpy(char* dst, const char* src);
void write_file_portal(int nargs, char** args, const char* file_name);
void connect_history(char* network_name);

#endif
