
#include <stdio.h>
#include <string.h>
#include <conio.h>

#define PROMPT      "NotAShell> "
#define CB_SIZE     256 /* console buffer size */
#define MAX_CMDBUF_SIZE CB_SIZE

char console_buffer[CB_SIZE + 1];

// note: must using 'getch'
#ifdef WIN32
#define mygetch()        getch()
#else
int mygetch();
#endif

#define getcmd_getch()      mygetch()
#define getcmd_putch(ch)    putchar(ch)
#define getcmd_cbeep()
//#define getcmd_cbeep()        getcmd_putch('\a')

#define CTL_CH(c)           ((c) - 'a' + 1)

#define CTL_BACKSPACE       ('\b')
#define DEL                 ((char)255)
#define DEL7                ((char)127)
#define CREAD_HIST_CHAR     ('!')

#define putnstr(str,n)    do {      \
    printf ("%.*s", (int)n, str);    \
} while (0)

#define BEGINNING_OF_LINE() {       \
    while (num) {                   \
    getcmd_putch(CTL_BACKSPACE);    \
    num--;                          \
    }                               \
}

#define ERASE_TO_EOL() {                     \
    if (num < eol_num) {                     \
    printf("%*s", (int)(eol_num - num), ""); \
    do {                                     \
    getcmd_putch(CTL_BACKSPACE);             \
    } while (--eol_num > num);               \
    }                                        \
}

#define REFRESH_TO_EOL() {      \
    if (num < eol_num) {        \
    wlen = eol_num - num;       \
    putnstr(buf + num, wlen);   \
    num = eol_num;              \
    }                           \
}

static void cread_add_char(char ichar, int insert, unsigned long *num,
                           unsigned long *eol_num, char *buf, unsigned long len)
{
    unsigned long wlen;

    /* room ??? */
    if (insert || *num == *eol_num) {
        if (*eol_num > len - 1) {
            //getcmd_cbeep();
            return;
        }
        (*eol_num)++;
    }

    if (insert) {
        wlen = *eol_num - *num;
        if (wlen > 1) {
            memmove(&buf[*num+1], &buf[*num], wlen-1);
        }

        buf[*num] = ichar;
        putnstr(buf + *num, wlen);
        (*num)++;
        while (--wlen) {
            getcmd_putch(CTL_BACKSPACE);
        }
    } else {
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
    while (strsize--) {
        cread_add_char(*str, insert, num, eol_num, buf, len);
        str++;
    }
}

static int cread_line(const char *const prompt, char *buf, unsigned int *len)
{
    unsigned long num = 0;
    unsigned long eol_num = 0;
    unsigned long wlen;
    char ichar;
    int insert = 1;
    int esc_len = 0;
    char esc_save[8];
    int init_len = (int)strlen(buf);

    if (init_len)
        cread_add_str(buf, init_len, 1, &num, &eol_num, buf, *len);

    while (1)
    {
        fflush(stdout);
        ichar = getcmd_getch();

        if ((ichar == '\n') || (ichar == '\r')) {
            getcmd_putch('\n');
            break;
        }

        /*
         * handle standard linux xterm esc sequences for arrow key, etc.
         */
        if (esc_len != 0) {
            if (esc_len == 1) {
                if (ichar == '[') {
                    esc_save[esc_len] = ichar;
                    esc_len = 2;
                } else {
                    cread_add_str(esc_save, esc_len, insert,
                              &num, &eol_num, buf, *len);
                    esc_len = 0;
                }
                continue;
            }

            switch (ichar) {

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

        switch (ichar) {
        case 0x1b:
            if (esc_len == 0) {
                esc_save[esc_len] = ichar;
                esc_len = 1;
            } else {
                puts("impossible condition #876\n");
                esc_len = 0;
            }
            break;

        case CTL_CH('a'):
            BEGINNING_OF_LINE();
            break;
        case CTL_CH('c'):    /* ^C - break */
            *buf = '\0';    /* discard input */
            return (-1);
        case CTL_CH('f'):
            if (num < eol_num) {
                getcmd_putch(buf[num]);
                num++;
            }
            break;
        case CTL_CH('b'):
            if (num) {
                getcmd_putch(CTL_BACKSPACE);
                num--;
            }
            break;
        case CTL_CH('d'):
            if (num < eol_num) {
                wlen = eol_num - num - 1;
                if (wlen) {
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
        case DEL:
        case DEL7:
        case 8:
            if (num) {
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
        case CTL_CH('p'):
        case CTL_CH('n'):
        {
            char * hline;

            //esc_len = 0;

            //if (ichar == CTL_CH('p'))
            //    hline = hist_prev();
            //else
            //    hline = hist_next();

            //if (!hline) {
            //    getcmd_cbeep();
            //    continue;
            //}

            /* nuke the current line */
            /* first, go home */
            BEGINNING_OF_LINE();

            /* erase to end of line */
            ERASE_TO_EOL();

            /* copy new line into place and display */
            strcpy(buf, hline);
            eol_num = (int)strlen(buf);
            REFRESH_TO_EOL();
            continue;
        }
#ifdef CONFIG_AUTO_COMPLETE
        case '\t': {
            int num2, col;

            /* do not autocomplete when in the middle */
            if (num < eol_num) {
                getcmd_cbeep();
                break;
            }

            buf[num] = '\0';
            col = strlen(prompt) + eol_num;
            num2 = num;
            if (cmd_auto_complete(prompt, buf, &num2, &col)) {
                col = num2 - num;
                num += col;
                eol_num += col;
            }
            break;
        }
#endif
        default:
            cread_add_char(ichar, insert, &num, &eol_num, buf, *len);
            break;
        }
    }

    *len = eol_num;
    buf[eol_num] = '\0';    /* lose the newline */

    //if (buf[0] && buf[0] != CREAD_HIST_CHAR)
    //    cread_add_to_hist(buf);
    //hist_cur = hist_add_idx;

    return 0;
}

int readline_into_buffer (const char *const prompt, char * buffer)
{
    char *p = buffer;
    unsigned int len=MAX_CMDBUF_SIZE;
    int rc;
    static int initted = 0;

    if (!initted) {
        //hist_init();
        initted = 1;
    }

    if (prompt)
        printf("%s", prompt);

    rc = cread_line(prompt, p, &len);
    return rc < 0 ? rc : len;
    //gets(p);
    //len = strlen(p);
    //return len;
}

int readline (const char *const prompt)
{
    console_buffer[0] = '\0';

    return readline_into_buffer(prompt, console_buffer);
}

int readline_test(void)
{
    static char lastcommand[CB_SIZE] = {0};
    int len;
    int rc = 1;

    while (1)
    {
        len = readline(PROMPT);
        if (len > 0)
        {
            strcpy(lastcommand, console_buffer);
            printf("<echo>: %s\n", lastcommand);
        }

        else if (len == 0)
        {
            printf("nothing input.\n");
        }

        if (len == -1)
        {
            puts("\n<INTERRUPT>");
        }
        else
        {
            //run_command(lastcommand);
        }
     
    }
    return 0;
}


int main(void)
{
    readline_test();
}

#ifdef linux

#include <termios.h> /* for tcxxxattr, ECHO, etc */
#include <unistd.h> /* for STDIN_FILENO */

/*simulate windows' getch(), it works!!*/
int mygetch (void){
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
#endif