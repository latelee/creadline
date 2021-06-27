#include <stdio.h>
#include <string.h>
#include "rl_cread_line.h"
#include "rl_common.h"

#ifndef SIMPLE_READLINE

/* history command BEGIN */

#ifndef CMD_LITE

static int hist_max = 0;
static int hist_add_idx = 0;
static int hist_cur = -1;
unsigned int hist_num = 0;

static char* hist_list[HIST_MAX];
static char hist_lines[HIST_MAX][HIST_SIZE + 1];     /* Save room for NULL */

static void hist_init(void)
{
    int i;

    hist_max = 0;
    hist_add_idx = 0;
    hist_cur = -1;
    hist_num = 0;

    for (i = 0; i < HIST_MAX; i++)
    {
        hist_list[i] = hist_lines[i];
        hist_list[i][0] = '\0';
    }
}

static void cread_add_to_hist(char *line)
{
    strcpy(hist_list[hist_add_idx], line);

    if (++hist_add_idx >= HIST_MAX)
        hist_add_idx = 0;

    if (hist_add_idx > hist_max)
        hist_max = hist_add_idx;

    hist_num++;
}

static char* hist_prev(void)
{
    char *ret;
    int old_cur;

    if (hist_cur < 0)
        return NULL;

    old_cur = hist_cur;
    if (--hist_cur < 0)
        hist_cur = hist_max;

    if (hist_cur == hist_add_idx) {
        hist_cur = old_cur;
        ret = NULL;
    } else
        ret = hist_list[hist_cur];

    return (ret);
}

static char* hist_next(void)
{
    char *ret;

    if (hist_cur < 0)
        return NULL;

    if (hist_cur == hist_add_idx)
        return NULL;

    if (++hist_cur > hist_max)
        hist_cur = 0;

    if (hist_cur == hist_add_idx) {
        ret = "";
    } else
        ret = hist_list[hist_cur];

    return (ret);
}
#endif
/* history command END */

#define getcmd_getch()      mygetc()
#define getcmd_putch(ch)    myputc(ch)
#define getcmd_cbeep()
//#define getcmd_cbeep()      getcmd_putch('\a')
#define getcmd_printf       myprintf
#define getcmd_puts         myputs

#define CTL_CH(c)           ((c) - 'a' + 1)

#define CTL_BACKSPACE       ('\b')
#define DEL                 ((char)255)
#define DEL7                ((char)127)
#define BACKSPACE           ((char)8)
#define CREAD_HIST_CHAR     ('!')

#define putnstr(str,n)    do {          \
    getcmd_printf ("%.*s", (int)n, str);\
} while (0)

#ifndef CMD_LITE

#define BEGINNING_OF_LINE() {       \
    while (num) {                   \
    getcmd_putch(CTL_BACKSPACE);    \
    num--;                          \
    }                               \
}

#define ERASE_TO_EOL() {                     \
    if (num < eol_num) {                     \
    getcmd_printf("%*s", (int)(eol_num - num), ""); \
    do {                                     \
    getcmd_putch(CTL_BACKSPACE);             \
    } while (--eol_num > num);               \
    }                                        \
}

// TODO
#define ERASE_TO_CUR() {            \
    cur_num = num;                  \
    while (num) {                   \
    getcmd_putch(CTL_BACKSPACE);    \
    num--;                          \
    }                               \
    getcmd_printf("%*s", (int)(cur_num), ""); \
}

#define REFRESH_TO_EOL() {      \
    if (num < eol_num) {        \
    wlen = eol_num - num;       \
    putnstr(buf + num, wlen);   \
    num = eol_num;              \
    }                           \
}
#endif /* CMD_LITE */

static void cread_add_char(char ichar, int insert, unsigned long *num,
                           unsigned long *eol_num, char *buf, unsigned long len)
{
    unsigned long wlen;

    /* room ??? */
    if (insert || *num == *eol_num)
    {
        if (*eol_num > len - 1)
        {
            getcmd_cbeep();
            return;
        }
        (*eol_num)++;
    }

    if (insert)
    {
        wlen = *eol_num - *num;
        if (wlen > 1)
        {
            memmove(&buf[*num+1], &buf[*num], wlen-1);
        }

        buf[*num] = ichar;
        putnstr(buf + *num, wlen);
        (*num)++;
        while (--wlen)
        {
            getcmd_putch(CTL_BACKSPACE);
        }
    }
    else
    {
        /* echo the character */
        wlen = 1;
        buf[*num] = ichar;
        putnstr(buf + *num, wlen);
        (*num)++;
    }
}

static void cread_add_str(char *str, int strsize, int insert, unsigned long *num,
                          unsigned long *eol_num, char *buf, unsigned long len)
{
    while (strsize--)
    {
        cread_add_char(*str, insert, num, eol_num, buf, len);
        str++;
    }
}

static int cread_line(const char *const prompt, char *buf, unsigned int *len)
{
    unsigned long num = 0;
    unsigned long eol_num = 0;
    unsigned long wlen;
    unsigned char ichar;
    int insert = 1;
    int esc_len = 0;
    char esc_save[8];
    int init_len = (int)strlen(buf);
    int cur_num = 0;

    if (init_len)
        cread_add_str(buf, init_len, 1, &num, &eol_num, buf, *len);

    while (1)
    {
        ichar = getcmd_getch();

        if ((ichar == '\n') || (ichar == '\r'))
        {
            getcmd_putch('\n');
            break;
        }

         /*
         * handle Windows arrow key, etc.
         * note: arrow keys have two char, the first is 0xe0
         */
        if (ichar == 0xe0)
        {
            ichar = getcmd_getch();
            switch(ichar)
            {
            case 'H':   /* up arrow */
                ichar = CTL_CH('p');
                break;
            case 'P':   /* down arrow */
                ichar = CTL_CH('n');
                break;
            case 'K':   /* left arrow */
                ichar = CTL_CH('b');
                break;
            case 'M':   /* right arrow */
                ichar = CTL_CH('f');
                break;
            case 'G':   /* Home */
                ichar = CTL_CH('a');
                break;
            case 'O':   /* End */
                ichar = CTL_CH('e');
                break;
            case 'S':   /* Delete */
                //ichar = CTL_CH('d');  // Windows style(delete character underneath ther cursor)
                ichar = BACKSPACE;      // Linux style(delete character left to the cursor)
                break;

            default:
                break;
            }
        }

        /*
         * handle standard linux xterm esc sequences for arrow key, etc.
         */
        if (esc_len != 0)
        {
            if (esc_len == 1)
            {
                if (ichar == '[')
                {
                    esc_save[esc_len] = ichar;
                    esc_len = 2;
                }
                else
                {
                    cread_add_str(esc_save, esc_len, insert,
                              &num, &eol_num, buf, *len);
                    esc_len = 0;
                }
                continue;
            }

            switch (ichar)
            {
            case 'D':    /* <- key */
                ichar = CTL_CH('b');
                esc_len = 0;
                break;
            case 'C':    /* -> key */
                ichar = CTL_CH('f');
                esc_len = 0;
                break;    /* pass off to ^F handler */
            case 'H':    /* Home key */
                ichar = CTL_CH('a');
                esc_len = 0;
                break;    /* pass off to ^A handler */
            case 'A':    /* up arrow */
                ichar = CTL_CH('p');
                esc_len = 0;
                break;    /* pass off to ^P handler */
            case 'B':    /* down arrow */
                ichar = CTL_CH('n');
                esc_len = 0;
                break;    /* pass off to ^N handler */
            default:
                esc_save[esc_len++] = ichar;
                cread_add_str(esc_save, esc_len, insert,
                          &num, &eol_num, buf, *len);
                esc_len = 0;
                continue;
            }
        }
        /* End of linux arrow key */

        switch (ichar)
        {
        /* linux esc sequences */
        case 0x1b:
            if (esc_len == 0)
            {
                esc_save[esc_len] = ichar;
                esc_len = 1;
            }
            else
            {
                getcmd_puts("impossible condition #876\n");
                esc_len = 0;
            }
            break;

#ifdef CMD_LITE
        case CTL_CH('a'):
        case CTL_CH('f'):
        case CTL_CH('b'):
        case CTL_CH('d'):
        case CTL_CH('k'):
        case CTL_CH('e'):
        case CTL_CH('o'):
        case CTL_CH('x'):
        case CTL_CH('u'):
        case CTL_CH('p'):
        case CTL_CH('n'):
            break;
#else
        case CTL_CH('a'):
            BEGINNING_OF_LINE();
            break;
        case CTL_CH('f'):
            if (num < eol_num)
            {
                getcmd_putch(buf[num]);
                num++;
            }
            break;
        case CTL_CH('b'):
            if (num)
            {
                getcmd_putch(CTL_BACKSPACE);
                num--;
            }
            break;
        case CTL_CH('d'):
            if (num < eol_num)
            {
                wlen = eol_num - num - 1;
                if (wlen)
                {
                    memmove(&buf[num], &buf[num+1], wlen);
                    putnstr(buf + num, wlen);
                }

                getcmd_putch(' ');
                do {
                    getcmd_putch(CTL_BACKSPACE);
                } while (wlen--);
                eol_num--;
            }
            break;
        case CTL_CH('k'):
            ERASE_TO_EOL();
            break;
        case CTL_CH('e'):
            REFRESH_TO_EOL();
            break;
        case CTL_CH('o'):
            insert = !insert;
            break;
        case CTL_CH('x'):
        case CTL_CH('u'):
            BEGINNING_OF_LINE();
            ERASE_TO_EOL();
            break;
        case CTL_CH('p'):
        case CTL_CH('n'):
            {
                char * hline;
                esc_len = 0;

                if (ichar == CTL_CH('p'))
                    hline = hist_prev();
                else
                    hline = hist_next();

                if (!hline)
                {
                    getcmd_cbeep();
                    continue;
                }

                /* nuke the current line */
                /* first, go home */
                BEGINNING_OF_LINE();

                /* erase to end of line */
                ERASE_TO_EOL();

                /* copy new line into place and display */
                strcpy(buf, hline);
                eol_num = (unsigned long)strlen(buf);
                REFRESH_TO_EOL();
                continue;
            }
#endif /* CMD_LITE */

        case CTL_CH('c'):    /* ^C - break */
            *buf = '\0';    /* discard input */
            return (-1);
        case DEL:
        case DEL7:
        case BACKSPACE:
            if (num)
            {
                wlen = eol_num - num;
                num--;
                memmove(&buf[num], &buf[num+1], wlen);
                getcmd_putch(CTL_BACKSPACE);
                putnstr(buf + num, wlen);
                getcmd_putch(' ');
                do {
                    getcmd_putch(CTL_BACKSPACE);
                } while (wlen--);
                eol_num--;
            }
            break;

// TODO
#ifdef CONFIG_AUTO_COMPLETE
        case '\t': 
        {
            int num2, col;

            /* do not autocomplete when in the middle */
            if (num < eol_num)
            {
                getcmd_cbeep();
                break;
            }

            buf[num] = '\0';
            col = strlen(prompt) + eol_num;
            num2 = num;
            if (cmd_auto_complete(prompt, buf, &num2, &col))
            {
                col = num2 - num;
                num += col;
                eol_num += col;
            }
            break;
        }
#endif  /* CONFIG_AUTO_COMPLETE */
        default:
            cread_add_char(ichar, insert, &num, &eol_num, buf, *len);
            break;
        }
    }

    *len = eol_num;
    buf[eol_num] = '\0';    /* lose the newline */

#ifndef CMD_LITE
    if (buf[0] && buf[0] != CREAD_HIST_CHAR)
        cread_add_to_hist(buf);
    hist_cur = hist_add_idx;
#endif
    return 0;
}

int readline_into_buffer (const char *const prompt, char* buffer)
{
    char* p = buffer;
    unsigned int len=MAX_CMDBUF_SIZE;
    int rc;

#ifndef CMD_LITE
    static int initted = 0;

    if (!initted)
    {
        hist_init();
        initted = 1;
    }
#endif

    if (prompt)
        myputs(prompt);

    rc = cread_line(prompt, p, &len);
    return rc < 0 ? rc : len;
}

#else /* SIMPLE_READLINE */

/* simple version, just call gets() */
int readline_into_buffer_simple(const char *const prompt, char* buffer)
{
    char* p = buffer;

    if (prompt)
        printf("%s", prompt);

    gets(p);
    fflush(stdout);

    return (int)strlen(p);
}

#endif /* SIMPLE_READLINE */

int readline (const char *const prompt, char* line_buf)
{
    line_buf[0] = '\0';

#ifndef SIMPLE_READLINE 
    return readline_into_buffer(prompt, line_buf);
#else /* SIMPLE_READLINE */
    return readline_into_buffer_simple(prompt, line_buf);
#endif
}
