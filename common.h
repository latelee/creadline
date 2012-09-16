#ifndef COMMAN_H
#define COMMAN_H

#include <stdarg.h>

#ifdef _cplusplus
extern "C" {
#endif

/* stdin */
int	mygetc(void);   /* To test if a char is ready... */
int mytstc(void);   /* To get that char */

/* stdout */
void myputc(const char c);  /* To put a char */
void myputs(const char *s); /* To put a string (accelerator) */
int myprintf(const char *fmt, ...);
int mysprintf(char * buf, const char *fmt, ...);
int myvsprintf(char *buf, const char *fmt, va_list args);

/* Ctrl-c */
int ctrlc (void);
int had_ctrlc (void);	/* have we had a Control-C since last clear? */
void clear_ctrlc (void);	/* clear the Control-C condition */
int disable_ctrlc (int);	/* 1 to disable, 0 to enable Control-C detect */

unsigned long   simple_strtoul(const char *cp,char **endp,unsigned int base);
unsigned long long  simple_strtoull(const char *cp,char **endp,unsigned int base);
long    simple_strtol(const char *cp,char **endp,unsigned int base);

#ifdef _cplusplus
};
#endif

#endif /* COMMAN_H */