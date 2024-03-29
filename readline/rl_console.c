#include <stdio.h>  // putchar
#include <stddef.h>
#include <ctype.h>
#include "rl_common.h"

#ifdef WIN32
#include <conio.h>

int mygetc(void)
{
    return getch();
}

/** 
 * return non-zero if a key pressed, zero if not.
 *
 */
int mytstc(void)
{
    return kbhit();
}

#else

#include <termios.h> /* for tcxxxattr, ECHO, etc */
#include <unistd.h> /* for STDIN_FILENO */
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

/*simulate windows' getch(), it works!!*/
int mygetc(void)
{
    int ch;
    struct termios oldt, newt;

    // get terminal input's attribute
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    //set termios' local mode
    newt.c_lflag &= ~(ECHO|ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    //read character from terminal input
    ch = getchar();

    //recover terminal's attribute
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

int mytstc(void)
{
    struct timeval tv;
    fd_set rdfs;
    int ch;
    struct termios oldt, newt;

    // get terminal input's attribute
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    //set termios' local mode
    newt.c_lflag &= ~(ECHO|ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    tv.tv_sec = 0;
    tv.tv_usec = 100;
    FD_ZERO(&rdfs);
    FD_SET (STDIN_FILENO, &rdfs);

    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
    ch = FD_ISSET(STDIN_FILENO, &rdfs);

    //recover terminal's attribute
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// another way of kbhit
#if 0
/*
 * kbhit() -- a keyboard lookahead monitor
 *
 * returns the number of characters available to read
 */
static int kbhit ( void )
{
    struct timeval tv;
    struct termios old_termios, new_termios;
    int            error;
    int            count = 0;
    tcgetattr( 0, &old_termios );
    new_termios              = old_termios;
    /*
     * raw mode
     */
    new_termios.c_lflag     &= ~ICANON;
    /*
     * disable echoing the char as it is typed
     */
    new_termios.c_lflag     &= ~ECHO;
    /*
     * minimum chars to wait for
     */
    new_termios.c_cc[VMIN]   = 1;
    /*
     * minimum wait time, 1 * 0.10s
     */
    new_termios.c_cc[VTIME]  = 1;
    error                    = tcsetattr( 0, TCSANOW, &new_termios );
    tv.tv_sec                = 0;
    tv.tv_usec               = 100;
    /*
     * insert a minimal delay
     */
    select( 1, NULL, NULL, NULL, &tv );
    error                   += ioctl( 0, FIONREAD, &count );
    error                   += tcsetattr( 0, TCSANOW, &old_termios );
    return( error == 0 ? count : -1 );
}  /* end of kbhit */
#endif // #if 0

#endif

/**
 * 输出'字符'，如myputc(1)，即输出ascii码1对应的字符，并非字符'1'
 *
 */
void myputc(const char c)
{
    if (c == '\n')
        myputc('\r');
    putchar(c);
}

void myputs(const char *s)
{
    while (*s)
        myputc(*s++);
}

// 缓冲区最大为512字节，如果打印信息超过此值，由系统处理，本函数不处理
int myprintf(const char *fmt, ...)
{
    va_list args;
    int i;
    char printbuffer[512];

    va_start(args, fmt);

    /* For this to work, printbuffer must be larger than
     * anything we ever want to print.
     */
    i = myvsprintf(printbuffer, fmt, args);
    va_end(args);

    /* Print the string */
    myputs(printbuffer);
    return i;
}

/**
 * sprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @fmt: The format string to use
 * @...: Arguments for the format string
 *
 * The function returns the number of characters written
 * into @buf.
 *
 * See the vsprintf() documentation for format string extensions over C99.
 */
int mysprintf(char * buf, const char *fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);
    i=myvsprintf(buf,fmt,args);
    va_end(args);
    return i;
}

/* test if ctrl-c was pressed */
static int ctrlc_disabled = 0;    /* see disable_ctrl() */
static int ctrlc_was_pressed = 0;
int ctrlc(void)
{
    if (!ctrlc_disabled) {
        if (mytstc()) {
            switch (mygetc()) {
            case 0x03:        /* ^C - Control C (ascii: 0x03) */
                ctrlc_was_pressed = 1;
                return 1;
            default:
                break;
            }
        }
    }
    return 0;
}

/* pass 1 to disable ctrlc() checking, 0 to enable.
 * returns previous state
 */
int disable_ctrlc(int disable)
{
    int prev = ctrlc_disabled;    /* save previous state */

    ctrlc_disabled = disable;
    return prev;
}

int had_ctrlc (void)
{
    return ctrlc_was_pressed;
}

void clear_ctrlc(void)
{
    ctrlc_was_pressed = 0;
}

/************************************************************************/

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
    unsigned long result = 0,value;

    if (*cp == '0') {
        cp++;
        if ((*cp == 'x') && isxdigit(cp[1])) {
            base = 16;
            cp++;
        }
        if (!base) {
            base = 8;
        }
    }
    if (!base) {
        base = 10;
    }
    while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
        ? toupper(*cp) : *cp)-'A'+10) < base) {
            result = result*base + value;
            cp++;
        }
        if (endp)
            *endp = (char *)cp;
        return result;
}

long simple_strtol(const char *cp,char **endp,unsigned int base)
{
    if(*cp=='-')
        return -simple_strtoul(cp+1,endp,base);
    return simple_strtoul(cp,endp,base);
}

int ustrtoul(const char *cp, char **endp, unsigned int base)
{
    unsigned long result = simple_strtoul(cp, endp, base);
    switch (**endp) {
    case 'G' :
        result *= 1024;
        /* fall through */
    case 'M':
        result *= 1024;
        /* fall through */
    case 'K':
    case 'k':
        result *= 1024;
        if ((*endp)[1] == 'i') {
            if ((*endp)[2] == 'B')
                (*endp) += 3;
            else
                (*endp) += 2;
        }
    }
    return result;
}

unsigned long long simple_strtoull (const char *cp, char **endp, unsigned int base)
{
    unsigned long long result = 0, value;

    if (*cp == '0') {
        cp++;
        if ((*cp == 'x') && isxdigit (cp[1])) {
            base = 16;
            cp++;
        }
        if (!base) {
            base = 8;
        }
    }
    if (!base) {
        base = 10;
    }
    while (isxdigit (*cp) && (value = isdigit (*cp)
        ? *cp - '0'
        : (islower (*cp) ? toupper (*cp) : *cp) - 'A' + 10) < base) {
            result = result * base + value;
            cp++;
        }
        if (endp)
            *endp = (char *) cp;
        return result;
}
