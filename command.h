/**
 * @file   command.h
 * @author Late Lee <latelee@163.com>
 * @date   Thu Sep 06 08:52:01 2012
 * 
 * @brief  
 * 
 * 
 */

#ifndef COMMAN_H
#define COMMAN_H

#define CONFIG_SYS_MAXARGS 16
//#define DEBUG_PARSER
/*
 * Monitor Command Table
 */
typedef int (*cmd_func)(int argc, char * const argv[]);

struct cmd_tbl_s {
	char	*name;		/* Command Name			*/
	int		maxargs;	/* maximum number of arguments	*/
    cmd_func cmd;       /* Implementation function	*/
	char	*usage;		/* Usage message	(short)	*/
#ifdef	CONFIG_SYS_LONGHELP
	char		*help;	/* Help  message	(long)	*/
#endif
#ifdef CONFIG_AUTO_COMPLETE
	/* do auto completion on the arguments */
	int		(*complete)(int argc, char * const argv[], char last_char, int maxv, char *cmdv[]);
#endif
};

typedef struct cmd_tbl_s	cmd_tbl_t;

int do_help(int argc, char * const argv[]);
int do_foo(int argc, char * const argv[]);
int do_bar(int argc, char * const argv[]);
int do_test(int argc, char * const argv[]);
int do_exit(int argc, char * const argv[]);

int run_command (const char* cmd);

#endif /* COMMAN_H */
