#ifndef MY_COMMAND_H
#define MY_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

int do_help(int argc, char * const argv[]);
int do_print(int argc, char * const argv[]);
int do_exit(int argc, char * const argv[]);

#ifdef __cplusplus
};
#endif

#endif  /* MY_COMMAND_H */