/**
 * @file   command.h
 * @author Late Lee <latelee@163.com>
 * @date   Thu Sep 06 08:52:01 2012
 * 
 * @brief  
 *         ����ģ�壬�ο�u-bootԴ����
 *         ���Զ�������������
 *         ע��
 *            1��u-boot����������Զ���Ķ��У���ʹ��lds���ӽű����˰汾����Ҫ
 *            2��cmd_table��Ҫ�Զ���
 * 
 */

#ifndef COMMAN_H
#define COMMAN_H

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_SYS_MAXARGS 10
#define CONFIG_SYS_HELP_CMD_WIDTH 8

//#define DEBUG_PARSER
/*
 * Monitor Command Table
 */
typedef int (*cmd_func)(int argc, char * const argv[]);

struct cmd_tbl_s {
    char    *name;        /* Command Name            */
    int     maxargs;    /* maximum number of arguments    */
    cmd_func cmd;       /* Implementation function    */
    char    *usage;        /* Usage message    (short)    */
#ifdef    CONFIG_SYS_LONGHELP
    char    *help;    /* Help  message    (long)    */
#endif
#ifdef CONFIG_AUTO_COMPLETE
    /* do auto completion on the arguments */
    int     (*complete)(int argc, char * const argv[], char last_char, int maxv, char *cmdv[]);
#endif
};

typedef struct cmd_tbl_s    cmd_tbl_t;

/* someone should implement the table */
extern cmd_tbl_t cmd_table[];

extern int _do_help (cmd_tbl_t *cmd_start, int cmd_items,
                     int argc, char * const argv[]);

int run_command (const char* cmd);

int run_command1(const char *cmd);

#ifdef __cplusplus
};
#endif

#endif /* COMMAN_H */
