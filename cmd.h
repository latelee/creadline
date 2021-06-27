#ifndef CMD_H
#define CMD_H

#ifdef WIN32
#include <windows.h>
#define mysleep(x) Sleep(x)
#else
#define mysleep(x) usleep(x*1000)
#endif

#ifdef __cplusplus
extern "C" {
#endif

int readline_cmd(int argc, char ** argv);

#ifdef __cplusplus
};
#endif

#endif  /* CMD_H */