/**
 * @file   cread_line.h
 * @author Late Lee 
 * @date   2012-9-5 22:52:20
 * @brief
 *
 *      参考u-boot的creadline实现，功能如下：
        1、支持Linux中readline大部分功能：
        1)、c-a:
        2)、c-e:
        2、支持历史命令，暂定最多20条记录；
        3、支持多条命令连续输入，以分号“;”连接。

        未完成功能：
        Ctrl-u功能未完成 Ctrl-y功能未完成


        Delte键处理：
        Windows：删除当前光标所在字符
        Linux  ：删除当前光标前一个字符
 *
 */

#ifndef CREAD_LINE_H
#define CREAD_LINE_H

//#define SIMPLE_READLINE   /* very simple one using gets() */

#define PROMPT          "NotAShell> "
#define CB_SIZE         256 /* console buffer size */
#define MAX_CMDBUF_SIZE CB_SIZE

/* history command BEGIN */
#define HIST_MAX		20
#define HIST_SIZE		MAX_CMDBUF_SIZE

int readline (const char *const prompt, char* line_buf);

#endif /* CREAD_LINE_H */
