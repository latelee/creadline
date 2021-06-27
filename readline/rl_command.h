/**
 * @file   rl_command.h
 * @author Late Lee <latelee@163.com>
 * @date   Thu Sep 06 08:52:01 2012
 * 
 * @brief  
 *         命令模板，参考u-boot源代码
 *         需自定义命令表及命令函数
 *         注：
 *            1、u-boot命令表存放在自定义的段中，需使用lds连接脚本，此版本不需要
 *            2、cmd_table需要自定义
 * 
 */

#ifndef RL_COMMAN_H
#define RL_COMMAN_H

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_SYS_MAXARGS 10
#define CONFIG_SYS_HELP_CMD_WIDTH 8

//#define DEBUG_PARSER
/*
 * Monitor Command Table
 */
typedef int (*cmd_func)(int argc, char* argv[]);

struct cmd_tbl_s {
    const char    *name;        /* Command Name            */
    int     maxargs;    /* maximum number of arguments    */
    cmd_func cmd;       /* Implementation function    */
    const char    *usage;        /* Usage message    (short)    */
#ifdef    CONFIG_SYS_LONGHELP
    char    *help;    /* Help  message    (long)    */
#endif
#ifdef CONFIG_AUTO_COMPLETE
    /* do auto completion on the arguments */
    int     (*complete)(int argc, char* argv[], char last_char, int maxv, char *cmdv[]);
#endif
};

typedef struct cmd_tbl_s    cmd_tbl_t;

// /* someone should implement the table */
// extern cmd_tbl_t cmd_table[];

void register_command(cmd_tbl_t* table, int len);

int do_help_default(int argc, char* argv[]);

int run_command (const char* cmd);

int run_command1(const char *cmd);

#ifdef __cplusplus
};
#endif

#endif /* COMMAN_H */
