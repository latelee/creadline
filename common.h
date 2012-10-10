/**
 * @file   commond.h
 * @author Late Lee <latelee@163.com>
 * @date   Thu Sep 2012
 * 
 * @brief  
 *         终端相关函数实现——在console.c文件实现
 * 
 * @log
 *         2012-09-26
           添加检测任意键按下的函数mytstc，不再使用Windows API方法。
           注：使用该函数后，输入缓冲区会有数据，需要使用mygetc读取。
 */
#ifndef COMMAN_H
#define COMMAN_H

#include <stdarg.h>

#ifdef _cplusplus
extern "C" {
#endif

/* stdin */
int mygetc(void);   /* To get that char */
int mytstc(void);   /* To test if a char is ready. */

/* stdout */
void myputc(const char c);  /* To put a char */
void myputs(const char *s); /* To put a string (accelerator) */
int myprintf(const char *fmt, ...);
int mysprintf(char * buf, const char *fmt, ...);
int myvsprintf(char *buf, const char *fmt, va_list args);

/* Ctrl-c */
int ctrlc (void);
int had_ctrlc (void);    /* have we had a Control-C since last clear? */
void clear_ctrlc (void);    /* clear the Control-C condition */
int disable_ctrlc (int);    /* 1 to disable, 0 to enable Control-C detect */

unsigned long   simple_strtoul(const char *cp,char **endp,unsigned int base);
unsigned long long  simple_strtoull(const char *cp,char **endp,unsigned int base);
long    simple_strtol(const char *cp,char **endp,unsigned int base);

// 需删除
void initIBL();

#ifdef _cplusplus
};
#endif

#endif /* COMMAN_H */