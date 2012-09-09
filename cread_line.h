/**
 * @file   cread_line.h
 * @author Late Lee 
 * @date   2012-9-5 22:52:20
 * @brief
 *
 *      参考u-boot的creadline实现，功能如下：
        1、支持Linux中readline大部分功能，具体如下：
        0)、上下左右方向键
        1)、c-a: 移到命令行首
        2)、c-e: 移到命令行尾
        3)、c-f: 前移一字符
        4)、c-b: 后移一字符
        5)、c-x/c-u: 删除整行
        6)、c-d: 删除光标当前字符
        7)、c-k: 删除当前光标至行尾字符
        8)、c-p: 前一条历史命令
        9)、c-n: 后一条历史命令
        10)、HOME/END: 移至行首/行尾
        11)、Delete：Windows-删除当前光标字符 Linux-删除光标前字符
        
        2、最大命令行缓冲区为256字符
        3、支持历史命令，最多10条记录；
        4、支持多条命令连续输入，以分号“;”连接。

        其它：
        未完成功能：
        Ctrl-u功能与readline库不相同
        Ctrl-y功能未完成


        Delte键处理：
        Windows：删除当前光标所在字符
        Linux  ：删除当前光标前一个字符
 *
 */

#ifndef CREAD_LINE_H
#define CREAD_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

//#define SIMPLE_READLINE   /* very simple one using gets() */

#define PROMPT          "NotAShell> "
#define CB_SIZE         256 /* console buffer size */
#define MAX_CMDBUF_SIZE CB_SIZE

/* history command BEGIN */
#define HIST_MAX		10
#define HIST_SIZE		MAX_CMDBUF_SIZE

int readline (const char *const prompt, char* line_buf);

#ifdef __cplusplus
};
#endif

#endif /* CREAD_LINE_H */
