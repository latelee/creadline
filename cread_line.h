/**
 * @file   cread_line.h
 * @author Late Lee 
 * @date   2012-9-5 22:52:20
 * @brief
 *
 *      参考u-boot的creadline实现，功能如下：
        1、支持Linux中readline大部分功能，具体如下(c:表示按Ctrl键，c-a：同时Ctrl和a)：
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
        10)、c-c：中断命令
        11)、HOME/END: 移至行首/行尾
        12)、Delete：Windows-删除当前光标字符 Linux-删除光标前一字符(按Linux处理)
        13)、Backspace：删除光标前一字符
        其它一些转义字符：
        14)、c-h：键值为0x8，按Backspace处理
        15)、c-i：键值为0x9，按tab处理，Linux下可能会是补齐操作。
        16)、c-j：键值为0xa：换行  c-m：键值为0xd：回车   注：在Linux和Windows中，两者表现相同。
        
        2、最大命令行缓冲区为256字符
        3、支持历史命令，最多10条记录；
        4、支持多条命令连续输入，以分号“;”连接。

        其它：
        未完成功能：
        Ctrl-u功能与readline库不相同
        Ctrl-y功能未完成


        Delte键处理：
        Windows：删除当前光标所在字符(程序按Linux处理)
        Linux  ：删除当前光标前一个字符
 *
 * log
       2012-09-18
       添加CMD_LITE宏，定义该宏时可节省内存，但仅支持Backspace和Delete键。
 *
 * note
       2012-09-26
       方向键及其它：
       Windows: 
                up:     0xe0 'H'
                down:   0xe0 'P'
                right:  0xe0 'M'
                left:   0xe0 'K'

                HOME:   0xe0 'G'
                END:    0xe0 'O'(大写字母O)
                Delete: 0xe0 'S'
        Linux：
                up:     0x1b 0x5b 0x41  ^[[A
                down:   0x1b 0x5b 0x42  ^[[B
                right:  0x1b 0x5b 0x43  ^[[C
                left:   0x1b 0x5b 0x44  ^[[D
                
                HOME:   ?
                END:    ? 
                Delete: 0x7f 
 */

#ifndef CREAD_LINE_H
#define CREAD_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

//#define SIMPLE_READLINE   /* very simple one using gets() */
//#define CMD_LITE    /* define it to save text and memory */

#define PROMPT          "NotAShell> "
#define CB_SIZE         256 /* console buffer size */
#define MAX_CMDBUF_SIZE CB_SIZE

#define HIST_MAX        10              /* history cmd number */
#define HIST_SIZE       MAX_CMDBUF_SIZE /* history cmd buffer size */

int readline (const char *const prompt, char* line_buf);

#ifdef __cplusplus
};
#endif

#endif /* CREAD_LINE_H */
