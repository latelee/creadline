#ifndef MY_COMMAND_H
#define MY_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

void cmd_init();
cmd_tbl_t* find_table(const char* cmdname);

int do_help_default(int argc, char* argv[]);
int do_print(int argc, char* argv[]);
int do_exit(int argc, char* argv[]);

#ifdef __cplusplus
};
#endif

#endif  /* MY_COMMAND_H */